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

#include "UpdateDownloaderThread.h"

#include <GCF/UpdaterComponent>
#include "FileDownloader.h"

#include <QFile>
#include <QTimer>
#include <QVector>

#include <GCF/UpdateInfo>
#include "UpdateXMLParser.h"

namespace GCF
{
    struct UpdateDownloaderThreadData
    {
        UpdateDownloaderThreadData() : dqCurrentIndex(-1), stopFlag(false), downloader(0) { }

        UpdateInfoList dqList;      // List of updates to download
        int dqCurrentIndex;         // Index to the current update being downloaded
        QVector<bool> dqSuccessFlagList;    // Flags that list success/failure of download
        bool stopFlag;
        int downloadFinishedCount;

        AbstractFileDownloader* downloader;

        bool isUpdateFullyAvailable(const UpdateInfo& info);
    };
}

GCF::UpdateDownloaderThread::UpdateDownloaderThread(QObject* parent)
:QThread(parent)
{
    d = new UpdateDownloaderThreadData;
}

GCF::UpdateDownloaderThread::~UpdateDownloaderThread()
{
    delete d;
}

void GCF::UpdateDownloaderThread::stop()
{
    if(!isRunning())
        return;

    d->stopFlag = true;

    // Stop downloading now!
    if(d->downloader)
        d->downloader->cancelDownload();

    GCF_UPDATER_LOG("Stop flag set. Thread will stop ASAP");
}

void GCF::UpdateDownloaderThread::run()
{
    // reset the stop flag
    d->stopFlag = false;

    // See if DownloadQueue.xml exists
    QString dqFile = UpdaterComponent::instance().resolveFileName("${DQ_XML}");
    if( !QFile::exists(dqFile) )
        return;

    UpdateXMLParser parser;
    if(!parser.parse(dqFile, d->dqList, true))
    {
        GCF_UPDATER_ERROR("Could not parse DownloadQueue.xml");
        return;
    }

    if(d->dqList.count() == 0)
    {
        GCF_UPDATER_LOG("No updates to download!");
        return;
    }

    d->dqCurrentIndex = -1;
    d->dqSuccessFlagList.resize( d->dqList.count() );
    d->dqSuccessFlagList.fill(false);

    // The UpdateDownloaderThread may be starting for the very first time now
    // __Or__ it may be continuing download from the last session. So we need
    // to read the dqFile XML and popuplate dqSuccessFlagList with appropriate
    // values.
    QFile file(dqFile);
    file.open(QFile::ReadOnly);

    QDomDocument dqDoc;
    dqDoc.setContent(&file);

    QDomElement dqRootE = dqDoc.documentElement();
    QDomElement dqUpdateE = dqRootE.firstChildElement("Update");
    while(!dqUpdateE.isNull())
    {
        QString name = dqUpdateE.firstChildElement("Name").text();
        QString version = dqUpdateE.firstChildElement("Version").text();
        UpdateInfo uInfo = d->dqList.findUpdate(name, version);
        int index = d->dqList.indexOf(uInfo);
        if( index >= 0 )
            d->dqSuccessFlagList[index] = (dqUpdateE.attribute("downloaded") == "true");

        dqUpdateE = dqUpdateE.nextSiblingElement("Update");
    }

    // Begin downloading updates.
    d->downloadFinishedCount = 0;
    QTimer::singleShot(0, this, SLOT(downloadNextUpdate()));

    // Start the local event loop
    exec();

    // Notify that updates are now downloaded
    emit finishedDownloadingUpdates(d->downloadFinishedCount);

    GCF_UPDATER_LOG("All done!");
}

void GCF::UpdateDownloaderThread::downloadNextUpdate()
{
    /* This function must be called only when the thread is actually operational. Not otherwise */
    if(!this->isRunning())
        return;

    if(d->stopFlag)
    {
        GCF_UPDATER_LOG("Stop flag was set. Finishing up this thread...");
        QMetaObject::invokeMethod(this, "allUpdatesDownloaded", Qt::QueuedConnection);
        return;
    }

    ++d->dqCurrentIndex;
    if(d->dqCurrentIndex >= d->dqList.count() || d->dqSuccessFlagList[d->dqCurrentIndex])
    {
        QMetaObject::invokeMethod(this, "allUpdatesDownloaded", Qt::QueuedConnection);
        return;
    }

    // What update should I download next?
    UpdateInfo uInfo = d->dqList.at(d->dqCurrentIndex);
    UpdateFileInfo uFile = uInfo.updateFileForThisSystem();
    if(!uFile.UpdateFileURL.isValid())
    {
        GCF_UPDATER_ERROR( QString("Invalid URL found for %1").arg(uInfo.name()) );
        QMetaObject::invokeMethod(this, "downloadNextUpdate", Qt::QueuedConnection);
    }

    // Release the old downloader if any
    if(d->downloader)
        delete d->downloader;
    d->downloader = 0;

    // Get a new downloader for the current update
    QString urlScheme = uFile.UpdateFileURL.scheme();
    d->downloader = GCF::AbstractFileDownloader::createDownloader(urlScheme, this);
    if(!d->downloader)
    {
        GCF_UPDATER_ERROR(
            QString("Could not download update for \"%1\". Unrecognized URL scheme: %2")
            .arg(uInfo.name()).arg(urlScheme)
            );
        QMetaObject::invokeMethod(this, "downloadNextUpdate", Qt::QueuedConnection);
        return;
    }

    // Configure the downloader
    connect(d->downloader, SIGNAL(downloadProgress(int)), this, SLOT(downloadProgress(int)));
    connect(d->downloader, SIGNAL(downloadCompleted()), this, SLOT(downloadCompleted()));
    connect(d->downloader, SIGNAL(downloadAborted(QString)), this, SLOT(downloadFailure()));
    connect(d->downloader, SIGNAL(downloadCancelled()), this, SLOT(downloadFailure()));
    d->downloader->setUrl( uFile.UpdateFileURL );

    // Begin downloading the file.
    GCF_UPDATER_LOG( QString("Downloading update file for %1-%2").arg(uInfo.name()).arg(uInfo.version()));
    d->downloader->download();
    d->downloader->setAutoRemoveDownloadedFile(true);
}

void GCF::UpdateDownloaderThread::downloadCompleted()
{
    /* This function must be called only when the thread is actually operational. Not otherwise */
    if(!this->isRunning())
        return;

    // What update did we just download?
    UpdateInfo uInfo = d->dqList.at(d->dqCurrentIndex);
    UpdateFileInfo uFile = uInfo.updateFileForThisSystem();

    // From now on the update file will be in the local file system.
    QString fName = QString("%1-%2.%3.%4.gvz")
                    .arg(uInfo.name())
                    .arg(uInfo.version())
                    .arg(uFile.Arch)
                    .arg(uFile.OS);
    uInfo.setUpdateFile(uFile.Arch, uFile.OS, QUrl::fromLocalFile(fName));

    // Copy the downloaded file into fName.
    fName = UpdaterComponent::instance().resolveFileName( QString("${INCOMING}/%1").arg(fName) );
    bool success = QFile::copy( d->downloader->downloadedFileName(), fName );
    if(success)
    {
        GCF_UPDATER_LOG( QString("Update for %1-%2 stored in %4")
                         .arg(uInfo.name()).arg(uInfo.version())
                         .arg(fName) );

        // Mark that this file was successfully downloaded.
        d->dqSuccessFlagList[d->dqCurrentIndex] = true;
    }
    else
        GCF_UPDATER_ERROR( QString("Could not copy %1").arg(d->downloader->downloadedFileName()) );

    // Release the currently used downloader
    if(d->downloader)
        d->downloader->deleteLater();
    d->downloader = 0;

    // Increment downloaded updates counter
    ++d->downloadFinishedCount;

    // Now download the next file in the queue.
    QMetaObject::invokeMethod(this, "downloadNextUpdate", Qt::QueuedConnection);
}

void GCF::UpdateDownloaderThread::downloadFailure()
{
    /* This function must be called only when the thread is actually operational. Not otherwise */
    if(!this->isRunning())
        return;

    UpdateInfo uInfo = d->dqList.at(d->dqCurrentIndex);
    GCF_UPDATER_ERROR( QString("Could not download update file for %1-%2").arg(uInfo.name()).arg(uInfo.version()) );

    // Release the currently used downloader
    if(d->downloader)
        d->downloader->deleteLater();
    d->downloader = 0;

    // Now download the next file in the queue.
    QMetaObject::invokeMethod(this, "downloadNextUpdate", Qt::QueuedConnection);
}

void GCF::UpdateDownloaderThread::downloadProgress(int percent)
{
    /* This function must be called only when the thread is actually operational. Not otherwise */
    if(!this->isRunning())
        return;

    double percentPerIndex = 100.0 / double(d->dqList.count());
    double percent2 = d->dqCurrentIndex*percentPerIndex + double(percent)*percentPerIndex/100.0;
    emit progress( int(percent2) );

    UpdateInfo uInfo = d->dqList.at(d->dqCurrentIndex);
    GCF_UPDATER_LOG( QString("Downloading update for %1-%2: %3 percent done..")
                     .arg(uInfo.name()).arg(uInfo.version()).arg(percent2) );
}

void GCF::UpdateDownloaderThread::allUpdatesDownloaded()
{
    /* This function must be called only when the thread is actually operational. Not otherwise */
    if(!this->isRunning())
        return;

    // Release the currently used downloader
    if(d->downloader)
        d->downloader->deleteLater();
    d->downloader = 0;

    GCF_UPDATER_LOG("Update files for all updates have been downloaded");

    // Now check which of the updates are completely downloaded and which are not.
    GCF_UPDATER_LOG( QString("Creating a list of fully downloaded updates from %1 downloaded ones").arg(d->dqList.count()) );
    UpdateInfoList fullyDownloadedList;
    for(int i=0; i<d->dqList.count(); i++)
    {
        UpdateInfo uInfo = d->dqList.at(i);
        if( !d->isUpdateFullyAvailable(uInfo) )
            continue;

        fullyDownloadedList.append(uInfo);
    }

    // Now dump the fullyDownloadedList into a AvailableUpdates.xml
    fullyDownloadedList.sort();
    GCF_UPDATER_LOG( QString("%1 fully downloaded updates found").arg(fullyDownloadedList.count()) );

    GCF_UPDATER_LOG("Dumping information about fully downloaded updates to AvailableUpdates.xml");
    QDomDocument doc = fullyDownloadedList.toDomDocument();
    QString auFileName = UpdaterComponent::instance().resolveFileName("${AVAIL_UPDATES_XML}");
    QFile auFile(auFileName);
    if( !auFile.open(QFile::WriteOnly) )
        GCF_UPDATER_ERROR("Cannot write into AvailableUpdates.xml");
    else
    {
        QString auXml = doc.toString(2);
        auFile.write( auXml.toAscii() );
        auFile.close();
    }

    // Now remove updates in fullyDownloadedList from d->dqList and dump the
    // remaining into DownloadQueue.xml, so that the remaining updates can
    // be downloaded the next time around.
    GCF_UPDATER_LOG("Creating a list of updates that are yet to be downloaded");
    UpdateInfoList yetToDownloadList;
    QList<bool> successFlagList;
    for(int i=0; i<d->dqList.count(); i++)
    {
        UpdateInfo uInfo = d->dqList.at(i);
        if( fullyDownloadedList.contains(uInfo) )
            continue;

        yetToDownloadList.append( uInfo );
        successFlagList.append( d->dqSuccessFlagList.at(i) );
    }

    // Dump this into DownloadQueue.xml
    GCF_UPDATER_LOG( QString("%1 updates are yet to be downloaded. Updating DownloadQueue.xml accordingly")
                     .arg(yetToDownloadList.count()) );
    QDomDocument dqDoc = yetToDownloadList.toDomDocument();
    if( yetToDownloadList.count() )
    {
        QDomElement dqUpdateE = dqDoc.documentElement().firstChildElement("Update");
        for(int i=0; i<successFlagList.count(); i++)
        {
            bool flag = successFlagList.at(i);
            dqUpdateE.setAttribute("downloaded", flag ? "true" : "false");
            dqUpdateE = dqUpdateE.nextSiblingElement("Update");
        }
    }
    QString dqXmlStr = yetToDownloadList.count() ? dqDoc.toString() : QString();

    QString dqFileName = UpdaterComponent::instance().resolveFileName("${DQ_XML}");
    QFile dqFile(dqFileName);
    dqFile.open(QFile::WriteOnly);
    dqFile.write(dqXmlStr.toAscii());
    dqFile.close();

    // Release all memory used
    d->dqList.clear();
    d->dqCurrentIndex = -1;
    d->dqSuccessFlagList.resize(0);
    d->stopFlag = false;
    if(d->downloader)
        delete d->downloader;
    d->downloader = 0;

    // Emit final progress update
    emit progress( 100 );

    // Since we are done, we can quit the thread now
    quit();
}

bool GCF::UpdateDownloaderThreadData::isUpdateFullyAvailable(const UpdateInfo& info)
{
    static QList<UpdateInfo> cyclicDepList;

    if(cyclicDepList.contains(info))
        return false;

    cyclicDepList.append(info);

    int index = dqList.indexOf(info);
    bool success = dqSuccessFlagList.at(index);
    if( !success )
    {
        cyclicDepList.removeLast();
        return false;
    }

    QList<UpdateInfo> deps = info.dependencies();
    if(deps.count() == 0)
    {
        cyclicDepList.removeLast();
        return true;
    }

    for(int i=0; i<deps.count(); i++)
    {
        UpdateInfo depInfo = deps.at(i);
        if( !isUpdateFullyAvailable(depInfo) )
        {
            cyclicDepList.removeLast();
            return false;
        }
    }

    cyclicDepList.removeLast();
    return true;
}

