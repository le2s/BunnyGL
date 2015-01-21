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

#include "CodeGeneratorComponent.h"
#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>

#include "ICodeGenerator.h"
#include "IVisNetwork.h"

#include <QProgressDialog>
#include <QMetaType>
#include <QMainWindow>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QMenu>
#include <QClipboard>

Q_DECLARE_METATYPE(ICodeGenerator*)

#include "CppCodeGenerator.h"

GCF_DEFINE_COMPONENT(CodeGeneratorComponent)

struct CodeGeneratorComponentData
{
    CodeGeneratorComponentData() : generatingCode(false) { }

    QList<ICodeGenerator*> codeGenList;
    bool generatingCode;
    QMenu* codeGenMenu;

    CppCodeGenerator cppCodeGenerator;
};

CodeGeneratorComponent & CodeGeneratorComponent::instance()
{
    static CodeGeneratorComponent* theInstance = GCF_CREATE_COMPONENT(CodeGeneratorComponent);
    return *theInstance;
}

CodeGeneratorComponent::CodeGeneratorComponent()
{
    d = new CodeGeneratorComponentData;

    d->codeGenMenu = new QMenu((QWidget*)0);
    connect(d->codeGenMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(codeGeneratorActionTriggered(QAction*)));

    registerCodeGenerator(&d->cppCodeGenerator);
}

CodeGeneratorComponent::~CodeGeneratorComponent()
{
    delete d;
}

QObject* CodeGeneratorComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    CodeGeneratorComponent* that = const_cast<CodeGeneratorComponent*>(this);

    if(comps.last() == "controller")
        return that;

    if(comps.last() == "cppCodeGen")
        return &d->cppCodeGenerator;

    return 0;
}

QWidget* CodeGeneratorComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    return 0;
}

QMenu* CodeGeneratorComponent::fetchMenu(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    if(comps.last() == "codeGenMenu")
        return d->codeGenMenu;

    return GCF::AbstractComponent::fetchMenu(completeName);
}

void CodeGeneratorComponent::initializeComponent()
{
    // Do nothing.
}

void CodeGeneratorComponent::finalizeComponent()
{
    // Do nothing
}

QObject* CodeGeneratorComponent::containerObject()
{
    return this;
}

QString CodeGeneratorComponent::productName() const
{
    return "Code Generator Manager";
}

QString CodeGeneratorComponent::organization() const
{
    return "VCreate Logic Private Limited";
}

QImage CodeGeneratorComponent::programLogo() const
{
    return QImage();
}

QString CodeGeneratorComponent::version() const
{
    return "1.0";
}

QString CodeGeneratorComponent::shortDescription() const
{
    return "Manager for code generators in VTK Designer 2";
}

QString CodeGeneratorComponent::homepage() const
{
    return "http://www.vcreatelogic.com/oss/vtkdesigner";
}

QString CodeGeneratorComponent::bugAddress() const
{
    return "http://www.vcreatelogic.com/oss/vtkdesigner/feedback/bugs";
}

const QList<GCF::AboutPerson> CodeGeneratorComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    if(retList.count() == 0)
    {
        retList << GCF::AboutPerson("Prashanth N Udupa", "prashanth@vcreatelogic.com", "http://www.vcreatelogic.com", "Founder, Vision, Design, Development");
        retList << GCF::AboutPerson("Brian Gee. Chacko", "brian.chacko@vcreatelogic.com", "http://www.vcreatelogic.com", "Implementation of code generators");
    }
    return retList;
}

const QList<GCF::AboutPerson> CodeGeneratorComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString CodeGeneratorComponent::license() const
{
    return "Dual License: GPL Version 2 for FOSS development, Commercial for others.";
}

QString CodeGeneratorComponent::copyrightStatement() const
{
    return QString();
}

void CodeGeneratorComponent::registerCodeGenerator(ICodeGenerator* codeGen)
{
    if(d->codeGenList.contains(codeGen))
        return;

    d->codeGenList.append(codeGen);

    QAction* action = d->codeGenMenu->addAction( codeGen->name() + " [Clipboard]" );
    action->setIcon( codeGen->icon() );
    action->setToolTip( codeGen->description() );
    action->setProperty("ICodeGenerator", qVariantFromValue<ICodeGenerator*>(codeGen));
    action->setProperty("exportType", "Clipboard");
    if(d->codeGenList.count() == 1)
        d->codeGenMenu->setDefaultAction(action); // perhaps I should leave this configurable?

    action = d->codeGenMenu->addAction( codeGen->name() + " [Disk]" );
    action->setIcon( codeGen->icon() );
    action->setToolTip( codeGen->description() );
    action->setProperty("ICodeGenerator", qVariantFromValue<ICodeGenerator*>(codeGen));
    action->setProperty("exportType", "Clipboard");
}

void CodeGeneratorComponent::unregisterCodeGenerator(ICodeGenerator* codeGen)
{
    if(d->generatingCode)
        return;

    int index = d->codeGenList.indexOf(codeGen);
    d->codeGenList.takeAt(index);

    QList<QAction*> actions = d->codeGenMenu->actions();
    delete actions.at(index*2+1);
    delete actions.at(index*2);

    if(actions.count())
        d->codeGenMenu->setDefaultAction(actions.first());
}

int CodeGeneratorComponent::codeGeneratorCount() const
{
    return d->codeGenList.count();
}

ICodeGenerator* CodeGeneratorComponent::codeGenerator(int index) const
{
    if(index < 0 || index >= d->codeGenList.count())
        return 0;

    return d->codeGenList[index];
}

ICodeGenerator* CodeGeneratorComponent::codeGenerator(const QString& name) const
{
    for(int i=0; i<d->codeGenList.count(); i++)
    {
        ICodeGenerator* gen = d->codeGenList[i];
        if(gen->name() == name)
            return gen;
    }

    return 0;
}

void CodeGeneratorComponent::codeGeneratorActionTriggered(QAction* action)
{
    if(!action)
        return;

    // First catch hold of the code generator that we need to use to generate the code
    ICodeGenerator* gen = action->property("ICodeGenerator").value<ICodeGenerator*>();
    if(!gen)
        return;
    QObject* genObj = gen->containerObject();

    // Prepare for export
    // Search for the visualization network
    QString name;
    GCF::AbstractComponent* comp = 0;
    QObject* visNetworkObj = findObject("IVisNetwork", name, &comp);
    IVisNetwork* visNetwork = qobject_cast<IVisNetwork*>(visNetworkObj);
    if(!visNetwork)
    {
        qDebug("%s: Cannot find IVisNetwork", Q_FUNC_INFO);
        return;
    }

    // Create a progress bar for showing updates
    QProgressDialog dialog( mainWindow() );
    connect(genObj, SIGNAL(progressValue(int)), &dialog, SLOT(setValue(int)));
    connect(genObj, SIGNAL(progressMessage(QString)), &dialog, SLOT(setLabelText(QString)));
    connect(&dialog, SIGNAL(cancelled()), genObj, SLOT(cancelCodeGeneration()));
    dialog.setWindowTitle( QString("%1 - Generating Code...").arg(gen->name()) );
    dialog.show();

    // Actually show the progress dialog box
    mainWindow()->setCursor(Qt::WaitCursor);
    qApp->processEvents();

    // Generate the code
    bool success = false;
    QString resultMsg;
    QString exportType = action->property("exportType").toString();

    if( exportType == "Disk")
    {
        // Find out from the user the directory into which the code should be generated
        QString dirName = QFileDialog::getExistingDirectory(mainWindow(),
                                                            "Select a directory into which you want the generated code to be saved",
                                                            qApp->applicationDirPath());
        if(!dirName.isEmpty())
        {
            QDir dir(dirName);
            success = gen->generateCode(dir, visNetwork, qobject_cast<ICodeGeneratorManager*>(this));
        }

        if(success)
            resultMsg = QString("%1: Code generated in %2").arg(gen->name()).arg(dirName);
        else
            resultMsg = QString("%1: Code not generated").arg(gen->name());
    }
    else if( exportType == "Clipboard" )
    {
        QMap<QString,QString> codeMap;
        success = gen->generateCode(codeMap, visNetwork, qobject_cast<ICodeGeneratorManager*>(this));

        if(success)
        {
            QString clipboardText;
            QMap<QString,QString>::const_iterator it = codeMap.constBegin();
            QMap<QString,QString>::const_iterator end = codeMap.constEnd();
            while(it != end)
            {
                clipboardText += "// " + it.key() + "\n\n";
                clipboardText += it.value();
                clipboardText += "////////////\n\n";
                ++it;
            }

            qApp->clipboard()->setText(clipboardText);
        }

        if(success)
            resultMsg = QString("%1: Code generated and copied to clipboard").arg(gen->name());
        else
            resultMsg = QString("%1: Code not generated. Clipboard is left unmodified.").arg(gen->name());
    }

    // All done
    mainWindow()->unsetCursor();

    QMessageBox::information(mainWindow(), "Code Generation Results", resultMsg);
}

#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisSystemNodeFactory.h"
#include "IVisSystemNodeDesc.h"
#include <QMetaObject>
#include <QFileDialog>
#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QMetaProperty>

void CodeGeneratorComponent::dumpUnsupportPropertiesInfo()
{
    // Figure out the file name to save
    QString fileName = QFileDialog::getSaveFileName(
                mainWindow(), "Select file name to dump",
                QDir::homePath(), "*.txt");
    if(fileName.isEmpty())
        return;

    // Open the file for writing
    QFile file(fileName);
    file.open(QFile::WriteOnly);
    QTextStream ts(&file);

    // Get the IVisSystemNodeFactoryRegistry interface
    IVisSystemNodeFactoryRegistry* nodeFactoryReg
            = GCF::findObject<IVisSystemNodeFactoryRegistry*>();
    QStringList classNames = nodeFactoryReg->nodeClassNameList();

    int classCount = 0;
    int propCount = 0;

    // Loop through class names and dump information
    Q_FOREACH(QString className, classNames)
    {
        bool success = false;
        IVisSystemNodeDesc* nodeDesc = nodeFactoryReg->nodeDesc(className);
        if(!nodeDesc)
            continue;

        ts << "\n\nClass: " << nodeDesc->nodeClassName() << "\n";

        const QMetaObject* mo = nodeDesc->qMetaObject();
        if(!mo)
        {
            ts << "    Meta object not found\n";
            continue;
        }

        // Loop through properties in meta object and
        // report info not found in nodeDesc
        for(int i=2; i<mo->propertyCount(); i++)
        {
            QMetaProperty prop = mo->property(i);
            QString propName( prop.name() );

            if(nodeDesc->hasPropertyInfo(propName))
                continue;

            ts << "    Property Info for \"" << propName << "\" of type \""
               << prop.typeName() << "\" not found\n";

            ++propCount;
            success = true;
        }

        if(success)
            ++classCount;
    }

    ts << "\n\nTotal of " << classCount << " classes and " << propCount << " properties\n\n";

    QMessageBox::information(mainWindow(), "Information Dumped",
                             "Information has been dumped into " + fileName);
}

GCF_EXPORT_COMPONENT_PLUGIN(CodeGeneratorComponent)

