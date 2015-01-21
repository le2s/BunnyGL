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

#ifndef UPDATE_INFO_H
#define UPDATE_INFO_H

#include <QString>
#include <QDateTime>
#include <QList>
#include <QUrl>
#include <QList>
#include <QPair>
#include <QDomDocument>
#include <GCF/Common>

namespace GCF
{
struct GCF_EXPORT_CLASS UpdateFileInfo
{
    QString Arch;
    QString OS;
    QUrl    UpdateFileURL;
};

struct UpdateInfoData;
class GCF_EXPORT_CLASS UpdateInfo
{
public:
    UpdateInfo();
    UpdateInfo(const UpdateInfo& other);
    ~UpdateInfo();

    UpdateInfo& operator = (const UpdateInfo& other);
    bool operator == (const UpdateInfo& other) const;
    bool operator != (const UpdateInfo& other) const;

    bool isValid() const;
    QString name() const;
    QString title() const;
    QString source() const;
    QIcon icon() const;
    QString type() const;
    bool isForceInstall() const;
    QString description() const;
    QDateTime release() const;
    QString version() const;
    QList<UpdateFileInfo> updateFiles() const;
    UpdateFileInfo updateFile(const QString& arch, const QString& os) const;
    UpdateFileInfo updateFileForThisSystem() const;
    QList<UpdateInfo> dependencies() const;

    bool operator < (const UpdateInfo& other) const;
    bool recentThan(const UpdateInfo& other) const;
    static int CompareVersion(const QString& v1, const QString& v2);

    QDomElement toDomElement(QDomDocument doc, bool forThisSystem=true) const;

protected:
    void setName(const QString& name);
    void setTitle(const QString& title);
    void setSource(const QString& src);
    void setIconFromUrl(const QUrl& url);
    void setType(const QString& type);
    void setForceInstall(bool fi);
    void setDescription(const QString& desc);
    void setRelease(const QDateTime& dt);
    void setVersion(const QString& version);
    void setUpdateFiles(const QList<UpdateFileInfo>& ufList);
    void setUpdateFile(const QString& arch, const QString& os, const QUrl& url);
    void addDependency(const UpdateInfo& updateInfo);
    void setValid(bool val);

private:
    UpdateInfoData* d;

    // Basically all classes from the Updater module
    // are friends.
    friend class UpdateInfoList;
    friend class UpdaterComponent;
    friend class UpdateXMLParser;
    friend class UpdateFinderThread;
    friend class UpdateDownloaderThread;
    friend struct UpdaterComponentData;
    friend struct UpdateXMLParserData;
    friend struct UpdateFinderThreadData;
    friend struct UpdateDownloaderThreadData;
};

class GCF_EXPORT_CLASS UpdateInfoList
{
public:
    UpdateInfoList();
    ~UpdateInfoList();

    bool containsUpdate(const QString& name, const QString& version) const;
    UpdateInfo findUpdate(const QString& name, const QString& version) const;
    UpdateInfo addIfNotFound(const QString& name, const QString& version);
    void sort();

    QDomDocument toDomDocument(bool forThisSystem=true) const;

    /*
        We are not subclassing UpdateInfoList from QList<GCF::UpdateInfo> because
        1. MSVC flags several warnings when we do so. Although using #pragma would
           solve the problem, we dont want to do that.
        2. Export / Import of a template class subclass was causing some problem
        */

    // Mimic methods from QList in here.
    const GCF::UpdateInfo& at(int index) const { return m_theList.at(index); }
    GCF::UpdateInfo& first() { return m_theList.first(); }
    const GCF::UpdateInfo& first() const { return m_theList.first(); }
    GCF::UpdateInfo& last() { return m_theList.first(); }
    const GCF::UpdateInfo& last() const { return m_theList.first(); }
    int count() const { return m_theList.count(); }
    GCF::UpdateInfo& operator[] (int index) { return m_theList[index]; }
    const GCF::UpdateInfo& operator[] (int index) const { return m_theList[index]; }
    void append(const GCF::UpdateInfo& info) { m_theList.append(info); }
    bool operator == (const GCF::UpdateInfoList& other) const { return m_theList == other.m_theList; }
    void clear() { m_theList.clear(); }
    QList<GCF::UpdateInfo>::iterator begin() { return m_theList.begin(); }
    QList<GCF::UpdateInfo>::iterator end() { return m_theList.end(); }
    QList<GCF::UpdateInfo>::const_iterator begin() const { return m_theList.begin(); }
    QList<GCF::UpdateInfo>::const_iterator end() const { return m_theList.begin(); }
    QList<GCF::UpdateInfo>::const_iterator constBegin() const { return m_theList.constBegin(); }
    QList<GCF::UpdateInfo>::const_iterator constEnd() const { return m_theList.constEnd(); }
    bool contains(const GCF::UpdateInfo& info) const { return m_theList.contains(info); }
    void removeAt(int index) { m_theList.removeAt(index); }
    void removeAll(const GCF::UpdateInfo& info) { m_theList.removeAll(info); }
    int indexOf(const GCF::UpdateInfo& info, int from=0) const { return m_theList.indexOf(info,from); }
    QList<GCF::UpdateInfo>& list() { return m_theList; }
    const QList<GCF::UpdateInfo>& list() const { return m_theList; }

private:
    QList<GCF::UpdateInfo> m_theList;
};
}

#include <QMetaType>
Q_DECLARE_METATYPE(GCF::UpdateInfo)

#endif

