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

#ifndef REMOTE_APP_ACCESS_POINT_P_H
#define REMOTE_APP_ACCESS_POINT_P_H

#include <GCF/RemoteAppAccessPoint>

namespace GCF
{
struct LANRemoteAppAccessPointData;
class LANRemoteAppAccessPoint : public RemoteAppAccessPoint
{
    Q_OBJECT

public:
    LANRemoteAppAccessPoint(QObject* parent=0);
    ~LANRemoteAppAccessPoint();

    void connectToApp(const QHostAddress& address, quint16 port, bool useSsl=false);
    QHostAddress appAddress() const;
    quint16 appPort() const;

    QAbstractSocket* socket() const;

    QString localInfo() const;
    QString peerInfo() const;

    QHostAddress localAddress() const;
    quint16 localPort() const;

    QHostAddress peerAddress() const;
    quint16 peerPort() const;

protected:
    bool isValidInternal() const;
    void disconnectInternal();
    bool sendMessageInternal(Message* message);

protected slots:
    void slotConnected();
    void slotDisconnected();
    void slotError();
    void slotStateChanged();
    void slotAboutToClose();
    void slotReadyRead();

private:
    LANRemoteAppAccessPointData* d;
};

struct HttpRemoteAppAccessPointData;
class HttpRemoteAppAccessPoint : public RemoteAppAccessPoint
{
    Q_OBJECT

public:
    HttpRemoteAppAccessPoint(QObject* parent=0);
    ~HttpRemoteAppAccessPoint();

    void connectToApp(const QUrl& url);
    QUrl appUrl() const;

    QString localInfo() const;
    QString peerInfo() const;

protected:
    bool isValidInternal() const;
    void disconnectInternal();
    bool sendMessageInternal(Message* message);

private slots:
    void slotReplyReceived();

private:
    HttpRemoteAppAccessPointData* d;
};

struct AsyncCallerData;
class AsyncCaller : public QTcpSocket
{
    Q_OBJECT

public:
    AsyncCaller(QObject* parent,
                QObject* notifyObject, const char* member,
                const QHostAddress& address, quint16 port,
                const QString& objectName, const QString& method,
                const QVariantList& args);
    ~AsyncCaller();

private slots:
    void slotConnected();
    void slotDisconnected();
    void slotError(QAbstractSocket::SocketError error);
    void slotReadyRead();

private:
    void notifyError();
    void notifyResult(const QVariant& result, bool success);

private:
    AsyncCallerData* d;
};
};

#endif
