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

#ifndef REQUESTHANDLERPOOL_H
#define REQUESTHANDLERPOOL_H

#include "ServerLoadInfoDialog.h"

#include <QObject>
#include <QLocalSocket>

class QProcess;
class RequestorSocket;

struct RequestHandlerPoolData;
class RequestHandlerPool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int PoolSize READ poolSize WRITE setPoolSize)

    friend class ServerLoadInfoDialog;

public:
    RequestHandlerPool(QObject *parent = 0);
    ~RequestHandlerPool();

    void setPoolSize(int n);
    int poolSize() const;
    
    int actualPoolSize() const;

    void setMaxRequestCount(int count);
    int maxRequestCount() const;

    int currentRequestCount() const;

    RequestorSocket* getRequestHandlerConnection(const QString& guid=QString());
    bool freeRequestHandlerConnection(RequestorSocket* socket);

    bool isHandlerForGUIDReady(const QString& guid) const;

signals:
    void serverBusy();

private:
    RequestorSocket* createLocalSocket(QProcess* proc);
    QProcess* createNewProcess();
    void closeAllProcesses();
    const QMap< QProcess*, QList<RequestorSocket*> > processSocketMap() const;
    
private slots:
    void localSocketDisconnected();
    void handshakeServerNewConnection();
    void handshakeServerReadyRead();
    void processTerminated();
    
private:
    RequestHandlerPoolData* d;
};

#include <QLocalSocket>

class RequestorSocket : public QLocalSocket
{
    Q_OBJECT
    
public:
    RequestorSocket(const QString& connectionString, QObject* parent=0);
    ~RequestorSocket();

    void doWrite(const QByteArray& bytes);
    
public slots:
    void tryConnect();

private slots:
    void writeBufferedData();

private:
    QString m_connectionString;
    QByteArray m_bytesToWrite;
};

#endif // REQUESTHANDLERPOOL_H
