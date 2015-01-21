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

#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include "Common.h"
#include <QObject>
#include <QObjectList>
#include <QHash>

namespace GCF
{

class GCF_EXPORT_CLASS AbstractObjectList : public QObject
{
    Q_OBJECT

public:
    virtual ~AbstractObjectList();
    virtual void clear(bool deleteObjects=true);

protected:
    AbstractObjectList(QObject* parent=0);
    virtual bool move(int fromIndex, int toIndex);
    bool insert(int index, QObject* object);
    bool addObject(QObject* object);
    bool removeObject(QObject* object);
    virtual void removeAt(int index) = 0;

private slots:
    void objectDestroyed(QObject* object);

signals:
    void objectRemovedAt(int index);
    void objectDestroyedAt(int index);

private:
    QObjectList m_objectList;
};

template <class T>
class ObjectList : public AbstractObjectList
{
public:
    ObjectList(QObject* parent=0) : AbstractObjectList(parent) { }
    ~ObjectList() { }

    bool add(T* object) {
        if(AbstractObjectList::addObject(object)) {
            m_list.append(object);
            return true;
        }
        return false;
    }

    bool remove(T* object) {
        return AbstractObjectList::removeObject(object);
    }

    bool insert(int index, T* object) {
        if(!AbstractObjectList::insert(index, object)) return false;
        if(index < 0) m_list.prepend(object);
        else if(index >= m_list.count()) m_list.append(object);
        else m_list.insert(index, object);
        return true;
    }

    bool move(int fromIndex, int toIndex) {
        if(!AbstractObjectList::move(fromIndex,toIndex)) return false;
        T* obj = m_list.at(fromIndex);
        m_list.removeAt(fromIndex);
        m_list.insert(toIndex, obj);
        return true;
    }

    const QList<T*> toList() const { return m_list; }
    QList<T*> toList() { return m_list; }

    int count() const { return m_list.count(); }
    T* at(int index) const {
        if(index < 0 || index >= m_list.count()) return 0;
        return m_list.at(index);
    }

    bool contains(T* object) const { return m_list.indexOf(object) >= 0; }
    int indexOf(T* object) const { return m_list.indexOf(object); }

protected:
    void removeAt(int index) { m_list.removeAt(index); }

protected:
    QList<T*> m_list;
};

template <class T>
class IContainerList : public AbstractObjectList
{
public:
    IContainerList(QObject* parent=0) : AbstractObjectList(parent) { }
    ~IContainerList() { }

    bool add(T* iface) {
        if(iface && AbstractObjectList::addObject(iface->containerObject())) {
            m_list.append(iface);
            return true;
        }
        return false;
    }

    bool remove(T* iface) {
        if(iface) return AbstractObjectList::removeObject(iface->containerObject());
        return false;
    }

    bool insert(int index, T* object) {
        if(!AbstractObjectList::insert(index, object->containerObject())) return false;
        if(index < 0) m_list.prepend(object);
        else if(index >= m_list.count()) m_list.append(object);
        else m_list.insert(index, object);
        return true;
    }

    bool move(int fromIndex, int toIndex) {
        if(!AbstractObjectList::move(fromIndex,toIndex)) return false;
        T* obj = m_list.at(fromIndex);
        m_list.removeAt(fromIndex);
        m_list.insert(toIndex, obj);
        return true;
    }

    const QList<T*> toList() const { return m_list; }
    QList<T*> toList() { return m_list; }

    int count() const { return m_list.count(); }
    T* at(int index) const {
        if(index < 0 || index >= m_list.count()) return 0;
        return m_list.at(index);
    }

    bool contains(T* iface) const { return m_list.indexOf(iface) >= 0; }
    int indexOf(T* iface) const { return m_list.indexOf(iface); }

protected:
    void removeAt(int index) { m_list.removeAt(index); }

protected:
    QList<T*> m_list;
};

template <class Key, class T>
class ObjectMap : public ObjectList<T>
{
public:
    ObjectMap(QObject* parent=0) : ObjectList<T>(parent) { }
    ~ObjectMap() { }

    void clear(bool deleteObjects=false) {
        m_keyIndexMap.clear();
        ObjectList<T>::clear(deleteObjects);
    }

    bool map(const Key& key, T* object) {
        if(m_keyIndexMap.contains(key)) return false;
        if(!ObjectList<T>::add(object)) return false;
        int index = ObjectList<T>::count()-1;
        m_keyIndexMap[key] = index;
        return true;
    }

    bool unmap(const Key& key) {
        if(!m_keyIndexMap.contains(key)) return false;
        int index = m_keyIndexMap[key];
        return ObjectList<T>::removeObject( ObjectList<T>::at(index) );
    }

    T* value(const Key& key) const {
        int index = m_keyIndexMap.value(key, -1);
        if(index < 0) return 0;
        return ObjectList<T>::at(index);
    }

    bool contains(const Key& key) const {
        return m_keyIndexMap.contains(key);
    }

    Key key(T* object) const {
        int index = ObjectList<T>::indexOf(object);
        if(index < 0) return Key();
        return m_keyIndexMap.key(index);
    }

    QList<Key> keys() const { return m_keyIndexMap.keys(); }
    QList<T*> values() const { return ObjectList<T>::m_list; }

protected:
    void removeAt(int index) {
        Key key = m_keyIndexMap.key(index);
        m_keyIndexMap.remove(key);
        ObjectList<T>::removeAt(index);
        QList<Key> keys = m_keyIndexMap.keys();
        Q_FOREACH(Key key, keys) {
            int value = m_keyIndexMap.value(key, -1);
            if(value > index)
                m_keyIndexMap[key] = value-1;
        }
    }

private:
    QHash<Key,int> m_keyIndexMap;
};

template <class Key, class T>
class IContainerMap : public IContainerList<T>
{
public:
    IContainerMap(QObject* parent=0) : IContainerList<T>(parent) { }
    ~IContainerMap() { }

    void clear(bool deleteObjects=false) {
        m_keyIndexMap.clear();
        IContainerList<T>::clear(deleteObjects);
    }

    bool map(const Key& key, T* object) {
        if(m_keyIndexMap.contains(key)) return false;
        if(!IContainerList<T>::add(object)) return false;
        int index = IContainerList<T>::count()-1;
        m_keyIndexMap[key] = index;
        return true;
    }

    bool unmap(const Key& key) {
        if(!m_keyIndexMap.contains(key)) return false;
        int index = m_keyIndexMap[key];
        return IContainerList<T>::remove( IContainerList<T>::at(index) );
    }

    T* value(const Key& key) const {
        int index = m_keyIndexMap.value(key, -1);
        if(index < 0) return 0;
        return IContainerList<T>::at(index);
    }

    bool contains(const Key& key) const {
        return m_keyIndexMap.contains(key);
    }

    Key key(T* object) const {
        int index = IContainerList<T>::indexOf(object);
        if(index < 0) return Key();
        return m_keyIndexMap.key(index);
    }

    QList<Key> keys() const { return m_keyIndexMap.keys(); }
    QList<T*> values() const { return IContainerList<T>::m_list; }

protected:
    void removeAt(int index) {
        Key key = m_keyIndexMap.key(index);
        m_keyIndexMap.remove(key);
        IContainerList<T>::removeAt(index);
        QList<Key> keys = m_keyIndexMap.keys();
        Q_FOREACH(Key key, keys) {
            int value = m_keyIndexMap.value(key, -1);
            if(value > index)
                m_keyIndexMap[key] = value-1;
        }
    }

private:
    QHash<Key,int> m_keyIndexMap;
};

}

#endif // OBJECTLIST_H
