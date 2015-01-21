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

#include <GCF/ObjectList.h>

GCF::AbstractObjectList::AbstractObjectList(QObject* parent)
    : QObject(parent)
{

}

GCF::AbstractObjectList::~AbstractObjectList()
{

}

void GCF::AbstractObjectList::clear(bool deleteObjects)
{
    while(m_objectList.count())
    {
        disconnect(m_objectList.first(), SIGNAL(destroyed(QObject*)), this, SLOT(objectDestroyed(QObject*)));
        this->removeAt(0);
        if(deleteObjects)
            delete m_objectList.takeFirst();
        else
            m_objectList.takeFirst();
    }
}

bool GCF::AbstractObjectList::move(int fromIndex, int toIndex)
{
    if(fromIndex == toIndex)
        return false;

    if(fromIndex < 0 || fromIndex >= m_objectList.count())
        return false;

    if(toIndex < 0 || toIndex >= m_objectList.count())
        return false;

    QObject* obj = m_objectList.at(fromIndex);
    m_objectList.removeAt(fromIndex);
    m_objectList.insert(toIndex, obj);
    return true;
}

bool GCF::AbstractObjectList::insert(int index, QObject* object)
{
    if(!object || m_objectList.contains(object))
        return false;
    if(index < 0)
        m_objectList.prepend(object);
    else if(index >= m_objectList.count())
        m_objectList.append(object);
    else
        m_objectList.insert(index, object);

    connect(object, SIGNAL(destroyed(QObject*)), this, SLOT(objectDestroyed(QObject*)));
    return true;
}

bool GCF::AbstractObjectList::addObject(QObject* object)
{
    if(!object || m_objectList.contains(object))
        return false;

    m_objectList.append(object);
    connect(object, SIGNAL(destroyed(QObject*)), this, SLOT(objectDestroyed(QObject*)));
    return true;
}

bool GCF::AbstractObjectList::removeObject(QObject* object)
{
    int index = m_objectList.indexOf(object);
    if(index < 0)
        return false;

    disconnect(object, SIGNAL(destroyed(QObject*)), this, SLOT(objectDestroyed(QObject*)));
    emit objectRemovedAt(index);
    m_objectList.removeAt(index);
    this->removeAt(index);
    return true;
}

void GCF::AbstractObjectList::objectDestroyed(QObject* object)
{
    int index = m_objectList.indexOf(object);
    if(index < 0)
        return;

    emit objectDestroyedAt(index);
    emit objectRemovedAt(index);
    m_objectList.removeAt(index);
    this->removeAt(index);
}
