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

#ifndef MAIN_WINDOW_COMPONENT_H
#define MAIN_WINDOW_COMPONENT_H

#include <GCF/AbstractComponent>
#include <GCF/IAboutData>
#include <GCF/Application>
#include "MainWindow.h"

namespace GCF
{
namespace Components
{

class MainWindow;
struct MainWindowComponentData;
class GCF_EXPORT_CLASS2 MainWindowComponent : public AbstractComponent, virtual public IAboutData
{
    Q_OBJECT
    Q_INTERFACES(IAboutData)
    Q_PROPERTY(QString AboutBackground READ aboutBackground WRITE setAboutBackground)
    Q_PROPERTY(QString ApplicationLogo READ applicationLogo WRITE setApplicationLogo)
    Q_PROPERTY(QString ApplicationIcon READ applicationIcon WRITE setApplicationIcon)
    Q_PROPERTY(QString SplashImage READ splashImage WRITE setSplashImage)
    GCF_DECLARE_STD_COMPONENT(MainWindowComponent)

    public:
        static MainWindowComponent & instance();
    ~MainWindowComponent();

    MainWindow* mainWindow() const;

    void setAboutBackground(const QString& pmFile);
    QString aboutBackground() const;

    void setApplicationLogo(const QString& pmFile);
    QString applicationLogo() const;

    void setApplicationIcon(const QString& appIcon);
    QString applicationIcon() const;

    void setSplashImage(const QString& pmFile);
    QString splashImage() const;

protected:
    MainWindowComponent();
    QAction* fetchAction(const QString& completeName) const;
    QObject* fetchObject(const QString& completeName) const;
    QWidget* fetchWidget(const QString& completeName) const;
    void addChildWidget(QWidget* parent, QWidget* child, const QString& hint=QString());
    void removeChildWidget(QWidget* parent, QWidget* child, const QString& hint=QString());
    void showChildWidget(QWidget* parent, QWidget* child, const QString& hint=QString());
    void hideChildWidget(QWidget* parent, QWidget* child, const QString& hint=QString());
    void initializeComponent();
    void finalizeComponent();
    void creationAnnounced();
    void componentCreated(AbstractComponent* comp);

public:
    QObject* containerObject();
    QString productName() const;
    QString organization() const;
    QImage programLogo() const;
    QString version() const;
    QString shortDescription() const;
    QString homepage() const;
    QString bugAddress() const;
    const QList<AboutPerson> authors() const;
    const QList<AboutPerson> credits() const;
    QString license() const;
    QString copyrightStatement() const;

protected slots:
    void shutdown();

protected:
    bool eventFilter(QObject* obj, QEvent* event);

private:
    friend class MainWindow;
    friend class AboutBox;
    MainWindowComponentData* d;
};
}

inline void UseStandardMainWindowContainer(const QString& appLogo=QString(),
                                           const QString& appIcon=QString(),
                                           const QString& splash=QString())
{
    GCF_REGISTER_COMPONENT(GCF::Components::MainWindowComponent);
    if(!appLogo.isEmpty())
        GCF::Components::MainWindowComponent::instance().setApplicationLogo(appLogo);
    if(!appIcon.isEmpty())
        GCF::Components::MainWindowComponent::instance().setApplicationIcon(appIcon);
    if(!splash.isEmpty())
        GCF::Components::MainWindowComponent::instance().setSplashImage(splash);

    if(gApp)
        QObject::connect(gApp, SIGNAL(allComponentsLoaded()), &GCF::Components::MainWindow::instance(), SLOT(showMaximized()));
    else
        GCF::Components::MainWindow::instance().showMaximized();

    GCF::Components::MainWindowComponent::instance().initialize();
}

}

#endif

