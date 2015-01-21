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

#ifndef APP_ACCESS_POINT_P_H
#define APP_ACCESS_POINT_P_H

#include <GCF/AppAccessPoint>

namespace GCF
{
class DefaultMessageEncoderDecoder : public QObject, virtual public IMessageEncoderDecoder
{
    Q_OBJECT
    Q_INTERFACES(IMessageEncoderDecoder)

public:
    DefaultMessageEncoderDecoder(QObject* parent=0);
    ~DefaultMessageEncoderDecoder();

    // IContainer implementation
    QObject* containerObject();

    // IMessageEncoderDecoder implementation
    bool canDecodeMessage(const QByteArray& ba) const;
    GCF::Message* decodeMessage(QByteArray& ba) const;
    QByteArray encodeMessage(const GCF::Message* msg);
    bool isCloseSocketUponSendMessage() const;
};

class HttpMessageEncoderDecoder : public QObject, virtual public IMessageEncoderDecoder
{
    Q_OBJECT
    Q_INTERFACES(IMessageEncoderDecoder)

public:
    HttpMessageEncoderDecoder(QObject* parent=0);
    ~HttpMessageEncoderDecoder();

    // IContainer implementation
    QObject* containerObject();

    // IMessageEncoderDecoder implementation
    bool canDecodeMessage(const QByteArray& ba) const;
    GCF::Message* decodeMessage(QByteArray& ba) const;
    QByteArray encodeMessage(const GCF::Message* msg);
    bool isCloseSocketUponSendMessage() const;

    GCF::Message* decodeMessageFromUrl(const QByteArray& url) const;
};

class JSONMessageEncoderDecoder : public QObject, virtual public IMessageEncoderDecoder
{
    Q_OBJECT
    Q_INTERFACES(IMessageEncoderDecoder)

public:
    JSONMessageEncoderDecoder(QObject* parent=0);
    ~JSONMessageEncoderDecoder();

    // IContainer implementation
    QObject* containerObject();

    // IMessageEncoderDecoder implementation
    bool canDecodeMessage(const QByteArray& ba) const;
    GCF::Message* decodeMessage(QByteArray& ba) const;
    QByteArray encodeMessage(const GCF::Message* msg);
    bool isCloseSocketUponSendMessage() const;
};
}

#endif
