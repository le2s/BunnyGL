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

#include "CVTKFullscreenComponent.h"
#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>
#include <QTextEdit>
#include <QApplication>
#include <QScrollBar>
#include <QMainWindow>
#include <QPointer>

#include "CFullscreenRenderWindow.h"

#include "IVisSystemNode.h"
#include "IVisNetwork.h"
#include <QTabWidget>
#include "QVTKWidget.h"
#include "vtkRenderWindow.h"

Q_DECLARE_METATYPE(vtkObject*)

GCF_DEFINE_COMPONENT(CVTKFullscreenComponent)

struct CVTKFullscreenComponentData
{
    CVTKFullscreenComponentData() : vtkOutputDisplay(0), visNetwork(0) { }

    QTabWidget* vtkOutputDisplay;
    IVisNetwork* visNetwork;
    QMap<IVisSystemNode*, vtkRenderWindow*> nodeRenderWindowMap;

    QPointer<CFullscreenRenderWindow> fsRenderWindow;
};

CVTKFullscreenComponent & CVTKFullscreenComponent::instance()
{
    static CVTKFullscreenComponent* theInstance = GCF_CREATE_COMPONENT(CVTKFullscreenComponent);
    return *theInstance;
}

CVTKFullscreenComponent::CVTKFullscreenComponent()
{
    d = new CVTKFullscreenComponentData;
}

CVTKFullscreenComponent::~CVTKFullscreenComponent()
{
    delete d;
}

void CVTKFullscreenComponent::showCurrentRenderWindowFullScreen()
{
    if( !d->fsRenderWindow.isNull() )
        return;

    if( !d->nodeRenderWindowMap.count() )
        return;

    // Find out the name of the active tab
    QString activeWin;
    if( d->vtkOutputDisplay )
        activeWin = d->vtkOutputDisplay->tabText( d->vtkOutputDisplay->currentIndex() );

    d->fsRenderWindow = new CFullscreenRenderWindow;
    QList<IVisSystemNode*> nodes = d->nodeRenderWindowMap.keys();
    vtkRenderWindow* activeWinPtr = 0;
    for(int i=0; i<nodes.count(); i++)
    {
        IVisSystemNode* node = nodes[i];
        vtkRenderWindow* renWin = d->nodeRenderWindowMap[node];
        d->fsRenderWindow->toolbar()->addRenderWindow(renWin, node->nodeName());

        if( node->nodeName() == activeWin )
            activeWinPtr = renWin;
    }

    if(!activeWinPtr)
        activeWinPtr = d->nodeRenderWindowMap[nodes.first()];
    d->fsRenderWindow->setRenderWindow( activeWinPtr );

    d->fsRenderWindow->show();
}

QObject* CVTKFullscreenComponent::fetchObject(const QString& completeName) const
{
    CVTKFullscreenComponent* that = const_cast<CVTKFullscreenComponent*>(this);
    
    if(completeName.split('.').last() == "controller")
        return that;

    return 0;
}

void CVTKFullscreenComponent::initializeComponent()
{
    // Do nothing.
}

void CVTKFullscreenComponent::finalizeComponent()
{
    // Do nothing
}

void CVTKFullscreenComponent::creationAnnounced()
{
    // Find out the tab widget that contains all the vtk render windows.
    QWidget* widget = findWidget("CVtkVisSystemComponent.vtkOutputDisplay");
    d->vtkOutputDisplay = widget ? qobject_cast<QTabWidget*>(widget) : 0;
    if( d->vtkOutputDisplay )
        connect(mainWindow(), SIGNAL(workspaceWidgetActivated(QWidget*)),
                this, SLOT(slotWorkspaceWidgetActivated(QWidget*)));

    // Find out the IVisNetwork component
    QString name; AbstractComponent* compPtr = 0;
    QObject* obj = findObject("IVisNetwork", name, &compPtr);
    if( obj )
    {
        d->visNetwork = qobject_cast<IVisNetwork*>(obj);
        connect(obj, SIGNAL(nodeAdded(IVisSystemNode*)),
                this, SLOT(slotNodeAdded(IVisSystemNode*)));
        connect(obj, SIGNAL(nodeRemoved(IVisSystemNode*)),
                this, SLOT(slotNodeRemoved(IVisSystemNode*)));
        AbstractComponent::addDependency(this, compPtr);
    }

    setActive(false);
}

void CVTKFullscreenComponent::slotNodeAdded(IVisSystemNode* node)
{
    if( !node )
        return;

    if( !node->nodeData().hasKey("vtkObject") )
        return;

    vtkObject* object = node->nodeData()["vtkObject"].value<vtkObject*>();
    if( !object )
        return;

    vtkRenderWindow* rWin = vtkRenderWindow::SafeDownCast( object );
    if( !rWin )
        return;

    d->nodeRenderWindowMap[node] = rWin;
}

void CVTKFullscreenComponent::slotNodeRemoved(IVisSystemNode* node)
{
    if( !node )
        return;

    if( d->nodeRenderWindowMap.contains(node) )
        d->nodeRenderWindowMap.remove( node );
}

void CVTKFullscreenComponent::slotWorkspaceWidgetActivated(QWidget* widget)
{
    setActive( widget == d->vtkOutputDisplay );
}

GCF_EXPORT_COMPONENT_PLUGIN(CVTKFullscreenComponent)

