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

#include "MPRViewerComponent.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisSystemNodeDesc.h"
#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"
#include "IVisNetworkConnection.h"
#include "IScriptEngineManager.h"

#include <GCF/IComponentPlugin>
#include <GCF/ComponentFactory>
#include <QApplication>
#include <QMenu>

#include "CGenericVisNodeDesc.h"
#include "CGenericVisNodeBase.h"

#include "MPRViewerVisNode.h"
#include "MPRViewerWidget.h"

struct MPRViewerComponentData
{
    MPRViewerComponentData() {
        nodeFactoryRegistryComp = 0;
        nodeFactoryRegistry = 0;
        visNetworkCanvas = 0;
        visNetwork = 0;
        scriptEngineManager = 0;
    }

    QMap<QString, CGenericVisNodeDesc*> nodeDescMap;
    GCF::AbstractComponent* nodeFactoryRegistryComp;
    IVisSystemNodeFactoryRegistry* nodeFactoryRegistry;
    IVisNetworkCanvas* visNetworkCanvas;
    IVisNetwork* visNetwork;
    IScriptEngineManager* scriptEngineManager;
};

GCF_DEFINE_COMPONENT(MPRViewerComponent)

MPRViewerComponent & MPRViewerComponent::instance()
{
    static MPRViewerComponent* theInstance = GCF_CREATE_COMPONENT(MPRViewerComponent);
    return *theInstance;
}

MPRViewerComponent::MPRViewerComponent()
{
    d = new MPRViewerComponentData;
}

MPRViewerComponent::~MPRViewerComponent()
{
    delete d;
}

QIcon MPRViewerComponent::nodeIcon()
{
    return qApp->windowIcon();
}

QObject* MPRViewerComponent::containerObject()
{
    return this;
}

QStringList MPRViewerComponent::nodeClassNameList()
{
    return d->nodeDescMap.keys();
}

IVisSystemNodeDesc* MPRViewerComponent::nodeDesc(QString nodeClassName)
{
    if(d->nodeDescMap.contains(nodeClassName))
        return d->nodeDescMap[nodeClassName];

    return 0;
}

bool MPRViewerComponent::canCreate(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(desc2)
        return desc2->createFunction();

    return 0;
}

IVisSystemNode* MPRViewerComponent::createNode(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(!desc2)
        return false;

    IVisSystemNode* node = desc2->createNode();
    if(node)
    {
        node->initializeNode();
        CGenericVisNodeBase* nodeBase = qobject_cast<CGenericVisNodeBase*>(node->containerObject());
        if(nodeBase)
            nodeBase->setNodeFactory(this);
    }

    return node;
}

void MPRViewerComponent::deleteNode(IVisSystemNode* node)
{
    node->finalizeNode();
}

QString MPRViewerComponent::productName() const
{
    return "MPRViewer Classes";
}

QString MPRViewerComponent::organization() const
{
    return "VCreate Logic Pvt Ltd";
}

QImage MPRViewerComponent::programLogo() const
{
    return QImage();
}

QString MPRViewerComponent::version() const
{
    return "1.0";
}

QString MPRViewerComponent::shortDescription() const
{
    return "Provides the MPRViewer component";
}

QString MPRViewerComponent::homepage() const
{
    return "MPRViewer Website";
}

QString MPRViewerComponent::bugAddress() const
{
    return "MPRViewer Website";
}

const QList<GCF::AboutPerson> MPRViewerComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> MPRViewerComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString MPRViewerComponent::license() const
{
    return "MPRViewer License";
}

QString MPRViewerComponent::copyrightStatement() const
{
    return "Copyright (c) VCreate Logic Pvt Ltd All rights reserved";
}

void MPRViewerComponent::initializeComponent()
{
    REGISTER_NODE(MPRViewerVisNode, d->nodeDescMap);
}

void MPRViewerComponent::finalizeComponent()
{
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void MPRViewerComponent::creationAnnounced()
{
    QString completeName;
    GCF::AbstractComponent* compPtr = 0;
    QObject* objectPtr = findObject("IVisSystemNodeFactoryRegistry", completeName, &compPtr);
    if(objectPtr)
    {
        IVisSystemNodeFactoryRegistry* nodeFacReg = qobject_cast<IVisSystemNodeFactoryRegistry*>(objectPtr);
        nodeFacReg->registerNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
        d->nodeFactoryRegistry = nodeFacReg;
        d->nodeFactoryRegistryComp = compPtr;
        GCF::AbstractComponent::addDependency(this, compPtr);
    }

    objectPtr = findObject("IVisNetwork", completeName, &compPtr);
    if(objectPtr)
    {
        d->visNetwork = qobject_cast<IVisNetwork*>(objectPtr);
        // This should be done to ensure that this component is finalized
        // after the network is destroyed.
        GCF::AbstractComponent::addDependency(this, compPtr);
    }

    objectPtr = findObject("IVisNetworkCanvas", completeName, &compPtr);
    if(objectPtr)
    {
        d->visNetworkCanvas = qobject_cast<IVisNetworkCanvas*>(objectPtr);
        // This should be done to ensure that this component is finalized
        // after the network is destroyed.
        GCF::AbstractComponent::addDependency(this, compPtr);
    }

    objectPtr = findObject("IScriptEngineManager", completeName, &compPtr);
    if(objectPtr)
    {
        d->scriptEngineManager = qobject_cast<IScriptEngineManager*>(objectPtr);
        // This should be done to ensure that this component is finalized
        // after the network is destroyed.
        GCF::AbstractComponent::addDependency(this, compPtr);
    }

    if(d->visNetworkCanvas)
    {
        QObject::connect(d->visNetworkCanvas->containerObject(),
                         SIGNAL(nodeContextMenuEvent(IVisSystemNode*,QMenu*,Qt::KeyboardModifiers)),
                         this, 
                         SLOT(slotNodeContextMenuEvent(IVisSystemNode*,QMenu*,Qt::KeyboardModifiers)));
    }
}

void MPRViewerComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
    {
        d->nodeFactoryRegistry = 0;
        d->nodeFactoryRegistryComp = 0;
    }
}

QObject* MPRViewerComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    MPRViewerComponent* that = const_cast<MPRViewerComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* MPRViewerComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    return 0;
}

void MPRViewerComponent::slotNodeContextMenuEvent(IVisSystemNode* node, QMenu* menu, Qt::KeyboardModifiers modifiers)
{
    MPRViewerVisNode* viewerNode = qobject_cast<MPRViewerVisNode*>(node->containerObject());
    if(!viewerNode)
        return;

    menu->addSeparator();
    QAction* showViewerAction = menu->addAction("Show MPR Viewer");
    connect(showViewerAction, SIGNAL(triggered()), viewerNode, SLOT(showMPRViewer()));
}

GCF_EXPORT_COMPONENT_PLUGIN(MPRViewerComponent)


