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

#ifndef I_BINARY_DATA_SERIALIZER_H
#define I_BINARY_DATA_SERIALIZER_H

#include <GCF/IDataTypeSupport>
#include <QVariant>
#include <QByteArray>

class IBinaryDataSerializer : virtual public IDataTypeSupport
{
public:
    virtual bool toBinary(const QVariant& value, QByteArray& binary) const = 0;    
    virtual bool fromBinary(const QByteArray& binary, QVariant& value) = 0;
};
Q_DECLARE_INTERFACE(IBinaryDataSerializer, "com.vcreatelogic.IBinaryDataSerializer/1.0")
#endif
