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
#include <GCF/ComponentFactory>
#include <GCF/ComponentSettings>
#include <GCF/INewUpdatesHandler>
#include <GCF/Logger>
#include <GCF/ComponentLoaderComponent>
#include <GCF/IComponentHostWindow>

#include "IconCache.h"
#include "UpdateSelectionDialog.h"

#include <QDir>
#include <QMap>
#include <QFile>
#include <QMenu>
#include <QTimer>
#include <QLabel>
#include <QAction>
#include <QProcess>
#include <QMenuBar>
#include <QPointer>
#include <QToolTip>
#include <QFileInfo>
#include <QDateTime>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QMainWindow>
#include <QDialogButtonBox>
#include <QAbstractFileEngine>

#include "UpdateXMLParser.h"
#include "UpdaterConfigPage.h"
#include "UpdateFinderThread.h"
#include "UpdateDownloaderThread.h"

#include <QSystemTrayIcon>

/**
\defgroup updatergroup Software Updater Module

GCF version 2.2 and later includes a module called Software Updater. To
read more about the updater click on \ref updaterarch.

*/

/**
\ingroup updatergroup
\class GCF::UpdaterComponent <GCF/UpdaterComponent>
\brief Provides the updater component for your application

This component class forms the main class for the \ref updater. If you want
to make use of the automatic software updates feature in your applications, then you
must activate the component.

\section updatercomponent1 Activating the component

To activate the component, you must include a code similar to the one shown
below in your application's main() function. The \ref gcf_CreateAppTemplateWizard tool
automatically generates this stub if needed.

\code
// Start the updater component immediately to check for updates..
QString updatesDir("http://www.mywebsite.com/updates/product/1.0");
GCF::UpdaterComponent::instance().setPrimaryServerSite(updatesDir);
GCF::UpdaterComponent::instance().setAllowThirdPartyServerSites(false);
GCF::UpdaterComponent::instance().setCheckUpdateFrequency(GCF::UpdaterComponent::ApplicationStartupButOnceADay);
GCF::UpdaterComponent::instance().setUseInbuiltGUIXML(false);
GCF::UpdaterComponent::instance().setUpdateSelectionStrategy(GCF::UpdaterComponent::SelectByNotifyingUser);
GCF::UpdaterComponent::instance().initialize();
\endcode

\section updatercomponent2 The GUIXML file

The next step would be to provide an appropriate GUIXML file for the UpdaterComponent. Shown below is a sample
GUIXML file.

\code
<component name="UpdaterComponent">

    <object name="controller" />

    <action name="checkForUpdates" title="Check for Updates"
            triggerSlot="UpdaterComponent.controller::checkForUpdates()"
            icon=":/GCF/checkForUpdates.png" />

    <menu mergeWith="MainWindowComponent.helpMenu">
        <menuitem actionName="UpdaterComponent.checkForUpdates" />
    </menu>

</component>
\endcode

\note The action name "checkForUpdates" is reserved for its intended use
\note The action name "downloadUpdates" is reserved for its intended use

\section updatercomponent3 Operation Modes

You can have the Software Updater work in any of the following modes

\li \ref updatercomponent31
\li \ref updatercomponent32
\li \ref updatercomponent33
\li \ref updatercomponent34

\subsection updatercomponent31 Manual Mode

In this mode, the user is expected to make a decision as to when he/she
wants to update the application. The application will perform no update
unless the user asks for it.

This means
\li The user triggers the finding of updates
\li Once updates are found, the user has to mark which ones to download
and install.
\li Once selected, the installation is automatic.

To have the updater run in tihs mode, you will need to

\li Set CheckUpdateFrequency to UponUserRequest
( See \ref GCF::UpdaterComponent::setCheckUpdateFrequency() )
\li Set UpdateSelectionStrategy to SelectByAskingUser
( See \ref GCF::UpdaterComponent::setUpdateSelectionStrategy() )

The GUIXML file for such a mode of operation would be

\code
<component name="UpdaterComponent">

    <action name="checkForUpdates" title="Check for Updates"
            triggerSlot="UpdaterComponent.controller::checkForUpdates()" />
    <action name="downloadUpdates" title="Download Updates"
            triggerSlot="UpdaterComponent.controller::downloadUpdates()" />

    <menu name="updatesMenu" title="&amp;Updates">
        <menuitem actionName="UpdaterComponent.checkForUpdates" />
        <menuitem actionName="UpdaterComponent.downloadUpdates" />
    </menu>

</component>
\endcode

\subsection updatercomponent32 Fully Automatic Mode

In this mode, the UpdaterComponent automatically looks for updates; downloads
and installs them.

This means
\li \b No user input is needed for finding of updates. Finding of updates is
automatically done.
\li \b No user input is needed for selection of found updates that need to
be installed. Once updates are found, they automatically get selected for
download and install.
\li Installation of selected updates is automatic.

To have the updater run in tihs mode, you will need to

\li Set CheckUpdateFrequency to ApplicationStartupButOnceADay. You
can also use ApplicationStartup, OnceInAnHour, OnceInTwoHours or
OnceInFiveHours ( See \ref GCF::UpdaterComponent::setCheckUpdateFrequency() )
\li Set UpdateSelectionStrategy to SelectAllUpdates or SelectCriticalUpdates
( See \ref GCF::UpdaterComponent::setUpdateSelectionStrategy() )

The GUIXML file for such a mode of operation would be

\code
<component name="UpdaterComponent">

    <!-- no actions or menu items -->

</component>
\endcode

\note If the actions and/or menuitems are shown, UpdaterComponent will
continue to operate in fully-automatic mode only. If the UpdaterComponent
is busy, then either \c checkForUpdates or \c downloadUpdates action will
be shown as disabled.

\subsection updatercomponent33 Semi-Automatic Mode (intruding)

In this mode, UpdaterComponent automatically looks for updates. But the selection
and download of updates are based on user input.

This means
\li \b No user input is needed for finding of updates.  Finding of updates
happen automatically.
\li User input is needed for selection of found updates that need to
be installed.
\li Once selected, installation is automatic.

The catch here is: once updates are found, the user is immediately prompted
with a dialog box (shown below) that lists out all the updates found. He has
to select the updates that need to be installed.

\image html updateselectiiondialog.png

To have the updater run in tihs mode, you will need to

\li Set CheckUpdateFrequency to ApplicationStartupButOnceADay. You
can also use ApplicationStartup, OnceInAnHour, OnceInTwoHours or
OnceInFiveHours ( See \ref GCF::UpdaterComponent::setCheckUpdateFrequency() )
\li Set UpdateSelectionStrategy to SelectByAskingUser
( See \ref GCF::UpdaterComponent::setUpdateSelectionStrategy() )

The GUIXML file for such a mode of operation would be

\code
<component name="UpdaterComponent">

    <!-- no actions or menu items -->

</component>
\endcode

\note If the actions and/or menuitems are shown, UpdaterComponent will
continue to operate in semi-automatic mode only. If the UpdaterComponent
is busy, then either checkForUpdates or downloadUpdates action will
be shown as disabled.

\subsection updatercomponent34 Semi-Automatic Mode (non-intruding)

This mode is similar as \ref updatercomponent33. The only difference is
we set the UpdateSelectionStrategy to SelectByNotifyingUser.
( See \ref GCF::UpdaterComponent::setUpdateSelectionStrategy() ).

GCF will show a notification tip when new updates become available. But the
user will have to explicitly trigger the download of updates. For example
in "VTK Designer" the following type of notification is shown.

\image html updaternotification.png

The GUIXML file for such a mode of operation would be

\code
<component name="UpdaterComponent">
    <action name="downloadUpdates" title="Download Updates"
            triggerSlot="UpdaterComponent.controller::downloadUpdates()" />

    <menu name="updatesMenu" title="&amp;Updates">
        <menuitem actionName="UpdaterComponent.downloadUpdates" />
    </menu>
</component>
\endcode

\section updatercomponent4 Showing Updater Preferences

By default the Update-Preferences dialog is embedded within the global
configuration dialog box. If you want to show it in an independent dialog
box, then you can use a GUIXML file like the one shown below.

\code
<component name="UpdaterComponent">

    <action name="showConfigDialog" title="Preferences"
            triggerSlot="UpdaterComponent.controller::showConfigurationDialog()" />

    <menu name="updatesMenu" title="&amp;Updates">
        <menuitem actionName="UpdaterComponent.showConfigDialog" />
    </menu>

</component>
\endcode

Now, when the user selects Updates -> Preferences, he will see a dialog box
like the one shown below.

\image html updaterpref.png

\section updatercomponent5 Enable/Disable updates

There are three ways to enable/disable updates:

\li Check/Uncheck the "Enable Updater" checkbox in the preferences dialog box
\li Call the \ref GCF::UpdaterComponent::setEnabled() method in code
\li Configure the "Enabled" property via the GUIXML file of the UpdaterComponent.

\section updatercomponent6 Logging

The UpdaterComponent maintains a detailed log of every action and result encountered.
The log file is available within the Updates/System folder found in the same
directory as QApplication::applicationDirPath().

\sa \ref updater, \ref logger
*/

namespace GCF
{
    struct UpdaterComponentData
    {
        UpdaterComponentData() : allowThirdPartyServerSites(false),
                                 checkUpdateFrequency( GCF::UpdaterComponent::ApplicationStartupButOnceADay ),
                                 initialized(false), newUpdatesHandler(0), log(0),
                                 useInbuiltGUIXML(false),
                                 updateSelectionStrategy(GCF::UpdaterComponent::SelectByAskingUser),
                                 autoDownloadUpdates(true), enabled(true),
                                 checkForUpdates(0), downloadUpdates(0), triggeredByUser(false),
                                 systemTrayIcon(false) { }

        GCF::UpdaterComponent* updater;
        QString primarySite;
        QStringList serverSiteList;
        bool allowThirdPartyServerSites;
        GCF::UpdaterComponent::CheckUpdateFrequency checkUpdateFrequency;
        bool initialized;
        INewUpdatesHandler* newUpdatesHandler;
        QMap<QString, QString> replacementTable;
        GCF::Log* log;
        bool useInbuiltGUIXML;
        GCF::UpdaterComponent::UpdateSelectionStrategy updateSelectionStrategy;
        bool autoDownloadUpdates;
        bool enabled;

        QAction* checkForUpdates;
        QAction* downloadUpdates;
        QString checkForUpdatesText; // the action text set by GUIXML
        QString downloadUpdatesText;
        bool triggeredByUser;
        QSystemTrayIcon* systemTrayIcon;

        QPointer<UpdateFinderThread> updateFinderThread;
        QPointer<UpdateDownloaderThread> updateDownloaderThread;

        void createUpdatesDirectory();
        void createFile(const QString& fileName);

        bool checkForAvailableUpdates();
        bool installAvailableUpdates();

        bool checkDownloadQueue();

        bool isUpdateXmlFileEmpty(const QString& fileName);

        void connectToThread(QThread* thread);
        void stopThread(QThread* thread);

        QPointer<QWidget> showInformation(const QString& msg, int msecs=3000, bool useSystemTrayIcon=true);
    };
};

GCF_DEFINE_COMPONENT(GCF::UpdaterComponent)

/**
This function returns reference to the only instance of UpdaterComponent.
*/
GCF::UpdaterComponent& GCF::UpdaterComponent::instance()
{
    static GCF::UpdaterComponent* theInstance = GCF_CREATE_COMPONENT(GCF::UpdaterComponent);
    return *theInstance;
}

/**
Constructor of the UpdaterComponent.
*/
GCF::UpdaterComponent::UpdaterComponent()
{
    d = new UpdaterComponentData;
    d->updater = this;

    d->replacementTable["${APPDIR}"] = qApp->applicationDirPath();
    d->replacementTable["${HOME}"] = QDir::homePath();
    d->replacementTable["${TEMPDIR}"] = QDir::tempPath();
    d->replacementTable["${UPDATES}"] = QString("%1/Updates").arg(qApp->applicationDirPath());
    d->replacementTable["${INCOMING}"] = QString("%1/Updates/Incoming").arg(qApp->applicationDirPath());
    d->replacementTable["${CACHE}"] = QString("%1/Updates/Cache").arg(qApp->applicationDirPath());
    d->replacementTable["${SYSTEM}"] = QString("%1/Updates/System").arg(qApp->applicationDirPath());
    d->replacementTable["${DQ_XML}"] = QString("%1/Updates/System/DownloadQueue.xml").arg(qApp->applicationDirPath());
    d->replacementTable["${UPDATES_XML}"] = QString("%1/Updates/System/Updates.xml").arg(qApp->applicationDirPath());
    d->replacementTable["${AVAIL_UPDATES_XML}"] = QString("%1/Updates/System/AvailableUpdates.xml").arg(qApp->applicationDirPath());
    d->replacementTable["${INST_UPDATES_XML}"] = QString("%1/Updates/System/InstalledUpdates.xml").arg(qApp->applicationDirPath());
}

/**
Destructor
*/
GCF::UpdaterComponent::~UpdaterComponent()
{
    delete d;
}

/**
Enables/Disables the updater. If the function is called with \c val as \c false, then any pending
updater operations will be immediately stopped. If the function is called with \c val as \c true,
then updater's default mode will be triggered.

\sa \ref updatercomponent3
*/
void GCF::UpdaterComponent::setEnabled(bool val)
{
    if(d->enabled == val)
        return;

    d->enabled = val;

    if(!d->initialized)
        return;

    if(d->enabled)
    {
        if( d->checkForAvailableUpdates() )
            return;

        GCF_UPDATER_LOG("Checking to see if a download queue already exists...");
        if( d->checkDownloadQueue() )
        {
            GCF_UPDATER_LOG("Download queue already exists. Resuming download from last session..");
            downloadUpdateFiles();
        }
        else
        {
            GCF_UPDATER_LOG("No download queue from last time. Checking to see if I can look for updates now.");
            if(this->canCheckForUpdatesNow())
            {
                GCF_UPDATER_LOG("Checking for updates...");
                checkForUpdates();
            }
            else
                GCF_UPDATER_LOG("No updates to look for");
        }
    }
    else
        stopUpdater();
}

/**
Returns true if the updater is enabled, false otherwise.
*/
bool GCF::UpdaterComponent::isEnabled() const
{
    return d->enabled;
}

/**
Sets the primary server site where the updater will look for updates.

\note The function does nothing if the UpdaterComponent has already
been initialized.
*/
void GCF::UpdaterComponent::setPrimaryServerSite(const QString& url)
{
    if( d->initialized )
        return;

    d->primarySite = url;
    GCF_UPDATER_LOG( QString("Primary server site set to: \"%1\"").arg(url) );
}

/**
Returns the primary server site from where updater will download updates.
*/
QString GCF::UpdaterComponent::primaryServerSite() const
{
    return d->primarySite;
}

/**
Sets whether UpdaterComponent can accept third party server sites or not.

\note The function does nothing if the UpdaterComponent has already
been initialized.
*/
void GCF::UpdaterComponent::setAllowThirdPartyServerSites(bool val)
{
    if( d->initialized )
        return;

    d->allowThirdPartyServerSites = val;
    GCF_UPDATER_LOG( QString("Allow third party sites set to \"%1\"").arg( QVariant(val).toString() ) );
    if(!d->allowThirdPartyServerSites)
        d->serverSiteList.clear();
}

/**
Returns true if third party updates are supported, false otherwise.
*/
bool GCF::UpdaterComponent::isAllowThirdPartyServerSites() const
{
    return d->allowThirdPartyServerSites;
}

/**
Adds a 3rd party server site. The function does nothing if \ref isAllowThirdPartyServerSites()
returns false

\note The function does nothing if the UpdaterComponent has already
been initialized.
*/
void GCF::UpdaterComponent::addServerSite(const QString& url)
{
    if( d->initialized )
        return;

    if(d->allowThirdPartyServerSites)
    {
        if(d->primarySite == url || d->serverSiteList.contains(url))
            return;

        d->serverSiteList.append(url);
        GCF_UPDATER_LOG( QString("Added 3rd Party Server site: \"%1\"").arg(url) );
    }
}

/**
Returns a list of all the 3rd party server sites that the UpdaterComponent will
look for updates.
*/
QStringList GCF::UpdaterComponent::serverSiteList() const
{
    return d->serverSiteList;
}

/**
\internal
*/
void GCF::UpdaterComponent::setUseInbuiltGUIXML(bool val)
{
    if(d->initialized)
        return;

    d->useInbuiltGUIXML = val;
}

/**
\internal
*/
bool GCF::UpdaterComponent::isUseInbuiltGUIXML() const
{
    return d->useInbuiltGUIXML;
}

/**
Configures the frequency with which the updater will check for updates.

Valid values are
\li \c GCF::UpdaterComponent::ApplicationStartup
\li \c GCF::UpdaterComponent::ApplicationStartupButOnceADay
\li \c GCF::UpdaterComponent::UponUserRequest
\li \c GCF::UpdaterComponent::OnceInAnHour
\li \c GCF::UpdaterComponent::OnceInTwoHours
\li \c GCF::UpdaterComponent::OnceInFiveHours
*/
void GCF::UpdaterComponent::setCheckUpdateFrequency(GCF::UpdaterComponent::CheckUpdateFrequency freq)
{
    if( d->initialized )
        return;

    if(d->checkUpdateFrequency == freq)
        return;

    d->checkUpdateFrequency = freq;

    GCF_UPDATER_LOG( QString("Update frequency set to: \"%1\"").arg(d->checkUpdateFrequency) );
}

/**
Returns the current update frequency value

Valid return values are
\li \c GCF::UpdaterComponent::ApplicationStartup
\li \c GCF::UpdaterComponent::ApplicationStartupButOnceADay
\li \c GCF::UpdaterComponent::UponUserRequest
\li \c GCF::UpdaterComponent::OnceInAnHour
\li \c GCF::UpdaterComponent::OnceInTwoHours
\li \c GCF::UpdaterComponent::OnceInFiveHours
*/
GCF::UpdaterComponent::CheckUpdateFrequency GCF::UpdaterComponent::checkUpdateFrequency() const
{
    return d->checkUpdateFrequency;
}

/**
\internal
*/
void GCF::UpdaterComponent::setNewUpdatesHandler(INewUpdatesHandler* h)
{
    if( d->initialized )
        return;

    if( d->newUpdatesHandler == h )
        return;

    d->newUpdatesHandler = h;
    if(d->newUpdatesHandler)
        GCF_UPDATER_LOG( QString("INewUpdatesHandler is: \"%1\"").arg(h->containerObject()->metaObject()->className()) );
    else
        GCF_UPDATER_LOG("INewUpdatesHandler is set to NULL");
}

/**
\internal
*/
INewUpdatesHandler* GCF::UpdaterComponent::newUpdatesHandler() const
{
    return d->newUpdatesHandler;
}

/**
Returns the hardware architecture on which the UpdaterComponent is currently run.

\note We only support x86 right now, so this function always returns "x86".
*/
QString GCF::UpdaterComponent::architecture() const
{
    return "x86";
}

/**
Returns the operating system on which the UpdaterComponent is currently run.

Valid return values are "Unix", "Linux" and "Win32".
*/
QString GCF::UpdaterComponent::os() const
{
#ifdef Q_OS_LINUX
    return "Linux";
#endif

#ifdef Q_OS_UNIX
    return "Unix";
#endif

#ifdef Q_OS_WIN32
    return "Win32";
#endif
}

/**
\internal
*/
QString GCF::UpdaterComponent::resolveFileName(const QString& fileName) const
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

/**
\internal
*/
QObject* GCF::UpdaterComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split(".");

    if(comps.last() == "controller")
        return const_cast<UpdaterComponent*>(this);

    return 0;
}

/**
\internal
*/
QWidget* GCF::UpdaterComponent::fetchWidget(const QString& completeName) const
{
    Q_UNUSED(completeName);
    return 0;
}

/**
\internal
*/
QAction* GCF::UpdaterComponent::fetchAction(const QString& completeName) const
{
    QStringList comps = completeName.split(".");

    if(comps.last() == "checkForUpdates")
    {
        if(!d->checkForUpdates)
            d->checkForUpdates = GCF::AbstractComponent::fetchAction(completeName);
        return d->checkForUpdates;
    }

    if(comps.last() == "downloadUpdates")
    {
        if(!d->downloadUpdates)
            d->downloadUpdates = GCF::AbstractComponent::fetchAction(completeName);

        return d->downloadUpdates;
    }

    return GCF::AbstractComponent::fetchAction(completeName);
}

/**
\internal
*/
void GCF::UpdaterComponent::componentCreated(GCF::AbstractComponent* comp)
{
    /*
    This method is called after a component has been completely initialized.
    The last component to initialzie in GCF is always ComponentLoaderComponent.
    It is at this point we need to check for recently installed updates and
    show a message to the user.
    */
    if(comp != &GCF::ComponentLoaderComponent::instance())
        return;

    // If the application was restarted just after installation of updates, we
    // will need to show some message to the user
    checkIfRestartAfterUpdateInstallation();
}

/**
\internal
*/
void GCF::UpdaterComponent::initializeComponent()
{
    // We make use of a custom/internal GUIXML file for UpdaterComponent.
    if(d->useInbuiltGUIXML)
        setGuiXmlFileName(":/GCF/guixml/UpdaterComponent.xml");

    /*
    Step 1: Create the Updates directory in ${APPDIR}, if it does-not already exist
    */
    d->createUpdatesDirectory();

    /*
    Step 2: Iniitialize the icon cache.
    */
    GCF::IconCache::instance();

    // Rest of the initialization process moved to creationAnnounced(), because we will
    // need to make use of GCF::ComponentSettings.
}

/**
\internal
*/
void GCF::UpdaterComponent::creationAnnounced()
{
    // If no INewUpdatesHandler has been set, select the default implementation
    // which is this object itself.
    GCF_UPDATER_LOG("Checking for INewUpdatesHandler...");
    if(!d->newUpdatesHandler)
    {
        GCF_UPDATER_LOG("INewUpdatesHandler not set. Using the default handler from now on..");
        setNewUpdatesHandler( qobject_cast<INewUpdatesHandler*>(this) );
    }
    else
        GCF_UPDATER_LOG( QString("INewUpdatesHandler is: \"%1\"")
            .arg(d->newUpdatesHandler->containerObject()->metaObject()->className()) );

    /*
    Step 2: Read 3rd party server sites information from the Updater directory.
    And enable setting from the settings file
    */
    QStringList thirdPartySites = settings()["ThirdPartySites"].toStringList();
    Q_FOREACH(QString tps, thirdPartySites)
        addServerSite(tps);
    d->enabled = settings().hasKey("EnableFlag") ? settings()["EnableFlag"].toBool() : d->enabled;

    /*
    Step 2.5: Cache the action text provided for checkForUpdates and
    downloadUpdates action.
    */
    if(d->checkForUpdates)
    {
        d->checkForUpdatesText = d->checkForUpdates->text();
        d->checkForUpdatesText = d->checkForUpdatesText.remove("&");
    }

    if(d->downloadUpdates)
    {
        d->downloadUpdatesText = d->downloadUpdates->text();
        d->downloadUpdatesText = d->downloadUpdatesText.remove("&");
    }

    // If the updater is disabled, then we just set the initialized flag
    // and return
    if(!d->enabled)
    {
        GCF_UPDATER_LOG("Updater is disabled.");
        d->initialized = true;
        return;
    }

    /*
    Step 3: Check AvailableUpdates.xml file to see if there are updates that need to be
    installed. If yes, launch UpdateInstaller program and install it.
        - After launching the UpdateInstaller program, quit this application.
    */
    GCF_UPDATER_LOG("Checking whether there are updates that need to be installed...");
    bool updatesAvailableForInstallation = d->checkForAvailableUpdates();
    if( updatesAvailableForInstallation )
    {
        GCF_UPDATER_LOG("Yes, updates are available. Asking the user to confirm if updates can be installed now");
        QMessageBox::StandardButton answer =
            QMessageBox::question(0, "Updates Available",
                        QString("One or more updates for %1 is available. "
                                "Do you want to install?").arg(qApp->applicationName()),
                        QMessageBox::Yes|QMessageBox::No);
        if(answer == QMessageBox::Yes)
        {
            GCF_UPDATER_LOG("User replied yes, installing updates now!");
            bool success = d->installAvailableUpdates();
            if( !success )
            {
                QMessageBox::information(0, "Installation error",
                    "Could launch UpdateInstaller to install updates "
                    "Please contact your vendor and resolve this issue.");

                GCF_UPDATER_LOG("Error in installing updates.");
            }
            else
            {
                GCF_UPDATER_LOG("Installing updates, quitting the application now!");
                GCF::ComponentFactory::instance().finalize();
                qApp->quit();
                exit(0);
            }
        }
    }

    /*
    Step 3: If the download queue from the last time is not empty, continue downloading
    those.
    */
    if(!updatesAvailableForInstallation)
    {
        GCF_UPDATER_LOG("Checking to see if a download queue already exists...");
        if( d->checkDownloadQueue() )
        {
            GCF_UPDATER_LOG("Download queue already exists. Resuming download from last session..");
            downloadUpdateFiles();
        }
        /*
        Step 4: Create and start UpdateFinderThread, asking it to look for updates from
        primary server site first and then other sites.
        */
        else
        {
            GCF_UPDATER_LOG("No download queue from last time. Checking to see if I can look for updates now.");
            if(this->canCheckForUpdatesNow())
            {
                GCF_UPDATER_LOG("Checking for updates...");
                checkForUpdates();
            }
            else
                GCF_UPDATER_LOG("No updates to look for");
        }
    }

    d->initialized = true;
}

/**
\internal
*/
void GCF::UpdaterComponent::finalizeComponent()
{
    stopUpdater();

    // Write third party sites into the settings file
    settings()["ThirdPartySites"] = d->serverSiteList;
    settings()["EnableFlag"] = d->enabled;
}

/**
\internal
*/
bool GCF::UpdaterComponent::canCheckForUpdatesNow() const
{
    if(d->checkUpdateFrequency&ApplicationStartup)
        // We assume this this function is only called by initialize()
        // which is triggered by the application's main() function.
        return true;

    if(d->checkUpdateFrequency&ApplicationStartupButOnceADay)
    {
        // Read the current time from the timestamp file.
        QString timeStampFile = resolveFileName("${SYSTEM}/UpdateTimeStamp");
        QFile file(timeStampFile);
        if( file.open(QFile::ReadOnly) )
        {
            QString timeStamp = file.readAll();
            QDateTime dateTime = QDateTime::fromString(timeStamp);
            if( dateTime.date() == QDate::currentDate() )
            {
                // We have already checked for updates once today, lets not do it again.
                return false;
            }
        }

        // Either the time stamp file did not exist, or was unreadable. Either case
        // lets check for updates now.
        return true;
    }

    if(d->checkUpdateFrequency&UponUserRequest)
        return false;

    if(d->checkUpdateFrequency&OnceInAnHour)
        QTimer::singleShot(60*60*1000, const_cast<UpdaterComponent*>(this), SLOT(checkForUpdates()));
    else if(d->checkUpdateFrequency&OnceInTwoHours)
        QTimer::singleShot(2*60*60*1000, const_cast<UpdaterComponent*>(this), SLOT(checkForUpdates()));
    else if(d->checkUpdateFrequency&OnceInFiveHours)
        QTimer::singleShot(5*60*60*1000, const_cast<UpdaterComponent*>(this), SLOT(checkForUpdates()));

    // Some invalid value for checkUpdateFrequency set. So,
    // lets not check for updates now.
    return false;
}

/**
\internal
*/
void GCF::UpdaterComponent::downloadUpdateFiles()
{
    if(d->updateDownloaderThread.isNull())
    {
        GCF_UPDATER_LOG("Creating UpdateDownloaderThread...");
        d->updateDownloaderThread = new UpdateDownloaderThread(this);
        connect(d->updateDownloaderThread, SIGNAL(finishedDownloadingUpdates(int)),
                this, SLOT(slotDownloadFinished(int)));
        d->connectToThread(d->updateDownloaderThread);
    }

    if(!d->updateDownloaderThread->isRunning())
    {
        GCF_UPDATER_LOG("Starting UpdateDownloaderThread...");
        d->updateDownloaderThread->start();
    }
}

/**
\internal
*/
void GCF::UpdaterComponent::checkIfRestartAfterUpdateInstallation()
{
    QString riuFile = resolveFileName("${SYSTEM}/RecentlyInstalledUpdates.xml");
    GCF::UpdateInfoList justInstalledList;
    GCF::UpdateXMLParser().parse(riuFile, justInstalledList);
    int count = justInstalledList.count();
    if( count )
    {
        Qt::CursorShape cursor = qApp->overrideCursor()->shape();

        if(cursor == Qt::WaitCursor)
            qApp->restoreOverrideCursor();

        QMessageBox::information( componentHostWidget(),
                                  QString("%1 updated").arg(qApp->applicationName()),
                                  QString("%1 updates were recently installed. Open the updater configuration "
                                          "dialog box to find out more information about the recently installed "
                                          "updates.").arg(count) );
        if(cursor == Qt::WaitCursor)
            qApp->setOverrideCursor(cursor);
    }

    QFile::remove(riuFile);
}

/**
This function triggers the checking of updates. If the UpdaterComponent is already
checking for updates at the time of the function-call, then the function does nothing.
*/
void GCF::UpdaterComponent::checkForUpdates()
{
    // We need to find out if checkForUpdates() was triggered by the
    // user or not. If triggered by the user, then we need to
    // show a message box when the thread finishes looking for updates.
    d->triggeredByUser = (d->checkForUpdates && (sender() == d->checkForUpdates));

    // Check to see if we are already checking for updates
    if(d->updateFinderThread && d->updateFinderThread->isRunning())
    {
        if(d->triggeredByUser)
            d->checkForUpdates->setEnabled(false);

        GCF_UPDATER_LOG("UpdateFinderThread is already working");
        return;
    }

    // TODO: If we have items in the DownloadQueue.xml / AvailableUpdates.xml
    // we should not be checking for new updates.
    if(d->checkForAvailableUpdates())
    {
        QMessageBox::information(componentHostWidget(),
            "Updates available for installation",
            QString("%1 has downloaded updates and is ready to install them. "
                    "Please restart the application to install the updates.")
                    .arg(qApp->applicationName()));
        return;
    }

    if( d->checkDownloadQueue() ||
        (!d->updateDownloaderThread.isNull() && d->updateDownloaderThread->isRunning()))
    {
        QMessageBox::information(componentHostWidget(),
            "Updates are being downloaded",
            "Cannot check for updates because some updates are currently being "
            "downloaded. Please check after sometime.");
        return;
    }

    if(d->updateFinderThread.isNull())
    {
        GCF_UPDATER_LOG("Creating UpdateFinderThread...");
        d->updateFinderThread = new UpdateFinderThread(this);
        connect(d->updateFinderThread, SIGNAL(updatesFound(int)),
                this, SLOT(slotUpdatesAvailable(int)));
        d->connectToThread(d->updateFinderThread);
    }

    // TODO: Show some visual cue in the main window that updates are being downloaded...
    if(!d->updateFinderThread->isRunning())
    {
        // Write the current date/time into a file.
        QString timeStampFile = resolveFileName("${SYSTEM}/UpdateTimeStamp");
        QFile file(timeStampFile);
        if( file.open(QFile::WriteOnly) )
        {
            QString timeStamp = QDateTime::currentDateTime().toString();
            file.write( timeStamp.toAscii() );
            file.close();
        }

        // Start the update finder thread.
        GCF_UPDATER_LOG("Starting UpdateFinderThread...");
        d->updateFinderThread->start();

        if(d->triggeredByUser)
        {
            QString msg = QString("%1 is looking for updates in the background.\n"
                                  "You will be notified shortly about the status of\n"
                                  "this operation.\n\n"
                                  "You can continue with %1 until then.\n").arg(qApp->applicationName());
            d->showInformation(msg);
        }
    }

    // Lets schedule another check for update in 1, 2 or 5 hours
    if(d->checkUpdateFrequency&OnceInAnHour)
        QTimer::singleShot(60*60*1000, this, SLOT(checkForUpdates()));
    else if(d->checkUpdateFrequency&OnceInTwoHours)
        QTimer::singleShot(2*60*60*1000, this, SLOT(checkForUpdates()));
    else if(d->checkUpdateFrequency&OnceInFiveHours)
        QTimer::singleShot(5*60*60*1000, this, SLOT(checkForUpdates()));
}

/**
This function triggers the download of updates. If updates are available, then
UpdaterComponent automatically selects updates or asks the user to do the
selection depending on the value returned by \ref updateSelectionStrategy().

\note If another download is underway, then the function does nothing.
*/
void GCF::UpdaterComponent::downloadUpdates()
{
    // We need to find out if checkForUpdates() was triggered by the
    // user or not. If triggered by the user, then we need to
    // show a message box when the thread finishes looking for updates.
    d->triggeredByUser = (d->downloadUpdates && (sender() == d->downloadUpdates));

    if(!d->newUpdatesHandler)
        return;

    // Check to see if we are already checking for updates
    if(d->updateDownloaderThread && d->updateDownloaderThread->isRunning())
    {
        if(d->triggeredByUser)
            d->downloadUpdates->setEnabled(false);

        GCF_UPDATER_LOG("UpdateDownloaderThread already downloading updates...");
        return;
    }

    // Read information about downloaded updates from ${APPDIR}/Updates/System/Updates.xml
    GCF_UPDATER_LOG("Reading information about updates to download from Updates.xml");
    QString updateXmlFileName = resolveFileName("${UPDATES_XML}");
    UpdateXMLParser parser;
    UpdateInfoList uInfoList;
    bool success = parser.parse(updateXmlFileName, uInfoList, true);
    if( !success )
        return;

    if(!uInfoList.count())
    {
        if(d->triggeredByUser)
            QMessageBox::information(componentHostWidget(), "No Updates Available",
                "There are no updates available for download at this point.");
        return;
    }

    // Lets have INewUpdatesHandler handle what updates to install and what to not.
    // TODO: Document INewUpdatesHandler interface
    GCF_UPDATER_LOG(QString("Found %1 updates. Asking INewUpdatesHandler to figure out which ones to install..").arg(uInfoList.count()));

    GCF::UpdaterComponent::UpdateSelectionStrategy strategy = d->updateSelectionStrategy;
    if(d->triggeredByUser && d->newUpdatesHandler->containerObject() == this)
        d->updateSelectionStrategy = GCF::UpdaterComponent::SelectByAskingUser;
    UpdateInfoList toInstallList = d->newUpdatesHandler->selectUpdates(uInfoList);
    GCF_UPDATER_LOG(QString("INewUpdatesHandler selected %1 updates..").arg(toInstallList.count()));
    if(d->triggeredByUser && d->newUpdatesHandler->containerObject() == this)
        d->updateSelectionStrategy = strategy;

    // Now study the toInstallList and dump all the dependencies into the list
    GCF_UPDATER_LOG("Resolving dependencies in toInstallList");
    for(int i=0; i<toInstallList.count(); i++)
    {
        UpdateInfo uInfo = toInstallList.at(i);

        QList<UpdateInfo> deps = uInfo.dependencies();
        for(int j=0; j<deps.count(); j++)
        {
            UpdateInfo depUpdateInfo = deps.at(j);
            if( !toInstallList.contains(depUpdateInfo) )
                toInstallList.append(depUpdateInfo);
        }
    }
    GCF_UPDATER_LOG(QString("%1 updates selected for installation. Dumping this into DownloadQueue.xml").arg(toInstallList.count()));

    // If no update was selected for insallation, then just quit
    if(!toInstallList.count())
        return;

    // Now lets dump the toInstallList into DownloadQueue.xml and trigger
    // UpdateDownloaderThread
    QString dqFileName = resolveFileName("${DQ_XML}");
    QDomDocument doc = toInstallList.toDomDocument();
    QString docStr = doc.toString(2);
    QFile file( dqFileName );
    file.open(QFile::WriteOnly);
    file.write( docStr.toAscii() );
    file.close();

    // Remove from ${UPDATES_XML} the updates that have been selected for download.
    for(int i=0; i<toInstallList.count(); i++)
        uInfoList.removeAll( toInstallList.at(i) );

    // Now dump the un-selected update information into Updates.xml
    QString unSelDocStr;
    if(uInfoList.count())
    {
        QDomDocument unSelDoc = uInfoList.toDomDocument();
        unSelDocStr = unSelDoc.toString();
    }
    file.setFileName(updateXmlFileName);
    file.open(QFile::WriteOnly);
    file.write( unSelDocStr.toAscii() );
    file.close();

    // Trigger UpdateDownloaderThread
    GCF_UPDATER_LOG("Beginning to download updates in download queue..");
    downloadUpdateFiles();
}

/**
This function shows a configuration dialog box.

\sa \ref updatercomponent4
*/
void GCF::UpdaterComponent::showConfigurationDialog()
{
    QDialog dialog( componentHostWidget() );

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    connect(buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    IConfigurationDialogPageProvider* configPageProvider = qobject_cast<IConfigurationDialogPageProvider*>(this);
    QWidget* configPage = configPageProvider->createPage(&dialog);
    configPage->layout()->setMargin(0);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->addWidget(configPage);
    layout->addWidget(buttonBox);

    if( dialog.exec() == QDialog::Rejected )
        return;

    configPageProvider->savePage(configPage);
}

/**
\internal
*/
void GCF::UpdaterComponent::finderProgress(int val)
{
    GCF_UPDATER_LOG(QString("%1: Finding updates...").arg(val));
    emit progress(val, "Finding updates...");

    if(d->checkForUpdates)
        d->checkForUpdates->setText( QString("Checking for updates... [%2 %]").arg(val) );
}

/**
\internal
*/
void GCF::UpdaterComponent::downloaderProgress(int val)
{
    GCF_UPDATER_LOG(QString("%1: Downloading updates...").arg(val));
    emit progress(val, "Downloading updates...");

    if(d->downloadUpdates)
        d->downloadUpdates->setText( QString("Downloading updates... [%2 %]").arg(val) );
}

/**
\internal
*/
void GCF::UpdaterComponent::slotUpdatesAvailable(int count)
{
    emit updatesAvailable(count);

    if(!count)
    {
        if(d->triggeredByUser)
            QMessageBox::information(componentHostWidget(), "No Updates Available",
                        "No new updates are available at this point.");
        return;
    }

    // TODO document INewUpdatesHandler::autoDownloadUpdates() method.
    GCF_UPDATER_LOG("Checking to see if downloading of updates can begin..");
    if(d->newUpdatesHandler && d->newUpdatesHandler->isAutoDownloadUpdates())
    {
        GCF_UPDATER_LOG("Automatically downloading found updates...");
        downloadUpdates();
    }
    else
    {
        if(d->triggeredByUser)
        {
            GCF_UPDATER_LOG("checkForUpdates() was triggered by the user. Asking the user what do do next...");
            QMessageBox::StandardButton answer =
                QMessageBox::question(componentHostWidget(), "Updates Available",
                            QString("%1 updates available. Do you want to download them now?").arg(count),
                            QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
            if(answer == QMessageBox::Yes)
            {
                GCF_UPDATER_LOG("User is asking for updates to be downloaded. Downloading updates.");
                downloadUpdates();
            }
            else
                GCF_UPDATER_LOG("User does not want to download updates now..");
        }
        else
            GCF_UPDATER_LOG("Nope! Will have to wait until user manually opts to download updates");
    }
}

/**
\internal
*/
void GCF::UpdaterComponent::slotDownloadFinished(int count)
{
    if(d->triggeredByUser)
    {
        if(count)
            QMessageBox::information(componentHostWidget(), "Updates Downloaded",
                QString("%1 updates have been downloaded are are now ready for installation. "
                        "Restart the application to install the updates").arg(count));
    }
}

/**
\internal
*/
void GCF::UpdaterComponent::slotThreadStarted()
{
    QThread* thread = qobject_cast<QThread*>(sender());
    if(!thread)
        return;

    if(d->checkForUpdates)
    {
        d->checkForUpdates->setEnabled(false);
        if(d->updateFinderThread && thread == d->updateFinderThread)
            d->checkForUpdates->setText("Checking for updates...");
    }
    if(d->downloadUpdates)
    {
        d->downloadUpdates->setEnabled(false);
        if(d->updateDownloaderThread && thread == d->updateDownloaderThread)
            d->downloadUpdates->setText("Downloading updates...");
    }
}

/**
\internal
*/
void GCF::UpdaterComponent::slotThreadFinished()
{
    if(d->checkForUpdates)
    {
        d->checkForUpdates->setEnabled(true);
        d->checkForUpdates->setText( d->checkForUpdatesText );
    }
    if(d->downloadUpdates)
    {
        d->downloadUpdates->setEnabled(true);
        d->downloadUpdates->setText( d->downloadUpdatesText );
    }
}

/**
\internal
*/
void GCF::UpdaterComponent::stopUpdater()
{
    GCF::HourGlass hourGlass;

    componentHostWidget()->setEnabled(false);
    d->stopThread(d->updateFinderThread);
    d->stopThread(d->updateDownloaderThread);
    componentHostWidget()->setEnabled(true);
}

/**
\internal
*/
QObject* GCF::UpdaterComponent::containerObject()
{
    return this;
}

/**
\internal
*/
QString GCF::UpdaterComponent::pageTitle() const
{
    return "Updater";
}

/**
\internal
*/
QIcon GCF::UpdaterComponent::pageIcon() const
{
    return QIcon(":/GCF/checkForUpdates.png");
}

/**
\internal
*/
QWidget* GCF::UpdaterComponent::createPage(QWidget* parent)
{
    return new GCF::UpdaterConfigPage(parent);
}

/**
\internal
*/
void GCF::UpdaterComponent::savePage(QWidget* page)
{
    GCF::UpdaterConfigPage* configPage = (GCF::UpdaterConfigPage*)(page);
    if(!configPage)
        return;

    QStringList sites = configPage->thirdPartySites();
    d->serverSiteList = sites;
    setEnabled( configPage->enableUpdaterFlag() );
    GCF_UPDATER_LOG("Server site list changed by configuration dialog box");
}

/**
\internal
*/
GCF::Log* GCF::UpdaterComponent::log() const
{
    if(!d->log)
    {
        QString logFile = resolveFileName("${SYSTEM}/Log.html");
        GCF::AbstractLogBackend* backend = GCF::Logger::instance().fileBackend(logFile);
        GCF::AbstractLogLayout*  htmlLayout = GCF::Logger::instance().htmlLayout();
        d->log = GCF::Logger::instance().log("UpdaterComponent", backend, htmlLayout);

        d->log->addBackend( GCF::Logger::instance().consoleBackend(), GCF::Logger::instance().simpleLayout() );
    }

    return d->log;
}

/**
This function specifies an update strategy to use while selecting from a list
of available updates.

Valid values are

\li \c GCF::UpdaterComponent::SelectAllUpdates
\li \c GCF::UpdaterComponent::SelectCriticalUpdates
\li \c GCF::UpdaterComponent::SelectByAskingUser
\li \c GCF::UpdaterComponent::SelectByNotifyingUser

\sa \ref updatercomponent3
*/
void GCF::UpdaterComponent::setUpdateSelectionStrategy(GCF::UpdaterComponent::UpdateSelectionStrategy s)
{
    if(d->initialized)
        return;

    d->updateSelectionStrategy = s;
    if(d->updateSelectionStrategy == SelectByNotifyingUser)
    {
        if(!QSystemTrayIcon::isSystemTrayAvailable())
            return;

        QIcon icon = qApp->windowIcon();
        if(icon.isNull())
            icon = QIcon(":/GCF/checkForUpdates.png");

        d->systemTrayIcon = new QSystemTrayIcon(icon, this);
        d->systemTrayIcon->setToolTip( QString("%1 Updater").arg(qApp->applicationName()) );
        d->systemTrayIcon->setVisible(true);
    }
}

/**
Returns the current update selection strategy. Valid return values are

\li \c GCF::UpdaterComponent::SelectAllUpdates
\li \c GCF::UpdaterComponent::SelectCriticalUpdates
\li \c GCF::UpdaterComponent::SelectByAskingUser
\li \c GCF::UpdaterComponent::SelectByNotifyingUser

\sa \ref updatercomponent3
*/
GCF::UpdaterComponent::UpdateSelectionStrategy GCF::UpdaterComponent::updateSelectionStrategy() const
{
    return d->updateSelectionStrategy;
}

/**
\internal
*/
bool GCF::UpdaterComponent::isAutoDownloadUpdates() const
{
    return d->autoDownloadUpdates;
}

/**
\internal
*/
GCF::UpdateInfoList GCF::UpdaterComponent::selectUpdates(const GCF::UpdateInfoList& updates)
{
    // Make a list of updates from the primary server site
    GCF::UpdateInfoList primaryUpdates;
    if( d->updateSelectionStrategy == GCF::UpdaterComponent::SelectAllUpdates ||
        d->updateSelectionStrategy == GCF::UpdaterComponent::SelectByAskingUser )
    {
        for(int i=0; i<updates.count(); i++)
        {
            GCF::UpdateInfo uInfo = updates.at(i);
            GCF::UpdateFileInfo ufInfo = uInfo.updateFileForThisSystem();
            if( ufInfo.UpdateFileURL.toString().startsWith( d->primarySite ) )
                primaryUpdates.append(uInfo);
        }
    }

    switch(d->updateSelectionStrategy)
    {
    case GCF::UpdaterComponent::SelectAllUpdates:
        return primaryUpdates;

    case GCF::UpdaterComponent::SelectCriticalUpdates: {
        GCF::UpdateInfoList retList;
        for(int i=0; i<primaryUpdates.count(); i++)
        {
            GCF::UpdateInfo uInfo = primaryUpdates.at(i);
            if(uInfo.isForceInstall() || uInfo.type() == "Critical")
                retList.append(uInfo);
        }

        return retList;
        }

    case GCF::UpdaterComponent::SelectByAskingUser: {
        GCF::UpdateSelectionDialog dlg(updates, componentHostWidget());
        if( dlg.exec() == QDialog::Accepted )
            return dlg.selectedUpdates();
        }
        break;

    case GCF::UpdaterComponent::SelectByNotifyingUser:
        /*
        No update will be selected at this stage. The user will only have to be notified
        that updates are available. Perhaps via a menu item or something. After that,
        the user will have to click on "Install Updates" to perform the actual installation
        */
        if(!d->downloadUpdates)
        {
            // Since this action has not been created via GUIXML, we have to defailt
            // SelectByAskingUser behavior
            d->updateSelectionStrategy = SelectByAskingUser;
            return selectUpdates(updates);
        }
        else
        {
            // Find out the menu within which the downloadUpdates QAction
            // is shown in this application
            QList<QWidget*> widgets = d->downloadUpdates->associatedWidgets();
            IComponentHostWindow* hostWindow = GCF::AbstractComponent::componentHostWindow();
            QMenuBar* menuBar = hostWindow->windowMenuBar();
            QWidget* menuWidget = menuBar ? menuBar : hostWindow->windowMenuWidget();
            QMenu* menu = 0;
            Q_FOREACH(QWidget* w, widgets)
            {
                menu = qobject_cast<QMenu*>(w);
                if( !menuWidget->actions().contains( menu->menuAction() ) )
                {
                    menu = 0;
                    continue;
                }
            }

            if(!menu)
            {
                // This means that the downloadUpdates item has to be in
                // the main-menu.
                d->updateSelectionStrategy = SelectByAskingUser;
                return selectUpdates(updates);
            }

            QString menuTitle = menu->title();
            menuTitle = menuTitle.remove("&");

            QString toolTipText = QString("Updates found. Click on "
                                          "%1 -> %2 to select and download "
                                          "required updates")
                                          .arg(menuTitle)
                                          .arg(d->downloadUpdatesText);

            d->showInformation(toolTipText);
        }
        break;
    }

    return GCF::UpdateInfoList();
}

/*
This function creates the directory structure of the Updates directory.
*/
void GCF::UpdaterComponentData::createUpdatesDirectory()
{
    /*
    - Updates
        - Incoming
        - Cache
        - System
            - AvailableUpdates.xml
            - InstalledUpdates.xml
            - DownloadQueue.xml
            - Updates.xml
        - Temp
    */

    bool newlyCreated = false;
    QDir dir( qApp->applicationDirPath() );

    if( !dir.exists("Updates") )
    {
        dir.mkdir("Updates");
        newlyCreated = true;
    }

    dir.cd("Updates");

    if( !dir.exists("Incoming") )
    {
        dir.mkdir("Incoming");
        newlyCreated = true;
    }

    if( !dir.exists("Cache") )
    {
        dir.mkdir("Cache");
        newlyCreated = true;
    }

    if( !dir.exists("System") )
    {
        dir.mkdir("System");
        newlyCreated = true;
    }

    if( !dir.exists("Temp") )
    {
        dir.mkdir("Temp");
        newlyCreated = true;
    }

    dir.cd("System");
    createFile(dir.absoluteFilePath("AvailableUpdates.xml"));
    createFile(dir.absoluteFilePath("InstalledUpdates.xml"));
    createFile(dir.absoluteFilePath("DownloadQueue.xml"));
    createFile(dir.absoluteFilePath("Updates.xml"));

    if(newlyCreated)
        GCF_UPDATER_LOG("Updater directory structure created");
}

/*
Creates file \c fileName if it doesnt already exist.
*/
void GCF::UpdaterComponentData::createFile(const QString& fileName)
{
    if( !QFile::exists(fileName) )
    {
        QFile file(fileName);
        file.open(QFile::WriteOnly);
        file.write(" ", 1);
        file.close();
        GCF_UPDATER_LOG(QString("Created file %1").arg(fileName));
    }
}

/*
Checks to see if AvailableUpdates.xml file contains any available updates information.
*/
bool GCF::UpdaterComponentData::checkForAvailableUpdates()
{
    return !isUpdateXmlFileEmpty("AvailableUpdates.xml");
}

/*
Launch the UpdateInstaller process and returns true on success.
*/
bool GCF::UpdaterComponentData::installAvailableUpdates()
{
    QString updateInstaller = "UINST"; // QString("%1/UINST").arg(qApp->applicationDirPath());
#ifndef QT_NO_DEBUG
    updateInstaller.append("d");
#endif

    updateInstaller += QString(" --appName %1").arg( QFileInfo(qApp->applicationFilePath()).fileName() );
    updateInstaller += QString(" --appPath %1").arg( replacementTable["${APPDIR}"] );
    updateInstaller += QString(" --guixmlPath %1").arg( GCF::AbstractComponent::guiXmlFileDirectory() );
    updateInstaller += QString(" --updatesPath %1").arg( replacementTable["${UPDATES}"] );

    GCF_UPDATER_LOG( updateInstaller );
    return QProcess::startDetached(updateInstaller);
}

/*
Returns true of there are files in the download queue that are not yet emptied.
*/
bool GCF::UpdaterComponentData::checkDownloadQueue()
{
    return !isUpdateXmlFileEmpty("DownloadQueue.xml");
}

/*
Looks at an XML file contained in ${APPDIR}/Updates/System directory and returns true
if it does not contain any information about updates.

Returns false otherwise.
*/
bool GCF::UpdaterComponentData::isUpdateXmlFileEmpty(const QString& fileName)
{
    QDir dir( qApp->applicationDirPath() );
    dir.cd("Updates");
    dir.cd("System");

    QString auXmlFileName = dir.absoluteFilePath(fileName);
    if( !QFile::exists(auXmlFileName) )
        return true;

    QFile auXmlFile(auXmlFileName);
    if( !auXmlFile.open(QFile::ReadOnly) )
        return true;

    QDomDocument auXml;
    if( !auXml.setContent( &auXmlFile ) )
        return true;

    QDomElement rootE = auXml.documentElement();
    if( rootE.tagName() != "Updates" )
        return true;

    QDomElement updateE = rootE.firstChildElement("Update");
    if( updateE.isNull() )
        return true;

    return false;
}

void GCF::UpdaterComponentData::connectToThread(QThread* thread)
{
    if(!thread)
        return;

    QObject::connect(thread, SIGNAL(started()), updater, SLOT(slotThreadStarted()));
    QObject::connect(thread, SIGNAL(finished()), updater, SLOT(slotThreadFinished()));
    QObject::connect(thread, SIGNAL(terminated()), updater, SLOT(slotThreadFinished()));
}

QPointer<QWidget> GCF::UpdaterComponentData::showInformation(const QString& msg, int msecs, bool useSystemTrayIcon)
{
    if(useSystemTrayIcon && systemTrayIcon)
    {
        QString title = QString("%1 Updater").arg(qApp->applicationName());
        systemTrayIcon->showMessage(title, msg);
        return QPointer<QWidget>();
    }

    static QPointer<QWidget> informationWidget;

    if(!informationWidget.isNull())
        delete informationWidget;

    QLabel* label = new QLabel();
    label->setText(msg);
    label->setWindowFlags(Qt::SubWindow|
                          Qt::FramelessWindowHint|
                          Qt::CustomizeWindowHint|
                          Qt::WindowStaysOnTopHint);
    label->setFrameStyle(QFrame::WinPanel|QFrame::Raised);

    QFont font = label->font();
    font.setPointSize( font.pointSize()+4 );
    label->setFont(font);

    QPalette pal = label->palette();
    pal.setColor(QPalette::Base, pal.window().color());
    label->setPalette(pal);

    label->show();
    qApp->processEvents();

    if(msecs > 0)
        QTimer::singleShot(msecs, label, SLOT(deleteLater()));

    informationWidget = QPointer<QWidget>(label);
    return informationWidget;
}

void GCF::UpdaterComponentData::stopThread(QThread* thread)
{
    if(!thread || !thread->isRunning())
        return;

    QPointer<QWidget> widget = showInformation( QString("Stopping %1 Updater").arg(qApp->applicationName()), 0, false);
    QMetaObject::invokeMethod(thread, "stop", Qt::DirectConnection);
    thread->blockSignals(true);
    while(thread->isRunning())
        qApp->processEvents();
    thread->blockSignals(true);
    if(!widget.isNull())
        delete widget;
}
