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

#include "RequestForwarder.h"
#include "RequestHandlerPool.h"

#include <QEvent>
#include <QQueue>
#include <QTcpSocket>
#include <QLocalSocket>
#include <QHostAddress>
#include <QApplication>
#include <QTimerEvent>
#include <QBasicTimer>

#include <GCF/Message>
#include <GCF/IMessageEncoderDecoder>
#include <GCF/AppAccessPoint>
#include <GCF/MethodInvocationHelper>

#include "ApplicationServerComponent.h"

struct RequestForwarderData
{
    RequestForwarderData() : tcpSocket(0),
        requestHandlerPool(0){ }

    QTcpSocket* tcpSocket;
    RequestHandlerPool* requestHandlerPool;
    QString clientInfo;

    // Data structures for dealing with requests
    QString clientGUID;
    QByteArray readBuffer;
    QBasicTimer readTimeout;
};

const int ReadTimeout = 10000;

RequestForwarder::RequestForwarder(QTcpSocket* socket, RequestHandlerPool* pool, QObject* parent)
    :QObject(parent)
{
    d = new RequestForwarderData;

    d->tcpSocket = socket;
    d->tcpSocket->setParent(this);

    d->requestHandlerPool = pool;
    d->clientInfo = QString("%1:%2")
            .arg(d->tcpSocket->peerAddress().toString())
            .arg(d->tcpSocket->peerPort());

    connect(d->tcpSocket, SIGNAL(readyRead()), this, SLOT(readRequest()));
    connect(d->tcpSocket, SIGNAL(disconnected()), this, SLOT(tcpSocketDisconnected()));

    if(d->tcpSocket->state() == QTcpSocket::ConnectedState)
        this->tcpSocketConnected();
    else
        connect(d->tcpSocket, SIGNAL(connected()), this, SLOT(tcpSocketConnected()));
}

RequestForwarder::~RequestForwarder()
{
    delete d;
}

QTcpSocket* RequestForwarder::clientConnection() const
{
    return d->tcpSocket;
}

RequestHandlerPool* RequestForwarder::requestHandlerPool() const
{
    return d->requestHandlerPool;
}

QString RequestForwarder::clientGUID() const
{
    return d->clientGUID;
}

void RequestForwarder::readRequest()
{
    // We first stop the read-timeout timer, because we have already read some data now.
    // We will restart the timer later if we figure that the complete request has
    // has not yet been read from the client.
    d->readTimeout.stop();

    // Read from the socket
    d->readBuffer += d->tcpSocket->readAll();

    // Try to figure out the request message
    while(!d->readBuffer.isEmpty())
    {
        GCF::Message* requestMessage = 0;
        IMessageEncoderDecoder* messageEncoderDecoder = GCF::AppAccessPoint::instance().messageEncoderDecoderFor(d->readBuffer);
        if(messageEncoderDecoder)
        {
            requestMessage = messageEncoderDecoder->decodeMessage( d->readBuffer );
            if(!requestMessage)
            {
                // Reset the read-time out timer. This means, that if
                // we dont receive any more data from the client even
                // after waiting for 10 long seconds, then perhaps we
                // are unable to decode the message. Maybe the client
                // sent the message in the wrong format.
                // We will need to just close the socket connection
                // and forget about it.
                d->readTimeout.start(ReadTimeout, this);
                break;
            }
        }
        else
            break;

        // Process the newly read request message
        if(requestMessage)
        {
            // Store some meta-data into the requestMessage
            requestMessage->customData().addKey( "ClientIP", d->tcpSocket->peerAddress().toString() );
            requestMessage->customData().addKey( "ClientPort", d->tcpSocket->peerPort() );
            requestMessage->customData().addKey( "ServerIP", d->tcpSocket->localAddress().toString() );
            requestMessage->customData().addKey( "ServerPort", d->tcpSocket->localPort() );
            if( !d->tcpSocket->dynamicPropertyNames().contains("SingleRequestClient") )
            {
                bool src = (requestMessage->messageType() == GCF::Message::AsyncInvokeMethod ||
                            requestMessage->messageType() == GCF::Message::AsyncInvokeMethods );
                if( requestMessage->customData().hasKey("KeepAlive") &&
                        requestMessage->customData().value("KeepAlive").toBool() )
                    src = false;
                d->tcpSocket->setProperty("SingleRequestClient", src);
                if(src)
                    GCF_AS_BROKER_LOG( QString("Client %1 is a Single Request Client").arg(d->clientInfo) );
            }

            if( d->clientGUID.isEmpty() )
            {
                d->clientGUID = requestMessage->customData().value("ClientGUID").toString();
                if(d->clientGUID.isEmpty())
                {
                    d->clientGUID = d->clientInfo;
                    requestMessage->customData().addKey("ClientGUID", d->clientGUID);
                }
            }

            new RequestForwarderHelper(requestMessage, messageEncoderDecoder, this);
        }
    }
}

void RequestForwarder::tcpSocketDisconnected()
{
    GCF_AS_BROKER_LOG( QString("Connection from client %1 closed").arg(d->clientInfo) );
    d->readTimeout.stop();
    d->tcpSocket = 0;
    this->deleteLater();
}

void RequestForwarder::tcpSocketConnected()
{
    /*
      Previously we used to start a timer and wait for 10 seconds for a
      message to show up. If no message showed up from the client - then
      we would disconnect the client.

      Because of this - we could create a remote application connection.
      But within 10 seconds if the client doesnt make any call - then the
      connection is gone!

      I think we should apply the read timeout only during "read".
      */

#if 0
    // Reset the read-time out timer. This means, that if
    // we dont receive any more data from the client even
    // after waiting for 10 long seconds, then perhaps we
    // are unable to decode the message. Maybe the client
    // sent the message in the wrong format.
    // We will need to just close the socket connection
    // and forget about it.
    d->readTimeout.stop();
    d->readTimeout.start(ReadTimeout, this);
#endif
}

bool RequestForwarder::event(QEvent* event)
{
    return QObject::event(event);
}

void RequestForwarder::timerEvent(QTimerEvent *te)
{
    if(d->readTimeout.isActive() && te->timerId() == d->readTimeout.timerId())
    {
        GCF_AS_BROKER_LOG( QString("Client %1 is being closed by the server due to readTimeout").arg(d->clientInfo) );
        d->readTimeout.stop();
        d->tcpSocket->close();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// RequestForwarderHelper implementation
///////////////////////////////////////////////////////////////////////////////////////////////////

struct RequestForwarderHelperData
{
    RequestForwarderHelperData() : tcpSocket(0), localSocket(0),
        requestHandlerPool(0), messageEncoderDecoder(0),
        requestMessage(0) {}

    QTcpSocket* tcpSocket;
    RequestorSocket* localSocket;
    RequestHandlerPool* requestHandlerPool;
    QString clientGUID;

    // Data structures for dealing with request
    IMessageEncoderDecoder* messageEncoderDecoder;
    GCF::Message* requestMessage;

    GCF::MethodTag methodTagForMessage(GCF::Message* message);
    void updateCallHistory(GCF::Message* message);
};

RequestForwarderHelper::RequestForwarderHelper(GCF::Message* requestMessage,
                                               IMessageEncoderDecoder* messageEncoderDecoder,
                                               RequestForwarder* parent)
    :QObject(parent)
{
    d = new RequestForwarderHelperData;

    d->requestMessage = requestMessage;
    d->messageEncoderDecoder = messageEncoderDecoder;
    d->requestMessage->customData().addKey("EncoderDecoderIndex",
                                           GCF::AppAccessPoint::instance().indexOfMessageEncoderDecoder(d->messageEncoderDecoder));

    if(parent)
    {
        d->tcpSocket = parent->clientConnection();
        d->requestHandlerPool = parent->requestHandlerPool();
        d->clientGUID = parent->clientGUID();

        connect(d->tcpSocket, SIGNAL(disconnected()), this, SLOT(tcpSocketDisconnected()));

        QMetaObject::invokeMethod(this, "forwardRequest", Qt::QueuedConnection);
    }
    else
        this->deleteLater();
}

RequestForwarderHelper::~RequestForwarderHelper()
{
    if(d->requestMessage)
    {
        this->forwardErrorResponse();
        delete d->requestMessage;
    }

    if(d->localSocket && d->requestHandlerPool)
        d->requestHandlerPool->freeRequestHandlerConnection(d->localSocket);

    if(d->tcpSocket)
    {
        bool src = d->tcpSocket->dynamicPropertyNames().contains("SingleRequestClient") &&
                d->tcpSocket->property("SingleRequestClient").toBool();
        if(src)
        {
            d->tcpSocket->disconnect();
            d->tcpSocket->close();
        }
    }

    delete d;
}

void RequestForwarderHelper::forwardRequest()
{
    GCF_AS_BROKER_DEBUG("Handling message " + d->requestMessage->toString());

    d->updateCallHistory(d->requestMessage);

    GCF::MethodTag methodTag = d->methodTagForMessage(d->requestMessage);
    if( methodTag == GCF::InvalidTag )
    {
        this->forwardErrorResponse();
        delete d->requestMessage;
        d->requestMessage = 0;
        this->deleteLater();
        return;
    }

    if( methodTag == GCF::AtomicWSTag || methodTag == GCF::UnknownTag )
    {
        GCF::Message* reply = GCF::AppAccessPoint::instance().handleMessage(d->requestMessage);

        if(reply)
        {
            reply->includeErrorInfo();
            GCF_AS_BROKER_LOG( QString("Forwarding response (%1 bytes) to %2:%3")
                               .arg( reply->toByteArray().size() )
                               .arg( d->requestMessage->customData()["ClientIP"].toString() )
                               .arg( d->requestMessage->customData()["ClientPort"].toString() ) );
        }

        this->forwardResponse(reply);
        delete d->requestMessage;
        d->requestMessage = 0;
        this->deleteLater();
        return;
    }

    // Create a local socket connection to the child process
    // if that has not already been created.
    if(!d->localSocket)
    {
        GCF_AS_BROKER_LOG( QString("Looking for free process to handle request of type %1 from %2:%3")
                           .arg( d->requestMessage->messageType() )
                           .arg( d->requestMessage->customData()["ClientIP"].toString() )
                           .arg( d->requestMessage->customData()["ClientPort"].toString() ) );

        QString guid = d->clientGUID;
        if(methodTag == GCF::ClientIndependentWSTag)
            guid.clear();

        d->localSocket = d->requestHandlerPool->getRequestHandlerConnection(guid);
        if(!d->localSocket)
        {
            GCF_AS_BROKER_LOG( QString("Unable to handle request at the moment. Server busy") );
            this->forwardErrorResponse(QString("Unable to handle request at the moment. Server busy"));
            delete d->requestMessage;
            d->requestMessage = 0;
            this->deleteLater();
            return;
        }

        d->localSocket->setParent(this);

        connect(d->localSocket, SIGNAL(readyRead()), this, SLOT(forwardResponse()));
        connect(d->localSocket, SIGNAL(disconnected()), this, SLOT(localSocketDisconnected()));
        if(d->localSocket->state() != QLocalSocket::ConnectedState)
            connect(d->localSocket, SIGNAL(connected()), this, SLOT(forwardRequest2()));
    }

    this->forwardRequest2();
}

void RequestForwarderHelper::forwardRequest2()
{
    if(d->localSocket && d->localSocket->state() == QLocalSocket::ConnectedState)
    {
        // Encode the request message in default GCF binary format
        IMessageEncoderDecoder* defaultEncoder = GCF::AppAccessPoint::instance().messageEncoderDecoderAt(0);
        QByteArray bytes = defaultEncoder->encodeMessage(d->requestMessage);
        GCF_AS_HANDLER_DEBUG( QString("Forwarding message ID %1 to %2")
                              .arg( d->requestMessage->id() )
                              .arg( d->localSocket->serverName() ) );

        // Send the request to the child process for handling.
        d->localSocket->doWrite(bytes);
        d->localSocket->flush();
    }
}

void RequestForwarderHelper::forwardResponse()
{
    /*
        We connect to the child process (the request-handler) via a local-socket.
        The child-process closes local socket connection immediately after
        handling one message. This is done to ensure that a child process
        doesnt get hogged by a single client. Now that we have received response
        for one message - we can expect that our local socket will be closed.

        When the local-socket gets closed, localSocketDisconnected() slot
        will get called causing the next request to be sent for processing.
        This should not happen, because the local socket is getting closed
        not because of error; but by design.

        So we need to break connection to localSocketDisconnected() from
        local-socket before hand.
    */
    disconnect(d->localSocket, 0, this,  0);
    d->requestHandlerPool->freeRequestHandlerConnection(d->localSocket);
    // connect(d->localSocket, SIGNAL(disconnected()), d->localSocket,  SLOT(deleteLater()));

    // Read the response
    QByteArray response;
    bool complete = false;

    while( d->localSocket->isOpen() && d->localSocket->bytesAvailable() && !complete )
    {
        response += d->localSocket->readAll();
        complete = d->messageEncoderDecoder->canDecodeMessage(response);
        qApp->processEvents();
    }

    if(!response.isEmpty())
    {
        GCF_AS_BROKER_LOG( QString("Forwarding response (%1 bytes) to %2:%3")
                           .arg( response.size() )
                           .arg( d->requestMessage->customData()["ClientIP"].toString() )
                           .arg( d->requestMessage->customData()["ClientPort"].toString() ) );

        // Send the response back to the client
        d->tcpSocket->write( response );
        d->tcpSocket->flush();
        d->tcpSocket->waitForBytesWritten();

        // Close if the socket is a single request socket
        bool src = d->tcpSocket->dynamicPropertyNames().contains("SingleRequestClient") &&
                d->tcpSocket->property("SingleRequestClient").toBool();
        if(src)
            d->tcpSocket->close();
    }

    // Cleanup resources
    d->localSocket->disconnect();
    d->localSocket->deleteLater();
    d->localSocket = 0;
    d->requestHandlerPool = 0;
    delete d->requestMessage;
    d->requestMessage = 0;

    this->deleteLater();
}

void RequestForwarderHelper::tcpSocketDisconnected()
{
    d->tcpSocket = 0;
    this->deleteLater();
}

void RequestForwarderHelper::localSocketDisconnected()
{
    if(d->requestMessage)
    {
        GCF_AS_BROKER_ERROR( QString("Server process unexpectedly closed while handling MessageID %1 from %2:%3 due to %4")
                             .arg( d->requestMessage->id() )
                             .arg( d->requestMessage->customData()["ClientIP"].toString() )
                             .arg( d->requestMessage->customData()["ClientPort"].toString() )
                             .arg( d->localSocket->errorString() ) );
    }

    disconnect(d->localSocket, 0, this, 0);
    this->deleteLater();
}

void RequestForwarderHelper::forwardErrorResponse(const QString &errMsg)
{
    QString errorMessageStr = errMsg;
    if(errorMessageStr.isEmpty())
        errorMessageStr = "Could not invoke message. Server process closed unexpectedly";

    if( !d->tcpSocket )
        return;

    // Prepare an error response for the message.
    GCF::Message* errorMessage = 0;
    if( d->requestMessage->messageType() == GCF::Message::AsyncInvokeMethod )
    {
        GCF::AsyncInvokeMethodMessage* msg = (GCF::AsyncInvokeMethodMessage*)d->requestMessage;

        errorMessage = new GCF::AsyncInvokeMethodResMessage(d->requestMessage);
        GCF::AsyncInvokeMethodResMessage* errMsg = (GCF::AsyncInvokeMethodResMessage*)errorMessage;
        errMsg->setObjectName( msg->objectName() );
        errMsg->setMethodName( msg->methodName() );
        errMsg->setRetValue(QVariant());
        errMsg->setSuccess(false);
        errMsg->setErrorMessage("Could not invoke message. Server process closed unexpectedly");
    }
    else if( d->requestMessage->messageType() == GCF::Message::AsyncInvokeMethods )
    {
        GCF::AsyncInvokeMethodsMessage* msg = (GCF::AsyncInvokeMethodsMessage*)d->requestMessage;

        errorMessage = new GCF::AsyncInvokeMethodsResMessage(d->requestMessage);
        GCF::AsyncInvokeMethodsResMessage* errMsg = (GCF::AsyncInvokeMethodsResMessage*)errorMessage;

        QList<GCF::Method> methodList = msg->invokeMethodList();
        Q_FOREACH(GCF::Method method, methodList)
        {
            method.Success = false;
            method.ErrorMessage = "Could not invoke message. Server process closed unexpectedly";
        }
        errMsg->setInvokedMethodList(methodList);
    }
    else
    {
        // Create a generic error message (which is nothing but an invalid message)
        errorMessage = new GCF::Message(d->requestMessage);
    }

    this->forwardResponse(errorMessage);
}

void RequestForwarderHelper::forwardResponse(GCF::Message* response)
{
    if(!d->tcpSocket || !d->tcpSocket->isOpen())
        return;

    // Forward the response
    QByteArray responseBytes;
    if(response && response->messageType() != GCF::Message::InvalidMessage)
        responseBytes = d->messageEncoderDecoder->encodeMessage(response);
    d->tcpSocket->write(responseBytes);
    d->tcpSocket->flush();
    d->tcpSocket->waitForBytesWritten();

    // Free memory used by messages
    delete response;
    
    // Close if the socket is a single request socket
    bool src = d->tcpSocket->dynamicPropertyNames().contains("SingleRequestClient") &&
            d->tcpSocket->property("SingleRequestClient").toBool();
    if(src)
        d->tcpSocket->close();
}

GCF::MethodTag RequestForwarderHelperData::methodTagForMessage(GCF::Message* message)
{
    if(!message)
        return GCF::AtomicWSTag;

    if(message->messageType() == GCF::Message::AsyncInvokeMethod)
    {
        GCF::AsyncInvokeMethodMessage* msg = (GCF::AsyncInvokeMethodMessage*)message;
        GCF::AbstractComponent* owner = 0;
        QObject* reqObjectPtr = GCF::AppAccessPoint::instance().findObject(msg->objectName(), &owner, true);
        int methodIdx = GCF::findMethod(reqObjectPtr, msg->methodName(), msg->argList());

        return GCF::methodTag(reqObjectPtr, methodIdx);
    }
    else if(message->messageType() == GCF::Message::AsyncInvokeMethods)
    {
        GCF::AsyncInvokeMethodsMessage* msg = (GCF::AsyncInvokeMethodsMessage*)message;

        // All the methods in the list must have the same tag.
        // Otherwise GCF::NormalWSTag will be returned
        int firstMethodTag = -1;
        Q_FOREACH(GCF::Method method, msg->invokeMethodList())
        {
            GCF::AbstractComponent* owner = 0;
            QObject* reqObjectPtr = GCF::AppAccessPoint::instance().findObject(method.ObjectName, &owner, true);
            int methodIdx = GCF::findMethod(reqObjectPtr, method.MethodName, method.Args);

            if(firstMethodTag < 0)
            {
                firstMethodTag = GCF::methodTag(reqObjectPtr, methodIdx);
                continue;
            }

            if(firstMethodTag != GCF::methodTag(reqObjectPtr, methodIdx))
                return GCF::NormalWSTag;
        }

        return (GCF::MethodTag)firstMethodTag;
    }

    return GCF::AtomicWSTag;
}

void RequestForwarderHelperData::updateCallHistory(GCF::Message* message)
{
    if(!message)
        return;

    if(message->messageType() == GCF::Message::AsyncInvokeMethod)
    {
        GCF::AsyncInvokeMethodMessage* msg = (GCF::AsyncInvokeMethodMessage*)message;
        GCF::AbstractComponent* owner = 0;
        QObject* reqObjectPtr = GCF::AppAccessPoint::instance().findObject(msg->objectName(), &owner, true);

        if(!reqObjectPtr)
            return;

        // Save the method call counter in reqObjectPtr.
        GCF::DataStore callCounterStore;
        if( reqObjectPtr->dynamicPropertyNames().contains("_CallCounterStore_") )
            callCounterStore = reqObjectPtr->property("_CallCounterStore_").value<GCF::DataStore>();

        int count = 0;
        if( callCounterStore.hasKey(msg->methodName()) )
            count = callCounterStore.value(msg->methodName()).toInt();
        ++count;
        callCounterStore.addKey(msg->methodName(), count);
        if(count == 1)
            reqObjectPtr->setProperty("_CallCounterStore_", qVariantFromValue<GCF::DataStore>(callCounterStore));
    }
    else if(message->messageType() == GCF::Message::AsyncInvokeMethods)
    {
        GCF::AsyncInvokeMethodsMessage* msg = (GCF::AsyncInvokeMethodsMessage*)message;

        Q_FOREACH(GCF::Method method, msg->invokeMethodList())
        {
            GCF::AbstractComponent* owner = 0;
            QObject* reqObjectPtr = GCF::AppAccessPoint::instance().findObject(method.ObjectName, &owner, true);

            if(!reqObjectPtr)
                continue;

            // Save the method call counter in reqObjectPtr.
            GCF::DataStore callCounterStore;
            if( reqObjectPtr->dynamicPropertyNames().contains("_CallCounterStore_") )
                callCounterStore = reqObjectPtr->property("_CallCounterStore_").value<GCF::DataStore>();

            int count = 0;
            if( callCounterStore.hasKey(method.MethodName) )
                count = callCounterStore.value(method.MethodName).toInt();
            ++count;
            callCounterStore.addKey(method.MethodName, count);
            if(count == 1)
                reqObjectPtr->setProperty("_CallCounterStore_", qVariantFromValue<GCF::DataStore>(callCounterStore));
        }
    }
}


