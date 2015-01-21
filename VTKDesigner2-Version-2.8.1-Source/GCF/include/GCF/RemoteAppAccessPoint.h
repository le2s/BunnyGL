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

#ifndef REMOTE_APP_ACCESS_POINT_H
#define REMOTE_APP_ACCESS_POINT_H

#include "Common.h"
#include "DataStore.h"

#include <QUrl>
#include <QObject>
#include <QTcpSocket>

class QHostAddress;
class QAbstractSocket;

namespace GCF
{

class RemoteObject;
class Message;

struct RemoteAppAccessPointData;
class GCF_EXPORT_CLASS RemoteAppAccessPoint : public QObject
{
    Q_OBJECT

public:
    static RemoteAppAccessPoint* getRemoteAppAccessPoint(const QHostAddress& address,
                                                         quint16 port, bool useSsl=false);
    static RemoteAppAccessPoint* getRemoteAppAccessPoint(const QUrl& url);
    static QList<RemoteAppAccessPoint*> getRemoteAppAccessPointList();
    static void closeAllRemoteAccessPoints();

    // Asynchronous call support
    static bool asyncCall(QObject* notifyObject, const char* member,
                          const QHostAddress& address, quint16 port,
                          const QString& objectName, const QString& method,
                          const QVariant& arg1=QVariant(), const QVariant& arg2=QVariant(),
                          const QVariant& arg3=QVariant(), const QVariant& arg4=QVariant(),
                          const QVariant& arg5=QVariant(), const QVariant& arg6=QVariant(),
                          const QVariant& arg7=QVariant(), const QVariant& arg8=QVariant());
    static bool asyncCall(QObject* notifyObject, const char* member,
                          const QHostAddress& address, quint16 port,
                          const QString& objectName, const QString& method,
                          const QVariantList& args);

    // Asynchronous call support (2)
    bool asyncCall(QObject* notifyObject, const char* member,
                   const QString& objectName, const QString& method,
                   const QVariant& arg1=QVariant(), const QVariant& arg2=QVariant(),
                   const QVariant& arg3=QVariant(), const QVariant& arg4=QVariant(),
                   const QVariant& arg5=QVariant(), const QVariant& arg6=QVariant(),
                   const QVariant& arg7=QVariant(), const QVariant& arg8=QVariant());
    bool asyncCall(QObject* notifyObject, const char* member,
                   const QString& objectName, const QString& method,
                   const QVariantList& args);

    bool isValid() const;
    RemoteObject* getRemoteObject(const QString& completeName);

    virtual QString localInfo() const = 0;
    virtual QString peerInfo() const = 0;

    virtual QHostAddress localAddress() const;
    virtual quint16 localPort() const;

    virtual QHostAddress peerAddress() const;
    virtual quint16 peerPort() const;

    DataStore customData() const;

    // Remoted method call
    Q_INVOKABLE qint32 call( const QString& objectName, const QString& method,
                             const QVariant& arg1=QVariant(), const QVariant& arg2=QVariant(),
                             const QVariant& arg3=QVariant(), const QVariant& arg4=QVariant(),
                             const QVariant& arg5=QVariant(), const QVariant& arg6=QVariant(),
                             const QVariant& arg7=QVariant(), const QVariant& arg8=QVariant());
    qint32 call(const QString& objectName, const QString& method, const QVariantList& args);

public slots:
    void disconnect();

signals:
    void connectedToApp();
    void disconnectedFromApp();
    void couldNotConnectToApp();

    void methodInvocationResult(qint32 callId, bool success, const QVariant& result, const QString& object, const QString& method);

protected:
    RemoteAppAccessPoint(QObject* parent=0);
    ~RemoteAppAccessPoint();

    // Must be implemented by subclasses
    virtual bool isValidInternal() const = 0;
    virtual void disconnectInternal() = 0;
    virtual bool sendMessageInternal(Message* message) = 0;

    void sendMessage(Message* message);
    void handleMessage(Message* message);
    bool handleMessage(Message* received, Message* sent);

    void activateObject(RemoteObject* rObj);
    void deactivateObject(RemoteObject* rObj);
    QList<RemoteObject*> remoteObjects() const;
    RemoteObject* findObject(quint32 id) const;

protected slots:
    void remoteObjectDestroyed(QObject* obj);

private slots:
    void activateAllObjects();
    void deactivateAllObjects();
    void sendPendingMessages();
    void discardPendingMessages();
    void checkConnectionStatus();

private:
    friend class RemoteObject;
    RemoteAppAccessPointData* d;
};
}

Q_DECLARE_METATYPE(GCF::RemoteAppAccessPoint*)

#endif
