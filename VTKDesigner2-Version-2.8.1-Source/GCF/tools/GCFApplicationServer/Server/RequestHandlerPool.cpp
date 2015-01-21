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

#include "RequestHandlerPool.h"
#include "ApplicationServerComponent.h"

#include <QMap>
#include <QDebug>
#include <QProcess>
#include <QVariant>
#include <QLocalServer>
#include <GCF/Application>
#include <GCF/Profiler>

#ifdef Q_WS_WIN
#include <windows.h>
#include <winbase.h>
#endif

qlonglong procPID(QProcess* proc)
{
    qlonglong pid = 0;

#ifdef Q_WS_WIN
    pid = (qlonglong)((PROCESS_INFORMATION*)(proc->pid())->dwProcessId);
#else
    pid = (qlonglong)proc->pid();
#endif

    return pid;
}

struct RequestHandlerPoolData
{
    int poolSize;
    int maxRequestCount;
    int currentRequestCount;

    QMap< QProcess*, QList<RequestorSocket*> > processSocketMap;
    QMap<QString, QProcess*> guidProcessMap;
    QMap<RequestorSocket*, QProcess*> socketProcessMap;

    QLocalServer handshakeServer;
    int processCounter;

    void associateGUID(const QString& guid, QProcess* proc) {
        if(guid.isEmpty())
            return;

        // First store GUID -> Process mapping
        guidProcessMap[guid] = proc;
        
        // Store the GUID in the list of GUID's already handled by the process
        QStringList procGUIDList = associatedGUIDList(proc);
        procGUIDList.append(guid);
        proc->setProperty("GUIDList", procGUIDList);

        GCF_AS_BROKER_LOG( QString("GUID %1 is handled by Process with PID %2")
                            .arg(guid).arg( procPID(proc) ) );
    }

    QStringList associatedGUIDList(QProcess* proc) {
        QStringList procGUIDList;
        if(proc->dynamicPropertyNames().contains("GUIDList"))
            procGUIDList = proc->property("GUIDList").toStringList();
        return procGUIDList;
    }
};

RequestHandlerPool::RequestHandlerPool(QObject *parent) :
    QObject(parent)
{
    d = new RequestHandlerPoolData;
    d->poolSize = gApp->parameterValue("RequestHandlerPool_PoolSize", "10").toInt();
    d->processCounter = 0;
    d->maxRequestCount = 0;
    d->currentRequestCount = 0;

    QLocalServer::removeServer("GCFApplicationServer-RequestBroker");
    d->handshakeServer.listen("GCFApplicationServer-RequestBroker");
    connect(&d->handshakeServer, SIGNAL(newConnection()), this, SLOT(handshakeServerNewConnection()));

    setMaxRequestCount(50);
    GCF_AS_BROKER_LOG( QString("Initializing request handler with %1 as pool size").arg(d->poolSize) );
}

RequestHandlerPool::~RequestHandlerPool()
{
    this->closeAllProcesses();
    delete d;
}

void RequestHandlerPool::setPoolSize(int n)
{
    if(n <= 0)
        return;

    // If more than n processes have already been created
    // we cannot do anything about it.
    if( d->processSocketMap.count() > n )
        return;

    d->poolSize = n;
    GCF_AS_BROKER_LOG( QString("Changing pool size to %1").arg(d->poolSize) );
}

int RequestHandlerPool::poolSize() const
{
    return d->poolSize;
}

int RequestHandlerPool::actualPoolSize() const
{
    return d->processSocketMap.count();
}

void RequestHandlerPool::setMaxRequestCount(int count)
{
    if(count <= 0 || count == d->maxRequestCount)
        return;

    d->maxRequestCount = count;
}

int RequestHandlerPool::maxRequestCount() const
{
    return d->maxRequestCount;
}

int RequestHandlerPool::currentRequestCount() const
{
    return d->currentRequestCount;
}

RequestorSocket* RequestHandlerPool::getRequestHandlerConnection(const QString& guid)
{
    if(d->currentRequestCount >= d->maxRequestCount)
    {
        emit serverBusy();
        return 0;
    }

    // If the guid is already handled by an existing QProcess
    // forward the request to the same process.
    if(!guid.isEmpty() && d->guidProcessMap.contains(guid))
    {
        QProcess* proc = d->guidProcessMap[guid];
        GCF_AS_BROKER_LOG( QString("GUID %1 is handled by Process with PID %2")
                            .arg(guid).arg( procPID(proc) ) );

        return createLocalSocket(proc);
    }

    // First find if we have a free process in the existing pool of processes
    QMap<int, QProcess*> processMap;
    QMap<int, QProcess*> guidCountMap;
    QMap<QProcess*, QList<RequestorSocket*> >::iterator it = d->processSocketMap.begin();
    QMap<QProcess*, QList<RequestorSocket*> >::iterator end = d->processSocketMap.end();
    while(it != end)
    {
        if( it.value().count() == 0 )
            guidCountMap[ it.key()->property("GUIDList").toStringList().count() ] = it.key();

        processMap[ it.value().count() ] = it.key();

        ++it;
    }

    bool oneChildProcPerGUID = (gApp->parameterValue("RequestHandlerPool_OneChildProcessPerGUID", "false") == "true");
    if(!oneChildProcPerGUID)
    {
        // Use process that with lowest GUID count for handling the request.
        QList<int> guidList = guidCountMap.keys();
        if(guidList.count())
        {
            QProcess* lowestGUIDHandlingProcess = guidCountMap[guidList.first()];

            d->associateGUID(guid, lowestGUIDHandlingProcess );
            return createLocalSocket( lowestGUIDHandlingProcess );
        }
    }

    // If d->poolSize > d->processSocketMap.count(), then we can afford to create
    // another process and add it to the pool.
    if( d->poolSize > d->processSocketMap.count() )
    {
        QProcess* proc = this->createNewProcess();
        d->associateGUID(guid, proc);
        return createLocalSocket(proc);
    }

    if(oneChildProcPerGUID)
        return 0;

    // Look for the process that has the lowest load right now and
    // create a local socket for that.
    QList<int> loadList = processMap.keys();
    if(loadList.count())
    {
        int lowestLoad = loadList.first();

        QProcess* proc = processMap[lowestLoad];
        d->associateGUID(guid, proc);
        return createLocalSocket(proc);
    }

    // No clue what to do now!
    return 0;
}

bool RequestHandlerPool::freeRequestHandlerConnection(RequestorSocket* localSocket)
{
    if(!d->socketProcessMap.contains(localSocket))
        return false;

    QProcess* proc = d->socketProcessMap[ localSocket ];
    d->socketProcessMap.remove( localSocket );
    d->processSocketMap[ proc ].removeAll( localSocket );
    d->currentRequestCount--;

    return true;
}

bool RequestHandlerPool::isHandlerForGUIDReady(const QString& guid) const
{
    if(guid.isEmpty())
        return false;

    QProcess* handlerProcess = d->guidProcessMap.value(guid);
    if(!handlerProcess)
        return false;

    if(handlerProcess->property("__Initialized__").toBool())
        return true;

    return false;
}

RequestorSocket* RequestHandlerPool::createLocalSocket(QProcess* proc)
{
    if(!proc || !d->processSocketMap.contains(proc) )
        return 0;

    QString connectionString = proc->property("__ConnectionString__").toString();

    RequestorSocket* socket = new RequestorSocket(connectionString, this);
    if(proc->property("__Initialized__").toBool())
        socket->tryConnect();

    d->processSocketMap[proc].append( socket );
    d->socketProcessMap[socket] = proc;

    d->currentRequestCount++;
    connect(socket, SIGNAL(disconnected()), this, SLOT(localSocketDisconnected()));
    connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(localSocketDisconnected()));

    return socket;
}

QProcess* RequestHandlerPool::createNewProcess()
{
    QString programFile = qApp->applicationFilePath();
    QString argument = QString("GCFApplicationServer-RequestHandler-%1").arg(++d->processCounter);
    GCF_AS_BROKER_LOG( QString("Creating new child process with %1 as connection string").arg(argument) );

    QProcess* proc = new QProcess(this);
    proc->setProperty("__ConnectionString__", argument);
    proc->setProperty("__Initialized__", false);
    connect(proc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processTerminated()));
    proc->start(programFile, QStringList(argument));

    d->processSocketMap[ proc ] = QList<RequestorSocket*>();

    // TODO: we need to check out the validity of this
    // waitForStarted() waits for 30 long seconds. This may not
    // be a good idea if the application server is large!
    proc->waitForStarted();

    qlonglong pid = 0;

#ifdef Q_WS_WIN
    pid = (qlonglong)((PROCESS_INFORMATION*)(proc->pid())->dwProcessId);
#else
    pid = (qlonglong)proc->pid();
#endif

    if(proc->state() == QProcess::Running)
        GCF_AS_BROKER_LOG( QString("Child process %1 with connection string %2 is running")
                            .arg( pid )
                            .arg( argument) );
    else
        GCF_AS_BROKER_LOG( QString("Child process %1 with connection string %2 is not running")
                            .arg( pid )
                            .arg( argument) );

    return proc;
}

void RequestHandlerPool::closeAllProcesses()
{
    QMap<QProcess*, QList<RequestorSocket*> > copy = d->processSocketMap;

    QMap<QProcess*, QList<RequestorSocket*> >::iterator it = copy.begin();
    QMap<QProcess*, QList<RequestorSocket*> >::iterator end = copy.end();
    while(it != end)
    {
        Q_FOREACH(RequestorSocket* socket, it.value())
            socket->close();

        disconnect(it.key(), 0, this, 0);

        RequestorSocket* tSocket = this->createLocalSocket(it.key());
        tSocket->doWrite(QByteArray("TerminateMessage"));

        it.key()->waitForFinished();

        ++it;
    }

    d->processSocketMap.clear();
    d->socketProcessMap.clear();
    d->guidProcessMap.clear();
}

const QMap< QProcess*, QList<RequestorSocket*> > RequestHandlerPool::processSocketMap() const
{
    return d->processSocketMap;
}

void RequestHandlerPool::localSocketDisconnected()
{
    RequestorSocket* localSocket = qobject_cast<RequestorSocket*>( sender() );
    this->freeRequestHandlerConnection(localSocket);
}

void RequestHandlerPool::handshakeServerNewConnection()
{
    QLocalSocket* localSocket = d->handshakeServer.nextPendingConnection();
    connect(localSocket, SIGNAL(readyRead()), this, SLOT(handshakeServerReadyRead()));
}

void RequestHandlerPool::handshakeServerReadyRead()
{
    QLocalSocket* localSocket = qobject_cast<QLocalSocket*>(sender());
    QString data( localSocket->readAll() );
    if( data.length() )
    {
        GCF_AS_BROKER_LOG( QString("Received handshake from %1").arg(data) );

        QList<QProcess*> processList = d->processSocketMap.keys();
        for(int i=0; i<processList.count(); i++)
        {
            if( processList.at(i)->property("__ConnectionString__").toString() == data )
            {
                processList.at(i)->setProperty("__Initialized__", true);
                QList<RequestorSocket*> sockets = d->processSocketMap[processList.at(i)];
                Q_FOREACH(RequestorSocket* socket, sockets)
                    QMetaObject::invokeMethod(socket, "tryConnect", Qt::QueuedConnection);

                break;
            }
        }

        localSocket->close();
        localSocket->deleteLater();
    }
}

void RequestHandlerPool::processTerminated()
{
    QProcess* process = qobject_cast<QProcess*>(sender());

    // Aneesh : The following lines were commented because it was observed that this causes
    //          some dead processes to remain in the guidProcessMap
    //    if( !d->processSocketMap.contains(process) )
    //        return;

    // If the process terminated then all the local-sockets
    // connected to it from here automatically get disconnected
    // and hence they will be freed!
    d->processSocketMap.remove( process );

    // Now remove all GUID's associated with the terminated process
    QStringList guidList = d->associatedGUIDList(process);
    Q_FOREACH(QString guid, guidList)
        d->guidProcessMap.remove(guid);
}

///////////////////////////////////////////////////////////////////////////////
// RequestorSocket
///////////////////////////////////////////////////////////////////////////////

#include <QTimer>

RequestorSocket::RequestorSocket(const QString& connectionString, QObject* parent)
    : QLocalSocket(parent), m_connectionString(connectionString)
{
    connect( this, SIGNAL(connected()), this, SLOT(writeBufferedData()) );
}

RequestorSocket::~RequestorSocket()
{

}

void RequestorSocket::doWrite(const QByteArray& bytes)
{
    if(this->state() == QLocalSocket::ConnectedState)
    {
        GCF_AS_BROKER_LOG( QString("Writing %1 bytes of data to %2")
                            .arg(bytes.size())
                            .arg(m_connectionString) );
        QLocalSocket::write(bytes);
        QLocalSocket::flush();
    }
    else
        m_bytesToWrite += bytes;
}

void RequestorSocket::tryConnect()
{
    if(this->state() == QLocalSocket::ConnectedState)
        return;

    GCF_AS_BROKER_LOG( QString("Trying to connect to %1").arg(m_connectionString) );
    QLocalSocket::connectToServer( m_connectionString );
}

void RequestorSocket::writeBufferedData()
{
    GCF_AS_BROKER_LOG( QString("Connected to %1").arg(m_connectionString) );

    if(m_bytesToWrite.size() == 0)
        return;

    GCF_AS_BROKER_LOG( QString("Writing %1 bytes of buffered data to %2")
                        .arg(m_bytesToWrite.size())
                        .arg(m_connectionString) );

    QLocalSocket::write(m_bytesToWrite);
    QLocalSocket::flush();

    m_bytesToWrite.clear();
}
