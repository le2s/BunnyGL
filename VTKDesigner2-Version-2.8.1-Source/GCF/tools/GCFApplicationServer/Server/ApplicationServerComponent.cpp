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

#include "ApplicationServerComponent.h"
#include "ApplicationServerWindow.h"

#include <QDir>
#include <QMenu>
#include <QStyle>
#include <QWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QApplication>
#include <QSystemTrayIcon>

#include <GCF/Application>
#include <GCF/AppAccessPointComponent>
#include <GCF/ComponentLoaderComponent>
#include <GCF/ComponentFactory>
#include <GCF/AppAccessPoint>
#include <GCF/ComponentLoaderComponent>
#include <GCF/Common>
#include <QTreeWidget>
#include <QDialog>
#include <QVBoxLayout>

#ifdef Q_WS_WIN
#include <QSysInfo>
#endif

#include <QLocale>
#include <QDateTime>
#include <QTime>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QApplication>

#include "XMLGCFMessage.h"
#include "GradientGenerator.h"
#include "RequestBroker.h"
#include "RequestHandler.h"
#include "RequestHandlerPool.h"

struct ApplicationServerComponentData
{
    ApplicationServerComponentData() :
        systemTrayIcon(0), serverPort(4232),
        processPoolSize(0), requestBroker(0),
        requestHandler(0), log(0),
        brokerLog(0), handlerLog(0){ }

    QSystemTrayIcon* systemTrayIcon;
    QMenu* systemTrayMenu;
    GradientGenerator gradientGenerator;

    QTime startTime;
    QPixmap logo;

    QString serverName;
    int serverPort;
    int processPoolSize;

    RequestBroker* requestBroker;
    RequestHandler* requestHandler;

    GCF::Log* log;
    GCF::Log* brokerLog;
    GCF::Log* handlerLog;
};

GCF_DEFINE_COMPONENT(ApplicationServerComponent)

bool ApplicationServerComponent::BrokerMode = true;

ApplicationServerComponent& ApplicationServerComponent::instance()
{
    static ApplicationServerComponent* theInstance = GCF_CREATE_COMPONENT(ApplicationServerComponent);
    return *theInstance;
}

ApplicationServerComponent::ApplicationServerComponent()
{
    d = new ApplicationServerComponentData;
    GCF::AbstractComponent::setMainWindow(&ApplicationServerWindow::instance());

    if(BrokerMode)
    {
        d->systemTrayMenu = new QMenu;
        QAction* quitAction = d->systemTrayMenu->addAction("Quit");
        QObject::connect(quitAction, SIGNAL(triggered()), this, SLOT(shutdown()));

        QIcon icon = qApp->windowIcon();
        if(icon.isNull())
            icon = QIcon(":/icons/vcl.png");
        if(icon.isNull())
            icon = qApp->style()->standardIcon(QStyle::SP_ComputerIcon);

        d->systemTrayIcon = new QSystemTrayIcon(icon, this);
        d->systemTrayIcon->setContextMenu(d->systemTrayMenu);
        d->systemTrayIcon->show();

        qApp->setWindowIcon(d->systemTrayIcon->icon());

        QObject::connect(d->systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                         this, SLOT(slotSystemTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    }

    d->logo = QPixmap(":/icons/logo.png");
    if(d->logo.isNull())
    {
        QFont font = QFont("Times New Roman", 24);
        QFontMetrics fm(font);
        QString text = "GCF Application Server";
        QRect textRect = fm.boundingRect(text);

        d->logo = QPixmap(textRect.size());
        d->logo.fill(Qt::white);

        QPainter paint(&d->logo);
        paint.setFont(font);
        paint.drawText(d->logo.rect(), Qt::AlignCenter, text);
    }
}

ApplicationServerComponent::~ApplicationServerComponent()
{
    delete &ApplicationServerWindow::instance();
    delete d;
}

void ApplicationServerComponent::setApplicationServerLogo(const QPixmap& pm)
{
    d->logo = pm;
}

void ApplicationServerComponent::setServerName(const QString& serverName)
{
    if(d->requestHandler)
        return;

    d->serverName = serverName;
}

QString ApplicationServerComponent::serverName() const
{
    return d->serverName;
}

void ApplicationServerComponent::setServerPort(int port)
{
    if(d->requestBroker)
        return;

    d->serverPort = port;
}

int ApplicationServerComponent::serverPort() const
{
    return d->serverPort;
}

void ApplicationServerComponent::setProcessPoolSize(int size)
{
    if(size <= 0 || d->processPoolSize == size)
        return;

    d->processPoolSize = size;
}

int ApplicationServerComponent::processPoolSize() const
{
    return d->processPoolSize;
}

QString ApplicationServerComponent::gcfVersion() const
{
    return GCF::VersionInfo::versionInfoString();
}

QString ApplicationServerComponent::os() const
{
#ifdef Q_WS_WIN
    QString retValue("Windows ");
    switch( QSysInfo::windowsVersion() )
    {
    case QSysInfo::WV_XP:
        retValue += "XP";
        break;
    case QSysInfo::WV_VISTA:
        retValue += "Vista";
        break;
    case QSysInfo::WV_2003:
        retValue += "Server 2003";
        break;
    case QSysInfo::WV_WINDOWS7:
        retValue += "7";
        break;
    default:
        break;
    }

    retValue = retValue.simplified();
    return retValue;
#endif

#ifdef Q_WS_X11
    return "Linux";
#endif
}

QString ApplicationServerComponent::location() const
{
    return QLocale::countryToString(QLocale::system().country());
}

QString ApplicationServerComponent::currentDateTime() const
{
    return QDateTime::currentDateTime().toString();
}

QString ApplicationServerComponent::upTime() const
{
    int secs = d->startTime.secsTo(QTime::currentTime());

    int hours = secs/(60*60);
    int minutes = (secs-hours*(60*60))/60;
    int seconds = secs-hours*(60*60)-minutes*60;

    QString ret = QString("%1:%2:%3").arg(hours).arg(minutes).arg(seconds);
    return ret;
}

QPixmap ApplicationServerComponent::applicationServerLogo() const
{
    return d->logo;
}

QVariantMap ApplicationServerComponent::serverInfo() const
{
    QVariantMap ret;
    ret["gcfVersion"] = this->gcfVersion();
    ret["os"] = this->os();
    ret["location"] = this->location();
    ret["currentDateTime"] = this->currentDateTime();
    ret["upTime"] = this->upTime();
    ret["applicationServerLogo"] = qVariantFromValue<QPixmap>(this->applicationServerLogo());

    return ret;
}

QVariantList ApplicationServerComponent::serverInfoAsList() const
{
    QVariantList ret;
    ret << this->gcfVersion() << this->os()
        << this->location() << this->currentDateTime()
        << this->upTime()
        << qVariantFromValue<QPixmap>(this->applicationServerLogo());

    return ret;
}

bool ApplicationServerComponent::isRequestHandlerPoolFull() const
{
    if(d->requestBroker)
        return d->requestBroker->requestHandlerPool()->actualPoolSize() >=
                d->requestBroker->requestHandlerPool()->poolSize();
    return true;
}

bool ApplicationServerComponent::startHandlerForGUID(const QString& guid) const
{
    // The idea is to leave it to the RequestHandlerPool, the headache of creating
    // of new process if one doesn't exist for the GUID already.

    if(d->requestBroker)
    {
        RequestorSocket* rSocket =
                d->requestBroker->requestHandlerPool()->getRequestHandlerConnection(guid);

        if(!rSocket)
            return false;

        d->requestBroker->requestHandlerPool()->freeRequestHandlerConnection(rSocket);
        return true;
    }

    return false;
}

bool ApplicationServerComponent::isHandlerForGUIDReady(const QString& guid) const
{
    if(!d->requestBroker)
        return false;

    return d->requestBroker->requestHandlerPool()->isHandlerForGUIDReady(guid);
}

GCF::Log* ApplicationServerComponent::log() const
{
    if(!d->log)
    {
        static QString logFile = QDir(this->guiXmlFileDirectory()).absoluteFilePath("GCFApplicationServer-Log.html");
        GCF::AbstractLogBackend* backend = GCF::Logger::instance().fileBackend(logFile);
        GCF::AbstractLogLayout*  htmlLayout = GCF::Logger::instance().htmlLayout();
        d->log = GCF::Logger::instance().log("GCFApplicationServer", backend, htmlLayout);

        d->log->addBackend( GCF::Logger::instance().consoleBackend(), GCF::Logger::instance().simpleLayout() );
    }

    return d->log;
}

GCF::Log* ApplicationServerComponent::brokerLog() const
{
    if(!d->brokerLog)
    {
        static QString logFile = QDir(this->guiXmlFileDirectory()).absoluteFilePath("GCFApplicationServer-Broker-Log.html");
        GCF::AbstractLogBackend* backend = GCF::Logger::instance().fileBackend(logFile);
        GCF::AbstractLogLayout*  htmlLayout = GCF::Logger::instance().htmlLayout();
        d->brokerLog = GCF::Logger::instance().log("GCFApplicationServer", backend, htmlLayout);

        d->brokerLog->addBackend( GCF::Logger::instance().consoleBackend(), GCF::Logger::instance().simpleLayout() );
    }

    return d->brokerLog;
}

GCF::Log* ApplicationServerComponent::handlerLog() const
{
    if(!d->handlerLog)
    {
        QString logFile = QString("%1-Log.html").arg(d->serverName);
        logFile = QDir(this->guiXmlFileDirectory()).absoluteFilePath(logFile);
        GCF::AbstractLogBackend* backend = GCF::Logger::instance().fileBackend(logFile);
        GCF::AbstractLogLayout*  htmlLayout = GCF::Logger::instance().htmlLayout();
        d->handlerLog = GCF::Logger::instance().log("GCFApplicationServer", backend, htmlLayout);

        d->handlerLog->addBackend( GCF::Logger::instance().consoleBackend(), GCF::Logger::instance().simpleLayout() );
    }

    return d->handlerLog;
}

QString ApplicationServerComponent::tidyLogString(const QString& funcInfo, const QString& msg, int lineNumber) const
{
    QString fnName = funcInfo;
    int index = fnName.indexOf("__thiscall") + 10 /* "__thiscall".length() */;
    fnName.remove(0, index);
    fnName = fnName.replace("::", ".");
    fnName.truncate( fnName.indexOf('(') );
    fnName = fnName.simplified();

    return QString("[%1] %2: %3").arg(lineNumber).arg(fnName).arg(msg);
}

void ApplicationServerComponent::initializeComponent()
{
    this->setComponentName("ApplicationServer");
    this->setGuiXmlFileName(":/guixml/ApplicationServer.xml");

    GCF::AppAccessPoint::instance().registerMessageEncoderDecoder(&XMLGCFMessageHandler::instance());
    d->startTime = QTime::currentTime();
}

void ApplicationServerComponent::aboutToFinalize()
{
    if(d->requestBroker)
        delete d->requestBroker;
    d->requestBroker = 0;

    if(d->requestHandler)
        delete d->requestHandler;
    d->requestHandler = 0;
}

void ApplicationServerComponent::finalizeComponent()
{

}

void ApplicationServerComponent::creationAnnounced()
{
    uint serverPort = gApp->parameterValue("GCFApplicationServer_ServerPort", QString::number(d->serverPort)).toUInt();
    if(serverPort < 65535)
        d->serverPort = serverPort;

    // GCF::AppAccessPoint::instance().setPort( quint16(d->serverPort) );
    GCF::AppAccessPoint::instance().setAllowAllApplications(true);
    // GCF::AppAccessPoint::instance().setAppAccessEnabled(true);

    if(BrokerMode)
    {
        d->requestBroker = new RequestBroker(this);
        d->requestBroker->setServerPort( d->serverPort );
        d->requestBroker->requestHandlerPool()->setPoolSize(d->processPoolSize);
        d->requestBroker->start();
    }
    else
    {
        d->requestHandler = new RequestHandler(d->serverName, this);
        connect( gApp, SIGNAL(allComponentsLoaded()),
                 this, SLOT(allComponentsLoaded()), Qt::QueuedConnection);
    }
}

QWidget* ApplicationServerComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    if(comps.last() == "window")
        return &ApplicationServerWindow::instance();

    return 0;
}

QObject* ApplicationServerComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    if(comps.last() == "Core")
        return const_cast<ApplicationServerComponent*>(this);

    if(comps.last() == "GradientGenerator")
        return &(d->gradientGenerator);

    if(comps.last() == "XmlHandler")
        return &(XMLGCFMessageHandler::instance());

    return 0;
}

void ApplicationServerComponent::addChildWidget(QWidget *parent, QWidget *child, const QString &hint)
{
    Q_UNUSED(hint);
    if(parent == &ApplicationServerWindow::instance())
        ApplicationServerWindow::instance().tabWidget()->addTab(child, child->windowIcon(), child->windowTitle());
}

void ApplicationServerComponent::removeChildWidget(QWidget *parent, QWidget *child, const QString &hint)
{
    Q_UNUSED(hint);
    if(parent == &ApplicationServerWindow::instance())
    {
        int index = ApplicationServerWindow::instance().tabWidget()->indexOf(child);
        ApplicationServerWindow::instance().tabWidget()->removeTab(index);
    }
}

void ApplicationServerComponent::componentCreated(GCF::AbstractComponent* comp)
{
    QList<IXmlDataSerializer*> serializers = GCF::findObjects<IXmlDataSerializer*>(comp);
    Q_FOREACH(IXmlDataSerializer* serializer, serializers)
        XMLGCFMessageHandler::instance().registerSerializer(serializer);
}

void ApplicationServerComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    QList<IXmlDataSerializer*> serializers = GCF::findObjects<IXmlDataSerializer*>(comp);
    Q_FOREACH(IXmlDataSerializer* serializer, serializers)
        XMLGCFMessageHandler::instance().unregisterSerializer(serializer);
}

void ApplicationServerComponent::allComponentsLoaded()
{
    GCF_AS_HANDLER_LOG( QString("Sending handshake from %1 to GCFApplicationServer-RequestBroker").arg(d->serverName) );

    QLocalSocket* handshakeSocket = new QLocalSocket(this);
    connect(handshakeSocket, SIGNAL(disconnected()), handshakeSocket, SLOT(deleteLater()));
    handshakeSocket->connectToServer("GCFApplicationServer-RequestBroker");
    handshakeSocket->waitForConnected();
    handshakeSocket->write(d->serverName.toLatin1());
    handshakeSocket->flush();
}

void ApplicationServerComponent::shutdown()
{
    GCF::finalize();
    qApp->quit();
}

void ApplicationServerComponent::slotSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick)
    {
        ApplicationServerWindow::instance().setRequestHandlerPool(d->requestBroker->requestHandlerPool());
        ApplicationServerWindow::instance().show();
        ApplicationServerWindow::instance().raise();
    }
}


