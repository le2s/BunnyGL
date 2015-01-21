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

#include "XMLGCFMessage.h"
#include "XMLGCFMessage_p.h"

#include <GCF/Message>

#include <QDomDocument>
#include <QList>
#include <QTextStream>
#include <QSet>

#include <QFile>
#include <stdio.h>

class AbstractMessageAdapterList : public QList<AbstractMessageAdapter*>
{
public:
    AbstractMessageAdapterList() { }
    ~AbstractMessageAdapterList() {
        QList<AbstractMessageAdapter*> copy = *this;
        this->clear();
        qDeleteAll(copy);
    }
};

static AbstractMessageAdapterList& MessageAdapterList()
{
    static AbstractMessageAdapterList theList;
    if(theList.count() == 0)
    {
        theList.append(new AsyncInvokeMethodMessageAdapter);
        theList.append(new AsyncInvokeMethodResMessageAdapter);
        theList.append(new AsyncInvokeMethodsMessageAdapter);
        theList.append(new AsyncInvokeMethodsResMessageAdapter);

        theList.append(new RequestAccessMessageAdapter);
        theList.append(new RequestAccessResMessageAdapter);
        theList.append(new RequestObjectMessageAdapter);
        theList.append(new RequestObjectResMessageAdapter);
        theList.append(new GetPropertyNamesMessageAdapter);
        theList.append(new GetPropertyNamesResMessageAdapter);
        theList.append(new InvokeMethodMessageAdapter);
        theList.append(new InvokeMethodResMessageAdapter);
        theList.append(new RequestConnectionMessageAdapter);
        theList.append(new RequestConnectionResMessageAdapter);
        theList.append(new SignalDeliveryMessageAdapter);
        theList.append(new ReleaseObjectMessageAdapter);
        theList.append(new ReleaseObjectResMessageAdapter);
        theList.append(new ReleaseAccessMessageAdapter);
        theList.append(new ReleaseAccessResMessageAdapter);
        theList.append(new ReleaseConnectionMessageAdapter);
        theList.append(new ReleaseConnectionResMessageAdapter);
        // Register the adapters here...
    }

    return theList;
}

static AbstractMessageAdapter* FindMessageAdapter(GCF::Message::MessageType type)
{
    for(int i=0; i<MessageAdapterList().count(); i++)
    {
        AbstractMessageAdapter* adapter = MessageAdapterList().at(i);
        if(adapter->messageType() == type)
            return adapter;
    }

    return 0;
}

QString XMLGCFMessage::ToXML(const GCF::Message* message)
{
    AbstractMessageAdapter* adapter = FindMessageAdapter(message->messageType());
    if(!adapter)
        return QString();

    QDomDocument doc = adapter->toDOM(message);
    QString xmlStr = doc.toString();

    if(!xmlStr.endsWith("\n"))
        xmlStr += "\n";

    return xmlStr;
}

GCF::Message* XMLGCFMessage::FromXML(const QString& xml)
{
    QDomDocument doc;
    if(!doc.setContent(xml))
        return 0;

    QDomElement rootE = doc.documentElement();
    if(rootE.tagName() != "GCFMessage")
        return 0;

    GCF::Message::MessageType type = (GCF::Message::MessageType)(rootE.attribute("Type").toInt());
    AbstractMessageAdapter* adapter = FindMessageAdapter(type);
    if(!adapter)
        return 0;

    return adapter->fromDOM(doc);
}


////////////////////////////////////////////////////////////////////////////////////
// XMLGCFMessageHandler
////////////////////////////////////////////////////////////////////////////////////

static const QByteArray GCFMessageHeader("<GCFMessage");
static const QByteArray GCFMessageFooter1("</GCFMessage>");
static const QByteArray GCFMessageFooter2("</GCFMessage>\n");

struct HttpPacket
{
    HttpPacket() : Size(0) { }

    QMap<QByteArray,QByteArray> Header;
    QByteArray Data;
    quint32 Size;

    bool isValid() const;
};

struct XMLGCFMessageHandlerData
{
    QMap<QByteArray, IXmlDataSerializer*> serializerMap;

    QList<GCF::Message*> xmlMessageList;
    HttpPacket extractPacket(const QByteArray& ba);

private:
    QByteArray nextLine(const QByteArray& rawData, int& offset);
};

XMLGCFMessageHandler& XMLGCFMessageHandler::instance()
{
    static XMLGCFMessageHandler theInstance;
    return theInstance;
}

XMLGCFMessageHandler::XMLGCFMessageHandler(QObject* parent)
: QObject(parent)
{
    d = new XMLGCFMessageHandlerData;
}

XMLGCFMessageHandler::~XMLGCFMessageHandler()
{
    delete d;
}

bool XMLGCFMessageHandler::isProbableXMLGCFMessage(const QByteArray& ba) const
{
    return ba.startsWith(GCFMessageHeader);
}

void XMLGCFMessageHandler::registerSerializer(IXmlDataSerializer* serializer)
{
    if(!serializer)
        return;

    if(d->serializerMap.contains(serializer->dataType()))
        return;

    d->serializerMap[serializer->dataType()] = serializer;
}

void XMLGCFMessageHandler::unregisterSerializer(IXmlDataSerializer* serializer)
{
    if(!serializer)
        return;

    if( d->serializerMap.contains(serializer->dataType()) &&
        d->serializerMap[serializer->dataType()] == serializer)
    {
        d->serializerMap.remove( serializer->dataType() );
    }
}

IXmlDataSerializer* XMLGCFMessageHandler::findSerializer(const QByteArray& typeName) const
{
    if( d->serializerMap.contains(typeName) )
        return d->serializerMap[typeName];

    return 0;
}


QObject* XMLGCFMessageHandler::containerObject()
{
    return this;
}

bool XMLGCFMessageHandler::canDecodeMessage(const QByteArray& ba) const
{
    if(ba.startsWith(GCFMessageHeader))
    {
        QByteArray ba2 = ba;
        while(ba2[ba2.length()-1] != '>')
            ba2 = ba2.remove(ba2.length()-1, 1);

        if((ba2.endsWith(GCFMessageFooter1) || ba2.endsWith(GCFMessageFooter2)))
            return true;

        return false;
    }

    HttpPacket packet = d->extractPacket(ba);
    return packet.isValid();
}

GCF::Message* XMLGCFMessageHandler::decodeMessage(QByteArray& ba) const
{
    if(ba.startsWith(GCFMessageHeader))
    {
        while(ba[ba.length()-1] != '>')
            ba = ba.remove(ba.length()-1, 1);

        if((ba.endsWith(GCFMessageFooter1) || ba.endsWith(GCFMessageFooter2)))
        {
            GCF::Message* msg = XMLGCFMessage::FromXML(ba);
            d->xmlMessageList.append(msg);
            ba.clear();
            return msg;
        }

        return 0;
    }

    HttpPacket packet = d->extractPacket(ba);
    if(packet.isValid())
    {
        GCF::Message* msg = XMLGCFMessage::FromXML(packet.Data);
        ba.remove(0, packet.Size);
        return msg;
    }

    return 0;
}

QByteArray XMLGCFMessageHandler::encodeMessage(const GCF::Message* msg)
{
    QByteArray ret;

    GCF::Message* inceptionMessage = msg->inceptionMessage();
    if(!inceptionMessage)
    {
        if(msg->customData().hasKey("InceptionMessage"))
        {
            void* inceptionMessagePtr = msg->customData().value("InceptionMessage").value<void*>();
            inceptionMessage = (GCF::Message*)inceptionMessagePtr;
        }
    }

    if(d->xmlMessageList.contains(inceptionMessage) || inceptionMessage)
    {
        ret = XMLGCFMessage::ToXML(msg).toLatin1();
        d->xmlMessageList.removeAll(inceptionMessage);
        return ret;
    }

    QString xmlStr = XMLGCFMessage::ToXML(msg);
    QByteArray msgBa = xmlStr.toLatin1();
    
    QTextStream ts(&ret, QIODevice::WriteOnly);
    ts << "Http/1.1 200 Ok\r\n";
    ts << "Content-Type: XMLGCFMessage\r\n";
    ts << "Content-Length: " << msgBa.size() << "\r\n";
    ts << "\r\n";
    ts.flush();

    ret += msgBa;

    return ret;
}

bool XMLGCFMessageHandler::isCloseSocketUponSendMessage() const
{
    return true;
}

bool HttpPacket::isValid() const
{
    if( !Header.count() || !Data.size() )
        return false;

    if( Header.contains("RequestType") && Header["RequestType"].toUpper() == "POST" )
    {
        if( Header.contains("Content-Type") )
        {
            static const QByteArray supportedContentType("XMLGCFMessage");
            return Header["Content-Type"] == supportedContentType;
        }
    }

    return false;
}

HttpPacket XMLGCFMessageHandlerData::extractPacket(const QByteArray& rawBytes)
{
    HttpPacket ret;

    int offset = 0;
    while(1)
    {
        QByteArray line = this->nextLine(rawBytes, offset);
        if(line.isEmpty())
            break;

        if(!ret.Header.count())
        {
            if( !(line.startsWith("POST") || line.startsWith("GET") || line.startsWith("PUT")) )
                return ret;

            // Contains RequestType and HttpXMLGCFVersion.
            int slashIndex = line.indexOf('/');
            QByteArray reqType = line.left(slashIndex).simplified();
            QByteArray HttpXMLGCFVer = line.mid(slashIndex+2).simplified();

            ret.Header["RequestType"] = reqType;
            ret.Header["HtmlVersion"] = HttpXMLGCFVer;
        }
        else
        {
            int colonIndex = line.indexOf(':');
            QByteArray key = line.left(colonIndex);
            QByteArray value = line.mid(colonIndex+2);
            ret.Header[key] = value;
        }
    }

    // Extract data part
    int contentLength = ret.Header.contains("Content-Length") ?
                        ret.Header["Content-Length"].toInt() : -1;
    ret.Data = rawBytes.mid(offset, contentLength);

    // Store size of HttpXMLGCF packet in rawbytes..
    ret.Size = offset + ret.Data.size();

    return ret;
}

QByteArray XMLGCFMessageHandlerData::nextLine(const QByteArray& rawData, int& offset)
{
    static const QByteArray delim("\r\n");
    int index = rawData.indexOf(delim, offset);
    if(index < 0)
        return QByteArray();

    QByteArray ret;
    if(index > offset)
        ret = rawData.mid(offset, index-offset);
    offset = index + delim.size();

    return ret;
}
