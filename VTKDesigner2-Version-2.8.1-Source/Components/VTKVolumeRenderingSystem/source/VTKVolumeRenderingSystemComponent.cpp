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

#include "VTKVolumeRenderingSystemComponent.h"
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

#include "CVtkAbstractVolumeMapper.h"
#include "CVtkUnstructuredGridVolumeMapper.h"
#include "CVtkProjectedTetrahedraMapper.h"
#include "CVtkUnstructuredGridVolumeRayCastMapper.h"
#include "CVtkUnstructuredGridVolumeZSweepMapper.h"
#include "CVtkVolumeMapper.h"
#include "CVtkFixedPointVolumeRayCastMapper.h"
#include "CVtkVolumeRayCastMapper.h"
#include "CVtkVolumeTextureMapper3D.h"
#include "CVtkEncodedGradientEstimator.h"
#include "CVtkVolumeRayCastFunction.h"
#include "CVtkVolume.h"
#include "CVtkVolumeProperty.h"
#include "CVtkCellCenterDepthSort.h"
#include "CVtkFiniteDifferenceGradientEstimator.h"
#include "CVtkUnstructuredGridBunykRayCastFunction.h"
#include "CVtkUnstructuredGridHomogeneousRayIntegrator.h"
#include "CVtkUnstructuredGridLinearRayIntegrator.h"
#include "CVtkUnstructuredGridPartialPreIntegration.h"
#include "CVtkUnstructuredGridPreIntegration.h"
#include "CVtkUnstructuredGridVolumeRayCastFunction.h"
#include "CVtkUnstructuredGridVolumeRayIntegrator.h"
#include "CVtkVisibilitySort.h"
#include "CVtkVolumeRayCastCompositeFunction.h"
#include "CVtkVolumeRayCastIsosurfaceFunction.h"
#include "CVtkVolumeRayCastMIPFunction.h"

struct VTKVolumeRenderingSystemComponentData
{
    VTKVolumeRenderingSystemComponentData() {
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

GCF_DEFINE_COMPONENT(VTKVolumeRenderingSystemComponent)

VTKVolumeRenderingSystemComponent & VTKVolumeRenderingSystemComponent::instance()
{
    static VTKVolumeRenderingSystemComponent* theInstance = GCF_CREATE_COMPONENT(VTKVolumeRenderingSystemComponent);
    return *theInstance;
}

VTKVolumeRenderingSystemComponent::VTKVolumeRenderingSystemComponent()
{
    d = new VTKVolumeRenderingSystemComponentData;
}

VTKVolumeRenderingSystemComponent::~VTKVolumeRenderingSystemComponent()
{
    delete d;
}

QIcon VTKVolumeRenderingSystemComponent::nodeIcon()
{
    return qApp->windowIcon();
}

QObject* VTKVolumeRenderingSystemComponent::containerObject()
{
    return this;
}

QStringList VTKVolumeRenderingSystemComponent::nodeClassNameList()
{
    return d->nodeDescMap.keys();
}

IVisSystemNodeDesc* VTKVolumeRenderingSystemComponent::nodeDesc(QString nodeClassName)
{
    if(d->nodeDescMap.contains(nodeClassName))
        return d->nodeDescMap[nodeClassName];

    return 0;
}

bool VTKVolumeRenderingSystemComponent::canCreate(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(desc2)
        return desc2->createFunction();

    return 0;
}

IVisSystemNode* VTKVolumeRenderingSystemComponent::createNode(QString nodeClassName)
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

void VTKVolumeRenderingSystemComponent::deleteNode(IVisSystemNode* node)
{
    node->finalizeNode();
}

QString VTKVolumeRenderingSystemComponent::productName() const
{
    return "VTKVolumeRenderingSystem Classes";
}

QString VTKVolumeRenderingSystemComponent::organization() const
{
    return "VCreate Logic Pvt Ltd";
}

QImage VTKVolumeRenderingSystemComponent::programLogo() const
{
    return QImage();
}

QString VTKVolumeRenderingSystemComponent::version() const
{
    return "1.0";
}

QString VTKVolumeRenderingSystemComponent::shortDescription() const
{
    return "Provides the VTKVolumeRenderingSystem component";
}

QString VTKVolumeRenderingSystemComponent::homepage() const
{
    return "VTKVolumeRenderingSystem Website";
}

QString VTKVolumeRenderingSystemComponent::bugAddress() const
{
    return "VTKVolumeRenderingSystem Website";
}

const QList<GCF::AboutPerson> VTKVolumeRenderingSystemComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> VTKVolumeRenderingSystemComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString VTKVolumeRenderingSystemComponent::license() const
{
    return "VTKVolumeRenderingSystem License";
}

QString VTKVolumeRenderingSystemComponent::copyrightStatement() const
{
    return "Copyright (c) VCreate Logic Pvt Ltd All rights reserved";
}

void VTKVolumeRenderingSystemComponent::initializeComponent()
{
    REGISTER_NODE(CVtkAbstractVolumeMapper, d->nodeDescMap);
    REGISTER_NODE(CVtkUnstructuredGridVolumeMapper, d->nodeDescMap);
    REGISTER_NODE(CVtkProjectedTetrahedraMapper, d->nodeDescMap);
    REGISTER_NODE(CVtkUnstructuredGridVolumeRayCastMapper, d->nodeDescMap);
    REGISTER_NODE(CVtkUnstructuredGridVolumeZSweepMapper, d->nodeDescMap);
    REGISTER_NODE(CVtkVolumeMapper, d->nodeDescMap);
    REGISTER_NODE(CVtkFixedPointVolumeRayCastMapper, d->nodeDescMap);
    REGISTER_NODE(CVtkVolumeRayCastMapper, d->nodeDescMap);
    REGISTER_NODE(CVtkVolumeTextureMapper3D, d->nodeDescMap);
    REGISTER_NODE(CVtkEncodedGradientEstimator, d->nodeDescMap);
    REGISTER_NODE(CVtkVolumeRayCastFunction, d->nodeDescMap);
    REGISTER_NODE(CVtkVolume, d->nodeDescMap);
    REGISTER_NODE(CVtkVolumeProperty, d->nodeDescMap);
    REGISTER_NODE(CVtkCellCenterDepthSort, d->nodeDescMap);
    REGISTER_NODE(CVtkFiniteDifferenceGradientEstimator, d->nodeDescMap);
    REGISTER_NODE(CVtkUnstructuredGridBunykRayCastFunction, d->nodeDescMap);
    REGISTER_NODE(CVtkUnstructuredGridHomogeneousRayIntegrator, d->nodeDescMap);
    REGISTER_NODE(CVtkUnstructuredGridLinearRayIntegrator, d->nodeDescMap);
    REGISTER_NODE(CVtkUnstructuredGridPartialPreIntegration, d->nodeDescMap);
    REGISTER_NODE(CVtkUnstructuredGridPreIntegration, d->nodeDescMap);
    REGISTER_NODE(CVtkUnstructuredGridVolumeRayCastFunction, d->nodeDescMap);
    REGISTER_NODE(CVtkUnstructuredGridVolumeRayIntegrator, d->nodeDescMap);
    REGISTER_NODE(CVtkVisibilitySort, d->nodeDescMap);
    REGISTER_NODE(CVtkVolumeRayCastCompositeFunction, d->nodeDescMap);
    REGISTER_NODE(CVtkVolumeRayCastIsosurfaceFunction, d->nodeDescMap);
    REGISTER_NODE(CVtkVolumeRayCastMIPFunction, d->nodeDescMap);
}

void VTKVolumeRenderingSystemComponent::finalizeComponent()
{
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void VTKVolumeRenderingSystemComponent::creationAnnounced()
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

void VTKVolumeRenderingSystemComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
    {
        d->nodeFactoryRegistry = 0;
        d->nodeFactoryRegistryComp = 0;
    }
}

QObject* VTKVolumeRenderingSystemComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    VTKVolumeRenderingSystemComponent* that = const_cast<VTKVolumeRenderingSystemComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* VTKVolumeRenderingSystemComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(VTKVolumeRenderingSystemComponent)


