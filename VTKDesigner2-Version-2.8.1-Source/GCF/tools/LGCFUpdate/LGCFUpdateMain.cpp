#include <QtCore>
#include <QtDebug>
#include <QtXml>

void lgupdate(const QString& fileName);
QStringList extractStrings(const QString &path);
QStringList extractStringsFromGUIXML(const QString& fileName);
QStringList extractStringsFromSource(const QString& fileName);
void writeStrings(const QString& fileName, const QStringList& strings);
void mergeStrings(const QString& fileName, const QStringList& strings);

int main(int argc, char** argv)
{
    // We create the core-application object for getting application
    // path etc.
    QCoreApplication a(argc, argv);

    // Find out the name of the project file from command line arguments
    QString projectFile;
    for(int i=1; i<a.arguments().count(); i++)
    {
        QString arg = a.arguments().at(i);
        if(!QFile::exists(arg))
            arg = QDir(a.applicationDirPath()).absoluteFilePath(arg);

        if(QFile::exists(arg))
        {
            projectFile = QFileInfo(arg).absoluteFilePath();

            QFileInfo fi(projectFile);
            if(fi.suffix() == "pro" && fi.isReadable())
                break;

            projectFile.clear();
        }
    }

    // If no project name was specified, report an error and
    if(projectFile.isEmpty())
    {
        qDebug("No project file was specified");
        return 0;
    }

    // Perform the lupdate thingy on GCF related strings
    ::lgupdate(projectFile);

    return 0;
}

void lgupdate(const QString& fileName)
{
    QDir fileDir = QFileInfo(fileName).absoluteDir();

    // Extract GCF_TRANSLATIONS from the project file.
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly))
    {
        qDebug("Could not open %1 for reading", qPrintable(fileName));
        return;
    }

    QStringList outputTsFiles;
    while(!file.atEnd())
    {
        QString line = file.readLine();
        line = line.simplified();

        // Process GCF_TRANSLATIONS += .... \
        //                                  \
        // ...
        // lines
        if(line.startsWith("GCF_TRANSLATIONS"))
        {
            QStringList linesToProcess;
            bool done = false;
            while(!done && !file.atEnd())
            {
                if(line.endsWith("\\"))
                    line = line.remove(line.length()-1, 1);
                else
                    done = true;

                linesToProcess += line;

                if(!done)
                {
                    line = file.readLine();
                    line = line.simplified();
                }
            }

            // Merge multiple lines in GCF_TRANSLATIONS into a single line
            QString gcftsLine = linesToProcess.join(" ").simplified();
            gcftsLine = gcftsLine.remove("GCF_TRANSLATIONS");
            gcftsLine = gcftsLine.remove("+=");
            gcftsLine = gcftsLine.remove("=");
            gcftsLine = gcftsLine.simplified();

            // Figure out the files to output
            QStringList tsFilesList = gcftsLine.split(" ", QString::SkipEmptyParts);
            Q_FOREACH(QString tsFile, tsFilesList)
            {
                // Filename could be relative or absolute. We need to make sure what kind they are
#ifdef Q_OS_WIN
                if(tsFile.length() >= 2 && tsFile.at(1) == ':') // absolute path
                    outputTsFiles << tsFile;
#else
                if(tsFile.at(0) == '/')
                    outputTsFiles << tsFile;
#endif
                else
                {
                    // file name is specified relatively
                    tsFile = fileDir.absoluteFilePath(tsFile);
                    tsFile = QFileInfo(tsFile).absoluteFilePath();
                    outputTsFiles << tsFile;
                }
            }
        }
    }

    // TODO: we need to test GCF_TRANSLATIONS += parsing more. For the moment, it is required that
    // the user leave two additional line breaks (newlines) after the GCF_TRANSLATIONS block

    // Next we extract all strings that need translation.
    QStringList strings = ::extractStrings(fileDir.absolutePath());
    qDebug("Found %d strings for translation", strings.count());

    // Now we need to write these strings out into each output file,
    // by merging already existing translations.
    Q_FOREACH(QString tsFile, outputTsFiles)
    {
        qDebug("Generating %s..", qPrintable(tsFile));
        if(QFile::exists(tsFile))
            ::mergeStrings(tsFile, strings);
        else
            ::writeStrings(tsFile, strings);
    }
}

QStringList extractStrings(const QString& path)
{
    QDirIterator it(path, QStringList() << "*.xml" << "*.h" << "*.cpp"
                    << "*.hpp" << "*.cxx" << "*.cc",
                    QDir::Files|QDir::Readable|QDir::NoSymLinks,
                    QDirIterator::Subdirectories);

    QStringList retList;
    while(it.hasNext())
    {
        it.next();
        QFileInfo fi = it.fileInfo();
        if(fi.suffix() == "xml")
            retList += ::extractStringsFromGUIXML(fi.absoluteFilePath());
        else
            retList += ::extractStringsFromSource(fi.absoluteFilePath());
    }

    retList << "Name" << "Value"; // for PropertyEditor.

    retList.removeDuplicates();
    qSort(retList);
    return retList;
}

QStringList extractStringsFromElement(const QDomElement& element)
{
    QStringList retList;

    // Look for title attribute
    QString title = element.attribute("title");
    if(!title.isEmpty())
        retList.append(title);

    // Look for attributes that start with tr:
    QDomNamedNodeMap attributes = element.attributes();
    for(int i=0; i<attributes.count(); i++)
    {
        QDomAttr attr = attributes.item(i).toAttr();
        if(attr.name().startsWith("tr:"))
            retList.append(attr.value());
    }

    QDomElement childE = element.firstChildElement();
    while(!childE.isNull())
    {
        // Look for strings in children of this element
        retList += ::extractStringsFromElement(childE);

        // Next element
        childE = childE.nextSiblingElement();
    }

    return retList;
}

QStringList extractStringsFromGUIXML(const QString& fileName)
{
    QStringList retList;

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly))
    {
        qDebug("Could not open GUIXML file (%s) for reading", qPrintable(fileName));
        return retList;
    }

    QDomDocument doc;
    QString errMsg; int lineNr=0, colNr=0;
    if(!doc.setContent(&file, &errMsg, &lineNr, &colNr))
    {
        qDebug("Could not parse GUIXML file (%s)\n Error (%d:%d) %s", qPrintable(fileName),
               lineNr, colNr, qPrintable(errMsg));
        return retList;
    }

    // We now look for two things in the GUIXML file
    // 1. title attribute in all GUIXML elements
    // 2. Any attribute that has a tr: prefixed to it.
    QDomElement rootE = doc.documentElement();
    retList = ::extractStringsFromElement(rootE);

    return retList;
}

QStringList extractStringsFromSource(const QString& fileName)
{
    QStringList retList;

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly))
    {
        qDebug("Could not open source file (%s) for reading", qPrintable(fileName));
        return retList;
    }

    while(!file.atEnd())
    {
        QString line = file.readLine();
        line = line.simplified();

        if(line.startsWith("Q_CLASSINFO"))
        {
            // Find out the key and value in Q_CLASSINFO
            line = line.remove(0, 12); // remove Q_CLASSINFO(
            line = line.remove(line.length()-1, 1);

            QStringList fields = line.split(",");
            if(fields.count() != 2)
                continue;

            QString key = fields.at(0).trimmed();
            key = key.remove(0, 1);
            key = key.remove(key.length()-1, 1);

            QString value = fields.at(1).trimmed();
            value = value.remove(0, 1);
            value = value.remove(value.length()-1, 1);

            if(key.endsWith(".DisplayName") || key == "SectionName")
                retList.append(value);
        }
        else if(line.startsWith("GCF_TRANSLATE"))
        {
            line = line.remove(0, 14); // remove GCF_TRANSLATE(
            while(line.at(line.length()-1) != ')')
                line = line.remove(line.length()-1, 1);
            line = line.remove(line.length()-1, 1);

            // Remove the first and last "
            if(line.isEmpty())
                continue;
            if(line.at(0) == '\"')
                line.remove(0, 1);
            if(line.isEmpty())
                continue;
            if(line.at(line.length()-1) == '\"')
                line.remove(line.length()-1, 1);
            if(line.isEmpty())
                continue;

            retList.append(line);
        }
    }

    return retList;
}

void writeStrings(const QString& fileName, const QStringList &strings)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly))
    {
        qDebug("   Cannot open file for writing");
        return;
    }

    QDomDocument doc("TS");

    QDomElement rootE = doc.createElement("TS");
    rootE.setAttribute("version", "2.0");
    rootE.setAttribute("language", "");
    rootE.setAttribute("sourcelanguage", "en");
    doc.appendChild(rootE);

    QDomElement contextE = doc.createElement("context");
    rootE.appendChild(contextE);

    QDomElement nameE = doc.createElement("name");
    nameE.appendChild(doc.createTextNode("GCF"));
    contextE.appendChild(nameE);

    Q_FOREACH(QString string, strings)
    {
        QDomElement messageE = doc.createElement("message");
        contextE.appendChild(messageE);

        QDomElement sourceE = doc.createElement("source");
        sourceE.appendChild(doc.createTextNode(string));
        messageE.appendChild(sourceE);

        QDomElement translationE = doc.createElement("translation");
        translationE.appendChild(doc.createTextNode(string));
        messageE.appendChild(translationE);
    }

    QString xml = doc.toString();
    file.write(xml.toLatin1());
}

void mergeStrings(const QString& fileName, const QStringList &strings)
{
    QMap<QString,QString> translationMap;
    QString sourceLanguage;
    QString language;

    // First lets load strings from the file that are already translated
    {
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly))
        {
            qDebug("    Cannot open file for reading");
            return;
        }

        QDomDocument doc;
        QString errMsg; int lineNr=0, colNr=0;
        if(!doc.setContent(&file, &errMsg, &lineNr, &colNr))
        {
            qDebug("    Cannot parse XML content (%d:%d) %s", lineNr, colNr, qPrintable(errMsg));
            return;
        }

        sourceLanguage = doc.documentElement().attribute("sourcelanguage");
        language = doc.documentElement().attribute("language");

        QDomElement contextE = doc.documentElement().firstChildElement("context");
        QDomElement messageE = contextE.firstChildElement("message");

        // The whole file should not contain any non-GCF contexts.
        while(!contextE.isNull())
        {
            if(contextE.firstChildElement("name").text() != "GCF")
            {
                qDebug("    This file contains translation context(s) other than GCF. The file will not be modified");
                return;
            }

            contextE = contextE.nextSiblingElement("context");
        }

        while(!messageE.isNull())
        {
            QDomElement sourceE = messageE.firstChildElement("source");
            QDomElement translationE = messageE.firstChildElement("translation");
            translationMap[sourceE.text()] = translationE.text();
            messageE = messageE.nextSiblingElement("message");
        }
    }

    // Now include new strings into the map; that are not already present in the map
    Q_FOREACH(QString string, strings)
    {
        if(!translationMap.contains(string))
            translationMap[string] = string;
    }

    // Now dump the whole thing into a file
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly))
    {
        qDebug("   Cannot open file for writing");
        return;
    }

    QDomDocument doc("TS");

    QDomElement rootE = doc.createElement("TS");
    rootE.setAttribute("version", "2.0");
    rootE.setAttribute("language", language);
    rootE.setAttribute("sourcelanguage", sourceLanguage);
    doc.appendChild(rootE);

    QDomElement contextE = doc.createElement("context");
    rootE.appendChild(contextE);

    QDomElement nameE = doc.createElement("name");
    nameE.appendChild(doc.createTextNode("GCF"));
    contextE.appendChild(nameE);

    QMap<QString,QString>::const_iterator it = translationMap.constBegin();
    QMap<QString,QString>::const_iterator end = translationMap.constEnd();
    while(it != end)
    {
        QDomElement messageE = doc.createElement("message");
        contextE.appendChild(messageE);

        QDomElement sourceE = doc.createElement("source");
        sourceE.appendChild(doc.createTextNode(it.key()));
        messageE.appendChild(sourceE);

        QDomElement translationE = doc.createElement("translation");
        translationE.appendChild(doc.createTextNode(it.value()));
        messageE.appendChild(translationE);

        ++it;
    }

    QString xml = doc.toString();
    file.write(xml.toUtf8());
}
