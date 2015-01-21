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
#include "UpdateXMLParser.h"
#include "FileDownloader.h"

#include <QUrl>
#include <QFile>
#include <QVariant>
#include <QFileInfo>
#include <QDomDocument>
#include <QCoreApplication>

/**
\internal
\ingroup updater
\class GCF::UpdateXMLParser UpdateXMLParser
\brief Downloads and parses UpdateXML files from server-sites.

This class downloads Updates.xml file given a server-site URL and updates a
\ref GCF::UpdateInfoList instance with the newly found updates. Dependencies are
resolved while updating the \ref GCF::UpdateInfoList.

The best way to use this class is

\code
GCF::UpdateXMLParser parser;
GCF::UpdateInfoList updates;
parser.beginParse( QUrl("http://www.vcreatelogic.com/vtkdesigner/2.7.1/updates"),
                   updates );

// Wait for the parseDone() signal.

// Now "updates" contains information about updates available from both the server
// sites.
\endcode
*/
namespace GCF
{
    struct UpdateXMLParserData
    {
        UpdateXMLParserData() : urlsAreAbsolute(false), updateInfoList(0) { }

        QUrl currentUrl;
        QString currentSource;
        bool urlsAreAbsolute;
        UpdateInfoList* updateInfoList;

        bool parse(const QDomElement& updateE, UpdateInfoList& uiList);
        bool parse(const QDomDocument& doc, UpdateInfoList& uiList);
    };
}

/**
Constructor
*/
GCF::UpdateXMLParser::UpdateXMLParser()
{
    d = new GCF::UpdateXMLParserData;
}

/**
Destructor. Normally not advisable to destroy this object when \ref isBusy() is
returning true.
*/
GCF::UpdateXMLParser::~UpdateXMLParser()
{
    delete d;
}

/**
Looks for a file called Updates.xml within updateXmlUrl, downloads it, parses it and updates
the \c uiList with UpdateInfo objects found in the file.

The function blocks, but does not freeze UI.
*/
bool GCF::UpdateXMLParser::parse(const QUrl& updateXmlUrl, UpdateInfoList& uiList)
{
    // Construct a URL for downloading the Updates.xml file within the
    // given URL location
    QUrl url = updateXmlUrl;
    url.setPath( QString("%1/Updates.xml").arg(url.path()) );

    QString urlString = url.toString();

    // Download the file
    QString updateXmlFile = GCF::AbstractFileDownloader::downloadFile(url);
    if(updateXmlFile.isEmpty())
    {
        GCF_UPDATER_LOG( QString("Cannot download %1 for parsing").arg(urlString) );
        return false;
    }

    // Load the XML file
    QFile file(updateXmlFile);
    if(!file.open(QFile::ReadOnly))
    {
        GCF_UPDATER_LOG( QString("Cannot open downloaded file %1 for parsing").arg(urlString) );
        return false;
    }

    if( file.size() <= qint64(1) )
    {
        GCF_UPDATER_LOG( QString("Cannot open downloaded file %1 for parsing").arg(urlString) );
        return false;
    }

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        GCF_UPDATER_LOG( QString("Cannot parse %1. Malformed XML.").arg(urlString) );
        return false;
    }

    // Setup global variables
    d->currentUrl = updateXmlUrl;
    d->urlsAreAbsolute = false;
    d->updateInfoList = &uiList;

    bool success = d->parse( doc, uiList );
    uiList.sort();

    d->currentUrl = QUrl();
    d->currentSource.clear();
    d->updateInfoList = 0;

    return success;
}

/**
Parses Updates.xml file in the local file system. The function returns only after the entire
file has been parsed and \c uiList has been updated with the parsed information.

The function returns true on successful parse, false otherwise.
*/
bool GCF::UpdateXMLParser::parse(const QString& fileName, UpdateInfoList& uiList, bool urlsAreAbsolute)
{
    if(d->updateInfoList)
        return false;

    QFile file(fileName);
    if( !file.open(QFile::ReadOnly) )
    {
        GCF_UPDATER_ERROR( QString("Cannot open \"%1\" for reading..").arg(fileName) );
        return false;
    }

    if(file.size() <= qint64(1))
        return true; // special case. parsing was successful, zero update infos found.

    QDomDocument doc;
    if( !doc.setContent( &file ) )
    {
        GCF_UPDATER_ERROR( QString("Cannot parse XML file \"%1\"").arg(fileName) );
        return false;
    }

    QFileInfo fi(fileName);
    d->currentUrl = QUrl::fromLocalFile( fi.absolutePath() );
    d->urlsAreAbsolute = urlsAreAbsolute;
    d->updateInfoList = &uiList;

    bool success = d->parse( doc, uiList );
    uiList.sort();

    d->currentUrl = QUrl();
    d->currentSource.clear();
    d->updateInfoList = 0;

    return success;
}

bool GCF::UpdateXMLParserData::parse(const QDomDocument& doc, UpdateInfoList& uiList)
{
    QDomElement rootE = doc.documentElement();
    if( rootE.tagName() != "Updates" )
    {
        GCF_UPDATER_ERROR("Invalid Update XML file");
        return false;
    }

    QString baseline = rootE.attribute("Baseline");
    if( baseline != qApp->applicationVersion() )
    {
        GCF_UPDATER_ERROR("Baseline of application doenst match with Update XML");
        return false;
    }

    currentSource = rootE.firstChildElement("Source").text();
    if(currentSource.isEmpty())
        currentSource = currentUrl.toString();

    QDomElement updateE = rootE.firstChildElement("Update");
    while( !updateE.isNull() )
    {
        parse(updateE, uiList);

        updateE = updateE.nextSiblingElement("Update");
    }

    GCF_UPDATER_LOG("Successfully parsed Update XML");
    return true;
}

bool GCF::UpdateXMLParserData::parse(const QDomElement& updateE, UpdateInfoList& uiList)
{
    QString name = updateE.firstChildElement("Name").text();
    QString source = updateE.firstChildElement("Source").text();
    QString iconUrlStr = updateE.firstChildElement("Icon").text();
    QString title = updateE.firstChildElement("Title").text();
    QString type = updateE.firstChildElement("Type").text();
    bool forceInstall = QVariant( updateE.firstChildElement("ForceInstall").text() ).toBool();
    QString desc = updateE.firstChildElement("Description").text();
    QDateTime release = QDateTime::fromString( updateE.firstChildElement("Release").text() );
    QString version = updateE.firstChildElement("Version").text();

    // Atleast a name and version number has to be provided
    if(name.isEmpty() || version.isEmpty())
    {
        GCF_UPDATER_ERROR("Name or version number of the update not speicifed for Update");
        return false;
    }

    // Resolve iconFileName
    if(!iconUrlStr.startsWith("file:///") &&
       !iconUrlStr.startsWith("ftp://")   &&
       !iconUrlStr.startsWith("http://")     )
       /// ### We need a way to find out if it is a complete path or relative path
       /// QUrl(iconUrlStr, QUrl::StrictMode) doesnt help!
    {
        QUrl iconUrl = currentUrl;
        iconUrl.setPath( QString("%1/%2").arg(currentUrl.path()).arg(iconUrlStr) );
        iconUrlStr = iconUrl.toString();
    }

    QList<UpdateFileInfo> uFileInfoList;
    QDomElement ufiE = updateE.firstChildElement("UpdateFile");
    while( !ufiE.isNull() )
    {
        UpdateFileInfo ufi;
        ufi.Arch = ufiE.attribute("Arch");
        ufi.OS = ufiE.attribute("OS");
        if(urlsAreAbsolute)
            ufi.UpdateFileURL = QUrl( ufiE.text() );
        else
        {
            ufi.UpdateFileURL = currentUrl;
            ufi.UpdateFileURL.setPath( QString("%1/%2").arg( currentUrl.path() )
                                                       .arg( ufiE.text() )
                                     );
        }

        uFileInfoList.append( ufi );

        ufiE = ufiE.nextSiblingElement("UpdateFile");
    }

    // Atleast one single updateFile has to be provided
    if(uFileInfoList.count() == 0)
    {
        GCF_UPDATER_WARNING(
            QString("No update file specified for %1 [%2]")
            .arg(name).arg(version)
            );
    }

    // Lets create an UpdateInfo instance now.
    UpdateInfo updateInfo = uiList.addIfNotFound(name, version);
    if(updateInfo.isValid())
    {
        GCF_UPDATER_ERROR(
            QString("Duplicate update record found for %1 [%2]. Aborting add")
            .arg(name)
            .arg(version)
            );
        return false;
    }
    updateInfo.setTitle(title);
    if(source.isEmpty())
        updateInfo.setSource(this->currentSource);
    else
        updateInfo.setSource(source);
    updateInfo.setIconFromUrl( QUrl(iconUrlStr) );
    updateInfo.setType(type);
    updateInfo.setForceInstall(forceInstall);
    updateInfo.setDescription(desc);
    updateInfo.setRelease(release);
    updateInfo.setUpdateFiles(uFileInfoList);
    updateInfo.setValid(true);

    // Look into the dependency list and add dependencies.
    QDomElement dependsOnE = updateE.firstChildElement("DependsOn");
    while(!dependsOnE.isNull())
    {
        QString dependsOn = dependsOnE.text();
        QString dname = dependsOn.section("[", 0, 0).simplified();
        QString dversion = dependsOn.section("[", 1, 1).section("]", 0, 0).simplified();

        UpdateInfo dUpdateInfo = uiList.addIfNotFound(dname, dversion);
        if(dUpdateInfo != updateInfo) // avoid in-your-face cyclic dependencies
            updateInfo.addDependency( dUpdateInfo );

        dependsOnE = dependsOnE.nextSiblingElement("DependsOn");
    }

    return true;
}

/**
\fn void GCF::UpdateXMLParser::updateXmlDownloadFailure()

This signal is emitted if the parser was unable to download the Updates.xml file.
*/

/**
\fn void GCF::UpdateXMLParser::updateXmlDownloaded()

This signal is emitted after the XML file has been downloaded
*/

/**
\fn void GCF::UpdateXMLParser::parseDone()

This signal is emitted after the parsing is done.
*/
