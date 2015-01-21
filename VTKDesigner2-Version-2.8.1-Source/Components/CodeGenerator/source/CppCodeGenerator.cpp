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

#include "CppCodeGenerator.h"
#include "IVisNetwork.h"
#include "IVisSystemNode.h"
#include "IVisSystemNodeDesc.h"
#include "IVisNetworkConnection.h"
#include "IVisSystemNodeConnectionPath.h"
#include "IVisSystemNodeIOData.h"

#include "IDefaultProperty.h"
#include "IDefaultPropertyManager.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMetaObject>
#include <QMetaProperty>
#include <QDataStream>
#include <QBuffer>

#include <GCF/AbstractComponent>
#include <QStringList>
#include <CGenericVisNodeDesc.h>

#include "CppCodeGenerator_p.h"

struct CppCodeGeneratorData
{
    CppCodeGeneratorData() : defaultPropertyManager(0), codeMap(0) { }

    CppCodeGenerator* codeGen;

    // Default param generators
    QList<ICppParamGenerator*> paramGenerators;

    // This list will be populated on a per node basis
    // This will help canGenerateProperty() and generateProperty() to do
    // their work.
    QList<ICppParamGenerator*> currentParamGenerators;

    // List of get-output and set-input code
    // generators
    QList<ICppConnectionGetOutputCodeGenerator*> getOutputGens;
    QList<ICppConnectionSetInputCodeGenerator*> setInputGens;

    // Store current context information
    bool stop;
    IVisNetwork* visNetwork;
    ICodeGeneratorManager* codeGenManager;
    QDir codeDir;
    IDefaultPropertyManager* defaultPropertyManager;
    QMap<QString,QString>* codeMap;

    void reportProgress(int percent, const QString& message);
    bool generateCode();
};

CppCodeGenerator::CppCodeGenerator(QObject* parent)
:QObject(parent)
{
    d = new CppCodeGeneratorData;
    d->codeGen = this;

    // Register the default list of param generators
    static StringParamGenerator stringGen;
    d->paramGenerators.append(&stringGen);
    static BoolParamGenerator boolGen;
    d->paramGenerators.append(&boolGen);
    static IntParamGenerator intGen;
    d->paramGenerators.append(&intGen);
    static IntListParamGenerator intListGen;
    d->paramGenerators.append(&intListGen);
    static DoubleParamGenerator doubleGen;
    d->paramGenerators.append(&doubleGen);
    static DoubleListParamGenerator doubleListGen;
    d->paramGenerators.append(&doubleListGen);
    static ColorParamGenerator colorGen;
    d->paramGenerators.append(&colorGen);

    // Register default get-output code generators
    static VtkAlgorithmGetOutputCodeGenerator getOutput1;
    d->getOutputGens.append(&getOutput1);
    static VtkObjectGetOutputCodeGenerator getOutput5;
    d->getOutputGens.append(&getOutput5);
    

    // Register default set-input code generators
    static VtkAlgorithmSetInputCodeGenerator setInput1;
    d->setInputGens.append(&setInput1);
    static VtkActorSetInputCodeGenerator setInput2;
    d->setInputGens.append(&setInput2);
    static VtkRendererSetInputCodeGenerator setInput3;
    d->setInputGens.append(&setInput3);
    static VtkRenderWindowSetInputCodeGenerator setInput4;
    d->setInputGens.append(&setInput4);
    static VtkImplicitBooleanSetInputCodeGenerator setInput5;
    d->setInputGens.append(&setInput5);
    static VtkSampleFunctionSetInputCodeGenerator setInput6;
    d->setInputGens.append(&setInput6);
    
}

CppCodeGenerator::~CppCodeGenerator()
{
    delete d;
}

QObject* CppCodeGenerator::containerObject()
{
    return this;
}

QString CppCodeGenerator::name() const
{
    return "QVTK/C++ Code Generator";
}

QIcon CppCodeGenerator::icon() const
{
    return QIcon();
}

QString CppCodeGenerator::description() const
{
    return "Generates Qt/VTK/C++ Code";
}

bool CppCodeGenerator::generateCode(const QDir& dir, IVisNetwork* network, ICodeGeneratorManager* codeGenMan)
{
    d->stop = false;
    d->visNetwork = network;
    d->codeGenManager = codeGenMan;
    d->codeDir = dir;
    d->codeMap = 0;

    bool success = d->generateCode();

    d->stop = false;
    d->visNetwork = 0;
    d->codeGenManager = 0;
    d->codeDir = QDir();
    d->codeMap = 0;

    return success;
}

bool CppCodeGenerator::generateCode(QMap<QString,QString>& code, IVisNetwork* network, ICodeGeneratorManager* codeGenMan)
{
    d->stop = false;
    d->visNetwork = network;
    d->codeGenManager = codeGenMan;
    d->codeDir = QDir();
    d->codeMap = &code;

    bool success = d->generateCode();

    d->stop = false;
    d->visNetwork = 0;
    d->codeGenManager = 0;
    d->codeDir = QDir();
    d->codeMap = 0;

    return success;
}

void CppCodeGenerator::cancelCodeGeneration()
{
    // TODO
}

QString CppCodeGenerator::productName() const
{
    return this->name();
}

QString CppCodeGenerator::organization() const
{
    return "VCreate Logic Private Limited";
}

QImage CppCodeGenerator::programLogo() const
{
    return QImage();
}

QString CppCodeGenerator::version() const
{
    return "1.0";
}

QString CppCodeGenerator::shortDescription() const
{
    return this->description();
}

QString CppCodeGenerator::homepage() const
{
    return "http://www.vcreatelogic.com/oss/vtkdesigner";
}

QString CppCodeGenerator::bugAddress() const
{
    return "http://www.vcreatelogic.com/oss/vtkdesigner/feedback/bugs";
}

const QList<GCF::AboutPerson> CppCodeGenerator::authors() const
{
    static QList<GCF::AboutPerson> retList;
    if(retList.count() == 0)
    {
        retList << GCF::AboutPerson("Prashanth N Udupa", "prashanth@vcreatelogic.com", "http://www.vcreatelogic.com", "Founder, Vision, Design, Development");
        retList << GCF::AboutPerson("Aadithya C Udupa", "udupa.adithya@gmail.com", "http://www.vcreatelogic.com", "Implementation of code generators");
    }
    return retList;
}

const QList<GCF::AboutPerson> CppCodeGenerator::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString CppCodeGenerator::license() const
{
    return "Dual License: GPL Version 2 for FOSS development, Commercial for others.";
}

QString CppCodeGenerator::copyrightStatement() const
{
    return QString();
}

bool CppCodeGenerator::canGenerateProperty(
                 IVisSystemNode* node,
                 IVisSystemNodeDesc* nodeDesc,
                 const QString& propName,
                 const QStringList& params,
                 const QVariant& value)
{
    // We have to generate code for any combination.
    // Because the buck stops here. :-)
    Q_UNUSED(node);
    Q_UNUSED(nodeDesc);
    Q_UNUSED(propName);
    Q_UNUSED(params);
    Q_UNUSED(value);

    return true;
}

bool CppCodeGenerator::generateProperty(
             IVisSystemNode* node,
             IVisSystemNodeDesc* nodeDesc,
             const QString& propName,
             const QStringList& params,
             const QVariant& value,
             QTextStream& ts, int tab)
{
    bool success = false;
    QString setMethodName = nodeDesc->propertySetMethod(propName);
    ts << QByteArray(tab, ' ') << node->nodeName() << "->" << setMethodName << "(";

    Q_FOREACH(ICppParamGenerator* paramGen, d->currentParamGenerators)
    {
        if( !paramGen->canGenerateParams(params, value) )
            continue;

        success = paramGen->generateParams(params, value, ts);
        if(!success)
            break;
    }

    if(!success)
        ts << "/* Cannot generate parameters (" << value.typeName() << ")" 
           << "(" << params.join(", ") << ") */";

    ts << ");\n";

    return success;
}

QList<ICppParamGenerator*> CppCodeGenerator::paramGenerators() const
{
    return d->paramGenerators;
}


bool CppCodeGenerator::canGenerateConnectionCode(IVisNetworkConnection* con)
{
    /*
    The standard connection code generator works only for
    input output connections. We just need to make that
    (con) doesnt correspond to a control connection
    */

    IVisSystemNode* senderNode = con->senderNode();
    IVisSystemNodeDesc* senderDesc = senderNode->nodeDesc();
    int senderPathIdx = con->senderPathIndex();

    IVisSystemNode* receiverNode = con->receiverNode();
    IVisSystemNodeDesc* receiverDesc = receiverNode->nodeDesc();
    int receiverPathIdx = con->receiverPathIndex();

    IVisSystemNodeConnectionPath* senderPath
        = senderDesc->connectionPath( senderPathIdx );
    IVisSystemNodeConnectionPath* receiverPath
        = receiverDesc->connectionPath( receiverPathIdx );

    if(senderPath->pathType() != IVisSystemNodeConnectionPath::OutputPath)
        return false;

    if(receiverPath->pathType() != IVisSystemNodeConnectionPath::InputPath)
        return false;

    return true;
}

bool CppCodeGenerator::generateConnectionCode(IVisNetworkConnection* con,
                                              QTextStream& ts, int tab)
{
    IVisSystemNode* senderNode = con->senderNode();
    IVisSystemNodeDesc* senderDesc = senderNode->nodeDesc();
    int senderPathIdx = con->senderPathIndex();

    IVisSystemNode* receiverNode = con->receiverNode();
    IVisSystemNodeDesc* receiverDesc = receiverNode->nodeDesc();
    int receiverPathIdx = con->receiverPathIndex();

    IVisSystemNodeConnectionPath* senderPath
        = senderDesc->connectionPath( senderPathIdx );
    IVisSystemNodeConnectionPath* receiverPath
        = receiverDesc->connectionPath( receiverPathIdx );

    // Find out if the sender implements ICppConnectionGetOutputCodeGenerator
    ICppConnectionGetOutputCodeGenerator* getOutputGen = 0;
    getOutputGen = qobject_cast<ICppConnectionGetOutputCodeGenerator*>(senderNode->containerObject());
    if(getOutputGen)
    {
        if( !getOutputGen->canGenerateGetOutputCode(senderNode, senderPath, con) )
            getOutputGen = 0;
    }
    if(!getOutputGen)
    {
        Q_FOREACH(ICppConnectionGetOutputCodeGenerator* gen, d->getOutputGens)
        {
            if(gen->canGenerateGetOutputCode(senderNode, senderPath, con))
            {
                getOutputGen = gen;
                break;
            }
        }
    }

    if(!getOutputGen)
        return false;

    // Find out if the receiver implements ICppConnectionSetInputCodeGenerator
    ICppConnectionSetInputCodeGenerator* setInputGen = 0;
    setInputGen = qobject_cast<ICppConnectionSetInputCodeGenerator*>(receiverNode->containerObject());
    if(setInputGen)
    {
        if( !setInputGen->canGenerateSetInputCode(receiverNode, receiverPath, con) )
            setInputGen = 0;
    }
    if(!setInputGen)
    {
        Q_FOREACH(ICppConnectionSetInputCodeGenerator* gen, d->setInputGens)
        {
            if(gen->canGenerateSetInputCode(receiverNode, receiverPath, con))
            {
                setInputGen = gen;
                break;
            }
        }
    }

    if(!setInputGen)
        return false;

    // Now we have both set-input and get-output generators.
    bool success = true;

    // Generate the get-output part
    QString outputCode;
    QString outputVariable;
    QTextStream outputCodeTs(&outputCode, QIODevice::WriteOnly);
    success = getOutputGen->generateGetOutputCode(senderNode, senderPath, con,
                                                  outputCodeTs, tab, 
                                                  outputVariable);
    if(!success)
        return false;

    // Generate the set-input part
    if(outputVariable.isEmpty())
        outputVariable = outputCode;
    else
        ts << outputCode;

    success = setInputGen->generateSetInputCode(receiverNode, receiverPath, con,
                                                outputVariable, ts, tab);

    // Phew! all done.
    return success;
}

// Actual code generator functions can be found here.
void CppCodeGeneratorData::reportProgress(int percent, const QString& message)
{
    emit codeGen->progressValue(percent);
    emit codeGen->progressMessage(message);
    qDebug("    CppCodeGenerator: %d: %s", percent, qPrintable(message));
}

bool CppCodeGeneratorData::generateCode()
{
    if(this->stop) // It could happen ;-)
        return false;

    if(!this->codeMap)
    {
        // Check if we can write a file called Pipeline.cpp into the output
        // directory.
        QString fileName = this->codeDir.absoluteFilePath("Pipeline.cpp");
        QFile file(fileName);
        if(!file.open(QFile::WriteOnly))
            return false;
    }

    // Lets generate the code
    QString codeString;
    QTextStream ts(&codeString);

    // Begin writing the output
    reportProgress(2, "Generating include files...");
    ts << "/* This code has been generated using VTK Designer Version "
          << qApp->applicationVersion() << " */\n\n";

    // Include the required classes
    ts << "#include <QApplication>\n";
    ts << "#include <QTabWidget>\n\n";
    ts << "#include \"QVTKWidget.h\"\n";
    ts << "#include \"vtkSmartPointer.h\"\n\n";
    QStringList classNames;
    for(int i=0; i<visNetwork->nodeCount(); i++)
    {
        IVisSystemNode* node = visNetwork->node(i);
        QString cn = node->nodeDesc()->nodeClassName();
        if(classNames.contains(cn))
            continue;

        classNames << cn;
        ts << "#include \"" << cn << ".h\"\n";
    }
    classNames.clear();
    ts << "\n\n";

    if(this->stop)
        return false;

    ts << "int main(int argc, char** argv)\n"
          "{\n"
          "    QApplication a(argc, argv);\n\n"
          "    QTabWidget outputWindow;\n"
          "    outputWindow.setWindowTitle( \"VTK Output Window\" );\n\n";

    if(this->stop)
        return false;

    // Generate node declarations
    for(int i=0; i<visNetwork->nodeCount(); i++)
    {
        IVisSystemNode* node = visNetwork->node(i);

        QString nodeName = node->nodeName();
        QString className = node->nodeDesc()->nodeClassName();
        ts << "    vtkSmartPointer<" << className << "> " << nodeName
           << " = " << className << "::New();\n";

        if(className == "vtkRenderWindow")
        {
            ts << "    QVTKWidget* vtkWidget_" << nodeName << " = new QVTKWidget;\n"
                  "    outputWindow.addTab(vtkWidget_" << nodeName << ", \"" << nodeName << "\");\n"
                  "    vtkWidget_" << nodeName << "->SetRenderWindow(" << nodeName << ");\n";
        }
    }

    ts << "\n";

    if(this->stop)
        return false;

    if(!this->defaultPropertyManager)
        this->defaultPropertyManager
            = GCF::findObject<IDefaultPropertyManager*>();

    ts << "    // Configure properties\n\n";

    // Configure properties
    for(int i=0; i<visNetwork->nodeCount(); i++)
    {
        IVisSystemNode* node = visNetwork->node(i);
        IVisSystemNodeDesc* nodeDesc = node->nodeDesc();
        QObject* nodeObj = node->containerObject();
        QString nodeName = node->nodeName();

        IDefaultProperty* defProps
            = this->defaultPropertyManager->defaultProperty(nodeObj);

        // Find out if the node is giving us any custom param generators
        ICppParamGeneratorRegistry* paramGenList = 0;
        paramGenList = qobject_cast<ICppParamGeneratorRegistry*>(node->containerObject());
        if(paramGenList)
            currentParamGenerators = paramGenList->paramGenerators() + paramGenerators;
        else
            currentParamGenerators = paramGenerators;

        int counter = 0;
        const QMetaObject* mo = nodeObj->metaObject();
        for(int j=QObject::staticMetaObject.propertyCount();
                j<mo->propertyCount(); j++)
        {
            QMetaProperty prop = mo->property(j);
            QString propName(prop.name());
            QVariant value = prop.read(nodeObj);
            QVariant defValue;

            if(defProps && defProps->hasDefaultValue(propName))
                defValue = defProps->defaultValue(propName);

            if(defValue.isValid() && defValue == value)
                continue;

            if( !nodeDesc->hasPropertyInfo(propName) )
            {
                ts << "    // Property info for " << nodeDesc->nodeClassName()
                   << "." << propName << " of type " << prop.typeName() 
                   << " not available\n";
                continue;
            }

            QStringList setParamTypes = nodeDesc->propertySetParameters(propName);
            if(!setParamTypes.count())
                continue;

            // Now we need to generate set-property code
            // Lets ask the node if it can handle the property
            // code generation by itself.
            bool success = false;
            ICppPropertyGenerator* propCodeGen = 0;
            propCodeGen = qobject_cast<ICppPropertyGenerator*>(node->containerObject());
            if(propCodeGen)
            {
                // Ok, lets ask the node to write the property
                success = propCodeGen->canGenerateProperty(node, nodeDesc,
                            propName, setParamTypes, value);
                if(success)
                    success = propCodeGen->generateProperty(node, nodeDesc,
                                propName, setParamTypes, value, ts, 4);
            }

            if(success)
            {
                ++counter;
                continue;
            }

            // the node did not want to generate code for this
            // particular property. Lets try it ourselves.
            codeGen->generateProperty(node, nodeDesc, propName, setParamTypes, 
                                   value, ts, 4);

            ++counter;
        }

        if(counter)
            ts << "\n";
    }

    if(this->stop)
        return false;

    // Write connections
    for(int i=0; i<visNetwork->connectionCount(); i++)
    {
        IVisNetworkConnection* con = visNetwork->connection(i);

        // Find out if the sender or receiver implements the 
        // ICppConnectionCodeGenerator interface. Then give them
        // a chance to generate the code.
        ICppConnectionCodeGenerator* conCodeGen = 0;
        conCodeGen = qobject_cast<ICppConnectionCodeGenerator*>(con->senderNode()->containerObject());
        if(conCodeGen)
        {
            if(conCodeGen->canGenerateConnectionCode(con))
            {
                if( conCodeGen->generateConnectionCode(con, ts, 4) )
                    continue;
            }
        }

        conCodeGen = qobject_cast<ICppConnectionCodeGenerator*>(con->receiverNode()->containerObject());
        if(conCodeGen)
        {
            if(conCodeGen->canGenerateConnectionCode(con))
            {
                if( conCodeGen->generateConnectionCode(con, ts, 4) )
                    continue;
            }
        }

        // If we are here, then both sender and receiver nodes
        // did not generate the connection code themselves
        bool success = codeGen->canGenerateConnectionCode(con);
        if( success )
            success &= codeGen->generateConnectionCode(con, ts, 4);
        
        if(!success)
        {
            IVisSystemNode* senderNode = con->senderNode();
            IVisSystemNodeDesc* senderDesc = senderNode->nodeDesc();
            int senderPathIdx = con->senderPathIndex();

            IVisSystemNode* receiverNode = con->receiverNode();
            IVisSystemNodeDesc* receiverDesc = receiverNode->nodeDesc();
            int receiverPathIdx = con->receiverPathIndex();

            IVisSystemNodeConnectionPath* senderPath
                = senderDesc->connectionPath( senderPathIdx );
            IVisSystemNodeConnectionPath* receiverPath
                = receiverDesc->connectionPath( receiverPathIdx );

            ts << "    // Cannot generate connection code for "
               << senderNode->nodeName() << "." << senderPath->pathName()
               << " (" << senderPath->pathDataType() << ") => "
               << receiverNode->nodeName() << "." << receiverPath->pathName()
               << " (" << receiverPath->pathDataType() << ")\n";
        }
    }

    ts << "\n"
       << "    outputWindow.resize(800, 800);\n"
       << "    outputWindow.show();\n\n"
       << "    return a.exec();\n";
    ts << "}\n\n";

    // Now that the code is generated, we can choose between writing to disk
    // and writing to the codeMap (if provided)
    if(this->codeMap)
        this->codeMap->insert("Pipeline", codeString);
    else
    {
        QString fileName = this->codeDir.absoluteFilePath("Pipeline.cpp");
        QFile file(fileName);
        file.open(QFile::WriteOnly);
        file.write(codeString.toLatin1());
    }

    return true;
}
