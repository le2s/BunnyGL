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

#ifndef UPDATE_INSTALLER_H
#define UPDATE_INSTALLER_H

#include <QWidget>
#include <QDomDocument>

#include <GCF/Logger>

struct UpdateInstallerData;
class UpdateInstaller : public QWidget
{
    Q_OBJECT

public:
    static UpdateInstaller& instance();
    ~UpdateInstaller();

protected:
    // These methods are basically helpers for FileObject
    // FileSystemObject and SystemObject
    QString resolveFileName(const QString& fileName) const;
    QWidget* installerWindow() const;
    void progress(int pc, const QString& msg);
    QDomDocument clcGuiXml() const;
    QString currentUpdate() const;

protected slots:
    void doInstallUpdates();

protected:
    // Protected constructor, use only instance().
    UpdateInstaller(QWidget* parent=0);

public:
    // Not for public access though
    GCF::Log* log() const;

private:
    friend class FileObject;
    friend class FileSystemObject;
    friend class SystemObject;
    friend struct UpdateInstallerData;
    friend int main(int, char**);

    static QString ApplicationName;
    static QString ApplicationPath;
    static QString GuiXmlDirPath;
    static QString UpdatesDirPath;

    UpdateInstallerData* d;
};


#define GCF_UINST_MARK_MSG(Msg) QString("%1(%2)  - %3").arg(__FILE__).arg(__LINE__).arg(Msg)
#define GCF_UINST_DEBUG(Msg) GCF_DEBUG(UpdateInstaller::instance().log(), GCF_UINST_MARK_MSG(Msg))
#define GCF_UINST_ERROR(Msg) GCF_ERROR(UpdateInstaller::instance().log(), GCF_UINST_MARK_MSG(Msg))
#define GCF_UINST_WARNING(Msg) GCF_WARNING(UpdateInstaller::instance().log(), GCF_UINST_MARK_MSG(Msg))
#define GCF_UINST_FATAL(Msg) GCF_FATAL(UpdateInstaller::instance().log(), GCF_UINST_MARK_MSG(Msg))
#define GCF_UINST_LOG(Msg) GCF_LOG(UpdateInstaller::instance().log(), GCF_UINST_MARK_MSG(Msg))

#endif

