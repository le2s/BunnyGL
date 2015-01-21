/****************************************************************************
**
** Copyright (C) UNO and VCreate Logic
**
** Use of this file is limited according to the terms specified by
** UNO and VCreate Logic.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "InfoVisComponent.h"
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

#include "InfoVisOutputViewer.h"

#include "CVtkTableAlgorithm.h"
#include "CVtkDataObjectToTable.h"
#include "CVtkDelimitedTextReader.h"
#include "CVtkFixedWidthTextReader.h"
#include "CVtkMergeTables.h"
#include "CVtkQtAbstractModelAdapter.h"
#include "CVtkQtTableModelAdapter.h"
#include "CVtkQtTreeModelAdapter.h"
#include "QAbstractItemViewVisNode.h"
#include "QListViewVisNode.h"
#include "QTableViewVisNode.h"
#include "QTreeViewVisNode.h"
#include "QColumnViewVisNode.h"

struct InfoVisComponentData
{
    InfoVisComponentData() {
        nodeFactoryRegistryComp = 0;
        nodeFactoryRegistry = 0;
        visNetworkCanvas = 0;
        visNetwork = 0;
        scriptEngineManager = 0;
        outputViewer = 0;
    }

    QMap<QString, CGenericVisNodeDesc*> nodeDescMap;
    GCF::AbstractComponent* nodeFactoryRegistryComp;
    IVisSystemNodeFactoryRegistry* nodeFactoryRegistry;
    IVisNetworkCanvas* visNetworkCanvas;
    IVisNetwork* visNetwork;
    IScriptEngineManager* scriptEngineManager;
    InfoVisOutputViewer* outputViewer;
};

GCF_DEFINE_COMPONENT(InfoVisComponent)

InfoVisComponent & InfoVisComponent::instance()
{
    static InfoVisComponent* theInstance = GCF_CREATE_COMPONENT(InfoVisComponent);
    return *theInstance;
}

InfoVisComponent::InfoVisComponent()
{
    d = new InfoVisComponentData;
}

InfoVisComponent::~InfoVisComponent()
{
    delete d;
}

QIcon InfoVisComponent::nodeIcon()
{
    return qApp->windowIcon();
}

QObject* InfoVisComponent::containerObject()
{
    return this;
}

QStringList InfoVisComponent::nodeClassNameList()
{
    return d->nodeDescMap.keys();
}

IVisSystemNodeDesc* InfoVisComponent::nodeDesc(QString nodeClassName)
{
    if(d->nodeDescMap.contains(nodeClassName))
        return d->nodeDescMap[nodeClassName];

    return 0;
}

bool InfoVisComponent::canCreate(QString nodeClassName)
{
    IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);

    CGenericVisNodeDesc* desc2 = 0;
    desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
    if(desc2)
        return desc2->createFunction();

    return 0;
}

IVisSystemNode* InfoVisComponent::createNode(QString nodeClassName)
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

void InfoVisComponent::deleteNode(IVisSystemNode* node)
{
    node->finalizeNode();
}

QString InfoVisComponent::productName() const
{
    return "InfoVis Classes";
}

QString InfoVisComponent::organization() const
{
    return "UNO";
}

QImage InfoVisComponent::programLogo() const
{
    return QImage();
}

QString InfoVisComponent::version() const
{
    return "1.0";
}

QString InfoVisComponent::shortDescription() const
{
    return "Provides the InfoVis component";
}

QString InfoVisComponent::homepage() const
{
    return "InfoVis Website";
}

QString InfoVisComponent::bugAddress() const
{
    return "InfoVis Website";
}

const QList<GCF::AboutPerson> InfoVisComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> InfoVisComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString InfoVisComponent::license() const
{
    return "InfoVis License";
}

QString InfoVisComponent::copyrightStatement() const
{
    return "Copyright (c) UNO and VCreate Logic. All rights reserved";
}

void InfoVisComponent::initializeComponent()
{
    REGISTER_NODE(CVtkTableAlgorithm, d->nodeDescMap);
    REGISTER_NODE(CVtkDataObjectToTable, d->nodeDescMap);
    REGISTER_NODE(CVtkDelimitedTextReader, d->nodeDescMap);
    REGISTER_NODE(CVtkFixedWidthTextReader, d->nodeDescMap);
    REGISTER_NODE(CVtkMergeTables, d->nodeDescMap);
    REGISTER_NODE(CVtkQtAbstractModelAdapter, d->nodeDescMap)
    REGISTER_NODE(CVtkQtTableModelAdapter, d->nodeDescMap)
    REGISTER_NODE(CVtkQtTreeModelAdapter, d->nodeDescMap)
    REGISTER_NODE(QAbstractItemViewVisNode, d->nodeDescMap)
    REGISTER_NODE(QListViewVisNode, d->nodeDescMap)
    REGISTER_NODE(QTableViewVisNode, d->nodeDescMap)
    REGISTER_NODE(QTreeViewVisNode, d->nodeDescMap)
    REGISTER_NODE(QColumnViewVisNode, d->nodeDescMap)
}

void InfoVisComponent::finalizeComponent()
{
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void InfoVisComponent::creationAnnounced()
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

void InfoVisComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
    {
        d->nodeFactoryRegistry = 0;
        d->nodeFactoryRegistryComp = 0;
    }
}

QObject* InfoVisComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    InfoVisComponent* that = const_cast<InfoVisComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* InfoVisComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    if(comps.last() == "outputViewer")
    {
        if(!d->outputViewer)
            d->outputViewer = new InfoVisOutputViewer;

        return d->outputViewer;
    }

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(InfoVisComponent)


