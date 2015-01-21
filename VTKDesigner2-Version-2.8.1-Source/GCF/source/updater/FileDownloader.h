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

#ifndef FILE_DOWNLOADER_H
#define FILE_DOWNLOADER_H

#include <QObject>
#include <QUrl>

namespace GCF
{
struct AbstractFileDownloaderData;
class AbstractFileDownloader : public QObject
{
    Q_OBJECT

protected:
    AbstractFileDownloader(const QString& scheme, QObject* parent=0);

public:
    static AbstractFileDownloader* createDownloader(const QString& scheme, QObject* parent=0);
    static QString downloadFile(const QUrl& url);

    virtual ~AbstractFileDownloader();

    void setUrl(const QUrl& url);
    QUrl url() const;

    virtual QString scheme() const;

    virtual bool canDownload() const;
    virtual bool isDownloaded() const;
    virtual bool isActive() const;
    virtual void download();
    virtual QString downloadedFileName() const;

    void setAutoRemoveDownloadedFile(bool val);
    bool isAutoRemoveDownloadedFile() const;

public slots:
    virtual void cancelDownload();

signals:
    void downloadProgress(int);
    void downloadStarted();
    void downloadCompleted();
    void downloadCancelled();
    void downloadAborted(const QString& errorMessage);

private:
    AbstractFileDownloaderData* d;
};

struct LocalFileDownloaderData;
class LocalFileDownloader : public AbstractFileDownloader
{
    Q_OBJECT

public:
    LocalFileDownloader(QObject* parent=0);
    ~LocalFileDownloader();

    bool canDownload() const;
    bool isDownloaded() const;
    bool isActive() const;
    void download();
    QString downloadedFileName() const;
    void cancelDownload();

protected:
    void timerEvent(QTimerEvent* te);

private:
    LocalFileDownloaderData* d;
};

struct FtpFileDownloaderData;
class FtpFileDownloader : public AbstractFileDownloader
{
    Q_OBJECT

public:
    FtpFileDownloader(QObject* parent=0);
    ~FtpFileDownloader();

    bool canDownload() const;
    bool isDownloaded() const;
    bool isActive() const;
    void download();
    QString downloadedFileName() const;
    void cancelDownload();

private slots:
    void ftpDone(bool error);
    void ftpCmdStarted(int id);
    void ftpCmdFinished(int id, bool error);
    void ftpStateChanged(int state);
    void ftpDataTransferProgress(qint64 done, qint64 total);

private:
    FtpFileDownloaderData* d;
};

struct HttpFileDownloaderData;
class HttpFileDownloader : public AbstractFileDownloader
{
    Q_OBJECT

public:
    HttpFileDownloader(QObject* parent=0);
    ~HttpFileDownloader();

    bool canDownload() const;
    bool isDownloaded() const;
    bool isActive() const;
    void download();
    QString downloadedFileName() const;
    void cancelDownload();

private slots:
    void httpDone(bool error);
    void httpReqStarted(int id);
    void httpReqFinished(int id, bool error);
    void httpReadProgress(int done, int total);
    void httpStateChanged(int state);

private:
    HttpFileDownloaderData* d;
};
}

#endif
