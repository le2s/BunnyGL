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

#include "UpdateInstaller.h"
#include "ui_UpdateInstaller.h"

#include "UpdateFileUncompressor.h"
#include "SystemObject.h"
#include "FileSystemObject.h"
#include "FileObject.h"

#include <QCoreApplication>
#include <QPointer>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QScriptEngine>
#include <QScriptValue>
#include <QMessageBox>
#include <QProcess>
#include <QtDebug>

QString UpdateInstaller::ApplicationName = QString();
QString UpdateInstaller::ApplicationPath = QString();
QString UpdateInstaller::GuiXmlDirPath = QString();
QString UpdateInstaller::UpdatesDirPath = QString();

// Helper structures
/*
I am sure there is an obvious question.
   Why are we not using GCF::UpdateInfo and GCF::UpdateXMLParser here?

GCF::UpdateInfo and GCF::UpdateXMLParser depend on the UpdaterComponent. We
dont want to pull all that dependency in here, hence a new way to read and
update UpdateXML files.
*/
struct Update
{
    Update() : ForceInstall(false), ReferenceCount(0) { }

    QString         Name;
    QString         Title;
    QString         IconFile;
    QString         Source;
    bool            ForceInstall;
    QString         Version;
    QString         UpdateFile;
    int             ReferenceCount;
    QDomElement     UpdateElement;
};

struct OkButtonDisabler
{
public:
    OkButtonDisabler(QPushButton* btn) {
        m_button = btn;
        m_button->setEnabled(false);
    }

    ~OkButtonDisabler() {
        m_button->setEnabled(true);
    }

private:
    QPushButton* m_button;
};

bool UpdateLessThanEvaluator(const Update& u1, const Update& u2)
{
    // We need updates with higher reference counts showing up
    // first in the list of updates. So we make use of >
    // operator to compare ref counts, and not < operator.
    return u1.ReferenceCount > u2.ReferenceCount;
}

struct UpdateInstallerData
{
    UpdateInstallerData() : log(0) { }

    Ui::UpdateInstaller ui;

    QMap<QString, QString> replacementTable;
    QString appDir;
    QString guixmlDir;
    QString guixmlFile; // here guixml refers to ComponentLoaderComponent.xml
    QString updatesDir;
    QString incomingUpdatesDir;
    QString cacheUpdatesDir;
    QString temporaryUpdatesDir;
    QString availableUpdatesFile;
    QString installedUpdatesFile;
    QString recentlyInstalledUpdatesFile;
    QString logFileName;
    QString baseline;

    QDomDocument guixml;
    QDomDocument availableUpdates;
    QDomDocument installedUpdates;
    QDomDocument recentlyInstalledUpdates;

    QString currentUpdateInfo;

    GCF::Log* log;

    // Helper methods
    void loadXmlFile(const QString& fileName, QDomDocument& doc);
    void saveXmlFile(const QDomDocument& doc, const QString& fileName);
    QList<Update> computeInstallOrder();
    bool installUpdate(const Update& update);
    Update& findUpdate(QList<Update>& updateList, const QString& updateName) const;
    void deleteDirectory(const QString& dir, bool deleteDir=true);
    void registerAsInstalled(const Update& u);

    int progressAlreadyMade;
    int progressStepAllocated;
    void defineProgress(int pam, int psa) {
        progressAlreadyMade = pam;
        progressStepAllocated = psa;
    }
    int computeProgressPercent(int pc);
};

UpdateInstaller& UpdateInstaller::instance()
{
    static QPointer<UpdateInstaller> ptr = new UpdateInstaller(0);
    return *(ptr.data());
}

UpdateInstaller::UpdateInstaller(QWidget* parent)
:QWidget(parent)
{
    d = new UpdateInstallerData;
    d->progressAlreadyMade = 0;
    d->progressStepAllocated = 0;

    // Key paths and file names
    d->appDir = ApplicationPath;
    d->guixmlDir = GuiXmlDirPath;
    d->guixmlFile = QString("%1/ComponentLoaderComponent.xml").arg(GuiXmlDirPath);
    d->updatesDir = UpdatesDirPath;
    d->incomingUpdatesDir = QString("%1/Incoming").arg(d->updatesDir);
    d->cacheUpdatesDir = QString("%1/Cache").arg(d->updatesDir);
    d->availableUpdatesFile = QString("%1/System/AvailableUpdates.xml").arg(d->updatesDir);
    d->installedUpdatesFile = QString("%1/System/InstalledUpdates.xml").arg(d->updatesDir);
    d->recentlyInstalledUpdatesFile = QString("%1/System/RecentlyInstalledUpdates.xml").arg(d->updatesDir);
    d->temporaryUpdatesDir = QString("%1/Temp").arg(d->updatesDir);
    d->logFileName = QString("%1/System/UINSTLog.html").arg(d->updatesDir);

    // Configure the replacement table
    d->replacementTable["${APP}"] = ApplicationName;
    d->replacementTable["${APPDIR}"] = d->appDir;
    d->replacementTable["${GUIXMLDIR}"] = GuiXmlDirPath;
    d->replacementTable["${HOME}"] = QDir::homePath();
    d->replacementTable["${TEMPDIR}"] = d->temporaryUpdatesDir;
    d->replacementTable["${UPDATES}"] = d->updatesDir;
    d->replacementTable["${INCOMING}"] = d->incomingUpdatesDir;
    d->replacementTable["${CACHE}"] = d->cacheUpdatesDir;
    d->replacementTable["${SYSTEM}"] = QString("%1/System").arg(d->updatesDir);

    d->ui.setupUi(this);

    QString prettyAppName = ApplicationName;
#ifdef Q_WS_WIN
    prettyAppName = prettyAppName.remove(".exe", Qt::CaseInsensitive);
#endif

    d->ui.label->setText( QString("Installing Updates for %1").arg(prettyAppName) );
    setWindowTitle( d->ui.label->text() );

    QMetaObject::invokeMethod(this, "doInstallUpdates", Qt::QueuedConnection);
}

UpdateInstaller::~UpdateInstaller()
{
    delete d;
}

QString UpdateInstaller::resolveFileName(const QString& fileName) const
{
    QMap<QString,QString>::const_iterator it = d->replacementTable.begin();
    QMap<QString,QString>::const_iterator end = d->replacementTable.end();
    QString ret = fileName;
    while(it != end)
    {
        ret = ret.replace(it.key(), it.value());
        ++it;
    }

    return ret;
}

QWidget* UpdateInstaller::installerWindow() const
{
    return const_cast<UpdateInstaller*>(this);
}

void UpdateInstaller::progress(int pc, const QString& msg)
{
    int pc2 = d->computeProgressPercent(pc);
    d->ui.progressBar->setValue(pc2);
    d->ui.textEdit->append(msg);
    GCF_UINST_LOG( QString("Progress [%1 percent] - %2").arg(pc2).arg(msg) );
}

QDomDocument UpdateInstaller::clcGuiXml() const
{
    return d->guixml;
}

QString UpdateInstaller::currentUpdate() const
{
    return d->currentUpdateInfo;
}

/*
From here on you can find functions that actually perform installation of updates.
Starting with doInstallUpdates(), which is launched after the window is shown.
*/
void UpdateInstaller::doInstallUpdates()
{
    OkButtonDisabler obd(d->ui.cmdOk);

    // First we need to figure out the order in which updates have to be installed.
    GCF_UINST_LOG("Beginning to install updates, with settings as outlined below");
    GCF_UINST_LOG( QString(" -- ApplicationName = %1").arg(ApplicationName) );
    GCF_UINST_LOG( QString(" -- ApplicationPath = %1").arg(ApplicationPath) );
    GCF_UINST_LOG( QString(" -- GuiXmlDirPath   = %1").arg(GuiXmlDirPath) );
    GCF_UINST_LOG( QString(" -- UpdatesDirPath  = %1").arg(UpdatesDirPath) );

    // Create a default GUIXML file if one is not already available.
    if(!QFile::exists(d->guixmlFile))
    {
        QFile gxf(d->guixmlFile);
        gxf.open(QFile::WriteOnly);
        gxf.write("<component name=\"ComponentLoaderComponent\">\n"
                  "  <object name=\"controller\"/>\n"
                  "</component>");
        gxf.close();
    }

    // Open key paths and files
    d->loadXmlFile(d->guixmlFile, d->guixml);
    d->loadXmlFile(d->availableUpdatesFile, d->availableUpdates);
    d->loadXmlFile(d->installedUpdatesFile, d->installedUpdates);

    // Whats the baseline against which we are installing?
    d->baseline = d->availableUpdates.documentElement().attribute("Baseline");

    d->defineProgress(0, 10);
    GCF_UINST_LOG("Computing install order...");
    QList<Update> availableUpdateList = d->computeInstallOrder();

    // Then we need to install one update at a time.
    int progressStep = int(80.0 / double( availableUpdateList.count() ));
    GCF_UINST_LOG( QString("Going to install %1 updates").arg(availableUpdateList.count()) );
    for(int i=0; i<availableUpdateList.count(); i++)
    {
        Update u = availableUpdateList[i];
        d->defineProgress(d->progressAlreadyMade+(i?progressStep:10), progressStep);
        progress(0, QString("Installing update \"%1\"").arg(u.Name));

        // Keep track of the current working directory, for restoration
        // after the update has been installed.
        QString cwd = QDir::currentPath();

        // Actually perform the installation
        d->currentUpdateInfo = QString("%1 Version %2").arg(u.Name).arg(u.Version);
        bool success = d->installUpdate(u);
        d->currentUpdateInfo.clear();

        if(success)
            GCF_UINST_LOG( QString(" - Update %1 installed successfully").arg(u.Name) );
        else
            GCF_UINST_ERROR( QString(" - Update %1 installation failed").arg(u.Name) );

        // Restore working directory
        QDir::setCurrent(cwd);

        // Release FileObject instances created during the installation of the
        // update.
        FileSystemObject::instance().releaseAllFileObjects();

        if( !success )
        {
            progress(100, "Cannot install updates. Please send the log file to your vendor..");
            return;
        }

        // Dump installed components information into the InstalledUpdates.xml file
        d->registerAsInstalled(u);

        // Remove update information from Updates.xml file
        d->availableUpdates.documentElement().removeChild( u.UpdateElement );

        // Move the file from the Incoming directory to Cache directory
        GCF_UINST_LOG("Moving update file from Incoming to Cache directory");
        FileSystemObject::instance().Move(
                QString("%1/%2").arg(d->incomingUpdatesDir).arg(u.UpdateFile),
                QString("%1/%2").arg(d->cacheUpdatesDir).arg(u.UpdateFile)
            );
    }

    // Cleaning up
    d->defineProgress(90, 10);
    progress(100, "Updates installed");

    // Save key files
    d->saveXmlFile(d->guixml, d->guixmlFile);
    d->saveXmlFile(d->availableUpdates, d->availableUpdatesFile);
    d->saveXmlFile(d->installedUpdates, d->installedUpdatesFile);
    d->saveXmlFile(d->recentlyInstalledUpdates, d->recentlyInstalledUpdatesFile);

    d->ui.textEdit->append("Updates have been installed.");
    d->ui.cmdOk->setEnabled(true);
}

/**
\internal
*/
GCF::Log* UpdateInstaller::log() const
{
    if(!d->log)
    {
        QString logFile = d->logFileName;
        GCF::AbstractLogBackend* backend = GCF::Logger::instance().fileBackend(logFile);
        GCF::AbstractLogLayout*  htmlLayout = GCF::Logger::instance().htmlLayout();
        d->log = GCF::Logger::instance().log("UINST", backend, htmlLayout);

        d->log->addBackend( GCF::Logger::instance().consoleBackend(), GCF::Logger::instance().simpleLayout() );
    }

    return d->log;
}

void UpdateInstallerData::loadXmlFile(const QString& fileName, QDomDocument& doc)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);

    bool success = doc.setContent( &file );
    if(success)
        GCF_UINST_LOG( QString("XML File %1 loaded successfully...").arg(fileName) );
    else
        GCF_UINST_ERROR( QString("XML File %1 load failure...").arg(fileName) );
}

void UpdateInstallerData::saveXmlFile(const QDomDocument& doc, const QString& fileName)
{
    QFile file(fileName);
    file.open(QFile::WriteOnly);

    QByteArray ba = doc.toString(2).toAscii();
    qint32 bytesWritten = file.write( ba );

    if(bytesWritten != ba.size())
        GCF_UINST_ERROR( QString("Could not write into file %1...").arg(fileName) );
    else
        GCF_UINST_LOG( QString("Written %2 bytes into %1 ").arg(fileName).arg(bytesWritten) );
}

QList<Update> UpdateInstallerData::computeInstallOrder()
{
    /*
    The Updates XML file will look something like this

    <Updates Baseline="2.7.1">

        <Update>

            <Name>ScriptEditorComponent</Name>
            <ForceInstall>true</ForceInstall>
            <Version>2.7.2</Version>
            <UpdateFile>ScriptEditorUpdate-2.7.2-Windows-x86.gvz</UpdateFile>
            <DependsOn>GCF [2.7.1]</DependsOn>
            <DependsOn>VTK Designer [2.7.1]</DependsOn>

        </Update>

        <Update>
            ....
        </Update>

    </Updates>

    Notice that all fields from the Updates.xml (hosted on the update-server-site) are
    not available here. This is because the update installer need not know about them.
    */

    // Parse the Updates XML document and create the updates list
    UpdateInstaller::instance().progress(0, "Loading information about available updates");

    QList<Update>       availableUpdateList;
    QStringList         dependsOnList;
    QDomElement rootE = availableUpdates.documentElement();
    QDomElement availUpdateE = rootE.firstChildElement("Update");
    while(!availUpdateE.isNull())
    {
        Update u;
        u.Name = availUpdateE.firstChildElement("Name").text();
        u.Title = availUpdateE.firstChildElement("Title").text();
        u.Source = availUpdateE.firstChildElement("Source").text();
        u.ForceInstall = (availUpdateE.firstChildElement("ForceInstall").text()=="true");
        u.Version = availUpdateE.firstChildElement("Version").text();
        u.UpdateFile = availUpdateE.firstChildElement("UpdateFile").text();
        u.IconFile = availUpdateE.firstChildElement("Icon").text();
        u.UpdateElement = availUpdateE;
        if(u.UpdateFile.startsWith("file:///"))
            u.UpdateFile.remove(0, 8); // remove the file:/// prefix.
        else if(u.UpdateFile.startsWith("file:"))
            u.UpdateFile.remove(0, 5);
        if(u.ForceInstall)
            ++(u.ReferenceCount); // atleast one person wants to install it.
        availableUpdateList.append(u);

        QDomElement dependsOnE = availUpdateE.firstChildElement("DependsOn");
        QStringList dependsList;
        while( !dependsOnE.isNull() )
        {
            dependsOnList.append( dependsOnE.text() );
            dependsOnE = dependsOnE.nextSiblingElement( "DependsOn" );
        }

        availUpdateE = availUpdateE.nextSiblingElement("Update");
    }

    // Now resolve dependency information
    UpdateInstaller::instance().progress(30, QString("Resolving dependencies in %1 updates..").arg(availableUpdateList.count()));
    for(int i=0; i<dependsOnList.count(); i++)
    {
        QString dependsOn = dependsOnList[i];
        Update& u = findUpdate(availableUpdateList, dependsOn);
        ++(u.ReferenceCount);
    }

    // Now we need to sort the availableUpdateList function in the decreasing order of
    // Update::ReferenceCount.
    UpdateInstaller::instance().progress(60, "Sorting updates..");
    qSort(availableUpdateList.begin(), availableUpdateList.end(), UpdateLessThanEvaluator);

    // Now remove unwanted updates, ie the ones with Update::ReferenceCount=0
    UpdateInstaller::instance().progress(90, "Releasing unwanted updates..");
    while( availableUpdateList.count() )
    {
        Update& u = availableUpdateList.last();
        if( u.ReferenceCount > 0 )
            break;

        availableUpdateList.removeLast();
    }

    // Return the sorted list now!
    UpdateInstaller::instance().progress(100, "Computed update install order..");
    return availableUpdateList;
}

bool UpdateInstallerData::installUpdate(const Update& update)
{
    /*
    Consists of the following steps.

    1. Extract the update file (20 % of the work)
    2. Run the Install.js script (60 % of the work)
    3. Delete the extracted file contents (20% of the work)
    */

    // First check if the update file actually exists.
    QString updateFile = QString("%1/%2").arg(incomingUpdatesDir).arg(update.UpdateFile);
    if( !QFile::exists(updateFile) )
    {
        QString msg = QString("ERROR: Update file \"%1\" not found").arg(update.UpdateFile);
        UpdateInstaller::instance().progress(100, msg);
        return false;
    }

    UpdateInstaller::instance().progress(0, "  Unpacking the update..");
    UpdateFileUncompressor uncompressor;
    uncompressor.setFileName( updateFile );
    uncompressor.setDestination( temporaryUpdatesDir );
    QDir ufDir = uncompressor.uncompress();

    UpdateInstaller::instance().progress(10, "  Preparing the update-installer runtime..");

    // Prepare the script engine to evaluate things properly.
    QScriptEngine scriptEngine;
    scriptEngine.globalObject().setProperty("System", scriptEngine.newQObject(&SystemObject::instance()));
    scriptEngine.globalObject().setProperty("FileSystem", scriptEngine.newQObject(&FileSystemObject::instance()));

    // Change the working directory to the directory where the update files have been extracted
    QDir::setCurrent( ufDir.absolutePath() );
    replacementTable["${CWD}"] = QDir::currentPath();

    // Load install script
    QString installJsFileName = ufDir.absoluteFilePath("Install.js");
    QFile installJsFile(installJsFileName);
    installJsFile.open(QFile::ReadOnly);
    QString installJs = installJsFile.readAll();
    installJsFile.close();

    // Execute the script now
    UpdateInstaller::instance().progress(20, "  Installing the update..");

    // First check for syntax erros in the script.
    if( !scriptEngine.canEvaluate(installJs) )
    {
        UpdateInstaller::instance().progress(100, "  Cannot evaluate Install.js..");
        return false;
    }

    // Now actually evaluate the script.
    QScriptValue evalResult = scriptEngine.evaluate(installJs);

    // Search for the "DoInstall" function
    QScriptValue doInstallFn = scriptEngine.globalObject().property("DoInstall");
    if( !doInstallFn.isValid() || !doInstallFn.isFunction() )
    {
        UpdateInstaller::instance().progress(100, "  Could not find DoInstall() in Install.js ..");
        return false;
    }

    // Call the DoInstall method
    QScriptValue result = doInstallFn.call();
    if( result.toBoolean() == false )
    {
        UpdateInstaller::instance().progress(100, "  DoInstall() reported error while performing install..");
        return false;
    }

    // Report that the installation was successful, and cleanup all the temporary objects created by DoInstall()
    UpdateInstaller::instance().progress(80, "  Cleaning up temporary files...");
    QDir::setCurrent(updatesDir);
    deleteDirectory( ufDir.absolutePath() );
    replacementTable.remove( "${CWD}" );

    UpdateInstaller::instance().progress(100, "  SUCCESS: Finished installing update!");
    return true;
}

Update& UpdateInstallerData::findUpdate(QList<Update>& updateList, const QString& updateName) const
{
    QString name = updateName.section('[', 0, 0).simplified();
    QString version = QString(updateName).remove(name).remove('[').remove(']').simplified();

    for(int i=0; i<updateList.count(); i++)
    {
        Update& u = updateList[i];
        if( u.Name == name && u.Version == version )
            return u;
    }

    static Update dummy;
    return dummy;
}

void UpdateInstallerData::deleteDirectory(const QString& dir, bool deleteDir)
{
    QDir d(dir);

    QFileInfoList list = d.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);
    for(int i=0; i<list.count(); i++)
    {
        QFileInfo fi = list[i];
        if( fi.isFile() )
            QFile::remove( fi.absoluteFilePath() );
        else
        {
            QDir d2 = d;
            d2.cd( fi.fileName() );
            deleteDirectory( d2.absolutePath() );
        }
    }

    if(deleteDir)
    {
        d.cdUp();
        d.rmdir( QFileInfo(dir).fileName() );
    }
}

void UpdateInstallerData::registerAsInstalled(const Update& u)
{
    // First add information about the update to the InstalledUpdates.xml
    QDomElement rootE = installedUpdates.documentElement();
    if(rootE.isNull())
    {
        rootE = installedUpdates.createElement("Updates");
        rootE.setAttribute("Baseline", baseline);
        installedUpdates.appendChild( rootE );
    }

    QDomElement updateE = installedUpdates.createElement("Update");
    rootE.appendChild(updateE);

    QDomElement nameE = installedUpdates.createElement("Name");
    nameE.appendChild( installedUpdates.createTextNode(u.Name) );
    updateE.appendChild(nameE);

    QDomElement titleE = installedUpdates.createElement("Title");
    titleE.appendChild( installedUpdates.createTextNode(u.Title) );
    updateE.appendChild(titleE);

    QDomElement sourceE = installedUpdates.createElement("Source");
    sourceE.appendChild( installedUpdates.createTextNode(u.Source) );
    updateE.appendChild(sourceE);

    QDomElement iconE = installedUpdates.createElement("Icon");
    iconE.appendChild( installedUpdates.createTextNode(u.IconFile) );
    updateE.appendChild(iconE);

    QDomElement versionE = installedUpdates.createElement("Version");
    versionE.appendChild( installedUpdates.createTextNode(u.Version) );
    updateE.appendChild(versionE);

    QDomElement updateFileE = installedUpdates.createElement("UpdateFile");
    updateFileE.appendChild( installedUpdates.createTextNode(u.UpdateFile) );
    updateE.appendChild(updateFileE);

    // Add the element to recentlyInstalledUpdates also.
    if(recentlyInstalledUpdates.isNull())
        recentlyInstalledUpdates = QDomDocument("InstalledUpdates");

    rootE = recentlyInstalledUpdates.documentElement();
    if(rootE.isNull())
    {
        rootE = installedUpdates.createElement("Updates");
        rootE.setAttribute("Baseline", baseline);
        recentlyInstalledUpdates.appendChild( rootE );
    }
    rootE.appendChild( updateE.cloneNode(true) );

    // Now remove from AvailableUpdates.xml information about the installed
    // update
    rootE = availableUpdates.documentElement();
    updateE = rootE.firstChildElement("Update");
    while(!updateE.isNull())
    {
        QString name = updateE.firstChildElement("Name").text();
        QString version = updateE.firstChildElement("Version").text();
        if(name == u.Name && version == u.Version)
        {
            rootE.removeChild(updateE);
            break;
        }

        updateE = updateE.nextSiblingElement("Update");
    }
}

int UpdateInstallerData::computeProgressPercent(int pc)
{
    int pc2 = int( (double(pc)/100.0)*double(progressStepAllocated) );
    int ret = progressAlreadyMade+pc2;
    if( ret > 100 )
        ret = 100;

    return ret;
}


