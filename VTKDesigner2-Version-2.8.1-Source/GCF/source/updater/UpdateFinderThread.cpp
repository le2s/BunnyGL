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

#include <GCF/UpdaterComponent>

#include "UpdateFinderThread.h"
#include <GCF/UpdateInfo>
#include "UpdateXMLParser.h"

#include <QFile>
#include <QTimer>

namespace GCF
{
    struct UpdateFinderThreadData
    {
        UpdateFinderThreadData() : currentSite(-1), stopFlag(false),
            updateXmlParser(0) { }

        QStringList serverSites;
        QString foundUpdatesFile;
        QString installedUpdatesFile;
        int currentSite;
        bool stopFlag;
        UpdateXMLParser* updateXmlParser;
        UpdateInfoList updateInfoList;
    };
}

GCF::UpdateFinderThread::UpdateFinderThread(QObject* parent)
:QThread(parent)
{
    d = new UpdateFinderThreadData;
}

GCF::UpdateFinderThread::~UpdateFinderThread()
{
    delete d;
}

void GCF::UpdateFinderThread::stop()
{
    if(!isRunning())
        return;

    if(!d->stopFlag)
        d->stopFlag = true;

    // Finding will stop in the next tick.
    GCF_UPDATER_LOG("Stop flag set. Thread will stop at the next tick");
}

void GCF::UpdateFinderThread::run()
{
    // Initialize thread values
    d->stopFlag = false;
    d->serverSites = UpdaterComponent::instance().serverSiteList();
    if( !UpdaterComponent::instance().primaryServerSite().isEmpty() )
        d->serverSites.prepend( UpdaterComponent::instance().primaryServerSite() );
    d->foundUpdatesFile = UpdaterComponent::instance().resolveFileName("${UPDATES_XML}");
    d->installedUpdatesFile = UpdaterComponent::instance().resolveFileName("${INST_UPDATES_XML}");

    // If we dont have any server site to look into, whats the point in going ahead?
    if(!d->serverSites.count())
    {
        GCF_UPDATER_LOG("No server sites registered");
        return;
    }

    // Delete all updates found in the previous run (if any)
    d->updateInfoList.clear();
    d->currentSite = 0;

    // Download UpdateXML files from all the server sites and parse them into
    // a single UpdateInfoList structure
    parseUpdateXmlFiles();

    // Check for stop flag
    if(d->stopFlag)
    {
        emit updatesFound(0);
        return;
    }

    // Remove UpdateInfo's for updates that are already installed
    removeInstalledUpdatesInfo();

    // Check for stop flag
    if(d->stopFlag)
    {
        emit updatesFound(0);
        return;
    }

    int nrUpdates = d->updateInfoList.count();

    // Dump the information about found updates into Updates.xml
    GCF_UPDATER_LOG( QString("Found %1 updates. Dumping information about "
                             "found updates into Updates.xml").arg(nrUpdates) );
    QDomDocument doc = d->updateInfoList.toDomDocument();
    QString docStr = doc.toString(2);
    QFile file( d->foundUpdatesFile );
    file.open(QFile::WriteOnly);
    file.write( docStr.toAscii() );
    file.close();

    // Announce updates-found to rest of the world
    emit updatesFound(nrUpdates);
    GCF_UPDATER_LOG("All done!");
}

void GCF::UpdateFinderThread::parseUpdateXmlFiles()
{
    /* This function must be called only when the thread is actually operational. Not otherwise */
    if(!this->isRunning())
        return;

    // Progress indicators
    int nrSites = d->serverSites.count();
    double pc = 0;
    double pcStep = 100.0 / double(nrSites);

    while(d->currentSite < nrSites)
    {
        if(d->stopFlag)
            return;

        // Parse the current server site.
        QString serverSite = d->serverSites.at(d->currentSite);
        QUrl serverSiteUrl(serverSite);

        GCF_UPDATER_LOG( QString("Looking for updates in %1...").arg(serverSite) );
        UpdateXMLParser().parse(serverSiteUrl, d->updateInfoList);

        pc += pcStep;
        ++d->currentSite;

        emit progress( int(pc) );
    }
}

void GCF::UpdateFinderThread::removeInstalledUpdatesInfo()
{
    /* This function must be called only when the thread is actually operational. Not otherwise */
    if(!this->isRunning())
        return;

    GCF_UPDATER_LOG( QString("Cached update information from all the server sites. %1 updates found..").arg(d->updateInfoList.count()) );
    GCF_UPDATER_LOG("Processing update lists..");

    int count = 0;

    // Read information about the installed updates.
    UpdateInfoList installedUpdates;
    UpdateXMLParser installedUpdatesParser;
    bool success = installedUpdatesParser.parse(d->installedUpdatesFile, installedUpdates, true);
    if( !success )
    {
        GCF_UPDATER_ERROR( QString("Cannot parse \"%1\"").arg(d->installedUpdatesFile) );
        quit();
        return;
    }

    // we need to remove entries from d->updateInfoList that are
    // already in installedUpdates.
    GCF_UPDATER_LOG("Discarding updates that have already been installed");
    count = d->updateInfoList.count();
    for(int i=d->updateInfoList.count()-1; i>=0; i--)
    {
        UpdateInfo uInfo = d->updateInfoList[i];
        if( installedUpdates.containsUpdate(uInfo.name(), uInfo.version()) )
        {
            GCF_UPDATER_DEBUG( QString("Discarding update %1 [%2] because it is already installed").arg(uInfo.name()).arg(uInfo.version()) );
            d->updateInfoList.removeAt(i);
        }

        // whats the use of keeping an update if it cant be installed
        // on this hardware for this operating system
        else if( uInfo.updateFileForThisSystem().UpdateFileURL.isEmpty() )
        {
            GCF_UPDATER_DEBUG( QString("Discarding update %1 [%2] because it is not compatible with this system").arg(uInfo.name()).arg(uInfo.version()) );
            d->updateInfoList.removeAt(i);
        }
    }

    // Now we need to run a dependency check to see which of the updates
    // are properly available and which arent.
    // An update is properly available if
    //    a) All the updates that it depends on are already installed ---OR---
    //    b) they are properly available.
    // So it is kind of a recursive definition.
    GCF_UPDATER_LOG("Checking for dependencies...");
    count = d->updateInfoList.count();
    for(int i=count-1; i>=0; i--)
    {
        UpdateInfo uInfo = d->updateInfoList[i];

        QList<UpdateInfo> deps = uInfo.dependencies();
        for(int j=0; j<deps.count(); j++)
        {
            // Check to see if dependencies are also in the updateInfo list
            if( !d->updateInfoList.contains( deps[j] ) )
            {
                // then may be perhaps the update was already installed?
                if( !installedUpdates.containsUpdate(deps[j].name(), deps[j].version()) )
                {
                    GCF_UPDATER_LOG( QString("Update %1-%2 requires dependency %3-%4, which was not found.")
                                     .arg(uInfo.name()).arg(uInfo.version())
                                     .arg(deps[j].name()).arg(deps[j].version()) );
                    d->updateInfoList.removeAt(i);
                    break;
                }
            }
        }
    }

    // Now the updateInfo list is clear of all dependencies. We are ready to go!
    quit();
}

