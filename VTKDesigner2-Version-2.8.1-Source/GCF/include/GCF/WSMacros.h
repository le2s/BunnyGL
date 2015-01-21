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


#ifndef WS_MACROS_H
#define WS_MACROS_H

/**
Tag functions with this macro to have the broker execute the function
instead of forwarding it to a child process. Developers have to be
responsible while using this macro. The macro has to be used only on
functions that return very quickly. The function's return value should
not be heavy.

\note Atomic functions are assumed to be client independent.
*/
#define GCF_ATOMIC_WS

/**
Web-service functions tagged with this macro, or with no macro, are
executed by a child process. The broker basically takes up the onus
of forwarding calls made to such functions to a child process.

\note These functions are assumed to be client specific. This means
that requests originating from the same client will be forwarded to
the child process that had previously handled requests for it. This
also means that if the client makes several requests at a time,
they will be handled one request after another. This surely means
long waiting times for the client.
*/
#define GCF_NORMAL_WS

/**
Web-service functions tagged with this macro behave very much like
\ref GCF_NORMAL_WS functions; but such functions are assumed to be
client independent.
*/
#define GCF_CLIENT_INDEPENDENT_WS

/**
Q_INVOKABLE functions tagged with this macro dont get exported
as web-service methods.
*/
#define GCF_NOT_WS

/**
This macro helps in figuring out the port number of the application
server.
*/
#define GCF_APPLICATION_SERVER_PORT GetGCFApplicationServerPort()

#include <GCF/AbstractComponent>
#include <GCF/ComponentFactory>

inline quint16 GetGCFApplicationServerPort()
{
    GCF::AbstractComponent* asComp = GCF::ComponentFactory::instance().findComponent("ApplicationServerComponent");
    if(!asComp)
        return 0;
    int serverPort = asComp->property("ServerPort").toInt();
    return quint16(serverPort);
}

/*! \def GCF_CURRENT_CLIENT_ID

This macro, when used within QObject subclasses, provides access to the
GUID of the remote browser client from which the call to one or more
function in the object was made.
*/
#define GCF_CURRENT_CLIENT_ID this->property("CurrentClientGUID").toString()

/*! \def GCF_CURRENT_CLIENT_IP

This macro, when used within QObject subclasses, provides access to the
IP address of the remote browser client from which the call to one or more
function in the object was made.
*/
#define GCF_CURRENT_CLIENT_IP this->property("ClientIP").toString()

/*! \def GCF_CURRENT_CLIENT_PORT

This macro, when used within QObject subclasses, provides access to the
port number of the remote browser client from which the call to one or more
function in the object was made.
*/
#define GCF_CURRENT_CLIENT_PORT this->property("ClientPort").toString()

/*! \def GCF_CURRENT_SERVER_IP

This macro, when used within QObject subclasses, provides access to the
IP address of the server computer where the call is being handled.
*/
#define GCF_CURRENT_SERVER_IP this->property("ServerIP").toString()

/*! \def GCF_CURRENT_SERVER_PORT

This macro, when used within QObject subclasses, provides access to the
port number of the server computer where the call is being handled.
*/
#define GCF_CURRENT_SERVER_PORT this->property("ServerPort").toString()

#endif
