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

#ifndef XML_GCF_MESSAGE_H
#define XML_GCF_MESSAGE_H

#include <QString>
#include <GCF/IMessageEncoderDecoder>
#include <GCF/IXmlDataSerializer>

namespace GCF { class Message; }

class XMLGCFMessage
{
public:
    static QString ToXML(const GCF::Message* message);
    static GCF::Message* FromXML(const QString& xml);
};

struct XMLGCFMessageHandlerData;
class XMLGCFMessageHandler : public QObject, virtual public IMessageEncoderDecoder,
        virtual public IXmlDataSerializerRegistry
{
    Q_OBJECT
    Q_INTERFACES(IMessageEncoderDecoder IXmlDataSerializerRegistry)

public:
    static XMLGCFMessageHandler& instance();
    ~XMLGCFMessageHandler();

    bool isProbableXMLGCFMessage(const QByteArray& ba) const;

    // IXmlDataSerializerRegistry implementation
    void registerSerializer(IXmlDataSerializer* serializer);
    void unregisterSerializer(IXmlDataSerializer* serializer);
    IXmlDataSerializer* findSerializer(const QByteArray& typeName) const;

    // IContainer implementation
    QObject* containerObject();

    // IMessageEncoderDecoder implementation
    bool canDecodeMessage(const QByteArray& ba) const;
    GCF::Message* decodeMessage(QByteArray& ba) const;
    QByteArray encodeMessage(const GCF::Message* msg);
    bool isCloseSocketUponSendMessage() const;

private:
    XMLGCFMessageHandler(QObject* parent=0);

private:
    XMLGCFMessageHandlerData* d;
};

#endif

