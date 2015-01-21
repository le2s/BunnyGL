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

#ifndef REQUESTFORWARDER_H
#define REQUESTFORWARDER_H

#include <QObject>
#include <QLocalSocket>
#include <GCF/Message>
#include <GCF/IMessageEncoderDecoder>

class QTcpSocket;
class RequestHandlerPool;

struct RequestForwarderData;
class RequestForwarder : public QObject
{
    Q_OBJECT

public:
    RequestForwarder(QTcpSocket* socket, RequestHandlerPool* pool, QObject* parent = 0);
    ~RequestForwarder();

    QTcpSocket* clientConnection() const;
    RequestHandlerPool* requestHandlerPool() const;
    QString clientGUID() const;

private slots:
    void readRequest();
    void tcpSocketDisconnected();
    void tcpSocketConnected();

protected:
    bool event(QEvent* event);
    void timerEvent(QTimerEvent *);

private:
    RequestForwarderData* d;
};

struct RequestForwarderHelperData;
class RequestForwarderHelper : public QObject
{
    Q_OBJECT

public:
    RequestForwarderHelper(GCF::Message* requestMessage, IMessageEncoderDecoder* messageEncoderDecoder, RequestForwarder* parent);
    ~RequestForwarderHelper();

private slots:
    void forwardRequest();
    void forwardRequest2();
    void forwardResponse();

    void tcpSocketDisconnected();
    void localSocketDisconnected();

protected:
    void forwardErrorResponse(const QString& errMsg=QString());
    void forwardResponse(GCF::Message* response);

private:
    RequestForwarderHelperData* d;
};

#endif // REQUESTFORWARDER_H
