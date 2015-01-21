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

#ifndef OBJECT_ITERATOR_H
#define OBJECT_ITERATOR_H

#include "Common.h"
#include <QString>

class QObject;

namespace GCF
{
class AbstractComponent;
class ComponentGui;
class ComponentGuiNode;
class DeclarativeSupport;
class DeclarativeEnvironment;

struct ObjectIteratorData;
class GCF_EXPORT_CLASS ObjectIterator
{
public:
    ObjectIterator();

    /*
        Use these constructors if you want to iterate only
        through objects in a specific component.
        */
    ObjectIterator(const QString& componentName);
    ObjectIterator(AbstractComponent* component);
    ~ObjectIterator();

    /*
        Use these constructors if you want to iterate first
        through a specific component, and then through everything
        else.
        */
    void setFirstComponent(GCF::AbstractComponent* comp);
    GCF::AbstractComponent* firstComponent();

    GCF::AbstractComponent* iteratingOnComponent() const;

    QObject* firstObject() const;
    QObject* currentObject() const;
    QString currentObjectName() const;
    AbstractComponent* currentObjectComponent() const;
    QObject* nextObject() const;

protected:
    bool firstComponent() const;
    bool nextComponent() const;
    QObject* nextObjectInternal() const;

    static QObject* object(ComponentGuiNode node);
    static QObject* object(const QString& completeName);

private:
    friend class AbstractComponent;
    friend class DeclarativeSupport;
    friend class DeclarativeEnvironment;
    friend class ComponentGuiNode;
    friend class ComponentGui;

    ObjectIteratorData* d;
};
}

#endif


