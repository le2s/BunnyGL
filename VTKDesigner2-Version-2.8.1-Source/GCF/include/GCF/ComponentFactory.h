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

#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

#include "Common.h"
#include "AbstractComponent.h"
#include <QObject>
#include <QList>

namespace GCF
{
struct ComponentFactoryData;
class GCF_EXPORT_CLASS ComponentFactory : public QObject
{
    Q_OBJECT

public:
    static ComponentFactory & instance();
    static ComponentFactory* instancePointer();
    ~ComponentFactory();

    void initialize();
    void finalize();

    const QMetaObject* componentMeta(const QString&  className) const;
    bool canCreate(const QString&  className) const;

    GCF::AbstractComponent* createComponent(const QString&  className);
    GCF::AbstractComponent* findComponent(const QString&  componentName) const;
    QList<GCF::AbstractComponent*> findComponents(const QString&  componentName) const;
    QList<GCF::AbstractComponent*> components() const;
    QStringList componentNames() const;

    QList<GCF::AbstractComponent*> activeComponents() const;
    QList<GCF::AbstractComponent*> inactiveComponents() const;

    void registerComponentInfo(const QString&  className, CreateComponentFunction createFn, const QMetaObject* meta);

protected:
    ComponentFactory();
    void registerComponentInfo();
    void registerComponent(GCF::AbstractComponent* comp);
    void unregisterComponent(GCF::AbstractComponent* comp);
    void announceComponentCreation(GCF::AbstractComponent* comp);
    void announceComponentDestruction(GCF::AbstractComponent* comp);

signals:
    void componentCreated(GCF::AbstractComponent* comp);
    void componentDestroyed(GCF::AbstractComponent* comp);

private:
    friend class AbstractComponent;
    friend class AbstractComponentPlugin;
    ComponentFactoryData* d;
};

}

#endif
