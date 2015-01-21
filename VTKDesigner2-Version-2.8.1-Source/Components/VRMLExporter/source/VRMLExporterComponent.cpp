/****************************************************************************
**
** Copyright (C) My Organization
**
** Use of this file is limited according to the terms specified by
** My Organization.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "VRMLExporterComponent.h"
#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>

#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"
#include "IVisSystemNode.h"
#include "vtkRenderWindow.h"
#include "vtkVRMLExporter.h"

#include <QMainWindow>
#include <QMenu>
#include <QDir>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QListWidget>
#include <QDialogButtonBox>

Q_DECLARE_METATYPE(vtkObject*)

GCF_DEFINE_COMPONENT(VRMLExporterComponent)

struct VRMLExporterComponentData
{
    VRMLExporterComponentData() : exportVRMLAction(0),
        contextMenuNode(0) { }

    QAction* exportVRMLAction;
    QList<IVisSystemNode*> renWinNodeList;
    IVisSystemNode* contextMenuNode;
};

VRMLExporterComponent & VRMLExporterComponent::instance()
{
    static VRMLExporterComponent* theInstance = GCF_CREATE_COMPONENT(VRMLExporterComponent);
    return *theInstance;
}

VRMLExporterComponent::VRMLExporterComponent()
{
    d = new VRMLExporterComponentData;
}

VRMLExporterComponent::~VRMLExporterComponent()
{
    delete d;
}

QAction* VRMLExporterComponent::fetchAction(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    if(comps.last() == "exportVRML")
    {
        if(!d->exportVRMLAction)
            d->exportVRMLAction = GCF::AbstractComponent::fetchAction(completeName);

        return d->exportVRMLAction;
    }

    return GCF::AbstractComponent::fetchAction(completeName);
}

QObject* VRMLExporterComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    VRMLExporterComponent* that = const_cast<VRMLExporterComponent*>(this);
    
    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* VRMLExporterComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    return 0;
}

void VRMLExporterComponent::initializeComponent()
{
    // Do nothing.
}

void VRMLExporterComponent::creationAnnounced()
{
    IVisNetwork* visNetwork = GCF::findObject<IVisNetwork*>();
    if(visNetwork)
    {
        connect(visNetwork->containerObject(),
                SIGNAL(nodeAdded(IVisSystemNode*)),
                this,
                SLOT(slotNodeAdded(IVisSystemNode*)));
        connect(visNetwork->containerObject(),
                SIGNAL(nodeRemoved(IVisSystemNode*)),
                this,
                SLOT(slotNodeRemoved(IVisSystemNode*)));
    }

    IVisNetworkCanvas* visCanvas = GCF::findObject<IVisNetworkCanvas*>();
    if(visCanvas)
    {
        connect(visCanvas->containerObject(),
            SIGNAL(nodeContextMenuEvent(IVisSystemNode*,QMenu*,Qt::KeyboardModifiers)),
            this,
            SLOT(slotNodeContextMenuEvent(IVisSystemNode*,QMenu*)));
    }

    if(d->exportVRMLAction)
        d->exportVRMLAction->setEnabled(false);
}

void VRMLExporterComponent::finalizeComponent()
{
    // Do nothing
}

QObject* VRMLExporterComponent::containerObject()
{
    return this;
}

QString VRMLExporterComponent::productName() const
{
    return "VRMLExporter";
}

QString VRMLExporterComponent::organization() const
{
    return "My Organization";
}

QImage VRMLExporterComponent::programLogo() const
{
    return QImage();
}

QString VRMLExporterComponent::version() const
{
    return "1.0";
}

QString VRMLExporterComponent::shortDescription() const
{
    return "Provides VRMLExporter";
}

QString VRMLExporterComponent::homepage() const
{
    return "My Organization website";
}

QString VRMLExporterComponent::bugAddress() const
{
    return "My Organization website";
}

const QList<GCF::AboutPerson> VRMLExporterComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> VRMLExporterComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString VRMLExporterComponent::license() const
{
    return "My Organization {{AppName}} License";
}

QString VRMLExporterComponent::copyrightStatement() const
{
    return QString();
}

void VRMLExporterComponent::slotNodeAdded(IVisSystemNode* node)
{
    if( node->nodeData().hasKey("vtkObject") )
    {
        vtkObject* vtkObjPtr = node->nodeData()["vtkObject"].value<vtkObject*>();
        vtkRenderWindow* renWin = vtkRenderWindow::SafeDownCast(vtkObjPtr);
        if(renWin)
            d->renWinNodeList.append(node);
    }

    if(d->exportVRMLAction)
        d->exportVRMLAction->setEnabled(d->renWinNodeList.count() != 0);
}

void VRMLExporterComponent::slotNodeRemoved(IVisSystemNode* node)
{
    if(d->renWinNodeList.contains(node))
        d->renWinNodeList.removeAll(node);

    if(d->contextMenuNode == node)
        d->contextMenuNode = 0;

    if(d->exportVRMLAction)
        d->exportVRMLAction->setEnabled(d->renWinNodeList.count() != 0);
}

void VRMLExporterComponent::slotNodeContextMenuEvent(IVisSystemNode* node, QMenu* menu)
{
    if(!d->renWinNodeList.contains(node))
        return;

    d->contextMenuNode = node;

    // Show the "Export to VRML" option in the context menu
    QAction* cmAction = menu->addAction("Export to VRML");

    // Connect the added action to an internal slot
    connect(cmAction, SIGNAL(triggered(bool)),
            this, SLOT(exportVRML2()));
}

void VRMLExporterComponent::exportVRML()
{
    if(!d->renWinNodeList.count())
        return;

    IVisSystemNode* selectedNode = 0;

    if(d->renWinNodeList.count() >= 2)
    {
        QDialog dialog( mainWindow() );

        QLabel* label = new QLabel(&dialog);
        label->setText("Select a render window to export");

        QListWidget* listWidget = new QListWidget(&dialog);
        Q_FOREACH(IVisSystemNode* node, d->renWinNodeList)
            listWidget->addItem( node->nodeName() );

        QDialogButtonBox* bbox = new QDialogButtonBox(&dialog);
        bbox->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
        connect(bbox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        connect(bbox, SIGNAL(rejected()), &dialog, SLOT(reject()));

        QVBoxLayout* layout = new QVBoxLayout(&dialog);
        layout->addWidget(label);
        layout->addWidget(listWidget);
        layout->addWidget(bbox);

        if( dialog.exec() == QDialog::Rejected )
            return;

        int selIndex = listWidget->currentRow();
        if(selIndex < 0)
            return;

        selectedNode = d->renWinNodeList.at(selIndex);
    }
    else
        selectedNode = d->renWinNodeList.first();

    if(selectedNode)
    {
        vtkObject* vtkObjPtr = selectedNode->nodeData()["vtkObject"].value<vtkObject*>();
        vtkRenderWindow* renWin = vtkRenderWindow::SafeDownCast(vtkObjPtr);
        exportVRML(renWin);
    }
}

void VRMLExporterComponent::exportVRML2()
{
    if(!d->contextMenuNode)   
        return;

    // User clicked on the "Export to VRML" option in the
    // context menu. Lets extract the vtkRenderWindow in the 
    // node
    vtkObject* vtkObjPtr = 
        d->contextMenuNode->nodeData()["vtkObject"].value<vtkObject*>();
    vtkRenderWindow* renWin = vtkRenderWindow::SafeDownCast(vtkObjPtr);

    // And export the render window
    exportVRML(renWin);
}

void VRMLExporterComponent::exportVRML(vtkRenderWindow* renWin)
{
    if(!renWin)
        return;

    QString fileName = QFileDialog::getSaveFileName(mainWindow(), 
                            "Choose VRML file name to save",
                            QDir::homePath(),
                            "VRML File (*.vrml)");
    if(fileName.isEmpty())
        return;

    {
        GCF::HourGlass hourGlass;
        vtkVRMLExporter* exporter = vtkVRMLExporter::New();
        exporter->SetInput( renWin );
        exporter->SetFileName( qPrintable(fileName) );
        exporter->Write();
        exporter->Delete();
    }

    QMessageBox::information(mainWindow(), "VRML Export Successful",
        QString("Successfully exported scene to \"%1\"").arg(fileName));
}

GCF_EXPORT_COMPONENT_PLUGIN(VRMLExporterComponent)

