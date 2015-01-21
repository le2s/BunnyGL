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

#include "RequestHandler.h"
#include "ApplicationServerComponent.h"

#include <QDebug>
#include <QMessageBox>
#include <QLocalSocket>

struct RequestHandlerData
{
    // Empty structure, for future use..
};

RequestHandler::RequestHandler(const QString& connectionString, QObject* parent)
    : QLocalServer(parent)
{
    d = new RequestHandlerData;

    QLocalServer::removeServer(connectionString);
    
    if( this->listen(connectionString) )
        GCF_AS_HANDLER_LOG( QString("Created local server %1").arg(connectionString) );
    else
        GCF_AS_HANDLER_ERROR( QString("Could not create local server %1").arg(connectionString) );
}

RequestHandler::~RequestHandler()
{
    delete d;
}

void RequestHandler::incomingConnection(quintptr socketDesc)
{
    RequestingClient* client = new RequestingClient(this);
    client->setSocketDescriptor(socketDesc);

    GCF_AS_HANDLER_LOG( QString("Handling incoming connection at local server %1").arg( this->serverName() ) );
}

///////////////////////////////////////////////////////////////////////////////
// RequestorSocket
///////////////////////////////////////////////////////////////////////////////

#include <GCF/AppAccessPoint>
#include <GCF/Message>
#include <GCF/AppAccessPoint>
#include <GCF/IMessageEncoderDecoder>

RequestingClient::RequestingClient(QObject* parent)
: QLocalSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

RequestingClient::~RequestingClient()
{

}

void RequestingClient::slotReadyRead()
{
    m_bytes += this->readAll();
    if(m_bytes == "TerminateMessage")
    {
        ApplicationServerComponent::instance().shutdown();
        return;
    }

    if(m_bytes.count() == 0)
        return;

    IMessageEncoderDecoder* defaultEncoderDecoder = GCF::AppAccessPoint::instance().messageEncoderDecoderAt(0);
    GCF::Message* request = defaultEncoderDecoder->decodeMessage(m_bytes);
    if( !request )
        return;

    QLocalServer* parentServer = qobject_cast<QLocalServer*>(this->parent());
    GCF_AS_HANDLER_DEBUG( QString("Handling request message %1 at local server %2")
                            .arg( request->id() )
                            .arg( parentServer->serverName() ) );
    GCF_AS_HANDLER_LOG( request->toString() );

    // Handle the incoming message
    GCF::Message* reply = GCF::AppAccessPoint::instance().handleMessage(request);
    if( !reply )
    {
        GCF_AS_HANDLER_ERROR( QString("Request message %1 at local server %2 not handled")
                            .arg( request->id() )
                            .arg( parentServer->serverName() ) );
        this->close();
    }
    else
    {    
        reply->includeErrorInfo();

        // Send back the response
        int encDecIndex = request->customData().value("EncoderDecoderIndex").toInt();
        if(encDecIndex >= 0)
            defaultEncoderDecoder = GCF::AppAccessPoint::instance().messageEncoderDecoderAt(encDecIndex);

        // Hack to make XMLGCFMessage handler work
        reply->customData().addKey("InceptionMessage", qVariantFromValue<void*>(request));

        QByteArray ba = defaultEncoderDecoder->encodeMessage(reply);
        this->write(ba);
        this->flush();
        this->close();

        GCF_AS_HANDLER_LOG( QString("%3 bytes of response sent for request message %1 from local server %2")
                                .arg( request->id() )
                                .arg( parentServer->serverName() )
                                .arg( ba.size() ) );
        GCF_AS_HANDLER_LOG( reply->toString() );
    }

    delete request;
    delete reply;
}


