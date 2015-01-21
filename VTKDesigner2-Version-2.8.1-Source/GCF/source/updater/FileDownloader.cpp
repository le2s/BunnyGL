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

#include "FileDownloader.h"

#include <QFile>
#include <QFtp>
#include <QHttp>
#include <QUrl>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QThread>
#include <QEventLoop>
#include <GCF/UpdaterComponent>

////////////////////////////////////////////////////////////////////////////
// Helper functions and classes
////////////////////////////////////////////////////////////////////////////

int calcProgress(qint64 done, qint64 total)
{
    double ratio = double(done)/double(total);
    return int(ratio*100);
}

namespace GCF
{
    class AbstractFileDownloaderCreator
    {
    public:
        virtual ~AbstractFileDownloaderCreator() { }
        virtual QString scheme() const = 0;
        virtual AbstractFileDownloader* createDownloader(QObject* parent=0) const = 0;
    };

    class LocalFileDownloaderCreator : public AbstractFileDownloaderCreator
    {
    public:
        QString scheme() const { return "file"; }
        AbstractFileDownloader* createDownloader(QObject* parent=0) const
            { return new LocalFileDownloader(parent); }
    };

    class FtpFileDownloaderCreator : public AbstractFileDownloaderCreator
    {
    public:
        QString scheme() const { return "ftp"; }
        AbstractFileDownloader* createDownloader(QObject* parent=0) const
            { return new FtpFileDownloader(parent); }
    };

    class HttpFileDownloaderCreator : public AbstractFileDownloaderCreator
    {
    public:
        QString scheme() const { return "http"; }
        AbstractFileDownloader* createDownloader(QObject* parent=0) const
            { return new HttpFileDownloader(parent); }
    };
}

////////////////////////////////////////////////////////////////////////////
// GCF::AbstractFileDownloader
////////////////////////////////////////////////////////////////////////////

/**
\ingroup updatergroup
\class GCF::AbstractFileDownloader AbstractFileDownloader

Base class for file downloaders used in GCF. File downloaders are used by the
\ref updater to download update files from server sites. Each subclass of
AbstractFileDownloader can download file from a specific category of sources
(eg. local, ftp, http etc).

You may never have to use this class or any of its subclasses directly,
unless you have a requirement to download a file from a FTP or HTTP
source.

\li \ref GCF::LocalFileDownloader - downloads from the local file system
\li \ref GCF::FtpFileDownloader - downloads from a FTP site
\li \ref GCF::HttpFileDownloader - downloads from a HTTP site

Usage

\code
GCF::AbstractFileDownloader* downloader = GCF::HttpFileDownloader(qApp);

downloader->setUrl( url );
downloader->download();

// wait for downloadCompleted() signal

QString downloadedFile = downloader->downloadedFileName();
\endcode

Alternatively, you can create an appropriate downloader based on the URL scheme
from which you are downloading the file.

\code
GCF::AbstractFileDownloader* downloader =
    GCF::AbstractFileDownloader::createDownloader("http");

if(downloader)
{
    // .. make use of the downloader here.
}
\endcode

You can also use a blocking download method

\code
QUrl url("http://www.somewebsite.com/directory/file.txt");
QString downloadedFile = GCF::AbstractFileDownloader::downloadFile(url);

if(!downloadedFile.isEmpty())
{
    // A copy of http://www.somewebsite.com/directory/file.txt
    // is now available in downloadedFile.
}
\endcode
*/
namespace GCF
{
    struct AbstractFileDownloaderData
    {
        QUrl url;
        QString scheme;
        bool autoRemove;
    };
}

/**
\fn void GCF::AbstractFileDownloader::downloadProgress(int)

This signal is emitted when there is a change in the download progress. The integer
parameter in the signal represents the percentage of download that is complete.
*/

/**
\fn void GCF::AbstractFileDownloader::downloadStarted()

This signal is emitted when the download has just begin
*/

/**
\fn void GCF::AbstractFileDownloader::downloadCompleted()

This signal is emitted when the download has just completed
*/

/**
\fn void GCF::AbstractFileDownloader::downloadCancelled()

This signal is emitted when the download has been cancelled. The signal is emitted only
if \ref cancelDownload() is called.
*/

/**
\fn void GCF::AbstractFileDownloader::downloadAborted(const QString& errorMessage)

This signal is emitted when the download has been aborted due to some network errors.
The \c errorMessage parameter contains the reason for abort.
*/

/**
This method creates a file-downloader that can download files from a given scheme. Currently
the following schemes are supported

\li "file"
\li "http"
\li "ftp"

Example
\code
QUrl url = QUrl::fromLocalFile("C:/Qt/4.4.1/README");

GCF::AbstractFileDownloader* downloader =
    GCF::AbstractFileDownloader::createDownloader( url.scheme() );

downloader->setUrl( url );
downloader->download();

// Wait for downloadCompleted() signal

\endcode
*/
GCF::AbstractFileDownloader* GCF::AbstractFileDownloader::createDownloader(const QString& scheme, QObject* parent)
{
    static QList<AbstractFileDownloaderCreator*> fdcList;
    if( fdcList.count() == 0 )
    {
        static LocalFileDownloaderCreator localCreator;
        fdcList.append( &localCreator );
        static FtpFileDownloaderCreator ftpCreator;
        fdcList.append( &ftpCreator );
        static HttpFileDownloaderCreator httpCreator;
        fdcList.append( &httpCreator );
    }

    for(int i=0; i<fdcList.count(); i++)
    {
        AbstractFileDownloaderCreator* fdc = fdcList[i];
        if( fdc->scheme() == scheme )
            return fdc->createDownloader( parent );
    }

    return 0;
}

// This internal thread class downloads a file in the background.
class FileDownloaderThread : public QThread
{
public:
    FileDownloaderThread(const QUrl& url) : m_url(url) { }
    ~FileDownloaderThread() { }

    void run() {
        QString scheme = m_url.scheme();
        GCF::AbstractFileDownloader* fd = GCF::AbstractFileDownloader::createDownloader(scheme);
        if(!fd)
            return;

        fd->setAutoRemoveDownloadedFile(false);
        fd->setUrl(m_url);
        if(!fd->canDownload())
        {
            delete fd;
            return;
        }

        connect(fd, SIGNAL(downloadCompleted()), this, SLOT(quit()), Qt::QueuedConnection);
        connect(fd, SIGNAL(downloadAborted(QString)), this, SLOT(quit()), Qt::QueuedConnection);

        fd->download();
        exec();

        if(fd->isDownloaded())
            m_downloadedFileName = fd->downloadedFileName();

        delete fd;
    }

    QString downloadedFileName() const {
        return m_downloadedFileName;
    }

private:
    QUrl m_url;
    QString m_downloadedFileName;
};


/**
This function internally makes use of an appropriate subclass of \ref GCF::AbstractFileDownloader
to download the file at \c url. The function returns the complete path of the on-disk-file
into which the downloaded contents were saved.

\note The function blocks the caller.
\note A modal event-loop is created to achieve the blocking effect. UI events will be processed as usual.
*/
QString GCF::AbstractFileDownloader::downloadFile(const QUrl& url)
{
    if( !url.isValid() )
        return QString();

    FileDownloaderThread fdThread(url);
    QEventLoop eventLoop;

    QObject::connect(&fdThread, SIGNAL(finished()), &eventLoop, SLOT(quit()), Qt::QueuedConnection);
    fdThread.start();
    eventLoop.exec();

    return fdThread.downloadedFileName();
}

/**
Constructs a file downloader for a given scheme. The parameter \c parent is passed
to QObject constructor.
*/
GCF::AbstractFileDownloader::AbstractFileDownloader(const QString& scheme, QObject* parent)
:QObject(parent)
{
    d = new GCF::AbstractFileDownloaderData;
    d->scheme = scheme;
    d->autoRemove = true;
}

/**
Destructor
*/
GCF::AbstractFileDownloader::~AbstractFileDownloader()
{
    delete d;
}

/**
Sets the URL from which the file has to be downloaded.

\note If the URL points to a directory, then the behavior is undefined.
*/
void GCF::AbstractFileDownloader::setUrl(const QUrl& url)
{
    d->url = url;
}

/**
Returns the URL from which the file is being downloaded
*/
QUrl GCF::AbstractFileDownloader::url() const
{
    return d->url;
}

/**
Returns the URL scheme that this downloader specializes in downloading.
*/
QString GCF::AbstractFileDownloader::scheme() const
{
    return d->scheme;
}

/**
Returns true/false depending on whether this downloader can download the
file at the URL \ref url()
*/
bool GCF::AbstractFileDownloader::canDownload() const
{
    return false;
}

/**
Returns true/false depending on whether the file at \ref url() has already
been downloaded or not.
*/
bool GCF::AbstractFileDownloader::isDownloaded() const
{
    return false;
}

/*
Returns true/false to indicate if the downloader is downloading something
right now.
*/
bool GCF::AbstractFileDownloader::isActive() const
{
    return false;
}

/**
Calling this functions causes the file-download to begin. If the file has
already been downloaded, the function does nothing. It is normally a good
idea to check with \ref isDownloaded() before calling this function.

\note The function returns immediately, without waiting for the file to be
completely downloaded.
*/
void GCF::AbstractFileDownloader::download()
{
    // Do nothing
}

/**
Returns the name of the file on the local file system where the downloaded file
is available.
*/
QString GCF::AbstractFileDownloader::downloadedFileName() const
{
    return QString();
}

/**
Configures whether the downloaded file should be automatically removed or not.
By default this flag is set to true, which means that when the file downloader
is destroyed the downloaded file is deleted.
*/
void GCF::AbstractFileDownloader::setAutoRemoveDownloadedFile(bool val)
{
    d->autoRemove = val;
}

/**
Returns whether the downloaded file will be deleted at object deletion or not.
*/
bool GCF::AbstractFileDownloader::isAutoRemoveDownloadedFile() const
{
    return d->autoRemove;
}

/**
Cancels download of the file at \c url(). The function does nothing if no
download is underway.
*/
void GCF::AbstractFileDownloader::cancelDownload()
{
    // Do nothing
}

////////////////////////////////////////////////////////////////////////////
// GCF::LocalFileDownloader
////////////////////////////////////////////////////////////////////////////

/**
\ingroup updatergroup
\class GCF::LocalFileDownloader LocalFileDownloader
\brief Downloads a file from the local filesystem

Even though QFile::copy() does the task of copying local files from one place
to another, it is advisable to use the timer and copy one block of data per unit
time. This is because, it is possible that the user of GCF is simultaneously
downloading several files. Sometimes in tandem with other file downloaders.
If the local file that is being downloaded takes a long time; then that will
hang the other downloads.

On the otherhand, local downloads need not actually download the file. It can
simply pass on the source file as destination file. At this moment however,
we assume the user of LocalFileDownloader will assume that the downloaded file
can be fiddled around with without worrying about whether it would mess up
the original source or not.

Usage
\code
GCF::AbstractFileDownloader* downloader =
    GCF::AbstractFileDownloader::createDownloader("file");

if( downloader )
{
    downloader->setUrl( QUrl::fromLocalFile("some_file_name") );
    downloader->download();
}
\endcode
*/
namespace GCF
{
    struct LocalFileDownloaderData
    {
        LocalFileDownloaderData() : source(0), destination(0),
                downloaded(false), timerId(-1) { }

        QFile* source;
        QTemporaryFile* destination;
        QString destFileName;
        bool downloaded;
        int timerId;

        static const qint64 blockSize;
    };
}

const qint64 GCF::LocalFileDownloaderData::blockSize = 32768;

/**
Constructor
*/
GCF::LocalFileDownloader::LocalFileDownloader(QObject* parent)
:GCF::AbstractFileDownloader("file", parent)
{
    d = new GCF::LocalFileDownloaderData;
}

/**
Destructor
*/
GCF::LocalFileDownloader::~LocalFileDownloader()
{
    if( this->isAutoRemoveDownloadedFile() )
    {
        if(!d->destFileName.isEmpty())
            QFile::remove(d->destFileName);
    }

    delete d;
}

bool GCF::LocalFileDownloader::canDownload() const
{
    QUrl url = this->url();
    QString localFile = url.toLocalFile();
    QFileInfo fi(localFile);
    return fi.exists() && fi.isReadable();
}

bool GCF::LocalFileDownloader::isDownloaded() const
{
    return d->downloaded;
}

bool GCF::LocalFileDownloader::isActive() const
{
    return d->source != 0;
}

void GCF::LocalFileDownloader::download()
{
    // Already downloaded
    if( d->downloaded )
        return;

    // Already started downloading
    if( d->timerId >= 0 )
        return;

    // Open source and destination files
    QString localFile = this->url().toLocalFile();
    d->source = new QFile(localFile, this);
    d->destination = new QTemporaryFile(this);
    d->destination->setAutoRemove(false);

    if( !d->source->open(QFile::ReadOnly) )
    {
        delete d->source;
        delete d->destination;
        d->source = 0;
        d->destination = 0;
        emit downloadAborted("Cannot open source file for reading.");
        return;
    }

    if( !d->destination->open() )
    {
        delete d->source;
        delete d->destination;
        d->source = 0;
        d->destination = 0;
        emit downloadAborted("Cannot open destination file for writing.");
        return;
    }

    // Start a timer and kickoff the copy process
    d->timerId = startTimer(0); // as fast as possible
    emit downloadStarted();
    emit downloadProgress(0);
}

QString GCF::LocalFileDownloader::downloadedFileName() const
{
    return d->destFileName;
}

void GCF::LocalFileDownloader::cancelDownload()
{
    if( d->timerId < 0 )
        return;

    killTimer( d->timerId );
    d->timerId = -1;

    d->destination->remove();
    delete d->destination;
    delete d->source;

    d->source = 0;
    d->destination = 0;

    emit downloadCancelled();
}

void GCF::LocalFileDownloader::timerEvent(QTimerEvent*)
{
    if( !d->source || !d->destination )
        return;

    QByteArray bytes = d->source->read(d->blockSize);
    d->destination->write(bytes);

    if( d->source->atEnd() )
    {
        d->downloaded = true;
        d->destFileName = d->destination->fileName();

        killTimer( d->timerId );
        d->timerId = -1;
        delete d->destination;
        delete d->source;
        d->source = 0;
        d->destination = 0;

        emit downloadProgress(100);
        emit downloadCompleted();
    }
    else
        emit downloadProgress( calcProgress(d->source->pos(), d->source->size()) );
}

////////////////////////////////////////////////////////////////////////////
// GCF::FtpFileDownloader
////////////////////////////////////////////////////////////////////////////

/**
\ingroup updatergroup
\class GCF::FtpFileDownloader FtpFileDownloader
\brief Downloads file from an FTP server.

Usage
\code
GCF::AbstractFileDownloader* downloader =
    GCF::AbstractFileDownloader::createDownloader("ftp");

if( downloader )
{
    downloader->setUrl( QUrl("ftp://ftp.mywebsite.com/pub/somefile.tgz") );
    downloader->download();
}
\endcode

\note At this point we do not support FTP downloads via a PROXY server, unless proxy
settings are configured globally in your system.
*/
namespace GCF
{
    struct FtpFileDownloaderData
    {
        FtpFileDownloaderData() : ftp(0), destination(0),
                downloaded(false), ftpCmdId(-1), aborted(false),
                readComplete(false) { }

        QFtp* ftp;
        QTemporaryFile* destination;
        QString destFileName;
        bool downloaded;
        int ftpCmdId;
        bool aborted;
        bool readComplete;
    };
}

/**
Constructor
*/
GCF::FtpFileDownloader::FtpFileDownloader(QObject* parent)
:GCF::AbstractFileDownloader("ftp", parent)
{
    d = new GCF::FtpFileDownloaderData;
}

/**
Destructor
*/
GCF::FtpFileDownloader::~FtpFileDownloader()
{
    if( this->isAutoRemoveDownloadedFile() )
        QFile::remove(d->destFileName);

    delete d;
}

bool GCF::FtpFileDownloader::canDownload() const
{
    // TODO: Check whether the ftp file actually exists or not.
    return true;
}

bool GCF::FtpFileDownloader::isDownloaded() const
{
    return d->downloaded;
}

bool GCF::FtpFileDownloader::isActive() const
{
    return d->ftp != 0;
}

void GCF::FtpFileDownloader::download()
{
    if( d->downloaded )
        return;

    if( d->ftp )
        return;

    d->ftp = new QFtp(this);
    connect(d->ftp, SIGNAL(done(bool)), this, SLOT(ftpDone(bool)));
    connect(d->ftp, SIGNAL(commandStarted(int)), this, SLOT(ftpCmdStarted(int)));
    connect(d->ftp, SIGNAL(commandFinished(int,bool)), this, SLOT(ftpCmdFinished(int,bool)));
    connect(d->ftp, SIGNAL(stateChanged(int)), this, SLOT(ftpStateChanged(int)));
    connect(d->ftp, SIGNAL(dataTransferProgress(qint64,qint64)), this, SLOT(ftpDataTransferProgress(qint64,qint64)));

    d->ftp->connectToHost( url().host(), url().port(21) );
    d->ftp->login();
}

QString GCF::FtpFileDownloader::downloadedFileName() const
{
    return d->destFileName;
}

void GCF::FtpFileDownloader::cancelDownload()
{
    if( d->ftp )
    {
        d->aborted = true;
        d->ftp->abort();
    }
}

void GCF::FtpFileDownloader::ftpDone(bool error)
{
    if( error )
    {
        if( d->aborted )
        {
            emit downloadCancelled();
            d->aborted = false;
        }
        else
            emit downloadAborted( d->ftp->errorString() );

        d->ftp->deleteLater();
        d->ftp = 0;
        d->ftpCmdId = -1;

        if(d->destination)
        {
            d->destination->remove();
            delete d->destination;
            d->destination = 0;
        }
    }
}

void GCF::FtpFileDownloader::ftpCmdStarted(int id)
{
    if( id != d->ftpCmdId )
        return;

    emit downloadStarted();
    emit downloadProgress(0);
}

void GCF::FtpFileDownloader::ftpCmdFinished(int id, bool error)
{
    if( id != d->ftpCmdId || error )
        return;

    // Release the FTP link
    disconnect(d->ftp, 0, this, 0);
    d->ftp->deleteLater();
    d->ftp = 0;

    d->downloaded = true;
    d->destFileName = d->destination->fileName();

    // Close the downloaded file
    delete d->destination;
    d->destination = 0;

    emit downloadProgress(100);
    emit downloadCompleted();

    // Reset other variables
    d->ftpCmdId = -1;
}

void GCF::FtpFileDownloader::ftpStateChanged(int state)
{
    switch(state)
    {
    case QFtp::Connected:
        // begin the download
        d->destination = new QTemporaryFile(this);
        d->destination->setAutoRemove(false);
        d->destination->open();
        d->ftpCmdId = d->ftp->get( url().path(), d->destination );
        break;
    case QFtp::Unconnected:
        // download was unconditionally aborted
        if(!d->readComplete)
        {
            emit downloadAborted("Download was aborted due to network errors.");

            disconnect(d->ftp, 0, this, 0);
            d->ftp->deleteLater();
            d->ftp = 0;
            d->ftpCmdId = -1;
            if(d->destination)
            {
                d->destination->remove();
                delete d->destination;
                d->destination = 0;
            }
        }
        break;
    }
}

void GCF::FtpFileDownloader::ftpDataTransferProgress(qint64 done, qint64 total)
{
    d->readComplete = (done == total);
    emit downloadProgress( calcProgress(done,total) );
}

////////////////////////////////////////////////////////////////////////////
// GCF::AbstractFileDownloader
////////////////////////////////////////////////////////////////////////////

/**
\ingroup updatergroup
\class GCF::HttpFileDownloader HttpFileDownloader
\brief Downloads file from an FTP server.

Usage
\code
GCF::AbstractFileDownloader* downloader =
    GCF::AbstractFileDownloader::createDownloader("http");

if( downloader )
{
    downloader->setUrl( QUrl("http://www.mywebsite.com/somefile.tgz") );
    downloader->download();
}
\endcode

\note At this point we do not support HTTP downloads via a PROXY server, unless proxy
settings are configured globally in your system.
*/

namespace GCF
{
    struct HttpFileDownloaderData
    {
        HttpFileDownloaderData() : http(0), destination(0), downloaded(false),
            httpReqId(-1), aborted(false), readComplete(false) { }

        QHttp* http;
        QTemporaryFile* destination;
        QString destFileName;
        bool downloaded;
        int httpReqId;
        bool aborted;
        bool readComplete;
    };
}

/**
Constructor
*/
GCF::HttpFileDownloader::HttpFileDownloader(QObject* parent)
:GCF::AbstractFileDownloader("http", parent)
{
    d = new GCF::HttpFileDownloaderData;
}

/**
Destructor
*/
GCF::HttpFileDownloader::~HttpFileDownloader()
{
    if( this->isAutoRemoveDownloadedFile() )
        QFile::remove(d->destFileName);

    delete d;
}

bool GCF::HttpFileDownloader::canDownload() const
{
    // TODO: Check whether the ftp file actually exists or not.
    return true;
}

bool GCF::HttpFileDownloader::isDownloaded() const
{
    return d->downloaded;
}

bool GCF::HttpFileDownloader::isActive() const
{
    return d->http != 0;
}

void GCF::HttpFileDownloader::download()
{
    if( d->downloaded )
        return;

    if( d->http )
        return;

    d->http = new QHttp(this);
    connect(d->http, SIGNAL(stateChanged(int)), this, SLOT(httpStateChanged(int)));
    connect(d->http, SIGNAL(requestStarted(int)), this, SLOT(httpReqStarted(int)));
    connect(d->http, SIGNAL(requestFinished(int,bool)), this, SLOT(httpReqFinished(int,bool)));
    connect(d->http, SIGNAL(dataReadProgress(int,int)), this, SLOT(httpReadProgress(int,int)));
    connect(d->http, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));

    /*
    // In a future update, authentication should also be supported.

    connect(d->http, SIGNAL(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)),
            this, SLOT(httpProxyAuth(QNetworkProxy,QAuthenticator*)));
    connect(d->http, SIGNAL(authenticationRequired(QString,QAuthenticator*)),
            this, SLOT(httpAuth(QString,QAuthenticator*)));
    */

    // Begin the download
    d->destination = new QTemporaryFile(this);
    d->destination->setAutoRemove(false);
    d->destination->open();
    d->http->setHost( url().host(), url().port(80) );
    d->httpReqId = d->http->get( url().path(), d->destination );
}

QString GCF::HttpFileDownloader::downloadedFileName() const
{
    return d->destFileName;
}

void GCF::HttpFileDownloader::cancelDownload()
{
    if( d->http )
    {
        d->aborted = true;
        d->http->abort();
    }
}

void GCF::HttpFileDownloader::httpDone(bool error)
{
    if( error )
    {
        if( d->aborted )
        {
            emit downloadCancelled();
            d->aborted = false;
        }
        else
            emit downloadAborted( d->http->errorString() );

        d->http->deleteLater();
        d->http = 0;
        d->httpReqId = -1;

        if(d->destination)
        {
            d->destination->remove();
            delete d->destination;
            d->destination = 0;
        }
    }
}

void GCF::HttpFileDownloader::httpReqStarted(int id)
{
    if( id != d->httpReqId )
        return;

    emit downloadStarted();
    emit downloadProgress(0);
}

void GCF::HttpFileDownloader::httpReqFinished(int id, bool error)
{
    if( id != d->httpReqId || error )
        return;

    // Release the FTP link
    disconnect(d->http, 0, this, 0);
    d->http->deleteLater();
    d->http = 0;

    d->downloaded = true;
    d->destFileName = d->destination->fileName();

    // Close the downloaded file
    delete d->destination;
    d->destination = 0;

    emit downloadProgress(100);
    emit downloadCompleted();

    // Reset other variables
    d->httpReqId = -1;
}

void GCF::HttpFileDownloader::httpReadProgress(int done, int total)
{
    d->readComplete = (done == total);
    emit downloadProgress( calcProgress(qint64(done),qint64(total)) );
}

void GCF::HttpFileDownloader::httpStateChanged(int state)
{
    switch(state)
    {
    case QHttp::Unconnected:
        // download was unconditionally aborted
        if(!d->readComplete)
        {
            emit downloadAborted("Download was aborted due to network errors.");

            disconnect(d->http, 0, this, 0);
            d->http->deleteLater();
            d->http = 0;
            d->httpReqId = -1;
            if(d->destination)
            {
                d->destination->remove();
                delete d->destination;
                d->destination = 0;
            }
        }
        break;
    }
}

