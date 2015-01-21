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

#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QLocalServer>
#include <QLocalSocket>

struct RequestHandlerData;
class RequestHandler : public QLocalServer
{
    Q_OBJECT

public:
    RequestHandler(const QString& connectionString, QObject* parent=0);
    ~RequestHandler();

protected:
    void incomingConnection(quintptr socketDesc);
    
private:
    RequestHandlerData* d;
};

class RequestingClient : public QLocalSocket
{
    Q_OBJECT
    
public:
    RequestingClient(QObject* parent=0);
    ~RequestingClient();
    
private slots:
    void slotReadyRead();

private:
    QByteArray m_bytes;
};

#endif // REQUESTHANDLER_H
