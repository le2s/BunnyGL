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

#ifndef I_METHOD_INVOCATION_HELPER_H
#define I_METHOD_INVOCATION_HELPER_H

#include <GCF/IDataTypeSupport>
#include <QVariant>
#include <QByteArray>
#include <QObject>
#include <QGenericArgument>
#include <QGenericReturnArgument>

class IMethodInvocationHelper : virtual public IDataTypeSupport
{
public:
    virtual bool invokeMethod(QObject *obj, const char *member,
                              Qt::ConnectionType connectionType,
                              QVariant& returnValue,
                              QGenericArgument val0, QGenericArgument val1,
                              QGenericArgument val2, QGenericArgument val3,
                              QGenericArgument val4, QGenericArgument val5,
                              QGenericArgument val6, QGenericArgument val7,
                              QGenericArgument val8, QGenericArgument val9) = 0;
};
Q_DECLARE_INTERFACE(IMethodInvocationHelper, "com.vcreatelogic.IMethodInvocationHelper/1.0")

#endif
