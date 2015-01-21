/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtCore>
#include <QtXml>

typedef QMap<QString,QString> StringStringMap;
static StringStringMap replacementTable;

bool sanityChecks(const QString& dirName);
void createDirectoryStructure(const QString& dirName);

// Code generation routines
void genProjectFile(const QString& dirName, const QStringList& nodeNames);
void genComponentSourceCode(const QString& compName, const QStringList& nodeNames);
void genNodesSourceCode(const QString& compName, const QStringList& nodeNames, const QStringList& baseClassNames);
void genGUIXML(const QString& compName);

// Helper functions
bool isVtkNode(const QString& nodeName) { return nodeName.startsWith("vtk"); }
QString nodeClassName(const QString& nodeName);
void deleteDirectory(const QString& dir, bool deleteDir=true);
void copyResourceFile(const QString& source, const QString& dest, const StringStringMap& replacementTable=StringStringMap());
QByteArray toAscii(const QString& string);

int main(int argc, char** argv)
{
    qDebug("\nVTK-Designer Node Factory Component Generator (VNF Component Gen)\n");

    if(argc <= 3)
    {
        qDebug("You must specify component name and atleast one node name. Usage\n"
               " %s <component-name> <organization> <node-name-1> <node-name-2> <node-name-3> ...\n"
               "Example:\n"
               " %s VCLAlgorithms VCreate_Logic_Pvt_Ltd CenterLineFilter BezierSurfaceSource BezierSurfaceWidget\n"
               "\n"
               "If you want to make use of inheritence between wrappers, then you can use the ':' operator\n"
               "to specify inheritence. Example\n"
               " %s VCLAlgorithms VCreate_Logic_Pvt_Ltd AbstractImageFilter BrightnessImageFilter:AbstractImageFilter BAWImageFilter:AbstractImageFilter\n",
               argv[0], argv[0], argv[0]);

        return -1;
    }

    QString compName = QString("%1").arg(argv[1]);
    QString organization = QString("%1").arg(argv[2]);
    QStringList nodeNames;
    QStringList baseClassNames;
    for(int i=3; i<argc; i++)
    {
        QString argStr = QString("%1").arg( argv[i] );
        QStringList argStrComps = argStr.split(':');
        nodeNames << argStrComps.first();
        if(argStrComps.count() == 2)
            baseClassNames << argStrComps.last();
        else
            baseClassNames << "CGenericVisNodeBase";
    }

    organization = organization.replace("_", " ");

    // Sanity checks. Check to see if compName directory already exists.
    // If yes, then we should ask the user to give a new component name
    // or delete the component directory
    if( !sanityChecks(compName) )
        return -1;

    qDebug("- Creating component %s with nodes [%s]", qPrintable(compName), qPrintable(nodeNames.join(" ")));

    // Step 1
    createDirectoryStructure(compName);

    // Step 2: Construct a default replacement table
    replacementTable["COMPONENT_NAME"] = compName;
    replacementTable["ORGANIZATION"] = organization;

    // Step 3: Create the project file
    genProjectFile(compName, nodeNames);

    // Step 4: Create component's header and CPP
    genComponentSourceCode(compName, nodeNames);

    // Step 5: Generate code for every node!
    genNodesSourceCode(compName, nodeNames, baseClassNames);

    // Step 6 Generate GUIXML file
    genGUIXML(compName);

    qDebug("VNF Component generated successfully!");

    return 0;
}

bool sanityChecks(const QString& dirName)
{
    qDebug("- Checking current working directory");
    QDir dir(".");
    if(dir.exists(dirName))
    {
        qDebug("   Directory %s already exists. All contents of this directory will be deleted!!!\n",
               qPrintable(dirName));
        deleteDirectory(dirName);
    }

    return true;
}

void createDirectoryStructure(const QString& dirName)
{
    qDebug("- Creating directory structure");
    QDir dir(".");
    dir.mkdir(dirName);
    dir.cd(dirName);

    dir.mkdir("guixml");
    dir.mkdir("icons");
    dir.mkdir("source");
}

void genProjectFile(const QString& dirName, const QStringList& nodeNames)
{
    qDebug("- Creating project file");

    StringStringMap map = replacementTable;
    QString hFiles, sFiles;

    hFiles = QString("source/%1Component.h \\\n").arg(dirName);
    for(int i=0; i<nodeNames.count(); i++)
        hFiles += QString("           source/%1.h \\\n").arg(nodeClassName(nodeNames[i]));
    map["HEADER_FILES"] = hFiles;

    sFiles = QString("source/%1Component.cpp \\\n").arg(dirName);
    for(int i=0; i<nodeNames.count(); i++)
        sFiles += QString("           source/%1.cpp \\\n").arg(nodeClassName(nodeNames[i]));
    map["SOURCE_FILES"] = sFiles;

    copyResourceFile(":/VNF/ProjectFile.pro", QString("./%1/%1.pro").arg(dirName), map);
    copyResourceFile(":/VNF/Project.qrc", QString("./%1/%1.qrc").arg(dirName), map);
}

void genComponentSourceCode(const QString& compName, const QStringList& nodeNames)
{
    qDebug("- Generating component source code");

    StringStringMap map = replacementTable;
    map["CLASS_NAME"] = QString("%1Component").arg(compName);

    QString regNodeStmts;
    QString nodeClassIncludes;

    for(int i=0; i<nodeNames.count(); i++)
    {
        QString ncName = nodeClassName(nodeNames[i]);
        regNodeStmts += QString("    REGISTER_NODE(%1, d->nodeDescMap);\n").arg(ncName);
        nodeClassIncludes += QString("#include \"%1.h\"\n").arg(ncName);
    }
    map["REIGSTER_NODE_STATEMENTS"] = regNodeStmts;
    map["NODE_CLASS_INCLUDES"] = nodeClassIncludes;

    map["CLASS_SCOPE"] = compName.toUpper();

    copyResourceFile(":/VNF/Component.h", QString("./%1/source/%1Component.h").arg(compName), map);
    copyResourceFile(":/VNF/Component.cpp", QString("./%1/source/%1Component.cpp").arg(compName), map);
}

void genNodesSourceCode(const QString& compName, const QStringList& nodeNames, const QStringList& baseClassNames)
{
    qDebug("- Generating vis-nodes source code");

    StringStringMap map = replacementTable;
    map["COMPONENT_CLASS_NAME"] = QString("%1Component").arg(compName);

    for(int i=0; i<nodeNames.count(); i++)
    {
        QString nodeName = nodeNames[i];
        QString ncName = nodeClassName(nodeName);
        QString bcName = nodeClassName(baseClassNames[i]);
        qDebug("    - Generating code for %s", qPrintable(ncName));

        map["CLASS_NAME"] = ncName;
        map["CLASS_SCOPE"] = ncName.toUpper();
        map["NODE_NAME"] = nodeName;
        map["BASE_CLASS"] = bcName;

        if(!isVtkNode(nodeName))
        {
            map["SET_VTK_OBJECT_IMPL"] = "";
            map["SET_VTK_OBJECT_DECL"] = "";
            map["CREATE_VTK_OBJECT"] = "";
            map["VTK_OBJECT_POINTER"] = "";
            map["VTK_CLASS_INCLUDE"] = "";
            map["DELETE_VTK_OBJECT"] = "";
        }
        else
        {
            QString ptrName = nodeName;
            ptrName = ptrName.remove(0, 3);
            ptrName[0] = ptrName[0].toLower();

            map["VTK_CLASS_INCLUDE"] = QString("#include \"%1.h\"").arg(nodeName);
            map["SET_VTK_OBJECT_DECL"] = "void setVtkObject(vtkObject* obj);";
            map["VTK_OBJECT_POINTER"] = QString("%1 *%2;").arg(nodeName).arg(ptrName);
            map["CREATE_VTK_OBJECT"] = QString("d->%1 = 0;\n"
                                               "    setVtkObject( %2::New() );")
                                               .arg(ptrName).arg(nodeName);
            QString implStr = QString(
                "void %1::setVtkObject(vtkObject* ptr)\n"
                "{\n"
                "    d->%2 = dynamic_cast<%3*>(ptr);\n"
                "    nodeData()[\"vtkObject\"] = qVariantFromValue<vtkObject*>(d->%2);\n"
                ).arg(ncName).arg(ptrName).arg(nodeName);
            if( bcName != "CGenericVisNodeBase")
                implStr += QString(
                "    %1::setVtkObject(ptr);\n"
                ).arg(bcName);
            implStr += "}\n\n";

            map["SET_VTK_OBJECT_IMPL"] = implStr;
            map["DELETE_VTK_OBJECT"] = QString("d->%1->Delete();").arg(ptrName);
        }

        copyResourceFile(":/VNF/Node.h", QString("./%1/source/%2.h").arg(compName).arg(ncName), map);
        copyResourceFile(":/VNF/Node.cpp", QString("./%1/source/%2.cpp").arg(compName).arg(ncName), map);
    }
}

void genGUIXML(const QString& compName)
{
    qDebug("- Generating GUIXML file");

    QString componentClassName = QString("%1Component").arg(compName);
    QString fileName = QString("./%1/guixml/%2.xml").arg(compName).arg(componentClassName);

    QDomDocument doc;

    QDomElement compE = doc.createElement("component");
    doc.appendChild(compE);
    compE.setAttribute("name", componentClassName);
    compE.setAttribute("title", QString("%1").arg(componentClassName));

    QDomElement objectE = doc.createElement("object");
    compE.appendChild(objectE);
    objectE.setAttribute("name", "controller");

    QString xmlStr = doc.toString();

    QFile file(fileName);
    file.open(QFile::WriteOnly);
    file.write( toAscii(xmlStr) );
    file.close();
}

QString nodeClassName(const QString& nodeName)
{
    if(nodeName == "CGenericVisNodeBase")
        return nodeName;

    QString ncName = nodeName;
    if(nodeName.startsWith("vtk"))
        return QString("CVtk%1").arg( ncName.remove(0,3) );

    return QString("%1VisNode").arg(nodeName);
}

void deleteDirectory(const QString& dir, bool deleteDir)
{
    QDir d(dir);

    QFileInfoList list = d.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);
    for(int i=0; i<list.count(); i++)
    {
        QFileInfo fi = list[i];
        if( fi.isFile() )
            QFile::remove( fi.absoluteFilePath() );
        else
        {
            QDir d2 = d;
            d2.cd( fi.fileName() );
            deleteDirectory( d2.absolutePath() );
        }
    }

    if(deleteDir)
    {
        d.cdUp();
        d.rmdir( QFileInfo(dir).fileName() );
    }
}

void copyResourceFile(const QString& source, const QString& dest, const StringStringMap& table)
{
    QFile resFile(source);
    resFile.open(QFile::ReadOnly);
    QString resStr = resFile.readAll();
    resFile.close();

    StringStringMap::const_iterator it = table.begin();
    StringStringMap::const_iterator end = table.end();
    while(it != end)
    {
        QString key = QString("((%1))").arg(it.key());
        resStr.replace(key, it.value());
        ++it;
    }

    QFile destFile(dest);
    destFile.open(QFile::WriteOnly);
    destFile.write( toAscii(resStr) );
    destFile.close();
}

QByteArray toAscii(const QString& string)
{
    QByteArray ret = string.toAscii();
#ifdef Q_WS_X11
    ret = ret.replace("\r\n", "\n");
#endif
    return ret;    
}



