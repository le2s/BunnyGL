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

#include "IconCache.h"

#include <QDir>
#include <QMap>
#include <QFile>
#include <QThread>
#include <QDomDocument>
#include <QDomElement>
#include <QPixmapCache>
#include <QMutex>
#include <QMutexLocker>

#include <GCF/UpdaterComponent>
#include "FileDownloader.h"

namespace GCF
{
    struct IconCacheData
    {
        QMutex mutex;
        QMap<QUrl, QString> urlIconFileMap;
        QList<QUrl> fetchingList;
        QPixmapCache pixmapCache;
        QThread* myThread;

        QString iconCacheDir;
        QString iconCacheXmlFile;
        void loadIconInformation();
        void saveIconInformation();
    };
}

GCF::IconCache& GCF::IconCache::instance()
{
    static IconCache iconCache;
    return iconCache;
}

GCF::IconCache::IconCache(QObject* parent)
:QObject(parent)
{
    d = new IconCacheData;
    d->myThread = QThread::currentThread();

    // Create icon cache directory
    d->iconCacheDir = GCF::UpdaterComponent::instance().resolveFileName("${CACHE}/Icons/");
    QDir().mkpath(d->iconCacheDir); // creates the path if one doesnt already exist

    // Parse and load the IconCache.xml
    d->iconCacheXmlFile = d->iconCacheDir + "IconCache.xml";
    d->loadIconInformation();
}

GCF::IconCache::~IconCache()
{
    d->saveIconInformation();

    delete d;
}

void GCF::IconCache::fetchIcon(const QUrl& url, bool forceFetch)
{
    if(!url.isValid())
        return;

    bool iconAlreadyFetched = true;

    QMutexLocker locker(&d->mutex);
    if(d->urlIconFileMap.contains(url) || d->fetchingList.contains(url))
    {
        if(forceFetch)
        {
            QString fileName = d->urlIconFileMap[url];
            QFile::remove(fileName);
            d->urlIconFileMap.remove(url);

            iconAlreadyFetched = false;
        }
    }
    else
        iconAlreadyFetched = false;

    if(iconAlreadyFetched)
        return;

    locker.unlock();
    fetchIcon2(url);
}

QIcon GCF::IconCache::icon(const QUrl& url)
{
    if(!url.isValid())
        return QIcon();

    QMutexLocker locker(&d->mutex);
    if(d->urlIconFileMap.contains(url))
    {
        QString fileName = d->urlIconFileMap[url];
        QPixmap pixmap;
        if( !d->pixmapCache.find(fileName, pixmap) )
        {
            pixmap = QPixmap(fileName);
            d->pixmapCache.insert(fileName, pixmap);
        }

        return QIcon(pixmap);
    }

    // Download the icon now!
    locker.unlock();
    fetchIcon2(url);

    // Return a null icon for now.
    return QIcon();
}

void GCF::IconCache::iconDownloaded()
{
    GCF::AbstractFileDownloader* downloader
        = qobject_cast<GCF::AbstractFileDownloader*>(sender());
    if(downloader)
    {
        QUrl url = downloader->url();
        QString extension = url.toString().split(".").last();
        QString downloadedFile = downloader->downloadedFileName();
        QString iconFileName = QString("${CACHE}/Icons/%1.%2")
                                .arg( QFileInfo(downloadedFile).fileName() )
                                .arg( extension );
        iconFileName = GCF::UpdaterComponent::instance().resolveFileName(iconFileName);

        QFile::copy(downloadedFile, iconFileName);

        QMutexLocker locker(&d->mutex);
        d->urlIconFileMap[url] = iconFileName;
        d->fetchingList.removeAll(url);

        GCF_UPDATER_LOG( QString("Downloaded icon %1 into %2").arg(url.toString()).arg(iconFileName) );
        downloader->deleteLater();

        emit iconFetched(url);
    }
}

void GCF::IconCache::iconDownloadAborted()
{
    GCF::AbstractFileDownloader* downloader
        = qobject_cast<GCF::AbstractFileDownloader*>(sender());
    if(downloader)
    {
        QMutexLocker locker(&d->mutex);
        QUrl url = downloader->url();
        d->fetchingList.removeAll(url);

        GCF_UPDATER_ERROR( QString("Could not download icon %1").arg(url.toString()) );
        downloader->deleteLater();
    }
}

void GCF::IconCache::fetchIcon2(const QUrl& url)
{
    if(!url.isValid())
        return;

    d->mutex.lock();
    if(d->fetchingList.contains(url))
    {
        d->mutex.unlock();
        return;
    }
    d->fetchingList.append(url);
    d->mutex.unlock();

    GCF::AbstractFileDownloader* downloader = 0;
    downloader = GCF::AbstractFileDownloader::createDownloader( url.scheme() );
    if( !downloader )
        return; // cant help it.

    connect(downloader, SIGNAL(downloadCompleted()), this, SLOT(iconDownloaded()));
    connect(downloader, SIGNAL(downloadAborted(QString)), this, SLOT(iconDownloadAborted()));
    downloader->setAutoRemoveDownloadedFile(true);
    downloader->setUrl(url);

    GCF_UPDATER_LOG( QString("Fetching icon %1..").arg(url.toString()) );
    downloader->download();
}

void GCF::IconCacheData::loadIconInformation()
{
    QMutexLocker locker(&mutex);

    QDomDocument doc;

    QFile file(iconCacheXmlFile);
    if(!file.open(QFile::ReadOnly))
        return;

    if( !doc.setContent( &file ) )
        return;

    QDomElement rootE = doc.documentElement();
    QDomElement iconE = rootE.firstChildElement("Icon");
    while(iconE.isNull() == false)
    {
        QString url = iconE.firstChildElement("Url").text();
        QString iconFile = iconE.firstChildElement("IconFile").text();
        iconFile = iconCacheDir + iconFile;
        urlIconFileMap[url] = iconFile;

        iconE = iconE.nextSiblingElement("Icon");
    }

    GCF_UPDATER_LOG("Loaded Icon Cache...");
}

void GCF::IconCacheData::saveIconInformation()
{
    QMutexLocker locker(&mutex);

    QFile file(iconCacheXmlFile);
    if( !file.open(QFile::WriteOnly) )
        return;

    QDomDocument doc("IconCache");

    QDomElement rootE = doc.createElement("Root");
    doc.appendChild(rootE);

    QMap<QUrl,QString>::const_iterator it = urlIconFileMap.constBegin();
    QMap<QUrl,QString>::const_iterator end = urlIconFileMap.constEnd();
    while( it != end )
    {
        QDomElement iconE = doc.createElement("Icon");
        rootE.appendChild(iconE);

        QString iconFile = it.value();
        iconFile.remove(0, iconCacheDir.length());

        QDomElement urlE = doc.createElement("Url");
        urlE.appendChild(doc.createTextNode(it.key().toString()));
        iconE.appendChild(urlE);

        QDomElement iconFileE = doc.createElement("IconFile");
        iconFileE.appendChild(doc.createTextNode(iconFile));
        iconE.appendChild(iconFileE);

        ++it;
    }

    file.write( doc.toString().toAscii() );
    file.close();
}

