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

#ifndef POINTER_H
#define POINTER_H

#include <QObject>
#include <QPointer>

namespace GCF
{

template <class T>
class Pointer
{
public:
    inline Pointer() { }
    inline Pointer(T* iface) { m_object = iface ? iface->containerObject() : 0; }
    inline Pointer(const Pointer& other) { m_object = other.m_object; }
    inline bool operator == (const Pointer& other) const { return m_object == other.m_object; }
    inline Pointer& operator = (const Pointer& other) {
        m_object = other.m_object;
        return *this;
    }

    inline bool isNull() const { return m_object.isNull(); }
    inline T* operator->() const { return qobject_cast<T*>(m_object.data()); }
    inline T& operator*() const { return *(qobject_cast<T*>(m_object.data())); }
    inline operator T*() const { return qobject_cast<T*>(m_object.data()); }
    inline T* data() const { return qobject_cast<T*>(m_object.data()); }

private:
    QPointer<QObject> m_object;
};

}

#endif // POINTER_H
