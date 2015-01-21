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

#include "FractalAlgorithmsComponent.h"
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

#include "CVtkSierpinskiTriangleSource.h"
#include "CVtkSierpinskiPyramidSource.h"
#include "CVtkMengerSquareSource.h"
#include "CVtkFractalTerrainSource.h"
#include "CVtkMengerSpongeSource.h"

struct FractalAlgorithmsComponentData
{
    FractalAlgorithmsComponentData() {
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

GCF_DEFINE_COMPONENT(FractalAlgorithmsComponent)

FractalAlgorithmsComponent & FractalAlgorithmsComponent::instance()
{
    static FractalAlgorithmsComponent* theInstance = GCF_CREATE_COMPONENT(FractalAlgorithmsComponent);
    return *theInstance;
}

FractalAlgorithmsComponent::FractalAlgorithmsComponent()
{
    d = new FractalAlgorithmsComponentData;
}

FractalAlgorithmsComponent::~FractalAlgorithmsComponent()
{
    delete d;
}

QIcon FractalAlgorithmsComponent::nodeIcon()
{
    return qApp->windowIcon();
}

QObject* FractalAlgorithmsComponent::containerObject()
{
    return this;
}

QStringList FractalAlgorithmsComponent::nodeClassNameList()
{
    return d->nodeDescMap.keys();
}

IVisSystemNodeDesc* FractalAlgorithmsComponent::nodeDesc(QString nodeClassName)
{
    if(d->nodeDescMap.contains(nodeClassName))
        return d->nodeDescMap[nodeClassName];

    return 0;
}

bool FractalAlgorithmsComponent::canCreate(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(desc2)
        return desc2->createFunction();

    return 0;
}

IVisSystemNode* FractalAlgorithmsComponent::createNode(QString nodeClassName)
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

void FractalAlgorithmsComponent::deleteNode(IVisSystemNode* node)
{
    node->finalizeNode();
}

QString FractalAlgorithmsComponent::productName() const
{
    return "FractalAlgorithms Classes";
}

QString FractalAlgorithmsComponent::organization() const
{
    return "VCreate Logic Pvt Ltd";
}

QImage FractalAlgorithmsComponent::programLogo() const
{
    return QImage();
}

QString FractalAlgorithmsComponent::version() const
{
    return "1.0";
}

QString FractalAlgorithmsComponent::shortDescription() const
{
    return "Provides the FractalAlgorithms component";
}

QString FractalAlgorithmsComponent::homepage() const
{
    return "FractalAlgorithms Website";
}

QString FractalAlgorithmsComponent::bugAddress() const
{
    return "FractalAlgorithms Website";
}

const QList<GCF::AboutPerson> FractalAlgorithmsComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> FractalAlgorithmsComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString FractalAlgorithmsComponent::license() const
{
    return "FractalAlgorithms License";
}

QString FractalAlgorithmsComponent::copyrightStatement() const
{
    return "Copyright (c) VCreate Logic Pvt Ltd All rights reserved";
}

void FractalAlgorithmsComponent::initializeComponent()
{
    REGISTER_NODE(CVtkSierpinskiTriangleSource, d->nodeDescMap);
    REGISTER_NODE(CVtkSierpinskiPyramidSource, d->nodeDescMap);
    REGISTER_NODE(CVtkMengerSquareSource, d->nodeDescMap);
    REGISTER_NODE(CVtkFractalTerrainSource, d->nodeDescMap);
    REGISTER_NODE(CVtkMengerSpongeSource, d->nodeDescMap);
}

void FractalAlgorithmsComponent::finalizeComponent()
{
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void FractalAlgorithmsComponent::creationAnnounced()
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

void FractalAlgorithmsComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
    {
        d->nodeFactoryRegistry = 0;
        d->nodeFactoryRegistryComp = 0;
    }
}

QObject* FractalAlgorithmsComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    FractalAlgorithmsComponent* that = const_cast<FractalAlgorithmsComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* FractalAlgorithmsComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(FractalAlgorithmsComponent)


