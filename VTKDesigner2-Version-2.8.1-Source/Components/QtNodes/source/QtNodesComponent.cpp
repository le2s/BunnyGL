/****************************************************************************
**
** Copyright (C) VCreate Logic Pvt Ltd
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Pvt Ltd.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "QtNodesComponent.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisSystemNodeDesc.h"
#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"
#include "IVisNetworkConnection.h"
#include "IScriptEngineManager.h"

#include <GCF/IComponentPlugin>
#include <GCF/ComponentFactory>
#include <QApplication>

#include "CGenericVisNodeDesc.h"
#include "CGenericVisNodeBase.h"

#include "QImageToVtkImageConverterVisNode.h"
#include "QWebPageToQImageConverterVisNode.h"
#include "QImageLoaderVisNode.h"

struct QtNodesComponentData
{
    QtNodesComponentData() {
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

GCF_DEFINE_COMPONENT(QtNodesComponent)

QtNodesComponent & QtNodesComponent::instance()
{
    static QtNodesComponent* theInstance = GCF_CREATE_COMPONENT(QtNodesComponent);
    return *theInstance;
}

QtNodesComponent::QtNodesComponent()
{
    d = new QtNodesComponentData;
}

QtNodesComponent::~QtNodesComponent()
{
    delete d;
}

QIcon QtNodesComponent::nodeIcon()
{
    return qApp->windowIcon();
}

QObject* QtNodesComponent::containerObject()
{
    return this;
}

QStringList QtNodesComponent::nodeClassNameList()
{
    return d->nodeDescMap.keys();
}

IVisSystemNodeDesc* QtNodesComponent::nodeDesc(QString nodeClassName)
{
    if(d->nodeDescMap.contains(nodeClassName))
        return d->nodeDescMap[nodeClassName];

    return 0;
}

bool QtNodesComponent::canCreate(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(desc2)
        return desc2->createFunction();

    return 0;
}

IVisSystemNode* QtNodesComponent::createNode(QString nodeClassName)
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

void QtNodesComponent::deleteNode(IVisSystemNode* node)
{
    node->finalizeNode();
}

QString QtNodesComponent::productName() const
{
    return "QtNodes Classes";
}

QString QtNodesComponent::organization() const
{
    return "VCreate Logic Pvt Ltd";
}

QImage QtNodesComponent::programLogo() const
{
    return QImage();
}

QString QtNodesComponent::version() const
{
    return "1.0";
}

QString QtNodesComponent::shortDescription() const
{
    return "Provides the QtNodes component";
}

QString QtNodesComponent::homepage() const
{
    return "QtNodes Website";
}

QString QtNodesComponent::bugAddress() const
{
    return "QtNodes Website";
}

const QList<GCF::AboutPerson> QtNodesComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> QtNodesComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString QtNodesComponent::license() const
{
    return "QtNodes License";
}

QString QtNodesComponent::copyrightStatement() const
{
    return "Copyright (c) VCreate Logic Pvt Ltd All rights reserved";
}

void QtNodesComponent::initializeComponent()
{
    REGISTER_NODE(QImageToVtkImageConverterVisNode, d->nodeDescMap);
    REGISTER_NODE(QWebPageToQImageConverterVisNode, d->nodeDescMap);
    REGISTER_NODE(QImageLoaderVisNode, d->nodeDescMap);
}

void QtNodesComponent::finalizeComponent()
{
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void QtNodesComponent::creationAnnounced()
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
}

void QtNodesComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
    {
        d->nodeFactoryRegistry = 0;
        d->nodeFactoryRegistryComp = 0;
    }
}

QObject* QtNodesComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    QtNodesComponent* that = const_cast<QtNodesComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* QtNodesComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(QtNodesComponent)


