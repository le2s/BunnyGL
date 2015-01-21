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

#ifndef UPDATER_COMPONENT_H
#define UPDATER_COMPONENT_H

#include <GCF/Common>
#include <GCF/AbstractComponent>
#include <GCF/IConfigurationDialogPageProvider>
#include <GCF/Logger>
#include <GCF/INewUpdatesHandler>

namespace GCF
{
class Log;

struct UpdaterComponentData;
class GCF_EXPORT_CLASS UpdaterComponent : public GCF::AbstractComponent,
        virtual public IConfigurationDialogPageProvider,
        virtual public INewUpdatesHandler
{
    Q_OBJECT
    Q_ENUMS(CheckUpdateFrequency)
    Q_INTERFACES(IConfigurationDialogPageProvider INewUpdatesHandler)
    Q_PROPERTY(CheckUpdateFrequency CheckUpdateFrequency READ checkUpdateFrequency WRITE setCheckUpdateFrequency)
    Q_PROPERTY(bool Enabled READ isEnabled WRITE setEnabled)
    GCF_DECLARE_COMPONENT(UpdaterComponent)

    public:
        static UpdaterComponent& instance();
    ~UpdaterComponent();

    void setEnabled(bool val);
    bool isEnabled() const;

    void setPrimaryServerSite(const QString& url);
    QString primaryServerSite() const;

    void setAllowThirdPartyServerSites(bool val);
    bool isAllowThirdPartyServerSites() const;

    Q_INVOKABLE void addServerSite(const QString& url);
    QStringList serverSiteList() const;

    void setUseInbuiltGUIXML(bool val);
    bool isUseInbuiltGUIXML() const;

    enum CheckUpdateFrequency
    {
        ApplicationStartup,
        ApplicationStartupButOnceADay,
        UponUserRequest,
        OnceInAnHour,
        OnceInTwoHours,
        OnceInFiveHours
    };
    void setCheckUpdateFrequency(CheckUpdateFrequency freq);
    CheckUpdateFrequency checkUpdateFrequency() const;

    void setNewUpdatesHandler(INewUpdatesHandler* h);
    INewUpdatesHandler* newUpdatesHandler() const;

    QString architecture() const;
    QString os() const;

    QString resolveFileName(const QString& fileName) const;

protected:
    UpdaterComponent();
    QObject* fetchObject(const QString& completeName) const;
    QWidget* fetchWidget(const QString& completeName) const;
    QAction* fetchAction(const QString& completeName) const;
    void componentCreated(GCF::AbstractComponent* comp);
    void initializeComponent();
    void creationAnnounced();
    void finalizeComponent();
    bool canCheckForUpdatesNow() const;
    void downloadUpdateFiles();
    void checkIfRestartAfterUpdateInstallation();

public slots:
    void checkForUpdates();
    void stopUpdater();

protected slots:
    void downloadUpdates();
    void showConfigurationDialog();
    void finderProgress(int val);
    void downloaderProgress(int val);
    void slotUpdatesAvailable(int count);
    void slotDownloadFinished(int count);
    void slotThreadStarted();
    void slotThreadFinished();

signals:
    void updatesAvailable(int count);
    void progress(int percent, const QString& msg);

public:
    // IContainer implementation
    QObject* containerObject();

    // IConfigurationDialogPageProvider implementation
    QString pageTitle() const;
    QIcon pageIcon() const;
    QWidget* createPage(QWidget* parent);
    void savePage(QWidget* page);

public:
    // Not for public access though
    Log* log() const;

public:
    enum UpdateSelectionStrategy
    {
        // These things apply to updates served only from the primary
        // server site. If you want more complicated updates handling;
        // then provide your own version of INewUpdatesHandler!!
        SelectAllUpdates,
        SelectCriticalUpdates,
        SelectByAskingUser,
        SelectByNotifyingUser
    };
    void setUpdateSelectionStrategy(UpdateSelectionStrategy s);
    UpdateSelectionStrategy updateSelectionStrategy() const;

    // INewUpdatesHandler implementation
    bool isAutoDownloadUpdates() const;
    GCF::UpdateInfoList selectUpdates(const GCF::UpdateInfoList& updates);

private:
    UpdaterComponentData* d;
};
}

#define GCF_UPDATER_MARK_MSG(Msg) QString("%1(%2)  - %3").arg(__FILE__).arg(__LINE__).arg(Msg)
#define GCF_UPDATER_DEBUG(Msg) GCF_DEBUG(GCF::UpdaterComponent::instance().log(), GCF_UPDATER_MARK_MSG(Msg))
#define GCF_UPDATER_ERROR(Msg) GCF_ERROR(GCF::UpdaterComponent::instance().log(), GCF_UPDATER_MARK_MSG(Msg))
#define GCF_UPDATER_WARNING(Msg) GCF_WARNING(GCF::UpdaterComponent::instance().log(), GCF_UPDATER_MARK_MSG(Msg))
#define GCF_UPDATER_FATAL(Msg) GCF_FATAL(GCF::UpdaterComponent::instance().log(), GCF_UPDATER_MARK_MSG(Msg))
#define GCF_UPDATER_LOG(Msg) GCF_LOG(GCF::UpdaterComponent::instance().log(), GCF_UPDATER_MARK_MSG(Msg))

#endif
