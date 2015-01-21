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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <GCF/AbstractComponent>
#include <GCF/INewUpdatesHandler>
#include <GCF/IBinaryDataSerializer>
#include <GCF/IMethodInvocationHelper>
#include <GCF/IDataToGenericArgumentConverter>
#include <GCF/ComponentFactory>

namespace GCF
{
class Log;

enum CheckUpdateFrequency
{
    ApplicationStartup,
    ApplicationStartupButOnceADay,
    UponUserRequest,
    OnceInAnHour,
    OnceInTwoHours,
    OnceInFiveHours
};

enum UpdateSelectionStrategy
{
    SelectAllUpdates,
    SelectCriticalUpdates,
    SelectByAskingUser,
    SelectByNotifyingUser
};

struct ApplicationData;
class GCF_EXPORT_CLASS Application : public QApplication
{
    Q_OBJECT

public:
    static Application& instance();

    Application(int& argc, char** argv);
    ~Application();

    // Methods for loading components
    void loadComponent(const QString& componentName, const QString& libraryName);
    void addComponentPath(const QString& path);
    QStringList componentPaths() const;

    // Methods for IPC
    void setIPCEnabled(bool val=true);
    bool isIPCEnabled() const;
    void setIPCName(const QString& name);
    QString ipcName() const;
    void setIPCPort(quint16 port);
    quint16 ipcPort() const;

    // Methods for Automatic updates
    void setAutoUpdatesEnabled(bool val=true);
    bool isAutoUpdatesEnabled() const;
    void setPrimaryServerSite(const QString& url);
    QString primaryServerSite() const;
    void setAllowThirdPartyServerSites(bool val);
    bool isAllowThirdPartyServerSites() const;
    void addServerSite(const QString& url);
    QStringList serverSiteList() const;
    void setCheckUpdateFrequency(GCF::CheckUpdateFrequency freq);
    GCF::CheckUpdateFrequency checkUpdateFrequency() const;
    void setNewUpdatesHandler(INewUpdatesHandler* h);
    INewUpdatesHandler* newUpdatesHandler() const;
    QString architecture() const;
    QString os() const;
    void setUpdateSelectionStrategy(GCF::UpdateSelectionStrategy s);
    GCF::UpdateSelectionStrategy updateSelectionStrategy() const;

    // Set/Get component host window!
    void setGuiXmlFileDirectory(const QString& absDirPath);
    QString guiXmlFileDirectory();
    void setMainWindow(QMainWindow* mainWindow);
    QMainWindow* mainWindow();
    void setComponentHostWindow(IComponentHostWindow* hostWindow);
    IComponentHostWindow* componentHostWindow();
    void setComponentHostWidget(QWidget* widget);
    QWidget* componentHostWidget();

    // Methods for finding components
    GCF::AbstractComponent* findComponent(const QString& componentName) const;

    // Methods for registering serializers
    void registerSerializer(IBinaryDataSerializer* serializer);
    void unregisterSerializer(IBinaryDataSerializer* serializer);
    IBinaryDataSerializer* findSerializer(const QByteArray& typeName);
    void registerHelper(IMethodInvocationHelper* helper);
    void unregisterHelper(IMethodInvocationHelper* helper);
    IMethodInvocationHelper* findHelper(const QByteArray& typeName);
    void registerConverter(IDataToGenericArgumentConverter* conv);
    void unregisterConverter(IDataToGenericArgumentConverter* conv);
    IDataToGenericArgumentConverter* findConverter(const QByteArray& typeName);
    void registerDataTypeSupport(QObject* object);
    void unregisterDataTypeSupport(QObject* object);

    // Helpers for invoking methods
    bool callMethod(QObject* object, const QString& methodName, const QVariantList& args,
                    QVariant& returnValue, bool publicOnly=true);
    int  findMethod(QObject* object, const QString& methodName, const QVariantList& args);
    bool callMethod(QObject* object, int methodIndex, const QVariantList& args,
                    QVariant& returnValue, bool publicOnly=true);

    // Methods to help figure out parameter values from the Application.xml
    QStringList parameterNames() const;
    bool hasParameter(const QString& parameter) const;
    QString parameterValue(const QString& name, const QString& defaultValue=QString()) const;

    // Log messages into the default application log
    void setLogObject(GCF::Log* log);
    GCF::Log* logObject() const;

    void log(const QString& msg);
    void debugLog(const QString& msg);
    void errorLog(const QString& msg);
    void warningLog(const QString& msg);
    void fatalLog(const QString& msg);

private slots:
    void loadConfiguration();

signals:
    void allComponentsLoaded();

private:
    ApplicationData* d;

public:
    // Support for QML components
    bool loadQMLComponent(const QString& qmlCompName, const QString& qmlCompGUIXMLFile);

    // Static functions (previously in IFileSystemUtils)
    static QString findFileInPath(const QString& fileName);
    static QStringList findFilesInPath(const QString& fileName);
    static void deleteDirectory(const QString& dir, bool deleteDir=false);
};
}

#define gApp (&(GCF::Application::instance()))

#endif // APPLICATION_H
