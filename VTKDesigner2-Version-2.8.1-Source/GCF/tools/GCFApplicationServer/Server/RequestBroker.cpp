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

#include "RequestBroker.h"
#include "RequestForwarder.h"
#include "RequestHandlerPool.h"
#include "ApplicationServerComponent.h"

#include <QDebug>
#include <QPushButton>
#include <QtNetwork>
#include <QHostAddress>
#include <QMessageBox>

struct RequestBrokerData
{
    RequestBrokerData() : serverPort(4232) { }
    
    RequestHandlerPool* requestHandlerPool;
    quint16 serverPort;
};

RequestBroker::RequestBroker(QObject* parent)
    : QTcpServer(parent)
{
    d = new RequestBrokerData;

    d->requestHandlerPool = new RequestHandlerPool(this);
    GCF_AS_BROKER_LOG("Registering request handler pool");

    connect(this, SIGNAL(newConnection()), this, SLOT(resolveTCPConnection()));
}

RequestBroker::~RequestBroker()
{
    delete d;
}

void RequestBroker::setServerPort(quint16 port)
{
    d->serverPort = port;
    GCF_AS_BROKER_LOG(QString("Request broker server port is %1").arg(port));
}

RequestHandlerPool* RequestBroker::requestHandlerPool() const
{
    return d->requestHandlerPool;
}

void RequestBroker::start()
{
    if(this->isListening())
        return;
        
    if(!this->listen(QHostAddress::Any, d->serverPort))
        GCF_AS_BROKER_ERROR(QString("Request broker cannot listen at %1").arg(d->serverPort));
    else
        GCF_AS_BROKER_LOG(QString("Request broker listening at %1").arg(d->serverPort));
}

void RequestBroker::stop()
{
    if(!this->isListening())
        return;
        
    this->close();
    GCF_AS_BROKER_LOG(QString("Request stopped listening at %1").arg(this->serverPort()));
}

void RequestBroker::resolveTCPConnection()
{
    QTcpSocket* clientConnection = this->nextPendingConnection();

    GCF_AS_BROKER_LOG(QString("Request initiated from client at %1:%2")
                        .arg(clientConnection->peerAddress().toString())
                        .arg(clientConnection->peerPort()));

    new RequestForwarder(clientConnection, d->requestHandlerPool, this);
}
