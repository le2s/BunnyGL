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

// Code generation routines
void genNodesSourceCode(const QString& compName, const QStringList& nodeNames, const QStringList& baseClassNames);

// Helper functions
bool isVtkNode(const QString& nodeName) { return nodeName.startsWith("vtk"); }
QString nodeClassName(const QString& nodeName);
void copyResourceFile(const QString& source, const QString& dest, const StringStringMap& replacementTable=StringStringMap());
QByteArray toAscii(const QString& string);

int main(int argc, char** argv)
{
    qDebug("\nVTK-Designer Visualization Node Generator (VisNodeGen)\n");

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

    // Check if the component and component/source exists!
    if( !QDir(compName).exists() || !QDir( QString("%1/source").arg(compName) ).exists() )
    {
        qDebug("Cannot find %s and/or %s/source directories", qPrintable(compName), qPrintable(compName));
        return -1;
    }

    qDebug("- Creating nodes [%s]", qPrintable(nodeNames.join(" ")));

    // Step 1: Construct a default replacement table
    replacementTable["COMPONENT_NAME"] = compName;
    replacementTable["ORGANIZATION"] = organization;

    // Step 2: Generate code for every node!
    genNodesSourceCode(compName, nodeNames, baseClassNames);

    qDebug("Nodes successfully created!");

    return 0;
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
        qDebug("    - Generating code for %s in %s/source", qPrintable(ncName), qPrintable(compName));

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

        copyResourceFile(":/VNG/Node.h", QString("./%1/source/%2.h").arg(compName).arg(ncName), map);
        copyResourceFile(":/VNG/Node.cpp", QString("./%1/source/%2.cpp").arg(compName).arg(ncName), map);
    }
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



