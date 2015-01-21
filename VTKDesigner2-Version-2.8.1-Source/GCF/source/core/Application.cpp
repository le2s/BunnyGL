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

#include <GCF/Application>
#include <GCF/ComponentLoaderComponent>
#include <GCF/AppAccessPointComponent>
#include <GCF/AppAccessPoint>
#include <GCF/UpdaterComponent>
#include <GCF/Message>
#include <GCF/MethodInvocationHelper>
#include <GCF/ComponentFactory>
#include <GCF/DomAccess>
#include <GCF/IComponentHostWindow>
#include <GCF/IXmlDataSerializer>
#include <GCF/IComponentHostWindow>
#include <GCF/Logger>
#include "../qml/QMLComponent.h"

#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QTimer>
#include <QPointer>
#include <QMainWindow>
#include <QProcessEnvironment>
#include <QFileInfo>
#include <QDir>

/**
\class GCF::Application
\brief <code>#include <GCF/Application></code><br/> Application class for GCF

This class inherits \c QApplication from Qt. GCF applications must create an instance of this
class within their \c main() function. This class makes it easy to initialize and cleanup
GCF resources in your application. The class also provides easy means to configure
different modules (IPC, Updater etc..) of GCF. You can make use of the \c gApp macro to get
hold of the only GCF::Application object in the application.

Notice how GCF::Application is used in a typical \c main() function

\includelineno Simple/Application/Main.cpp

When the application object is executed, it looks for the ApplicationXML file and parses the contents
of the ApplicationXML file.

\sa \ref componentarch, \ref componentarch_applicationxml
*/

static GCF::Application* GCFApplicationInstance = 0;

namespace GCF
{
    class HostWidget : public IComponentHostWindow
    {
        QPointer<QWidget> m_widget;

    public:
        HostWidget(QWidget* widget) {
            m_widget = widget;
        }
        ~HostWidget() { }

        GCF_DECLARE_QUERYABLE;

        virtual QMenuBar* windowMenuBar() const {
            return 0;
        }
        virtual void setWindowMenuBar(QMenuBar* menuBar) {
            Q_UNUSED(menuBar);
            return;
        }
        virtual void addWindowToolBar(QToolBar* toolbar) {
            Q_UNUSED(toolbar)
        }
        virtual void setWindowMenuWidget(QWidget* widget) {
            Q_UNUSED(widget);
        }
        virtual QWidget* windowMenuWidget() const {
            return 0;
        }
        virtual QWidget* windowWidget() const {
            return m_widget;
        }
    };

    struct ApplicationData
    {
        ApplicationData() : configurationLoaded(false), applicationLog(0) { }
        bool configurationLoaded;
        QMap<QString,QString> parameters;

        void loadComponents(QDomElement componentsE);
        GCF::Log* applicationLog;
        GCF::Log* log();
    };

    GCF_BEGIN_QUERY_TABLE(HostWidget)
    GCF_END_QUERY_TABLE(HostWidget)
}

GCF::Application& GCF::Application::instance()
{
    return *GCFApplicationInstance;
}

/**
Constructor
*/
GCF::Application::Application(int& argc, char** argv)
                 :QApplication(argc, argv)
{
    d = new ApplicationData;
    GCFApplicationInstance = this;

    QString guixmlDir =
            QDir(this->applicationDirPath()).absoluteFilePath("guixml");
    GCF::AbstractComponent::setGuiXmlFileDirectory(guixmlDir);

    GCF::initialize();

    QTimer::singleShot(10, this, SLOT(loadConfiguration()));
}

/**
Destructor
*/
GCF::Application::~Application()
{
    GCF::finalize();
    delete d;
    GCFApplicationInstance = 0;
}

/**
Loads a component whose name is \c componentName from the \c libraryName file. The libraryName
parameter can contain the full path of the library file.
*/
void GCF::Application::loadComponent(const QString& componentName, const QString& libraryName)
{
    GCF::ComponentLoaderComponent::instance().loadComponent2(componentName, libraryName);
}

/**
This function loads QML component by name \c qmlCompName and processes the GUIXML file at \c qmlCompGUIXMLFile
for the component. The \c qmlCompGUIXMLFile is looked in the following directories

\li The resource file system
\li \ref GCF::AbstractComponent::guiXmlFileDirectory()
\li Application directory
\li Current working directory
\li Every directory in PATH environment variable
\li On Linux: Every directory in LD_LIBRARY_PATH environment variable
*/
bool GCF::Application::loadQMLComponent(const QString& qmlCompName, const QString& qmlCompGUIXMLFile)
{
    QString guixmlFile;
    if( qmlCompGUIXMLFile.startsWith(":") )
        guixmlFile = qmlCompGUIXMLFile;
    else if ( QDir(GCF::AbstractComponent::guiXmlFileDirectory()).exists(qmlCompGUIXMLFile) )
        guixmlFile = QDir(GCF::AbstractComponent::guiXmlFileDirectory()).absoluteFilePath(qmlCompGUIXMLFile);
    else
        guixmlFile = GCF::Application::findFileInPath(qmlCompGUIXMLFile);

    if(guixmlFile.isEmpty())
        return false;

    GCF::QMLComponent* qmlComp = new GCF::QMLComponent(qmlCompName, guixmlFile);
    qmlComp->initialize();
    return true;
}

/**
\internal
*/
void GCF::Application::addComponentPath(const QString& path)
{
    GCF::ComponentLoaderComponent::instance().addComponentDirectory(path);
}

/**
\internal
*/
QStringList GCF::Application::componentPaths() const
{
    return this->libraryPaths();
}

/**
This function can be used to enable or disable the IPC module of GCF. This function should be called
after setting IPC-Name (\ref setIPCName()) and IPC-port (\ref setIPCPort()) if needed.

\sa \ref ipcarch
*/
void GCF::Application::setIPCEnabled(bool val)
{
    if(val)
        GCF::AppAccessPointComponent::instance().initialize();
    GCF::AppAccessPoint::instance().setAppAccessEnabled(val);
}

/**
This function returns true if the IPC module of GCF is enabled. False otherwise.

\sa \ref ipcarch
*/
bool GCF::Application::isIPCEnabled() const
{
    return GCF::AppAccessPoint::instance().isAppAccessEnabled();
}

/**
Sets the IPC-name for this application. GCF::RemoteAppDiscovery will "discover" applications
that have the same name as the name specified through this function. If no name is specified,
then the IPC-name is assumed to be then name of the application itself.

\sa \ref ipcarch
*/
void GCF::Application::setIPCName(const QString& name)
{
    return GCF::AppAccessPoint::instance().setAccessPointName(name);
}

/**
Returns the IPC-name of this application.
*/
QString GCF::Application::ipcName() const
{
    return GCF::AppAccessPoint::instance().accessPointName();
}

/**
Sets the TCP/IP port number on which the IPC module's GCF::AppAccessPoint should host itself.

\sa \ref ipcarch
*/
void GCF::Application::setIPCPort(quint16 port)
{
    GCF::AppAccessPoint::instance().setPort(port);
}

/**
Returns the TCP/IP port number on which the IPC module's GCF::AppAccessPoint is hosted.

\sa \ref ipcarch
*/
quint16 GCF::Application::ipcPort() const
{
    return GCF::AppAccessPoint::instance().port();
}

/**
This function can be used to enable/disable the Updater module in GCF. This function should be
called after all other configurable parameters of the Updater module have been set.

\sa \ref updaterarch
*/
void GCF::Application::setAutoUpdatesEnabled(bool val)
{
    if(val)
        GCF::UpdaterComponent::instance().initialize();
    GCF::UpdaterComponent::instance().setEnabled(val);
}

/**
This function returns true if the Updater module in GCF is enabled, false otherwise.
*/
bool GCF::Application::isAutoUpdatesEnabled() const
{
    return GCF::UpdaterComponent::instance().isEnabled();
}

/**
This function sets the primary server site that the Updater module in GCF should look for to
find updates. The function should be called before calling \ref setAutoUpdatesEnabled().
Once the Updater module has been enabled, the primary-server-site cannot be changed!

\sa \ref updaterarch
*/
void GCF::Application::setPrimaryServerSite(const QString& url)
{
    GCF::UpdaterComponent::instance().setPrimaryServerSite(url);
}

/**
This function returns the primary server site set via \ref setPrimaryServerSite()

\sa \ref updaterarch
*/
QString GCF::Application::primaryServerSite() const
{
    return GCF::UpdaterComponent::instance().primaryServerSite();
}

/**
This function is used to enable or disable support for third party server-sites. Once the Updater
module has been enabled, this flag cannot be changed.

\sa \ref updaterarch
*/
void GCF::Application::setAllowThirdPartyServerSites(bool val)
{
    GCF::UpdaterComponent::instance().setAllowThirdPartyServerSites(val);
}

/**
Returns true of the support for third-party server sites is enabled.

\sa \ref updaterarch
*/
bool GCF::Application::isAllowThirdPartyServerSites() const
{
    return GCF::UpdaterComponent::instance().isAllowThirdPartyServerSites();
}

/**
This function can be used to add a third-party server site to the list of server
sites. The Updater module will look for updates in the primary server site first and
then into the server sites added via this function. Third-party server sites
cannot be added, if \ref isAllowThirdPartyServerSites() returns false.

\note No mechanism is provided to remove a server-site.

\sa \ref updaterarch
*/
void GCF::Application::addServerSite(const QString& url)
{
    GCF::UpdaterComponent::instance().addServerSite(url);
}

/**
\ref This function returns a list of server sites added via the \ref addServerSite() method.

\sa \ref updaterarch
*/
QStringList GCF::Application::serverSiteList() const
{
    return GCF::UpdaterComponent::instance().serverSiteList();
}

/**
Configures the frequency with which the Updater module will check for updates.

Valid values are
\li \c GCF::ApplicationStartup
\li \c GCF::ApplicationStartupButOnceADay
\li \c GCF::UponUserRequest
\li \c GCF::OnceInAnHour
\li \c GCF::OnceInTwoHours
\li \c GCF::OnceInFiveHours

By default the update-frequency is assumed to be \c GCF::ApplicationStartupButOnceADay

\sa \ref updaterarch
*/
void GCF::Application::setCheckUpdateFrequency(GCF::CheckUpdateFrequency freq)
{
    GCF::UpdaterComponent::CheckUpdateFrequency freq2 =
        GCF::UpdaterComponent::CheckUpdateFrequency( int(freq) );
    GCF::UpdaterComponent::instance().setCheckUpdateFrequency(freq2);
}

/**
Returns the update frequency of the Updater module.

\sa \ref updaterarch
*/
GCF::CheckUpdateFrequency GCF::Application::checkUpdateFrequency() const
{
    return GCF::CheckUpdateFrequency( (int)
                GCF::UpdaterComponent::instance().checkUpdateFrequency()
            );
}

/**
\internal
*/
void GCF::Application::setNewUpdatesHandler(INewUpdatesHandler* h)
{
    GCF::UpdaterComponent::instance().setNewUpdatesHandler(h);
}

/**
\internal
*/
INewUpdatesHandler* GCF::Application::newUpdatesHandler() const
{
    return GCF::UpdaterComponent::instance().newUpdatesHandler();
}

/**
Returns a string that describes the hardware architecture on which this application is running.
*/
QString GCF::Application::architecture() const
{
    return GCF::UpdaterComponent::instance().architecture();
}

/**
Returns a string that describes the OS on which this application is running.
*/
QString GCF::Application::os() const
{
    return GCF::UpdaterComponent::instance().os();
}

/**
This function specifies an update strategy to use while selecting from a list
of available updates.

Valid values are

\li \c GCF::SelectAllUpdates
\li \c GCF::SelectCriticalUpdates
\li \c GCF::SelectByAskingUser
\li \c GCF::SelectByNotifyingUser

By default the update-selection-strategy is set to \c GCF::SelectByAskingUser.

\sa \ref updaterarch
*/
void GCF::Application::setUpdateSelectionStrategy(GCF::UpdateSelectionStrategy s)
{
    GCF::UpdaterComponent::UpdateSelectionStrategy s2 =
        GCF::UpdaterComponent::UpdateSelectionStrategy( int(s) );
    GCF::UpdaterComponent::instance().setUpdateSelectionStrategy(s2);
}

/**
Returns the update selection strategy used by the Updater module in GCF.

\sa \ref updaterarch
*/
GCF::UpdateSelectionStrategy GCF::Application::updateSelectionStrategy() const
{
    return GCF::UpdateSelectionStrategy( (int)
                GCF::UpdaterComponent::instance().updateSelectionStrategy()
            );
}

/**
Sets the GUIXML directory from which GCF loads all the GUIXML files. By default it is a
directory called \c guixml whithin the same directory as the application executable.
*/
void GCF::Application::setGuiXmlFileDirectory(const QString& absDirPath)
{
    if( QFileInfo(absDirPath).isAbsolute() )
        GCF::AbstractComponent::setGuiXmlFileDirectory(absDirPath);
    else
    {
        QString guixmlDir = QDir(this->applicationDirPath()).absoluteFilePath(absDirPath);
        GCF::AbstractComponent::setGuiXmlFileDirectory(guixmlDir);
    }
}

/**
Returns the directory from which GUIXML files are loaded.
*/
QString GCF::Application::guiXmlFileDirectory()
{
    return GCF::AbstractComponent::guiXmlFileDirectory();
}

/**
Sets the main-window container of the GCF application. GCF applications must set an application
container using this method or by using the \ref setComponentHostWidget() method.
*/
void GCF::Application::setMainWindow(QMainWindow* mainWindow)
{
    GCF::AbstractComponent::setMainWindow(mainWindow);
}

/**
This function returns the main-window of the GCF application. The function returns NULL if the
application container used is not a \c QMainWindow.
*/
QMainWindow* GCF::Application::mainWindow()
{
    return GCF::AbstractComponent::mainWindow();
}

/**
\internal
*/
void GCF::Application::setComponentHostWindow(IComponentHostWindow* hostWindow)
{
    GCF::AbstractComponent::setComponentHostWindow(hostWindow);
}

/**
\internal
*/
IComponentHostWindow* GCF::Application::componentHostWindow()
{
    return GCF::AbstractComponent::componentHostWindow();
}

/**
This function can be used to set any QWidget as the application-container-widget. If the
widget passed as parameter is a \c QMainWindow, then this function calls \ref setMainWindow().
*/
void GCF::Application::setComponentHostWidget(QWidget* widget)
{
    if(qobject_cast<QMainWindow*>(widget))
    {
        this->setMainWindow((QMainWindow*)widget);
        return;
    }

    IComponentHostWindow* hostWindow = dynamic_cast<IComponentHostWindow*>(widget);
    if(hostWindow)
    {
        this->setComponentHostWindow(hostWindow);
        return;
    }

    static GCF::HostWidget hostWidget(widget);
    this->setComponentHostWindow(&hostWidget);
}

/**
Returns a pointer to the application-container-widget.
*/
QWidget* GCF::Application::componentHostWidget()
{
    return GCF::AbstractComponent::componentHostWidget();
}

/**
This function searches for a component by name \c componentName and returns a pointer to it.
*/
GCF::AbstractComponent* GCF::Application::findComponent(const QString& componentName) const
{
    return GCF::ComponentFactory::instance().findComponent(componentName);
}

/**
\internal
*/
void GCF::Application::registerSerializer(IBinaryDataSerializer* serializer)
{
    GCF::Message::registerSerializer(serializer);
}

/**
\internal
*/
void GCF::Application::unregisterSerializer(IBinaryDataSerializer* serializer)
{
    GCF::Message::unregisterSerializer(serializer);
}

/**
\internal
*/
IBinaryDataSerializer* GCF::Application::findSerializer(const QByteArray& typeName)
{
    return GCF::Message::findSerializer(typeName);
}

/**
\internal
*/
void GCF::Application::registerHelper(IMethodInvocationHelper* helper)
{
    GCF::registerHelper(helper);
}

/**
\internal
*/
void GCF::Application::unregisterHelper(IMethodInvocationHelper* helper)
{
    GCF::unregisterHelper(helper);
}

/**
\internal
*/
IMethodInvocationHelper* GCF::Application::findHelper(const QByteArray& typeName)
{
    return GCF::findHelper(typeName);
}

/**
\internal
*/
void GCF::Application::registerConverter(IDataToGenericArgumentConverter* conv)
{
    GCF::registerConverter(conv);
}

/**
\internal
*/
void GCF::Application::unregisterConverter(IDataToGenericArgumentConverter* conv)
{
    GCF::unregisterConverter(conv);
}

/**
\internal
*/
IDataToGenericArgumentConverter* GCF::Application::findConverter(const QByteArray& typeName)
{
    return GCF::findConverter(typeName);
}

/**
\internal
*/
void GCF::Application::registerDataTypeSupport(QObject* object)
{
    if(!object)
        return;

    IBinaryDataSerializer* serializer = qobject_cast<IBinaryDataSerializer*>(object);
    if(serializer)
        this->registerSerializer(serializer);

    IMethodInvocationHelper* helper = qobject_cast<IMethodInvocationHelper*>(object);
    if(helper)
        this->registerHelper(helper);

    IDataToGenericArgumentConverter* converter = qobject_cast<IDataToGenericArgumentConverter*>(object);
    if(converter)
        this->registerConverter(converter);

    IXmlDataSerializer* xmlSerializer = qobject_cast<IXmlDataSerializer*>(object);
    if(xmlSerializer)
    {
        IXmlDataSerializerRegistry* xmlSerializerRegistry =
                GCF::findObject<IXmlDataSerializerRegistry*>();
        if(xmlSerializerRegistry)
            xmlSerializerRegistry->registerSerializer(xmlSerializer);
    }
}

/**
\internal
*/
void GCF::Application::unregisterDataTypeSupport(QObject* object)
{
    if(!object)
        return;

    IBinaryDataSerializer* serializer = qobject_cast<IBinaryDataSerializer*>(object);
    if(serializer)
        this->unregisterSerializer(serializer);

    IMethodInvocationHelper* helper = qobject_cast<IMethodInvocationHelper*>(object);
    if(helper)
        this->unregisterHelper(helper);

    IDataToGenericArgumentConverter* converter = qobject_cast<IDataToGenericArgumentConverter*>(object);
    if(converter)
        this->unregisterConverter(converter);

    IXmlDataSerializer* xmlSerializer = qobject_cast<IXmlDataSerializer*>(object);
    if(xmlSerializer)
    {
        IXmlDataSerializerRegistry* xmlSerializerRegistry =
                GCF::findObject<IXmlDataSerializerRegistry*>();
        if(xmlSerializerRegistry)
            xmlSerializerRegistry->unregisterSerializer(xmlSerializer);
    }
}

/**
\internal
*/
bool GCF::Application::callMethod(QObject* object, const QString& methodName,
                                  const QVariantList& args, QVariant& returnValue,
                                  bool publicOnly)
{
    return GCF::callMethod(object, methodName, args, returnValue, publicOnly);
}

/**
\internal
*/
int  GCF::Application::findMethod(QObject* object, const QString& methodName,
                                  const QVariantList& args)
{
    return GCF::findMethod(object, methodName, args);
}

/**
\internal
*/
bool GCF::Application::callMethod(QObject* object, int methodIndex,
                                  const QVariantList& args, QVariant& returnValue,
                                  bool publicOnly)
{
    return GCF::callMethod(object, methodIndex, args, returnValue, publicOnly);
}

/**
This method can be used to fetch parameters from the ApplicationXML file. The ApplicationXML file can
contain a \c <Parameters> XML element within which one or more \c <Parameter> XML elements can be found.
Each \c <Parameter> XML element can have an attribute called \c Name, which would contain the name of
the parameter and the text of the XML element would be its value. For example,

\verbatim
<Application>

    <Components>
        <!-- ... -->
    </Components>

    <!-- ... -->

    <Parameters>
        <Parameter Name="DICOMService_BaseDirectory">C:/Work/Documentation/DICOMSamples</Parameter>
        <Parameter Name="DICOMService_Series">PHENIX</Parameter>
    </Parameters>

    <!-- ... -->

</Application>
\endverbatim

Parameters in an ApplicationXML are loaded before any component is loaded. This means that components
can easily look for parameters from during their initialization and/or finalization routines.
*/
QStringList GCF::Application::parameterNames() const
{
    return d->parameters.keys();
}

/**
This function returns true if the parameter by name \c name exists in the list of parameters loaded
via the ApplicationXML file.

\sa parameterNames()
*/
bool GCF::Application::hasParameter(const QString& name) const
{
    return d->parameters.contains(name);
}

/**
This function returns value of the parameter whose name is \c name. If no such parameter was found, then
the \c defaultValue is returned.
*/
QString GCF::Application::parameterValue(const QString& name, const QString& defaultValue) const
{
    return d->parameters.value(name, defaultValue);
}

/**
This function can be used to set a application-wide GCF::Log object. Once a log object is set, it cannot be
changed ever. If no log object is specified; then a default log object is created when the first
normal, debug, warning, error or fatal log message is logged.
*/
void GCF::Application::setLogObject(GCF::Log* log)
{
    if(d->applicationLog)
        return;

    d->applicationLog = log;
}

/**
Returns pointer to the application's log object.
*/
GCF::Log* GCF::Application::logObject() const
{
    return d->applicationLog;
}

/**
Logs a normal message to the application log.
*/
void GCF::Application::log(const QString& msg)
{
    d->log()->log(msg);
}

/**
Logs a debug message to the application log.
*/
void GCF::Application::debugLog(const QString& msg)
{
    d->log()->debugLog(msg);
}

/**
Logs a error message to the application log.
*/
void GCF::Application::errorLog(const QString& msg)
{
    d->log()->errorLog(msg);
}

/**
Logs a warning message to the application log.
*/
void GCF::Application::warningLog(const QString& msg)
{
    d->log()->warningLog(msg);
}

/**
Logs a fatal message to the application log.
*/
void GCF::Application::fatalLog(const QString& msg)
{
    d->log()->fatalLog(msg);
}

/**
\internal
*/
void GCF::Application::loadConfiguration()
{
    if(d->configurationLoaded)
        return;

    d->configurationLoaded = true;
    if(this->applicationName().isEmpty())
    {
        QString appName = QFileInfo(this->arguments().first()).baseName();
#ifndef QT_NO_DEBUG
        if(appName.endsWith("d"))
            appName.remove(appName.length()-1, 1);
#endif
        this->setApplicationName(appName);
    }

    QString xmlFile
            = QDir(GCF::AbstractComponent::guiXmlFileDirectory())
              .absoluteFilePath(this->applicationName() + ".xml");
    if(!QFile::exists(xmlFile))
        xmlFile = QDir(GCF::AbstractComponent::guiXmlFileDirectory()).absoluteFilePath("Application.xml");

    if(!QFile::exists(xmlFile))
    {
        qDebug("Cannot find application-configuration file: %s", qPrintable(xmlFile));
        return;
    }

    qDebug("Loading application-configuration from %s", qPrintable(xmlFile));

    QFile file(xmlFile);
    if( !file.open(QFile::ReadOnly) )
        return;

    QDomDocument doc;
    doc.setContent(&file);

    QDomElement rootE = doc.documentElement();
    if(rootE.tagName() != "Application")
        return;

    QString appName = rootE.attribute("Name");
    if(!appName.isEmpty())
        this->setApplicationName(appName);

    // Load parameters
    QDomElement parametersEle = rootE.firstChildElement("Parameters");
    if(!parametersEle.isNull())
    {
        QDomElement parameterEle = parametersEle.firstChildElement("Parameter");
        while(!parameterEle.isNull())
        {
            QString name = parameterEle.attribute("Name");
            if(name.isEmpty())
                name = parameterEle.firstChildElement("Name").text();

            QString value = parameterEle.attribute("Value");
            if(value.isEmpty())
                value = parameterEle.text();
            if(value.isEmpty())
                value = parameterEle.firstChildElement("Value").text();

            if(!name.isEmpty())
                d->parameters[name] = value;

            parameterEle = parameterEle.nextSiblingElement("Parameter");
        }
    }

    // Load all the components
    d->loadComponents(GCF::DomAccess(doc).query("Application.Components").element());

    // Get component-loader-component to load itself!
    GCF::ComponentLoaderComponent::instance().initialize();
    emit allComponentsLoaded();

    // Configure IPC (Application name will be used as the name for the access point)
    QDomElement ipcEle = rootE.firstChildElement("IPC");
    if(!ipcEle.isNull())
    {
        quint16 port = (quint16)(ipcEle.attribute("Port").toUInt());
        QString ipcName = ipcEle.attribute("Name");

        if(ipcName.isEmpty())
            ipcName = appName;

        this->setIPCEnabled(true);
        this->setIPCPort(port);
        this->setIPCName(ipcName);
    }

    // Configure automatic updates
    QDomElement updaterE = rootE.firstChildElement("Updater");

    QDomElement serverSitesE = updaterE.firstChildElement("ServerSites");
    if(!serverSitesE.isNull())
    {
        if(this->isAllowThirdPartyServerSites())
        {
            QDomElement siteE = serverSitesE.firstChildElement("Site");
            while(!siteE.isNull())
            {
                this->addServerSite(siteE.text());
                siteE = siteE.nextSiblingElement("Site");
            }
        }
    }
}

void GCF::ApplicationData::loadComponents(QDomElement componentsE)
{
    QApplication::setOverrideCursor( Qt::WaitCursor );

    QDomElement componentE = componentsE.firstChildElement();
    while(!componentE.isNull())
    {
        if(componentE.tagName() == "Component")
        {
            QString libName = componentE.attribute("Library");
            QString name = componentE.attribute("Name");
            if(!libName.isEmpty() && !name.isEmpty())
                gApp->loadComponent(name, libName);
        }
        else if(componentE.tagName() == "QMLComponent")
        {
            QString name = componentE.attribute("Name");
            QString guixmlFileName = componentE.attribute("GUIXML");
            if(guixmlFileName.isEmpty())
                guixmlFileName = QString("%1.xml").arg(name);
            if(!name.isEmpty())
            {
                bool val = gApp->loadQMLComponent(name, guixmlFileName);
                if(!val)
                    qDebug() << "Could not load QML component " << name;
                else
                    qDebug() << "Loaded QML component " << name;
            }
        }

        componentE = componentE.nextSiblingElement();
    }

    QApplication::restoreOverrideCursor();
}

GCF::Log* GCF::ApplicationData::log()
{
    if(applicationLog)
        return applicationLog;

    QString logFile = QString("%1/%2.txt").arg(qApp->applicationDirPath()).arg(qApp->applicationName());
    GCF::AbstractLogBackend* fileBackend = GCF::Logger::instance().fileBackend(logFile);
    GCF::AbstractLogLayout* layout = GCF::Logger::instance().dateLayout();
    applicationLog = GCF::Logger::instance().log("Application Context", fileBackend, layout);
    return applicationLog;
}

namespace GCF
{

QStringList DirectoriesInPath()
{
    static QStringList dirsInPath;
    if(dirsInPath.isEmpty())
    {
        QString path = QProcessEnvironment::systemEnvironment().value("PATH");
#ifdef Q_OS_WIN
        dirsInPath = path.split(";", QString::SkipEmptyParts);
#else
        path = QString("%1:%2").arg(path).arg(QProcessEnvironment::systemEnvironment().value("LD_LIBRARY_PATH"));
        dirsInPath = path.split(":", QString::SkipEmptyParts);
#endif
        if(QDir::currentPath() != qApp->applicationDirPath())
            dirsInPath.prepend(QDir::currentPath());
        dirsInPath.prepend(qApp->applicationDirPath());
        dirsInPath.removeDuplicates();
    }

    return dirsInPath;
}

}

/**
This function looks for \c fileName in the application directory followed by each and every directory
in PATH environment variable. It returns a complete path of the file where it was first found. If the
file was not found, then a NULL string is returned.

- On Windows the search order is
    - application directory (\c QCoreApplication::applicationDirPath())
    - current working directory (\c QDir::currentDirPath())
    - each and every directory in the PATH environment variable

- On Linux the search order is
    - application directory (\c QCoreApplication::applicationDirPath())
    - current working directory (\c QDir::currentDirPath())
    - each and every directory in the PATH environment variable
    - each and every directory in the LD_LIBRARY_PATH environment variable

\note This function is faster than calling findFilesInPath(...).first().

\note This function was previously a part of the \c IFileSystemUtils interface. It has since been
moved into the this class. The \c IFilsSystemUtils interface now redirects calls to this function.
  */
QString GCF::Application::findFileInPath(const QString& fileName)
{
    if(fileName.isEmpty())
        return QString();

    Q_FOREACH(QString dir, GCF::DirectoriesInPath())
    {
        QString resolvedFileName = QString("%1/%2").arg(dir).arg(fileName);
        resolvedFileName.replace("\\", "/");
        QFileInfo fi( resolvedFileName );
        if(fi.exists())
            return fi.absoluteFilePath();
    }

    return QString();
}

/**
This function returns a list of complete-paths where the given \c fileName is found on the system. If the file was
not found anywhere; then an empty list is returned.

- On Windows the search order is
    - application directory (\c QCoreApplication::applicationDirPath())
    - current working directory (\c QDir::currentDirPath())
    - each and every directory in the PATH environment variable

- On Linux the search order is
    - application directory (\c QCoreApplication::applicationDirPath())
    - current working directory (\c QDir::currentDirPath())
    - each and every directory in the PATH environment variable
    - each and every directory in the LD_LIBRARY_PATH environment variable

\note This function was previously a part of the \c IFileSystemUtils interface. It has since been
moved into the this class. The \c IFilsSystemUtils interface now redirects calls to this function.
  */
QStringList GCF::Application::findFilesInPath(const QString& fileName)
{
    if(fileName.isEmpty())
        return QStringList();

    QStringList retList;
    Q_FOREACH(QString dir, GCF::DirectoriesInPath())
    {
        QString resolvedFileName = QString("%1/%2").arg(dir).arg(fileName);
        resolvedFileName.replace("\\", "/");
        QFileInfo fi( resolvedFileName );
        if(fi.exists())
            retList << fi.absoluteFilePath();
    }

    return retList;
}

/**
This function recursively deletes the contents of directory whose name/path is specified by \c dir. If
\c deleteDir is true, then the directory is also removed at the end.

\note This function was previously a part of the \c IFileSystemUtils interface. It has since been
moved into the this class. The \c IFilsSystemUtils interface now redirects calls to this function.
  */
void GCF::Application::deleteDirectory(const QString& dirPath, bool deleteDir)
{
    QDir dir(dirPath);
    QFileInfoList list = dir.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);
    for(int i=0; i<list.count(); i++)
    {
        QFileInfo fi = list[i];
        if( fi.isFile() )
            QFile::remove( fi.absoluteFilePath() );
        else
        {
            QDir d2 = dir;
            d2.cd( fi.fileName() );
            deleteDirectory( d2.absolutePath(), true );
        }
    }

    if(deleteDir)
    {
        QDir d2(dir);
        d2.cdUp();
        d2.rmdir( dir.dirName() );
    }
}



