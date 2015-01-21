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

#include <GCF/Common>
#include <GCF/AbstractComponent>
#include <GCF/ComponentFactory>
#include <GCF/ObjectIterator>

#include <QAction>
#include <QWidget>
#include <QObject>
#include <QMenu>
#include <QToolBar>

#ifdef GCF_ACTIVEQT_AVAILABLE
#include <QAxWidget>
#endif

#ifdef GCF_KPARTS_AVAILABLE
#include "kparts/part.h"
#endif

/**
\class GCF::ObjectIterator GCF/ObjectIterator
\brief Iterates through all exposed objects in a GCF application

A GCF application can consist of several components. Each component can expose one or more
objects. This class helps in iterating through all such exposed-objects in a GCF application.

\section objectiterator1 Iterating through all objects

To iterate through all objects in an application you can

\code
GCF::ObjectIterator it;
QObject* obj = it.firstObject();

while(obj)
{
    // Use the obj pointer

    obj = it.nextObject();
}
\endcode

For every object you can make use of the currentObject(), currentObjectName() and
currentObjectComponent() to figure out a pointer to the current object, its complete name
and its owning component.

\section objectiterator2 Iterating through objects in a specific component

Many a times you might want to just iterate through objects in a specific component.
In such cases you can construct the iterator based on the component name or pointer.

\code
GCF::ObjectIterator it(componentName);
QObject* obj = it.firstObject();

while(obj)
{
    // Use the obj pointer

    obj = it.nextObject();
}
\endcode

This mode of iteration is usually used within the \ref GCF::AbstractComponent::componentCreated()
and \ref GCF::AbstractComponent::componentDestroyed() implementations.

\section objectiterator3 Iterating through objects starting from a specific component

Sometimes you might want to iterate through all objects in the application, but want to start
looking for objects from a specific component. In such cases you can

\code
GCF::AbstractComponent* comp
    = GCF::ComponentFactory::instance().findComponent(componentName);

GCF::ObjectIterator it;
it.setFirstComponent( comp );
QObject* obj = it.firstObject();

while(obj)
{
    // Use the obj pointer

    obj = it.nextObject();
}
\endcode

\sa ref GCF::findObject<>() methods
*/

namespace GCF
{
    struct ObjectIteratorData
    {
        ObjectIteratorData() : componentIndex(-1),
            iterateOnComponent(0), firstComponent(0) { }

        struct
        {
            QObject* object;
            QString name;
            GCF::AbstractComponent* component;
            GCF::ComponentGui gui;
            int guiNodeIndex;

            void reset() {
                object = 0;
                name.clear();
                component = 0;
                gui = GCF::ComponentGui();
                guiNodeIndex = -1;
            }
        } current;

        // Component List
        QList<GCF::AbstractComponent*> components;
        int componentIndex;

        GCF::AbstractComponent* iterateOnComponent;
        GCF::AbstractComponent* firstComponent;
    };
}

/**
Default constructor. Constructs the iterator to iterate through all exposed-objects in the
GCF application.
*/
GCF::ObjectIterator::ObjectIterator()
{
    d = new ObjectIteratorData;
}

/**
Constructs the iterator to iterate through objects exposed by a component whose name is
\c componentName.
*/
GCF::ObjectIterator::ObjectIterator(const QString& componentName)
{
    d = new ObjectIteratorData;
    if(!componentName.isEmpty())
    	d->iterateOnComponent = GCF::ComponentFactory::instance().findComponent(componentName);
    else
    	d->iterateOnComponent = 0;
}

/**
Constructs the iterator to iterate through objects exposed by the component pointed to by
\c comp.
*/
GCF::ObjectIterator::ObjectIterator(AbstractComponent* component)
{
    d = new ObjectIteratorData;
    d->iterateOnComponent = component;
}

/**
Destructor
*/
GCF::ObjectIterator::~ObjectIterator()
{
    delete d;
}

/**
Begins iterating through all objects starting with objects exposed by \c comp. The function
does nothing if the iterator was initialized on a component.

\note Set the first component before calling \c firstObject()
*/
void GCF::ObjectIterator::setFirstComponent(GCF::AbstractComponent* comp)
{
    if(d->iterateOnComponent)
        return;

    d->firstComponent = comp;
}

/**
Returns the first component set using the \ref setFirstComponent() method
*/
GCF::AbstractComponent* GCF::ObjectIterator::firstComponent()
{
    return d->firstComponent;
}

/**
Returns a pointer to the component whose objects are being iterated by this
iterator.
*/
GCF::AbstractComponent* GCF::ObjectIterator::iteratingOnComponent() const
{
    return d->iterateOnComponent;
}

/**
Returns a pointer the first object.
*/
QObject* GCF::ObjectIterator::firstObject() const
{
    // Start with the first component.
    bool success = firstComponent();
    if(!success)
        return 0;

    // Keeping looking until an object is found.
    while(d->componentIndex < d->components.count() && !d->current.object)
        nextObject();

    return d->current.object;
}

/**
Returns a pointer to the current object
*/
QObject* GCF::ObjectIterator::currentObject() const
{
    return d->current.object;
}

/**
Returns name of the current object
*/
QString GCF::ObjectIterator::currentObjectName() const
{
    return d->current.name;
}

/**
Returns pointer to the component that owns the current object
*/
GCF::AbstractComponent* GCF::ObjectIterator::currentObjectComponent() const
{
    return d->current.component;
}

bool GCF::ObjectIterator::firstComponent() const
{
    // Initialize lists
    d->current.reset();
    d->components.clear();
    d->componentIndex = -1;

    // Catch hold of the component list
    if(d->iterateOnComponent)
        d->components << d->iterateOnComponent;
    else
    {
        d->components = GCF::ComponentFactory::instance().components();
        if(d->firstComponent && d->components.contains(d->firstComponent))
        {
            d->components.removeAll(d->firstComponent);
            d->components.prepend(d->firstComponent);
        }
    }
    d->componentIndex = 0;
    if(!d->components.count())
        return false;

    // Start with the first component
    d->current.component = d->components.at( d->componentIndex );
    d->current.gui = d->current.component->componentGui();
    d->current.guiNodeIndex = -1;

    return true;
}

bool GCF::ObjectIterator::nextComponent() const
{
    ++d->componentIndex;
    if(d->componentIndex >= d->components.count())
        return false;

    d->current.component = d->components.at( d->componentIndex );
    d->current.gui = d->current.component->componentGui();
    d->current.guiNodeIndex = -1;

    return true;
}

/**
Returns pointer to the next object. The function returns NULL when there are no
more objects to iterate.
*/
QObject* GCF::ObjectIterator::nextObject() const
{
    // Reset information about the current object
    d->current.object = 0;
    d->current.name.clear();
    d->current.component = 0;

    // If there is no way to fetch more object information,
    // then return NULL
    if(d->componentIndex < 0 || d->componentIndex >= d->components.count())
        return 0;

    // Look for the next object
    QObject* obj = 0;
    while(!obj)
    {
        obj = this->nextObjectInternal();
        if(obj)
            break;

        if(d->current.guiNodeIndex >= d->current.gui.nodeCount())
        {
            // We are done looking at nodes in the current component,
            // so lets go to the next component.
            bool success = nextComponent();
            if(!success)
                return 0;
        }
    }

    GCF::ComponentGuiNode node = d->current.gui.node(d->current.guiNodeIndex);
    d->current.object = obj;
    d->current.name = node.completeName();
    d->current.component = node.component();

    return d->current.object;
}

QObject* GCF::ObjectIterator::nextObjectInternal() const
{
    // This function has to look into the immediate next gui node
    // and see if an object exists in there.
    ++d->current.guiNodeIndex;
    GCF::ComponentGuiNode node = d->current.gui.node(d->current.guiNodeIndex);
    QObject* objPtr = this->object(node);
    return objPtr;
}

QObject* GCF::ObjectIterator::object(GCF::ComponentGuiNode node)
{
    if(!node.isValid())
        return 0;

    QObject* object = 0;
    switch(node.type())
    {
    case GCF::ComponentGuiNode::Object:
        object = node.hasKey("QObject") ? node["QObject"].value<QObject*>() : 0;
        break;
    case GCF::ComponentGuiNode::Widget:
        object = node.hasKey("QWidget") ? node["QWidget"].value<QWidget*>() : 0;
        break;
    case GCF::ComponentGuiNode::Action:
        object = node.hasKey("QAction") ? node["QAction"].value<QAction*>() : 0;
        break;
    case GCF::ComponentGuiNode::Menu:
        object = node.hasKey("QMenu") ? node["QMenu"].value<QMenu*>() : 0;
        break;
    case GCF::ComponentGuiNode::Toolbar:
        object = node.hasKey("QToolBar") ? node["QToolBar"].value<QToolBar*>() : 0;
        break;
#ifdef GCF_ACTIVEQT_AVAILABLE
    case GCF::ComponentGuiNode::ActiveX:
        object = node.hasKey("QAxWidget") ? node["QAxWidget"].value<QAxWidget*>() : 0;
        break;
#endif
#ifdef GCF_KPARTS_AVAILABLE
    case GCF::ComponentGuiNode::KPart:
        object = node.hasKey("KParts::Part") ? node["KParts::Part"].value<KParts::Part*>() : 0;
        break;
#endif
    default:
        object = node.hasKey("QObject") ? node["QObject"].value<QObject*>() : 0;
        break;
    }

    return object;
}

QObject* GCF::ObjectIterator::object(const QString& completeName)
{
    if(completeName.isEmpty())
        return 0;

    QString compName = completeName.split('.').first();
    GCF::AbstractComponent* comp = GCF::ComponentFactory::instance().findComponent(compName);
    if(!comp)
        return 0;

    GCF::ComponentGuiNode guiNode = comp->componentGui().findNode(completeName);
    if(!guiNode.isValid())
        return 0;

    return GCF::ObjectIterator::object(guiNode);
}

