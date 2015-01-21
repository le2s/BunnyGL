#include <GCF/Application>

#include <QUrl>
#include <QDir>

#include "BrowserWindowComponent.h"
#include "BrowserWindow.h"

int main(int argc, char** argv)
{
    GCF::Application a(argc, argv);
    a.setApplicationName("SimpleBrowser");
    a.setApplicationVersion("1.0.0");
    a.setGuiXmlFileDirectory("../examples/Updater/SimpleBrowser/GUIXML");

    // Register the browser component first
    GCF_REGISTER_COMPONENT(BrowserWindowComponent);
    BrowserWindowComponent::instance().initialize();

    // Configure the updater and have it check for updates immediately.
    QString serverSiteDir = QString("%1/../examples/Updater/SimpleBrowser/ServerSite").arg(qApp->applicationDirPath());
    QString updatesDir = QUrl::fromLocalFile( QFileInfo(serverSiteDir).absoluteFilePath() ).toString();
    qDebug("Primary Updates Directory: %s", qPrintable(updatesDir));
    a.setPrimaryServerSite(updatesDir);
    a.setAllowThirdPartyServerSites(false);
    a.setCheckUpdateFrequency(GCF::ApplicationStartupButOnceADay);
    a.setUpdateSelectionStrategy(GCF::SelectByNotifyingUser);
    a.setAutoUpdatesEnabled(true);

    // Now show the application window.
    BrowserWindow::instance().showMaximized();

    return a.exec();
}
