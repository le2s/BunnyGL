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

#include <GCF/ComponentFactory>
#include <GCF/AbstractComponent>
#include <QMetaObject>
#include <QApplication>
#include <QEvent>
#include <QMetaObject>
#include <QMetaMethod>

#include <GCF/RemoteAppAccessPoint>
#include <GCF/AppAccessPointComponent>
#include <GCF/RemoteAppDiscovery>
#include <GCF/AppAccessPoint>
#include <GCF/UpdaterComponent>
#include <GCF/Message>
#include <GCF/IBinaryDataSerializer>
#include <GCF/MethodInvocationHelper>
#include <GCF/IMethodInvocationHelper>
#include <GCF/IDataToGenericArgumentConverter>

namespace GCF
{
    struct ComponentInfo
    {
        QString className;
        CreateComponentFunction createFn;
        const QMetaObject* metaObject;
    };
}

struct GCF::ComponentFactoryData
{
    ComponentFactoryData() : initialized(false) { }

    bool initialized;
    QList<GCF::AbstractComponent*> componentList;
    QList<GCF::ComponentInfo> componentInfoList;
};

GCF::ComponentFactory & GCF::ComponentFactory::instance()
{
    return *(GCF::ComponentFactory::instancePointer());
}

GCF::ComponentFactory* GCF::ComponentFactory::instancePointer()
{
    static bool firstTime = true;
    static GCF::ComponentFactory* theFactory = new GCF::ComponentFactory;
    if(firstTime)
    {
        theFactory->setParent(qApp);
        theFactory->initialize();
        firstTime = false;
    }

    return theFactory;
}

GCF::ComponentFactory::ComponentFactory()
{
    d = new GCF::ComponentFactoryData;
}

GCF::ComponentFactory::~ComponentFactory()
{
    if(d->componentList.count())
        qWarning("Deleting GCF::ComponentFactory before calling finalize()");
    delete d;
}

void GCF::ComponentFactory::initialize()
{
    if(d->initialized)
        return;
    registerComponentInfo();
    d->initialized = true;
}

void GCF::ComponentFactory::finalize()
{
    // These things have to be stopped before finalization of
    // components can begin.
    if( findComponent("AppAccessPointComponent") )
    {
        GCF::RemoteAppDiscovery::instance().stopDiscovery(true);
        GCF::RemoteAppAccessPoint::closeAllRemoteAccessPoints();
        GCF::AppAccessPoint::instance().setAppAccessEnabled(false);
    }

    if( findComponent("UpdaterComponent") )
    {
        GCF::UpdaterComponent::instance().stopUpdater();
    }

    Q_FOREACH(GCF::AbstractComponent* comp, d->componentList)
        comp->aboutToFinalize();

    while(d->componentList.count())
        d->componentList.first()->finalize();
}

const QMetaObject* GCF::ComponentFactory::componentMeta(const QString& className) const
{
    for(int i=0; i<d->componentInfoList.count(); i++)
    {
        if(d->componentInfoList[i].className == className)
            return d->componentInfoList[i].metaObject;
    }
    return 0;
}

bool GCF::ComponentFactory::canCreate(const QString& className) const
{
    for(int i=0; i<d->componentInfoList.count(); i++)
    {
        if(d->componentInfoList[i].className == className)
            return true;
    }
    return false;
}

GCF::AbstractComponent* GCF::ComponentFactory::createComponent(const QString& className)
{
    // Now create the new component.
    for(int i=0; i<d->componentInfoList.count(); i++)
    {
        if(d->componentInfoList[i].className == className)
        {
            GCF::AbstractComponent* component = d->componentInfoList[i].createFn();
            if(component)
                component->setParent(this);
            QMetaObject::invokeMethod(component, "initialize", Qt::QueuedConnection);
            return component;
        }
    }
    return 0;
}

GCF::AbstractComponent* GCF::ComponentFactory::findComponent(const QString& componentName) const
{
        if(componentName.isEmpty())
                return 0;

    for(int i=0; i<d->componentList.count(); i++)
    {
        if(d->componentList[i]->componentName() == componentName || d->componentList[i]->inherits(qPrintable(componentName)))
            return d->componentList[i];
    }
    return 0;
}

QList<GCF::AbstractComponent*> GCF::ComponentFactory::findComponents(const QString& componentName) const
{
    QList<GCF::AbstractComponent*> ret;
    for(int i=0; i<d->componentList.count(); i++)
    {
        if(d->componentList[i]->componentName() == componentName || d->componentList[i]->inherits(qPrintable(componentName)))
            ret.append(d->componentList[i]);
    }
    return ret;
}

QList<GCF::AbstractComponent*> GCF::ComponentFactory::components() const
{
    return d->componentList;
}

QStringList GCF::ComponentFactory::componentNames() const
{
    QStringList ret;
    for(int i=0; i<d->componentList.count(); i++)
        ret.append(d->componentList[i]->metaObject()->className());
    return ret;
}

QList<GCF::AbstractComponent*> GCF::ComponentFactory::activeComponents() const
{
    QList<GCF::AbstractComponent*> ret;
    for(int i=0; i<d->componentList.count(); i++)
    {
        GCF::AbstractComponent* comp = d->componentList[i];
        if(comp->isActive())
            ret.append(comp);
    }
    return ret;
}

QList<GCF::AbstractComponent*> GCF::ComponentFactory::inactiveComponents() const
{
    QList<GCF::AbstractComponent*> ret;
    for(int i=0; i<d->componentList.count(); i++)
    {
        GCF::AbstractComponent* comp = d->componentList[i];
        if(!comp->isActive())
            ret.append(comp);
    }
    return ret;
}

void GCF::ComponentFactory::registerComponent(GCF::AbstractComponent* comp)
{
    d->componentList.append(comp);
}

void GCF::ComponentFactory::unregisterComponent(GCF::AbstractComponent* comp)
{
    d->componentList.removeAll(comp);
}

void GCF::ComponentFactory::announceComponentCreation(GCF::AbstractComponent* comp)
{
    for(int i=0; i<d->componentList.count(); i++)
    {
        GCF::AbstractComponent* component = d->componentList[i];
        if(component == comp)
            continue;
        component->componentCreated(comp);
    }

    emit componentCreated(comp);

    // Hard-coded hack: Look for IBinaryDataSerializer implementations
    // in the newly added component and register them.
    QList<IBinaryDataSerializer*> serializers = GCF::findObjects<IBinaryDataSerializer*>(comp);
    Q_FOREACH(IBinaryDataSerializer* serializer, serializers)
        GCF::Message::registerSerializer(serializer);

    QList<IMethodInvocationHelper*> helpers = GCF::findObjects<IMethodInvocationHelper*>(comp);
    Q_FOREACH(IMethodInvocationHelper* helper, helpers)
        GCF::registerHelper(helper);

    QList<IDataToGenericArgumentConverter*> converters = GCF::findObjects<IDataToGenericArgumentConverter*>(comp);
    Q_FOREACH(IDataToGenericArgumentConverter* conv, converters)
        GCF::registerConverter(conv);
}

void GCF::ComponentFactory::announceComponentDestruction(GCF::AbstractComponent* comp)
{
    // Hard-coded hack: Look for IBinaryDataSerializer implementations
    // in the in-destruction component and unregister them
    QList<IBinaryDataSerializer*> serializers = GCF::findObjects<IBinaryDataSerializer*>(comp);
    Q_FOREACH(IBinaryDataSerializer* serializer, serializers)
        GCF::Message::unregisterSerializer(serializer);

    QList<IMethodInvocationHelper*> helpers = GCF::findObjects<IMethodInvocationHelper*>(comp);
    Q_FOREACH(IMethodInvocationHelper* helper, helpers)
        GCF::unregisterHelper(helper);

    QList<IDataToGenericArgumentConverter*> converters = GCF::findObjects<IDataToGenericArgumentConverter*>(comp);
    Q_FOREACH(IDataToGenericArgumentConverter* conv, converters)
        GCF::unregisterConverter(conv);

    for(int i=0; i<d->componentList.count(); i++)
    {
        GCF::AbstractComponent* component = d->componentList[i];
        if(component == comp)
            continue;
        component->componentDestroyed(comp);
    }

    emit componentDestroyed(comp);
}

void GCF::ComponentFactory::registerComponentInfo(const QString& className, CreateComponentFunction createFn, const QMetaObject* meta)
{
    for(int i=0; i<d->componentInfoList.count(); i++)
    {
        GCF::ComponentInfo info = d->componentInfoList[i];
        if(info.className == className)
        {
            info.createFn = createFn;
            info.metaObject = meta;
            return;
        }
    }

    GCF::ComponentInfo info;
    info.className = className;
    info.createFn = createFn;
    info.metaObject = meta;

    // Hack to remove GCF::Components:: prefix if it is found
    static QString prefix = "GCF::Components::";
    if(info.className.startsWith(prefix))
        info.className.remove(0, prefix.length());

    d->componentInfoList.push_back(info);
}

/****************************************************************************
Component Factory Table
****************************************************************************/

#define GCF_COMPONENT_FACTORY_TABLE_BEGIN void GCF::ComponentFactory::registerComponentInfo() {

#define GCF_COMPONENT_FACTORY_TABLE_ENTRY(ClassName) \
    { \
        GCF::ComponentInfo info; \
        info.className = #ClassName; \
        info.createFn = ClassName::CreateInstance; \
        info.metaObject = &ClassName::staticMetaObject; \
        d->componentInfoList.push_back(info); \
    }

#define GCF_COMPONENT_FACTORY_TABLE_END }

#include "ComponentFactoryTable"

