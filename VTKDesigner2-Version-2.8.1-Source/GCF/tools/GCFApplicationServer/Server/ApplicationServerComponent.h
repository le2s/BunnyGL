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

#ifndef APPLICATION_SERVER_COMPONENT_H
#define APPLICATION_SERVER_COMPONENT_H

#include <GCF/AbstractComponent>
#include <GCF/WSMacros>
#include <GCF/Logger>

#include <QPixmap>
#include <QSystemTrayIcon>

class ApplicationServerWindow;
class RequestingClient;

struct ApplicationServerComponentData;
class ApplicationServerComponent : public GCF::AbstractComponent
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(ApplicationServerComponent)
    Q_PROPERTY(int ServerPort READ serverPort WRITE setServerPort)
    Q_PROPERTY(int ProcessPoolSize READ processPoolSize WRITE setProcessPoolSize)

public:
    static bool BrokerMode;
    static ApplicationServerComponent& instance();
    ~ApplicationServerComponent();

    void setApplicationServerLogo(const QPixmap& pm);

    void setServerName(const QString& serverName);
    QString serverName() const;

    void setServerPort(int port);
    int serverPort() const;
    
    void setProcessPoolSize(int size);
    int processPoolSize() const;

    // Basic service methods
    Q_INVOKABLE GCF_ATOMIC_WS QString gcfVersion() const;
    Q_INVOKABLE GCF_ATOMIC_WS QString os() const;
    Q_INVOKABLE GCF_ATOMIC_WS QString location() const; // country only.
    Q_INVOKABLE GCF_ATOMIC_WS QString currentDateTime() const;
    Q_INVOKABLE GCF_ATOMIC_WS QString upTime() const;
    Q_INVOKABLE GCF_ATOMIC_WS QPixmap applicationServerLogo() const;
    Q_INVOKABLE GCF_ATOMIC_WS QVariantMap serverInfo() const;
    Q_INVOKABLE GCF_ATOMIC_WS QVariantList serverInfoAsList() const;
    Q_INVOKABLE GCF_ATOMIC_WS bool isRequestHandlerPoolFull() const;
    Q_INVOKABLE GCF_ATOMIC_WS bool startHandlerForGUID(const QString& guid) const;
    Q_INVOKABLE GCF_ATOMIC_WS bool isHandlerForGUIDReady(const QString& guid) const;

public:
    // Not for public access though
    GCF::Log* log() const;
    GCF::Log* brokerLog() const;
    GCF::Log* handlerLog() const;
    QString tidyLogString(const QString& funcInfo, const QString& msg, int lineNumber) const;

protected:
    ApplicationServerComponent();
    void initializeComponent();
    void aboutToFinalize();
    void finalizeComponent();
    void creationAnnounced();
    QWidget* fetchWidget(const QString& completeName) const;
    QObject* fetchObject(const QString& completeName) const;
    void addChildWidget(QWidget *parent, QWidget *child, const QString &hint);
    void removeChildWidget(QWidget *parent, QWidget *child, const QString &hint);

    void componentCreated(GCF::AbstractComponent* comp);
    void componentDestroyed(GCF::AbstractComponent* comp);

private slots:
    void allComponentsLoaded();
    void shutdown();
    void slotSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    friend class ApplicationServerWindow;
    friend class RequestingClient;
    ApplicationServerComponentData* d;
};

#define GCF_AS_MARK_MSG(Msg) ApplicationServerComponent::instance().tidyLogString(Q_FUNC_INFO, Msg, __LINE__)
#define GCF_AS_DEBUG(Msg) GCF_DEBUG(ApplicationServerComponent::instance().log(), GCF_AS_MARK_MSG(Msg))
#define GCF_AS_ERROR(Msg) GCF_ERROR(ApplicationServerComponent::instance().log(), GCF_AS_MARK_MSG(Msg))
#define GCF_AS_WARNING(Msg) GCF_WARNING(ApplicationServerComponent::instance().log(), GCF_AS_MARK_MSG(Msg))
#define GCF_AS_FATAL(Msg) GCF_FATAL(ApplicationServerComponent::instance().log(), GCF_AS_MARK_MSG(Msg))
#define GCF_AS_LOG(Msg) GCF_LOG(ApplicationServerComponent::instance().log(), GCF_AS_MARK_MSG(Msg))

#define GCF_AS_BROKER_MARK_MSG(Msg) ApplicationServerComponent::instance().tidyLogString(Q_FUNC_INFO, Msg, __LINE__)
#define GCF_AS_BROKER_DEBUG(Msg) GCF_DEBUG(ApplicationServerComponent::instance().brokerLog(), GCF_AS_BROKER_MARK_MSG(Msg))
#define GCF_AS_BROKER_ERROR(Msg) GCF_ERROR(ApplicationServerComponent::instance().brokerLog(), GCF_AS_BROKER_MARK_MSG(Msg))
#define GCF_AS_BROKER_WARNING(Msg) GCF_WARNING(ApplicationServerComponent::instance().brokerLog(), GCF_AS_BROKER_MARK_MSG(Msg))
#define GCF_AS_BROKER_FATAL(Msg) GCF_FATAL(ApplicationServerComponent::instance().brokerLog(), GCF_AS_BROKER_MARK_MSG(Msg))
#define GCF_AS_BROKER_LOG(Msg) GCF_LOG(ApplicationServerComponent::instance().brokerLog(), GCF_AS_BROKER_MARK_MSG(Msg))

#define GCF_AS_HANDLER_MARK_MSG(Msg) ApplicationServerComponent::instance().tidyLogString(Q_FUNC_INFO, Msg, __LINE__)
#define GCF_AS_HANDLER_DEBUG(Msg) GCF_DEBUG(ApplicationServerComponent::instance().handlerLog(), GCF_AS_HANDLER_MARK_MSG(Msg))
#define GCF_AS_HANDLER_ERROR(Msg) GCF_ERROR(ApplicationServerComponent::instance().handlerLog(), GCF_AS_HANDLER_MARK_MSG(Msg))
#define GCF_AS_HANDLER_WARNING(Msg) GCF_WARNING(ApplicationServerComponent::instance().handlerLog(), GCF_AS_HANDLER_MARK_MSG(Msg))
#define GCF_AS_HANDLER_FATAL(Msg) GCF_FATAL(ApplicationServerComponent::instance().handlerLog(), GCF_AS_HANDLER_MARK_MSG(Msg))
#define GCF_AS_HANDLER_LOG(Msg) GCF_LOG(ApplicationServerComponent::instance().handlerLog(), GCF_AS_HANDLER_MARK_MSG(Msg))

#endif

