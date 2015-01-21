/****************************************************************************
**
** Copyright (C) Adithya
**
** Use of this file is limited according to the terms specified by
** Adithya.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CustomAlgorithmsComponent.h"
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

#include "CVtkRectangleSource.h"
#include "CVtkCircleSource.h"

struct CustomAlgorithmsComponentData
{
    CustomAlgorithmsComponentData() {
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

GCF_DEFINE_COMPONENT(CustomAlgorithmsComponent)

CustomAlgorithmsComponent & CustomAlgorithmsComponent::instance()
{
    static CustomAlgorithmsComponent* theInstance = GCF_CREATE_COMPONENT(CustomAlgorithmsComponent);
    return *theInstance;
}

CustomAlgorithmsComponent::CustomAlgorithmsComponent()
{
    d = new CustomAlgorithmsComponentData;
}

CustomAlgorithmsComponent::~CustomAlgorithmsComponent()
{
    delete d;
}

QIcon CustomAlgorithmsComponent::nodeIcon()
{
    return qApp->windowIcon();
}

QObject* CustomAlgorithmsComponent::containerObject()
{
    return this;
}

QStringList CustomAlgorithmsComponent::nodeClassNameList()
{
    return d->nodeDescMap.keys();
}

IVisSystemNodeDesc* CustomAlgorithmsComponent::nodeDesc(QString nodeClassName)
{
    if(d->nodeDescMap.contains(nodeClassName))
        return d->nodeDescMap[nodeClassName];

    return 0;
}

bool CustomAlgorithmsComponent::canCreate(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(desc2)
        return desc2->createFunction();

    return 0;
}

IVisSystemNode* CustomAlgorithmsComponent::createNode(QString nodeClassName)
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

void CustomAlgorithmsComponent::deleteNode(IVisSystemNode* node)
{
    node->finalizeNode();
}

QString CustomAlgorithmsComponent::productName() const
{
    return "Algorithms from Adithya Udupa";
}

QString CustomAlgorithmsComponent::organization() const
{
    return "Adithya";
}

QImage CustomAlgorithmsComponent::programLogo() const
{
    return QImage();
}

QString CustomAlgorithmsComponent::version() const
{
    return "1.0";
}

QString CustomAlgorithmsComponent::shortDescription() const
{
    return "Provides the custom algorithms written by Adithya";
}

QString CustomAlgorithmsComponent::homepage() const
{
    return "http://www.adithya.info";
}

QString CustomAlgorithmsComponent::bugAddress() const
{
    return "CustomAlgorithms Website";
}

const QList<GCF::AboutPerson> CustomAlgorithmsComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> CustomAlgorithmsComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString CustomAlgorithmsComponent::license() const
{
    return "CustomAlgorithms License";
}

QString CustomAlgorithmsComponent::copyrightStatement() const
{
    return "Copyright (c) Adithya All rights reserved";
}

void CustomAlgorithmsComponent::initializeComponent()
{
    REGISTER_NODE(CVtkRectangleSource, d->nodeDescMap);
    REGISTER_NODE(CVtkCircleSource, d->nodeDescMap);
}

void CustomAlgorithmsComponent::finalizeComponent()
{
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void CustomAlgorithmsComponent::creationAnnounced()
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

void CustomAlgorithmsComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
    {
        d->nodeFactoryRegistry = 0;
        d->nodeFactoryRegistryComp = 0;
    }
}

QObject* CustomAlgorithmsComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    CustomAlgorithmsComponent* that = const_cast<CustomAlgorithmsComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* CustomAlgorithmsComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(CustomAlgorithmsComponent)


