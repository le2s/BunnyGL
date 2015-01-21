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

#include "ImagingAlgorithmsComponent.h"
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

#include "CVtkImageAlgorithm.h"
#include "CVtkThreadedImageAlgorithm.h"
#include "CVtkImageThreshold.h"
#include "CVtkImageGaussianSmooth.h"
#include "CVtkImageGradient.h"
#include "CVtkImageGradientMagnitude.h"
#include "CVtkImageExtractComponents.h"
#include "CVtkImageMedian3D.h"

struct ImagingAlgorithmsComponentData
{
    ImagingAlgorithmsComponentData() {
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

GCF_DEFINE_COMPONENT(ImagingAlgorithmsComponent)

ImagingAlgorithmsComponent & ImagingAlgorithmsComponent::instance()
{
    static ImagingAlgorithmsComponent* theInstance = GCF_CREATE_COMPONENT(ImagingAlgorithmsComponent);
    return *theInstance;
}

ImagingAlgorithmsComponent::ImagingAlgorithmsComponent()
{
    d = new ImagingAlgorithmsComponentData;
}

ImagingAlgorithmsComponent::~ImagingAlgorithmsComponent()
{
    delete d;
}

QIcon ImagingAlgorithmsComponent::nodeIcon()
{
    return qApp->windowIcon();
}

QObject* ImagingAlgorithmsComponent::containerObject()
{
    return this;
}

QStringList ImagingAlgorithmsComponent::nodeClassNameList()
{
    return d->nodeDescMap.keys();
}

IVisSystemNodeDesc* ImagingAlgorithmsComponent::nodeDesc(QString nodeClassName)
{
    if(d->nodeDescMap.contains(nodeClassName))
        return d->nodeDescMap[nodeClassName];

    return 0;
}

bool ImagingAlgorithmsComponent::canCreate(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(desc2)
        return desc2->createFunction();

    return 0;
}

IVisSystemNode* ImagingAlgorithmsComponent::createNode(QString nodeClassName)
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

void ImagingAlgorithmsComponent::deleteNode(IVisSystemNode* node)
{
    node->finalizeNode();
}

QString ImagingAlgorithmsComponent::productName() const
{
    return "ImagingAlgorithms Classes";
}

QString ImagingAlgorithmsComponent::organization() const
{
    return "VCreate Logic Pvt Ltd";
}

QImage ImagingAlgorithmsComponent::programLogo() const
{
    return QImage();
}

QString ImagingAlgorithmsComponent::version() const
{
    return "1.0";
}

QString ImagingAlgorithmsComponent::shortDescription() const
{
    return "Provides the ImagingAlgorithms component";
}

QString ImagingAlgorithmsComponent::homepage() const
{
    return "ImagingAlgorithms Website";
}

QString ImagingAlgorithmsComponent::bugAddress() const
{
    return "ImagingAlgorithms Website";
}

const QList<GCF::AboutPerson> ImagingAlgorithmsComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> ImagingAlgorithmsComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString ImagingAlgorithmsComponent::license() const
{
    return "ImagingAlgorithms License";
}

QString ImagingAlgorithmsComponent::copyrightStatement() const
{
    return "Copyright (c) VCreate Logic Pvt Ltd All rights reserved";
}

void ImagingAlgorithmsComponent::initializeComponent()
{
    REGISTER_NODE(CVtkImageAlgorithm, d->nodeDescMap);
    REGISTER_NODE(CVtkThreadedImageAlgorithm, d->nodeDescMap);
    REGISTER_NODE(CVtkImageThreshold, d->nodeDescMap);
    REGISTER_NODE(CVtkImageGaussianSmooth, d->nodeDescMap);
    REGISTER_NODE(CVtkImageGradient, d->nodeDescMap);
    REGISTER_NODE(CVtkImageGradientMagnitude, d->nodeDescMap);
    REGISTER_NODE(CVtkImageExtractComponents, d->nodeDescMap);
    REGISTER_NODE(CVtkImageMedian3D, d->nodeDescMap);
}

void ImagingAlgorithmsComponent::finalizeComponent()
{
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void ImagingAlgorithmsComponent::creationAnnounced()
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

void ImagingAlgorithmsComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
    {
        d->nodeFactoryRegistry = 0;
        d->nodeFactoryRegistryComp = 0;
    }
}

QObject* ImagingAlgorithmsComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    ImagingAlgorithmsComponent* that = const_cast<ImagingAlgorithmsComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* ImagingAlgorithmsComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(ImagingAlgorithmsComponent)


