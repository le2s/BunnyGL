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

#include <GCF/RemoteAppAccessPoint>
#include <GCF/Message>
#include <GCF/RemoteObject>
#include <GCF/AppAccessPointComponent>
#include <GCF/AppAccessPoint>

#include "MutexHelper.h"
#include "RemoteAppAccessPoint_p.h"

#include <QApplication>
#include <QHostAddress>
#include <QTimer>
#include <QThread>
#include <QEventLoop>

// All instances of GCF::RemoteAppAccessPoint are managed by an
// instance of this class.

///////////////////////////////////////////////////////////////////////////////

namespace GCF
{
    struct RemoteAppAccessPointManager : QList<GCF::RemoteAppAccessPoint*>
    {
        ~RemoteAppAccessPointManager() {
            QList<GCF::RemoteAppAccessPoint*> copy = *this;
            this->clear();

            for(int i=0; i<copy.count(); i++)
                copy[i]->deleteLater();
        }

        GCF::LANRemoteAppAccessPoint* findRAP(const QHostAddress& address, quint16 port) const {
            QList<GCF::RemoteAppAccessPoint*> copy(*this);
            Q_FOREACH(GCF::RemoteAppAccessPoint* rap, copy) {
                GCF::LANRemoteAppAccessPoint* lan_rap =
                    qobject_cast<GCF::LANRemoteAppAccessPoint*>(rap);
                if(!lan_rap)
                    continue;

                if(lan_rap->appAddress() == address && lan_rap->appPort() == port)
                    return lan_rap;
            }

            return 0;
        }

        GCF::HttpRemoteAppAccessPoint* findRAP(const QUrl& url) const {
            QList<GCF::RemoteAppAccessPoint*> copy(*this);
            Q_FOREACH(GCF::RemoteAppAccessPoint* rap, copy) {
                GCF::HttpRemoteAppAccessPoint* http_rap =
                    qobject_cast<GCF::HttpRemoteAppAccessPoint*>(rap);
                if(!http_rap)
                    continue;

                if(http_rap->appUrl() == url)
                    return http_rap;

                return 0;
            }

            return 0;
        }
    };

    static RemoteAppAccessPointManager& RAPManager()
    {
        static RemoteAppAccessPointManager theManager;
        return theManager;
    }
}

///////////////////////////////////////////////////////////////////////////////

/**
\class GCF::RemoteAppAccessPoint GCF/RemoteAppAccessPoint
\ingroup gcfipc
\brief Manages connection to a remote application

This class helps an appliction to connect to a remote application. All
communication with that remote application will be routed via this class.

You can have only once instance of this class for a remote application.
Use the \ref getRemoteAppAccessPoint() method to get a pointer to an
instance of this class for a remote application.

Use deleteLater() method to destroy instances of this class. The constructor
and destructor of this class is made protected to ensure property object
creation and destruction.
*/

/**
Returns a pointer to an instance of \ref GCF::RemoteAppAccessPoint
for a given host address and port. The function ensures that only once
instance of \ref GCF::RemoteAppAccessPoint is created for every
(address, port) pair.

\note useSsl is ignored as of now.
*/
GCF::RemoteAppAccessPoint* GCF::RemoteAppAccessPoint::getRemoteAppAccessPoint(const QHostAddress& address, quint16 port, bool useSsl)
{
    GCF::LANRemoteAppAccessPoint* lan_rap = RAPManager().findRAP(address, port);
    if(lan_rap)
        return lan_rap;

    lan_rap = new GCF::LANRemoteAppAccessPoint;
    lan_rap->connectToApp(address, port, useSsl);

    return lan_rap;
}

/**
Returns a pointer to an instance of \ref GCF::RemoteAppAccessPoint
for a given URL. This function ensures that only one instance of
\ref GCF::RemoteAppAccessPoint is created for every URL.
*/
GCF::RemoteAppAccessPoint* GCF::RemoteAppAccessPoint::getRemoteAppAccessPoint(const QUrl& url)
{
    GCF::HttpRemoteAppAccessPoint* http_rap = RAPManager().findRAP(url);
    if(http_rap)
        return http_rap;

    http_rap = new GCF::HttpRemoteAppAccessPoint;
    http_rap->connectToApp(url);

    return http_rap;
}

/**
This function returns a list of all remote application access points
in the system. The returned list may consist of both active and inactive
RAPs.
*/
QList<GCF::RemoteAppAccessPoint*> GCF::RemoteAppAccessPoint::getRemoteAppAccessPointList()
{
    return RAPManager();
}

/**
This function returns a list of all remote application access points
in the system. The returned list may consist of both active and inactive
RAPs.
*/
void GCF::RemoteAppAccessPoint::closeAllRemoteAccessPoints()
{
    QList<GCF::RemoteAppAccessPoint*> rapList = RAPManager();
    RAPManager().clear();

    for(int i=0; i<rapList.count(); i++)
    {
        rapList[i]->disconnect();
        rapList[i]->deleteLater();
    }
}

/**
\todo
*/
bool GCF::RemoteAppAccessPoint::asyncCall(QObject* notifyObject, const char* member,
                 const QHostAddress& address, quint16 port,
                 const QString& objectName, const QString& method,
                 const QVariant& arg1, const QVariant& arg2,
                 const QVariant& arg3, const QVariant& arg4,
                 const QVariant& arg5, const QVariant& arg6,
                 const QVariant& arg7, const QVariant& arg8)
{
    QVariantList args;
    if( arg1.isValid() )
        args << arg1;
    if( arg2.isValid() )
        args << arg2;
    if( arg3.isValid() )
        args << arg3;
    if( arg4.isValid() )
        args << arg4;
    if( arg5.isValid() )
        args << arg5;
    if( arg6.isValid() )
        args << arg6;
    if( arg7.isValid() )
        args << arg7;
    if( arg8.isValid() )
        args << arg8;

    return GCF::RemoteAppAccessPoint::asyncCall(notifyObject, member, address, port, objectName, method, args);
}

/**
\todo
*/
bool GCF::RemoteAppAccessPoint::asyncCall(QObject* notifyObject, const char* member,
                 const QHostAddress& address, quint16 port,
                 const QString& objectName, const QString& method,
                 const QVariantList& args)
{
    QString memberStr = QMetaObject::normalizedSignature(member+1);
    QString paramStr = memberStr.section('(', 1, 1).section(')', 0, 0);
    QStringList params = paramStr.split(',', QString::SkipEmptyParts);
    if( params.count() != 2 )
        return false;

    if( !(params.first().trimmed() == "QVariant" && params.last().trimmed() == "bool") )
        return false;

    new AsyncCaller(qApp, notifyObject, member, address, port, objectName, method, args);
    return true;
}

/**
This function helps make an asynchrnonous call to \c methodName with arguments passed
via parameters arg1 through arg8. When the response to the method call becomes available,
the \c member method in \c notify object is called.
*/
bool GCF::RemoteAppAccessPoint::asyncCall(QObject* notifyObject, const char* member,
               const QString& objectName, const QString& methodName,
               const QVariant& arg1, const QVariant& arg2,
               const QVariant& arg3, const QVariant& arg4,
               const QVariant& arg5, const QVariant& arg6,
               const QVariant& arg7, const QVariant& arg8)
{
    return GCF::RemoteAppAccessPoint::asyncCall(notifyObject, member,
            this->peerAddress(), this->peerPort(),
            objectName, methodName, arg1, arg2, arg3,
            arg4, arg5, arg6, arg7, arg8);
}

/**
This function helps make an asynchrnonous call to \c methodName with arguments passed
via parameters \c args list. When the response to the method call becomes available,
the \c member method in \c notify object is called.
*/
bool GCF::RemoteAppAccessPoint::asyncCall(QObject* notifyObject, const char* member,
               const QString& objectName, const QString& methodName, const QVariantList& args)
{
    return GCF::RemoteAppAccessPoint::asyncCall(notifyObject, member,
            this->peerAddress(), this->peerPort(), objectName, methodName, args);
}

///////////////////////////////////////////////////////////////////////////////

namespace GCF
{
    struct RemoteAppAccessPointData
    {
        RemoteAppAccessPointData() : accessGranted(false),
            sessionId(-1) { }

        GCF::DataStore customData;
        QMap<QString,GCF::RemoteObject*> remoteObjectMap;
        QMap<QObject*,QString> remoteObjectMap2;
        bool accessGranted;
        qint32 sessionId;

        // Manage sent messages
        QList<GCF::Message*> sentMessages;
        QMutex sendMessageLock;
        GCF::Message* findSentMessage(qint32 msgId) {
            for(int i=0; i<sentMessages.count(); i++)
                if( sentMessages[i]->id() == msgId )
                    return sentMessages[i];
            return 0;
        }

        // Managing pending messages
        QMutex pendingMessageLock;
        QList<GCF::Message*> pendingMessages;
        void addPendingMessage(GCF::Message* msg) {
            pendingMessageLock.lock();
            if( !pendingMessages.contains(msg) )
                pendingMessages.append(msg);
            pendingMessageLock.unlock();
        }
    };
}

/**
\internal
*/
GCF::RemoteAppAccessPoint::RemoteAppAccessPoint(QObject* parent)
: QObject(parent)
{
    d = new GCF::RemoteAppAccessPointData;
    QTimer::singleShot(20000, this, SLOT(checkConnectionStatus()));

    RAPManager().append(this);
}

/**
\internal
*/
GCF::RemoteAppAccessPoint::~RemoteAppAccessPoint()
{
    RAPManager().removeAll(this);

    disconnect();
    delete d;
}

/**
Returns true if this remote application access point is valid. A valid
RAP is one that has already established connection with the remote
application and is now ready to send and receive messages.
*/
bool GCF::RemoteAppAccessPoint::isValid() const
{
    return d->accessGranted && this->isValidInternal();
}

/**
Disconnects this remote application access point. After disconnection
the \ref disconnectedFromApp() signal is emitted.

\note All pending messages will be cancelled.
\note All GCF::RemoteObjects created through this RAP will be deactivated.
*/
void GCF::RemoteAppAccessPoint::disconnect()
{
    discardPendingMessages();

    deactivateAllObjects();

    if(this->isValid())
    {
        this->disconnectInternal();
        emit disconnectedFromApp();
    }
    else
        emit couldNotConnectToApp();

    d->accessGranted = false;

}

/**
\internal
  */
QHostAddress GCF::RemoteAppAccessPoint::localAddress() const
{
    return QHostAddress();
}

/**
\internal
  */
quint16 GCF::RemoteAppAccessPoint::localPort() const
{
    return 0;
}

/**
\internal
  */
QHostAddress GCF::RemoteAppAccessPoint::peerAddress() const
{
    return QHostAddress();
}

/**
\internal
  */
quint16 GCF::RemoteAppAccessPoint::peerPort() const
{
    return 0;
}

/**
Provides access to a data-store in which you can store custom key-value
pairs associated with this RAP.
*/
GCF::DataStore GCF::RemoteAppAccessPoint::customData() const
{
    return d->customData;
}

/**
Use this method to make a function call on an object without actually
having to fetch a GCF::RemoteObject. This can be used for making
pure-service calls.

Take a look at the following code for example:

\code
QUrl url("http://www.myserver.com:8080");
GCF::RemoteAppAccessPoint* rap = GCF::RemoteAppAccessPoint::getRemoteAppAccessPoint(url);
GCF::RemoteObject* rObj = rap->getRemoteObject("Component.Object");
rObj->call("methodName", arg1, arg2...);

QObject::connect(rObj, SIGNAL(methodInvocationResult(qint32,bool,QVariant,QString,QString)),
                 receiver, SLOT(slotMethodInvocationResult(qint32,bool,QVariant,QString,QString)));
\endcode

The above code can now be written as,

\code
QUrl url("http://www.myserver.com:8080");
GCF::RemoteAppAccessPoint* rap = GCF::RemoteAppAccessPoint::getRemoteAppAccessPoint(url);
rap->call("Component.Object", "methodName", arg1, arg2, ...);

QObject::connect(rap, SIGNAL(methodInvocationResult(qint32,bool,QVariant,QString,QString)),
                 receiver, SLOT(slotMethodInvocationResult(qint32,bool,QVariant,QString,QString)));
\endcode
*/
qint32 GCF::RemoteAppAccessPoint::call(const QString& objectName, const QString& method,
                     const QVariant& arg1, const QVariant& arg2,
                     const QVariant& arg3, const QVariant& arg4,
                     const QVariant& arg5, const QVariant& arg6,
                     const QVariant& arg7, const QVariant& arg8)
{
    QVariantList args;
    if( arg1.isValid() )
        args << arg1;
    if( arg2.isValid() )
        args << arg2;
    if( arg3.isValid() )
        args << arg3;
    if( arg4.isValid() )
        args << arg4;
    if( arg5.isValid() )
        args << arg5;
    if( arg6.isValid() )
        args << arg6;
    if( arg7.isValid() )
        args << arg7;
    if( arg8.isValid() )
        args << arg8;

    return this->call(objectName, method, args);
}

/**
Use this method to make a function call on an object without actually
having to fetch a GCF::RemoteObject. This can be used for making
pure-service calls.

Take a look at the following code for example:

\code
QUrl url("http://www.myserver.com:8080");
GCF::RemoteAppAccessPoint* rap = GCF::RemoteAppAccessPoint::getRemoteAppAccessPoint(url);
GCF::RemoteObject* rObj = rap->getRemoteObject("Component.Object");
QVariantList args = QVariantList() << arg1 << arg2 ....;
rObj->call("methodName", args);

QObject::connect(rObj, SIGNAL(methodInvocationResult(qint32,bool,QVariant,QString,QString)),
                 receiver, SLOT(slotMethodInvocationResult(qint32,bool,QVariant,QString,QString)));
\endcode

The above code can now be written as,

\code
QUrl url("http://www.myserver.com:8080");
GCF::RemoteAppAccessPoint* rap = GCF::RemoteAppAccessPoint::getRemoteAppAccessPoint(url);
QVariantList args = QVariantList() << arg1 << arg2 ....;
rap->call("Component.Object", "methodName", args);

QObject::connect(rap, SIGNAL(methodInvocationResult(qint32,bool,QVariant,QString,QString)),
                 receiver, SLOT(slotMethodInvocationResult(qint32,bool,QVariant,QString,QString)));
\endcode
*/
qint32 GCF::RemoteAppAccessPoint::call(const QString& objectName, const QString& method, const QVariantList& args)
{
    GCF::AsyncInvokeMethodMessage* msg = new GCF::AsyncInvokeMethodMessage;
    msg->setObjectName(objectName);
    msg->setMethodName(method);
    msg->setArgList(args);

    qint32 msgId = msg->id();

    sendMessage(msg);

    return msgId;
}

/**
Fetches a pointer to a remote object whose name is as provided in
\c completeName.
*/
GCF::RemoteObject* GCF::RemoteAppAccessPoint::getRemoteObject(const QString& completeName)
{
    GCF::RemoteObject* rObj = 0;

    if(d->remoteObjectMap.contains(completeName))
        rObj = d->remoteObjectMap[completeName];

    if(!rObj)
    {
        rObj = new GCF::RemoteObject(this);
        rObj->setObjectName(completeName);

        d->remoteObjectMap[completeName] = rObj;
        d->remoteObjectMap2[rObj] = completeName;

        connect(rObj, SIGNAL(destroyed(QObject*)),
                this, SLOT(remoteObjectDestroyed(QObject*)));
    }

    this->activateObject(rObj);

    return rObj;
}

/**
\internal
*/
void GCF::RemoteAppAccessPoint::sendMessage(GCF::Message* message)
{
    if(!message)
        return;

    // Only RequestAccess messages can be sent when RAP is in-valid.
    if( !isValid() && message->messageType() != GCF::Message::RequestAccess )
    {
        delete message;
        return;
    }

    // Only request messages can be sent..
    if( message->messageDirection() != GCF::Message::ClientToServer &&
        message->messageDirection() != GCF::Message::Both )
    {
        delete message;
        return;
    }

    // Set the session ID
    message->setSessionId(d->sessionId);

    // Try to acquire the send message lock. If thats not possible then
    // queue the message for sending at a later stage and quit.
    if( !d->sendMessageLock.tryLock() )
    {
        d->addPendingMessage(message);
        // I wouldnt use this because, queued connections are scheduled really quick.
        // I would much rather use a QTimer for this
        // QMetaObject::invokeMethod(this, "sendPendingMessages", Qt::QueuedConnection);
        QTimer::singleShot(500, this, SLOT("sendPendingMessages()"));
        return;
    }

    // Prepare a GCF::MutexUnlocker to unlock when done
    GCF::MutexUnlocker unlocker(&(d->sendMessageLock));

    // Check if the message was already sent.
    if( d->sentMessages.contains(message) )
        return;

    // Actually send the message
    if( this->sendMessageInternal(message) )
    {
        // Register the sent messages
        d->sentMessages.append(message);

        // Log message if possible
        if( GCF::AppAccessPointComponent::instance().messageLog() )
        {
            QString log = QString("%1: Sent to %2 - ").arg(localInfo()).arg(peerInfo());
            log += message->toString();
            GCF::AppAccessPointComponent::instance().appendMessageLog( log );
        }
    }
    else
        delete message;
}

/**
\internal
*/
void GCF::RemoteAppAccessPoint::handleMessage(GCF::Message* msg)
{
    if(!msg)
        return;

    GCF::Message* sent = d->findSentMessage(msg->id());

    // Find out the session id.
    if(msg->messageType() == GCF::Message::RequestAccessRes)
        d->sessionId = msg->sessionId();

    // Handle the incoming message
    bool success = handleMessage(msg, sent);
    if( !success )
    {
        qDebug("Message ID %d of type %s was not handled", msg->id(),
            qPrintable(GCF::Message::messageTypeString(msg->messageType())));
    }

    // Delete both sent and received messages
    delete msg;

    d->sendMessageLock.lock();
    d->sentMessages.removeAll(sent);
    d->sendMessageLock.unlock();
    delete sent;
}

/**
\internal
*/
bool GCF::RemoteAppAccessPoint::handleMessage(Message* received, Message* sent)
{
    switch(received->messageType())
    {
    case GCF::Message::RequestAccessRes: {
        GCF::RequestAccessResMessage* msg = (GCF::RequestAccessResMessage*)received;
        if( msg->isAccessGranted() )
        {
            d->accessGranted = true;
            emit connectedToApp();
            qDebug("Connected to %s", qPrintable(peerInfo()));

            this->activateAllObjects();
        }
        else
            QMetaObject::invokeMethod(this, "disconnect", Qt::QueuedConnection);
            // d->socket->close();
        } break;

    case GCF::Message::RequestObjectRes: {
        GCF::RequestObjectResMessage* msg = (GCF::RequestObjectResMessage*)received;

        // from the sent message, figure out the GCF::RemoteObject pointer.
        GCF::RemoteObject* rObj = sent->customData()["GCF::RemoteObject"].value<GCF::RemoteObject*>();

        // Its is quite possible that the remote object was deleted, before
        // access grant had come to us. So, it is important to check for that
        // condition.
        if( d->remoteObjectMap2.contains(rObj) )
            rObj->setId( msg->objectId() );
        else
        {
            GCF::ReleaseObjectMessage* msg2 = new GCF::ReleaseObjectMessage;
            msg2->setObjectId( msg->objectId() );
            sendMessage(msg2);
        }
        } break;

    case GCF::Message::AsyncInvokeMethodRes: {
        GCF::AsyncInvokeMethodResMessage* msg = (GCF::AsyncInvokeMethodResMessage*)received;

        qint32 id = sent ? sent->id() : msg->id();
        emit methodInvocationResult(id, msg->isSuccess(), msg->retValue(), msg->objectName(), msg->methodName());
        } break;

    // For all object based messages, ask the respective object to deal with it.
    case GCF::Message::GetPropertyNamesRes:
    case GCF::Message::SetPropertyRes:
    case GCF::Message::GetPropertyRes:
    case GCF::Message::InvokeMethodRes:
    case GCF::Message::RequestConnectionRes:
    case GCF::Message::SignalDelivery:
    case GCF::Message::ReleaseObject:
    case GCF::Message::ReleaseObjectRes:
    case GCF::Message::ReleaseConnection:
    case GCF::Message::ReleaseConnectionRes: {
        GCF::RemoteObject* rObj = this->findObject( received->objectId() );
        if( !rObj )
            return false;

        if(received->messageType() == GCF::Message::ReleaseObject)
        {
            rObj->setId(-1);
            return true;
        }

        return rObj->handleMessage(received, sent);
        } break;
    default:
        return false;
    }

    return true;
}

/**
\internal
*/
void GCF::RemoteAppAccessPoint::activateObject(GCF::RemoteObject* rObj)
{
    // Send a request object message only if the object has been validated.
    // Otherwise, queue the message for later despatch.
    if( isValid() && !rObj->isActive() )
    {
        // Create a message to send to the server to request this object.
        GCF::RequestObjectMessage* msg = new GCF::RequestObjectMessage;
        msg->setObjectName(rObj->objectName());
        msg->customData()["GCF::RemoteObject"] = qVariantFromValue<GCF::RemoteObject*>(rObj);

        // Send the message
        this->sendMessage(msg);
    }
}

/**
\internal
*/
void GCF::RemoteAppAccessPoint::activateAllObjects()
{
    QList<GCF::RemoteObject*> remoteObjects = d->remoteObjectMap.values();
    Q_FOREACH(GCF::RemoteObject* rObj, remoteObjects)
        this->activateObject(rObj);
}

/**
\internal
*/
void GCF::RemoteAppAccessPoint::deactivateObject(GCF::RemoteObject* rObj)
{
    if(!rObj || !d->remoteObjectMap2.contains(rObj))
        return;

    rObj->setId(-1);
}

/**
\internal
*/
QList<GCF::RemoteObject*> GCF::RemoteAppAccessPoint::remoteObjects() const
{
    return d->remoteObjectMap.values();
}

/**
\internal
*/
GCF::RemoteObject* GCF::RemoteAppAccessPoint::findObject(quint32 id) const
{
    QList<GCF::RemoteObject*> remoteObjects = d->remoteObjectMap.values();
    Q_FOREACH(GCF::RemoteObject* rObj, remoteObjects)
        if(rObj->id() == (qint32)id)
            return rObj;

    return 0;
}

/**
\internal
*/
void GCF::RemoteAppAccessPoint::deactivateAllObjects()
{
    QList<GCF::RemoteObject*> remoteObjectList = d->remoteObjectMap.values();
    Q_FOREACH(GCF::RemoteObject* rObj, remoteObjectList)
        rObj->setId(-1);
}

/**
\internal
*/
void GCF::RemoteAppAccessPoint::remoteObjectDestroyed(QObject* obj)
{
    if( d->remoteObjectMap2.contains(obj) )
    {
        QString name = d->remoteObjectMap2[obj];

        d->remoteObjectMap2.remove(obj);
        d->remoteObjectMap.remove(name);
    }
}

/**
\internal
*/
void GCF::RemoteAppAccessPoint::sendPendingMessages()
{
    if( !d->pendingMessageLock.tryLock() )
    {
        // I wouldnt use this because, queued connections are scheduled really quick.
        // I would much rather use a QTimer for this
        // QMetaObject::invokeMethod(this, "sendPendingMessages", Qt::QueuedConnection);
        QTimer::singleShot(500, this, SLOT("sendPendingMessages()"));
        return;
    }

    QList<GCF::Message*> msgList = d->pendingMessages;
    d->pendingMessages.clear();
    d->pendingMessageLock.unlock();

    for(int i=0; i<msgList.count(); i++)
    {
        GCF::Message* msg = msgList[i];
        sendMessage(msg);
    }
}

/**
\internal
*/
void GCF::RemoteAppAccessPoint::discardPendingMessages()
{
    d->pendingMessageLock.lock();
    qDeleteAll(d->pendingMessages);
    d->pendingMessages.clear();
    d->pendingMessageLock.unlock();
}


void GCF::RemoteAppAccessPoint::checkConnectionStatus()
{
    if(this->isValid())
        return;

    emit couldNotConnectToApp();

    QList<GCF::RemoteObject*> remoteObjects = this->remoteObjects();
    for(int i=0; i<remoteObjects.count(); i++)
    {
        QMetaObject::invokeMethod( remoteObjects.at(i),
            "couldNotActivate", Qt::DirectConnection);
    }
}

///////////////////////////////////////////////////////////////////////////////

#include <QTcpSocket>

namespace GCF
{
    struct LANRemoteAppAccessPointData
    {
        LANRemoteAppAccessPointData() : socket(0), port(0) { }

        QTcpSocket* socket;
        QHostAddress address;
        quint16 port;

        QByteArray unprocessedData;
    };
}

GCF::LANRemoteAppAccessPoint::LANRemoteAppAccessPoint(QObject* parent)
: GCF::RemoteAppAccessPoint(parent)
{
    d = new LANRemoteAppAccessPointData;

    d->socket = new QTcpSocket(this);
}

GCF::LANRemoteAppAccessPoint::~LANRemoteAppAccessPoint()
{
    delete d;
}

void GCF::LANRemoteAppAccessPoint::connectToApp(const QHostAddress& address, quint16 port, bool useSsl)
{
    Q_UNUSED(useSsl);

    connect(d->socket, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(d->socket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(d->socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError()));
    connect(d->socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(slotStateChanged()));
    connect(d->socket, SIGNAL(aboutToClose()), this, SLOT(slotAboutToClose()));
    connect(d->socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

    d->socket->connectToHost(address, port);
    d->address = address;
    d->port = port;
}

QHostAddress GCF::LANRemoteAppAccessPoint::appAddress() const
{
    return d->address;
}

quint16 GCF::LANRemoteAppAccessPoint::appPort() const
{
    return d->port;
}

QAbstractSocket* GCF::LANRemoteAppAccessPoint::socket() const
{
    return d->socket;
}

QString GCF::LANRemoteAppAccessPoint::localInfo() const
{
    return QString("%1 (%2)").arg( d->socket->localAddress().toString() )
                             .arg( d->socket->localPort() );
}

QString GCF::LANRemoteAppAccessPoint::peerInfo() const
{
    return QString("%1 (%2)").arg( d->socket->peerAddress().toString() )
                             .arg( d->socket->peerPort() );
}

QHostAddress GCF::LANRemoteAppAccessPoint::localAddress() const
{
    return d->socket->localAddress();
}

quint16 GCF::LANRemoteAppAccessPoint::localPort() const
{
    return d->socket->localPort();
}

QHostAddress GCF::LANRemoteAppAccessPoint::peerAddress() const
{
    return d->socket->peerAddress();
}

quint16 GCF::LANRemoteAppAccessPoint::peerPort() const
{
    return d->socket->peerPort();
}

bool GCF::LANRemoteAppAccessPoint::isValidInternal() const
{
    return d->socket && d->socket->isValid();
}

void GCF::LANRemoteAppAccessPoint::disconnectInternal()
{
    QObject::disconnect(d->socket, 0, this, 0);

    d->socket->disconnectFromHost();
    d->socket->close();
}

bool GCF::LANRemoteAppAccessPoint::sendMessageInternal(GCF::Message* message)
{
    if(!d->socket || !d->socket->isValid())
        return false;

    QByteArray ba = message->toByteArray();
    if( d->socket->write(ba) < 0 )
    {
        qDebug("Error while sending message: %s", qPrintable(message->toString()));
        return false;
    }

    return true;
}

void GCF::LANRemoteAppAccessPoint::slotConnected()
{
    // Send a RequestAccess message to the server
    GCF::RequestAccessMessage* msg = new GCF::RequestAccessMessage;
    msg->setApplicationName( GCF::AppAccessPoint::instance().accessPointName() );
    this->sendMessage( msg );
}

void GCF::LANRemoteAppAccessPoint::slotDisconnected()
{
    this->disconnect();
}

void GCF::LANRemoteAppAccessPoint::slotError()
{
    qDebug("Socket Error: %s", qPrintable(d->socket->errorString()));
}

void GCF::LANRemoteAppAccessPoint::slotStateChanged()
{
    /*
    switch(d->socket->state())
    {
    case QAbstractSocket::UnconnectedState:
        qDebug() << d->remoteAppAddress.toString() << "[" << d->remoteAppPort << "]"
                 << " - Unconnected";
        break;
    case QAbstractSocket::HostLookupState:
        qDebug() << d->remoteAppAddress.toString() << "[" << d->remoteAppPort << "]"
                 << " - HostLookupState";
        break;
    case QAbstractSocket::ConnectingState:
        qDebug() << d->remoteAppAddress.toString() << "[" << d->remoteAppPort << "]"
                 << " - ConnectingState";
        break;
    case QAbstractSocket::ConnectedState:
        qDebug() << d->remoteAppAddress.toString() << "[" << d->remoteAppPort << "]"
                 << " - ConnectedState";
        break;
    case QAbstractSocket::BoundState:
        qDebug() << d->remoteAppAddress.toString() << "[" << d->remoteAppPort << "]"
                 << " - BoundState";
        break;
    case QAbstractSocket::ListeningState:
        qDebug() << d->remoteAppAddress.toString() << "[" << d->remoteAppPort << "]"
                 << " - ListeningState";
        break;
    case QAbstractSocket::ClosingState:
        qDebug() << d->remoteAppAddress.toString() << "[" << d->remoteAppPort << "]"
                 << " - ClosingState";
        break;
    }
    */
}

void GCF::LANRemoteAppAccessPoint::slotAboutToClose()
{
    // Do nothing..
}

void GCF::LANRemoteAppAccessPoint::slotReadyRead()
{
    if( !d->socket->bytesAvailable() )
        return;

    // Lets first take unprocessed data from the last slotReadyRead()
    QByteArray ba;
    if( d->unprocessedData.size() )
    {
        ba = d->unprocessedData;
        d->unprocessedData.clear();
    }

    // Now add the new data to it.
    if( ba.size() )
        ba.append( d->socket->readAll() );
    else
        ba = d->socket->readAll();

    // Now unpack a message from the byte array
    while(1)
    {
        GCF::Message* msg = GCF::Message::fromByteArray(ba);
        if( !msg )
            break;

        this->handleMessage(msg);
    }

    // If there are any unprocessed bytes left, then lets keep it for the next batch
    if( ba.size() )
        d->unprocessedData = ba;

    // Lets just check the socket one more time if more bytes are available for us
    // to read. If yes, lets schedule a call to this function again
    if( d->socket->bytesAvailable() )
        QMetaObject::invokeMethod(this, "slotReadyRead", Qt::QueuedConnection);
}

///////////////////////////////////////////////////////////////////////////////

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace GCF
{
    struct HttpRemoteAppAccessPointData
    {
        QUrl url;

        QNetworkAccessManager networkAccessManager;
    };
}

GCF::HttpRemoteAppAccessPoint::HttpRemoteAppAccessPoint(QObject* parent)
: GCF::RemoteAppAccessPoint(parent)
{
    d = new HttpRemoteAppAccessPointData;
}

GCF::HttpRemoteAppAccessPoint::~HttpRemoteAppAccessPoint()
{
    delete d;
}

void GCF::HttpRemoteAppAccessPoint::connectToApp(const QUrl& url)
{
    d->url = url;

    if(d->url.isValid() && (d->url.scheme() == "http" || d->url.scheme() == "https"))
    {
        // Send a RequestAccess message to the server
        GCF::RequestAccessMessage* msg = new GCF::RequestAccessMessage;
        msg->setApplicationName( GCF::AppAccessPoint::instance().accessPointName() );
        sendMessage( msg );
    }
    else
        QMetaObject::invokeMethod(this, "disconnect", Qt::QueuedConnection);
}

QUrl GCF::HttpRemoteAppAccessPoint::appUrl() const
{
    return d->url;
}

QString GCF::HttpRemoteAppAccessPoint::localInfo() const
{
    return "Local Computer";
}

QString GCF::HttpRemoteAppAccessPoint::peerInfo() const
{
    return d->url.toString();
}

bool GCF::HttpRemoteAppAccessPoint::isValidInternal() const
{
    return true;
}

void GCF::HttpRemoteAppAccessPoint::disconnectInternal()
{
    // Send the ReleaseAccessMessage now.
    GCF::ReleaseAccessMessage* msg = new GCF::ReleaseAccessMessage;
    this->sendMessage(msg);
}

bool GCF::HttpRemoteAppAccessPoint::sendMessageInternal(GCF::Message* message)
{
    // The following message types cannot be requested, because of the
    // natuer of communication.
    // - RequestConnection
    // - ReleaseConnection
    if( message->messageType() == GCF::Message::RequestConnection ||
        message->messageType() == GCF::Message::ReleaseConnection )
        return false;

    QByteArray ba = message->toByteArray();

    QNetworkRequest req;
    req.setUrl(d->url);
    req.setOriginatingObject(this);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "GCFMessage");
    req.setHeader(QNetworkRequest::ContentLengthHeader, ba.size());

    QNetworkReply* reply = d->networkAccessManager.post(req, ba);

    // Special attention should be given to ReleaseAccess message. We should wait until
    // the reply has finished.
    if(message->messageType() == GCF::Message::ReleaseAccess)
    {
        QEventLoop eventLoop;
        while(!reply->isFinished())
            eventLoop.processEvents();

        delete reply;
    }
    else
    {
        QObject::connect(reply, SIGNAL(finished()), this, SLOT(slotReplyReceived()));
        QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotReplyError()));
    }

    return true;
}

void GCF::HttpRemoteAppAccessPoint::slotReplyReceived()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply)
        return;

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray ba = reply->readAll();
        GCF::Message* msg = GCF::Message::fromByteArray(ba);
        this->handleMessage(msg);
    }
    else
    {
        this->disconnect();
    }

    reply->deleteLater();
}

///////////////////////////////////////////////////////////////////////////////

#include <GCF/IMessageEncoderDecoder>

namespace GCF
{
    struct AsyncCallerData
    {
        QObject* notifyObject;
        QByteArray notifyMember;
        GCF::AsyncInvokeMethodMessage* asyncMsg;
        IMessageEncoderDecoder* encoderDecoder;
        QByteArray responseBytes;
    };
}

GCF::AsyncCaller::AsyncCaller(QObject* parent,
            QObject* notifyObject, const char* member,
            const QHostAddress& address, quint16 port,
            const QString& objectName, const QString& method,
            const QVariantList& args)
            : QTcpSocket(parent)
{
    d = new AsyncCallerData;
    d->notifyObject = notifyObject;

    QByteArray ba(member+1);
    ba.truncate( ba.indexOf('(') );
    d->notifyMember = ba;

    d->asyncMsg = new GCF::AsyncInvokeMethodMessage;
    d->asyncMsg->setObjectName(objectName);
    d->asyncMsg->setMethodName(method);
    d->asyncMsg->setArgList(args);

    d->encoderDecoder = GCF::AppAccessPoint::instance().messageEncoderDecoderAt(0);

    connect( this, SIGNAL(connected()), this, SLOT(slotConnected()) );
    connect( this, SIGNAL(disconnected()), this, SLOT(slotDisconnected()) );
    connect( this, SIGNAL(error(QAbstractSocket::SocketError)),
             this, SLOT(slotError(QAbstractSocket::SocketError)) );
    connect( this, SIGNAL(readyRead()), this, SLOT(slotReadyRead()) );

    this->connectToHost( address, port );
}

GCF::AsyncCaller::~AsyncCaller()
{
    delete d->asyncMsg;
    d->encoderDecoder = 0;
    delete d;
}

void GCF::AsyncCaller::slotConnected()
{
    QByteArray msgBa = d->encoderDecoder->encodeMessage(d->asyncMsg);
    this->write(msgBa);
    this->flush();
}

void GCF::AsyncCaller::slotDisconnected()
{
    this->notifyError();
    this->close();
    this->deleteLater();
}

void GCF::AsyncCaller::slotError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    this->slotDisconnected();
}

void GCF::AsyncCaller::slotReadyRead()
{
    QByteArray bytes = d->responseBytes + this->readAll();
    GCF::Message* reply = d->encoderDecoder->decodeMessage(bytes);
    if( !reply )
    {
        d->responseBytes = bytes;
        return;
    }

    if(reply->messageType() != GCF::Message::AsyncInvokeMethodRes)
    {
        delete reply;
        this->slotDisconnected();
        return;
    }

    GCF::AsyncInvokeMethodResMessage* reply2 = (GCF::AsyncInvokeMethodResMessage*)reply;
    this->notifyResult(reply2->retValue(), reply2->isSuccess());
    delete reply;
    this->close();
    this->deleteLater();
}

void GCF::AsyncCaller::notifyError()
{
    this->notifyResult(QVariant(), false);
}

void GCF::AsyncCaller::notifyResult(const QVariant& result, bool success)
{
    if(!d->notifyObject || d->notifyMember.isEmpty())
        return;

    QMetaObject::invokeMethod(d->notifyObject, d->notifyMember.constData(),
        Qt::DirectConnection, Q_ARG(QVariant,result), Q_ARG(bool,success));
    d->notifyObject = 0;
    d->notifyMember.clear();
}

