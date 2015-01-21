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

#ifndef METHOD_INVOCATION_HELPER_H
#define METHOD_INVOCATION_HELPER_H

#include "Common.h"

#include <QVariant>
#include <QByteArray>
#include <QGenericArgument>

class IMethodInvocationHelper;
class IDataToGenericArgumentConverter;

namespace GCF
{
class MethodInvocationData;

bool GCF_EXPORT_METHOD callMethod(QObject* object, const QString& methodName, const QVariantList& args, QVariant& returnValue, bool publicOnly=true);
bool GCF_EXPORT_METHOD toGenericArgument(QGenericArgument& arg, const QVariant& value, const char* type, MethodInvocationData& miData);
int  GCF_EXPORT_METHOD findMethod(QObject* object, const QString& methodName, const QVariantList& args);

enum MethodTag
{
    AtomicWSTag,
    NormalWSTag,
    ClientIndependentWSTag,
    UnknownTag,
    InvalidTag
};
GCF_EXPORT_METHOD GCF::MethodTag methodTag(QObject* object, int methodIndex);
GCF_EXPORT_METHOD bool callMethod(QObject* object, int methodIndex, const QVariantList& args, QVariant& returnValue, bool publicOnly=true);

GCF_EXPORT_METHOD void registerHelper(IMethodInvocationHelper* helper);
GCF_EXPORT_METHOD void unregisterHelper(IMethodInvocationHelper* helper);
GCF_EXPORT_METHOD IMethodInvocationHelper* findHelper(const QByteArray& typeName);

GCF_EXPORT_METHOD void registerConverter(IDataToGenericArgumentConverter* conv);
GCF_EXPORT_METHOD void unregisterConverter(IDataToGenericArgumentConverter* conv);
GCF_EXPORT_METHOD IDataToGenericArgumentConverter* findConverter(const QByteArray& typeName);

GCF_EXPORT_METHOD QByteArray& errorCode();
}

#endif
