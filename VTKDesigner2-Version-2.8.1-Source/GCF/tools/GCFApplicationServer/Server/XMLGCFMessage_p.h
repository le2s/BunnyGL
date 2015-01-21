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

#ifndef XML_GCF_MESSAGE_P_H
#define XML_GCF_MESSAGE_P_H

#include <GCF/Message>
#include <QDomDocument>

class AbstractMessageAdapter
{
public:
    static QDomElement encodeVariant(const QVariant& variant, QDomDocument doc);
    static QVariant extractVariant(QDomElement element);

    virtual ~AbstractMessageAdapter() { }

    GCF::Message::MessageType messageType() const { return m_type; }
    GCF::Message* fromDOM(QDomDocument doc);
    QDomDocument toDOM(const GCF::Message* message);

protected:
    AbstractMessageAdapter(GCF::Message::MessageType type) : m_type(type) { }
    virtual void readBody(QDomDocument doc, GCF::Message* message) = 0;
    virtual void writeBody(const GCF::Message* message, QDomDocument doc) = 0;
    virtual GCF::Message* createMessage() = 0;

private:
    void readHeader(QDomDocument doc, GCF::Message* message);
    void readFooter(QDomDocument doc, GCF::Message* message);

    void writeHeader(const GCF::Message* message, QDomDocument doc);
    void writeFooter(const GCF::Message* message, QDomDocument doc);

private:
    GCF::Message::MessageType m_type;
};


class AsyncInvokeMethodMessageAdapter : public AbstractMessageAdapter
{
public:
    AsyncInvokeMethodMessageAdapter();
    ~AsyncInvokeMethodMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class AsyncInvokeMethodResMessageAdapter : public AbstractMessageAdapter
{
public:
    AsyncInvokeMethodResMessageAdapter();
    ~AsyncInvokeMethodResMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class AsyncInvokeMethodsMessageAdapter : public AbstractMessageAdapter
{
public:
    AsyncInvokeMethodsMessageAdapter();
    ~AsyncInvokeMethodsMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class AsyncInvokeMethodsResMessageAdapter : public AbstractMessageAdapter
{
public:
    AsyncInvokeMethodsResMessageAdapter();
    ~AsyncInvokeMethodsResMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class RequestAccessMessageAdapter : public AbstractMessageAdapter
{
public:
    RequestAccessMessageAdapter();
    ~RequestAccessMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class RequestAccessResMessageAdapter : public AbstractMessageAdapter
{
public:
    RequestAccessResMessageAdapter();
    ~RequestAccessResMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class RequestObjectMessageAdapter : public AbstractMessageAdapter
{
public:
    RequestObjectMessageAdapter();
    ~RequestObjectMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class RequestObjectResMessageAdapter : public AbstractMessageAdapter
{
public:
    RequestObjectResMessageAdapter();
    ~RequestObjectResMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class GetPropertyNamesMessageAdapter : public AbstractMessageAdapter
{
public:
    GetPropertyNamesMessageAdapter();
    ~GetPropertyNamesMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class GetPropertyNamesResMessageAdapter : public AbstractMessageAdapter
{
public:
    GetPropertyNamesResMessageAdapter();
    ~GetPropertyNamesResMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class InvokeMethodMessageAdapter : public AbstractMessageAdapter
{
public:
    InvokeMethodMessageAdapter();
    ~InvokeMethodMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class InvokeMethodResMessageAdapter : public AbstractMessageAdapter
{
public:
    InvokeMethodResMessageAdapter();
    ~InvokeMethodResMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class RequestConnectionMessageAdapter : public AbstractMessageAdapter
{
public:
    RequestConnectionMessageAdapter();
    ~RequestConnectionMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class RequestConnectionResMessageAdapter : public AbstractMessageAdapter
{
public:
    RequestConnectionResMessageAdapter();
    ~RequestConnectionResMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class SignalDeliveryMessageAdapter : public AbstractMessageAdapter
{
public:
    SignalDeliveryMessageAdapter();
    ~SignalDeliveryMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class ReleaseObjectMessageAdapter : public AbstractMessageAdapter
{
public:
    ReleaseObjectMessageAdapter();
    ~ReleaseObjectMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class ReleaseObjectResMessageAdapter : public AbstractMessageAdapter
{
public:
    ReleaseObjectResMessageAdapter();
    ~ReleaseObjectResMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class ReleaseAccessMessageAdapter : public AbstractMessageAdapter
{
public:
    ReleaseAccessMessageAdapter();
    ~ReleaseAccessMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class ReleaseAccessResMessageAdapter : public AbstractMessageAdapter
{
public:
    ReleaseAccessResMessageAdapter();
    ~ReleaseAccessResMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class ReleaseConnectionMessageAdapter : public AbstractMessageAdapter
{
public:
    ReleaseConnectionMessageAdapter();
    ~ReleaseConnectionMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

class ReleaseConnectionResMessageAdapter : public AbstractMessageAdapter
{
public:
    ReleaseConnectionResMessageAdapter();
    ~ReleaseConnectionResMessageAdapter();

    void readBody(QDomDocument doc, GCF::Message* message);
    void writeBody(const GCF::Message* message, QDomDocument doc);
    GCF::Message* createMessage();
};

#endif

