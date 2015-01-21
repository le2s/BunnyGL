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

#ifndef I_XML_DATA_SERIALIZER_H
#define I_XML_DATA_SERIALIZER_H

#include <GCF/IContainer>
#include <GCF/IDataTypeSupport>

#include <QVariant>
#include <QByteArray>
#include <QDomElement>
#include <QDomDocument>

class IXmlDataSerializer : virtual public IDataTypeSupport
{
public:
    virtual bool toXml(const QVariant& value, QDomElement element, QDomDocument document) = 0;
    virtual bool fromXml(QDomElement element, QVariant& value) = 0;
};
Q_DECLARE_INTERFACE(IXmlDataSerializer, "com.vcreatelogic.IXmlDataSerializer/1.0")

class IXmlDataSerializerRegistry : virtual public IContainer
{
public:
    virtual void registerSerializer(IXmlDataSerializer* serializer) = 0;
    virtual void unregisterSerializer(IXmlDataSerializer* serializer) = 0;
    virtual IXmlDataSerializer* findSerializer(const QByteArray& typeName) const = 0;
};
Q_DECLARE_INTERFACE(IXmlDataSerializerRegistry, "com.vcreatelogic.IXmlDataSerializerRegistry/1.0")

#endif
