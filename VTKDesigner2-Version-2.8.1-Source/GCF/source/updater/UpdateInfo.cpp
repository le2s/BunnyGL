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

#include <GCF/UpdateInfo>

#include <QAtomicInt>
#include <QVariant>
#include <QApplication>
#include <QStyle>
#include <GCF/UpdaterComponent>

#include "IconCache.h"

/**
\internal
\ingroup updatergroup
\struct GCF::UpdateFileInfo UpdateFileInfo
\brief Describes an update file

This structure describes an update file in terms of the file name and
hardware architecture, OS for which it is designed.
*/

/**
\var QString GCF::UpdateFileInfo::Arch

Hardware architecture for which the update file is created. This value is
compared with string returned by \ref GCF::UpdaterComponent::architecture()
to figure out whether the update applies for the current hardware or not.
*/

/**
\var QString GCF::UpdateFileInfo::OS

Operating system for which the update file is created. This value is
compared with the string returned by \ref GCF::UpdaterComponent::os()
to figure out whether the update applies for the current OS or not.
*/

/**
\var QUrl GCF::UpdateFileInfo::UpdateFileURL

The complete URL from where the update file can be downloaded.
*/

/**
\internal
\ingroup updatergroup
\class GCF::UpdateInfo GCF/UpdateInfo
\brief Provides information about one update

This class provides information about an update in terms of the following
\li Name
\li Force Install Flag
\li Description
\li Release Date
\li Version
\li List of update files applicable to this update
\li List of dependencies for this update

UpdateInfo objects are created while parsing \ref updatesxml. The \ref UpdateXMLParser
class parses \ref updatesxml and creates UpdateInfo instances. Dependencies are
resolved by \ref UpdateXMLParser itself. Incase dependencies could not be resolved, then
the corresponding UpdateInfo instance is flagged invalid. (\ref GCF::UpdateInfo::isValid())

UpdateInfo instances are shallow copied. This means that copied instances of UpdateInfo
internally refer to the same data store, so modifying from once instance causes all copied
instances to be modified as well.
*/
namespace GCF
{
    struct UpdateInfoData
    {
        UpdateInfoData() : dependencyCount(0), refCount(1)  { }
        void ref() { refCount.ref(); }
        void deref() { if( !refCount.deref() ) delete this; }

        QString name;
        QString title;
        QString source;
        QIcon icon;
        QUrl iconUrl;
        bool valid;
        QString type;
        bool forceInstall;
        QString description;
        QDateTime release;
        QString version;
        QList<GCF::UpdateFileInfo> updateFiles;
        QList<GCF::UpdateInfo> dependencies;
        int dependencyCount; // how many updates are dependent on this.

        QAtomicInt refCount; // new decision to keep reference counts as QAtomicInts..
    };
}

/**
Constructs an invalid updatei nfo.
*/
GCF::UpdateInfo::UpdateInfo()
{
    d = new UpdateInfoData;
    d->valid = false;
    d->type = "NoType";
}

/**
\todo document this function
*/
GCF::UpdateInfo::UpdateInfo(const UpdateInfo& other)
{
    d = other.d;
    d->ref();
}

/**
\todo document this function
*/
GCF::UpdateInfo::~UpdateInfo()
{
    d->deref();
    d = 0;
}

/**
\todo document this function
*/
GCF::UpdateInfo& GCF::UpdateInfo::operator = (const UpdateInfo& other)
{
    if(d == other.d)
        return *this;

    d->deref();
    d = other.d;
    d->ref();

    return *this;
}

/**
\todo document this function
*/
bool GCF::UpdateInfo::operator == (const UpdateInfo& other) const
{
    return d == other.d;
}

/**
\todo document this function
*/
bool GCF::UpdateInfo::operator != (const UpdateInfo& other) const
{
    return d != other.d;
}

/**
\todo document this function

\note If this function returns true, then it may not necessarily mean that all its dependencies
are valid as well.
*/
bool GCF::UpdateInfo::isValid() const
{
    return d->valid;
}

/**
\todo document this function
*/
QString GCF::UpdateInfo::name() const
{
    return d->name;
}

/**
\todo document this function
*/
QString GCF::UpdateInfo::title() const
{
    return d->title;
}

/**
\todo document this function
*/
QString GCF::UpdateInfo::source() const
{
    return d->source;
}

/**
\todo document this function
*/
QIcon GCF::UpdateInfo::icon() const
{
    if(d->icon.isNull())
        d->icon = GCF::IconCache::instance().icon(d->iconUrl);

    return d->icon;
}

/**
\todo document this function
*/
QString GCF::UpdateInfo::type() const
{
    if(d->type.isEmpty())
        d->type = "NoType";

    return d->type;
}

/**
\todo document this function
*/
bool GCF::UpdateInfo::isForceInstall() const
{
    return d->forceInstall;
}

/**
\todo document this function
*/
QString GCF::UpdateInfo::description() const
{
    return d->description;
}

/**
\todo document this function
*/
QDateTime GCF::UpdateInfo::release() const
{
    return d->release;
}

/**
\todo document this function
*/
QString GCF::UpdateInfo::version() const
{
    return d->version;
}

/**
\todo document this function
*/
QList<GCF::UpdateFileInfo> GCF::UpdateInfo::updateFiles() const
{
    return d->updateFiles;
}

/**
\todo document this function
*/
GCF::UpdateFileInfo GCF::UpdateInfo::updateFile(const QString& arch, const QString& os) const
{
    for(int i=0; i<d->updateFiles.count(); i++)
    {
        GCF::UpdateFileInfo uFileInfo = d->updateFiles[i];
        if(uFileInfo.Arch == arch && uFileInfo.OS == os)
            return uFileInfo;
    }

    return GCF::UpdateFileInfo();
}

/**
\todo document this function
*/
GCF::UpdateFileInfo GCF::UpdateInfo::updateFileForThisSystem() const
{
    QString arch = GCF::UpdaterComponent::instance().architecture();
    QString os   = GCF::UpdaterComponent::instance().os();
    return updateFile(arch, os);
}

/**
\todo document this function
*/
QList<GCF::UpdateInfo> GCF::UpdateInfo::dependencies() const
{
    return d->dependencies;
}

/**
\todo document this function
*/
bool GCF::UpdateInfo::operator < (const UpdateInfo& other) const
{
    // UpdateInfo with the highest reference count should install first.
    return d->dependencyCount > other.d->dependencyCount;
}

/**
\todo document this function
*/
bool GCF::UpdateInfo::recentThan(const UpdateInfo& other) const
{
    if(d->name == other.d->name)
        return (CompareVersion(d->version, other.d->version) > 0);

    return false;
}

/**
This function compares two version strings \c v1 and \c v2 and returns
-1, 0 or +1 based on the following rule

\li Returns 0 if v1 == v2
\li Returns -1 if v1 < v2
\li Returns +1 if v1 > v2

The function is very similar to \c strcmp(), except that it works on version strings.

Example:
\code

GCF::UpdaterComponent::CompareVersion("2.0", "2.1"); // Returns -1
GCF::UpdaterComponent::CompareVersion("2.1", "2.0"); // Returns +1
GCF::UpdaterComponent::CompareVersion("2.0", "2.0"); // Returns 0
GCF::UpdaterComponent::CompareVersion("2.1", "2.1"); // Returns 0

GCF::UpdaterComponent::CompareVersion("2.0", "2.x"); // Returns -1
GCF::UpdaterComponent::CompareVersion("2.x", "2.0"); // Returns +1
GCF::UpdaterComponent::CompareVersion("2.0", "2.x"); // Returns -1
GCF::UpdaterComponent::CompareVersion("2.x", "2.0"); // Returns +1

GCF::UpdaterComponent::CompareVersion("2.0.12.4", "2.1.10.4"); // Returns -1
GCF::UpdaterComponent::CompareVersion("2.0.12.x", "2.0.x");    // Returns -1
GCF::UpdaterComponent::CompareVersion("2.1.12.x", "2.0.x");    // Returns +1
GCF::UpdaterComponent::CompareVersion("2.1.12.x", "2.x");      // Returns -1
GCF::UpdaterComponent::CompareVersion("2.x", "2.1.12.x");      // Returns +1

\endcode
*/
int GCF::UpdateInfo::CompareVersion(const QString& v1, const QString& v2)
{
    // This code has been taken from the KDUpdater project, with
    // permission from KDAB.

    // Check for equality
    if( v1 == v2 )
        return 0;

    // Split version numbers across .
    QStringList v1_comps = v1.split('.');
    QStringList v2_comps = v2.split('.');

    // Check each component of the version
    int index = 0;
    while(1)
    {
        if( index >= v1_comps.count() || index >= v2_comps.count() )
            break;

        bool v1_ok, v2_ok;
        int v1_comp = v1_comps[index].toInt(&v1_ok);
        int v2_comp = v2_comps[index].toInt(&v2_ok);

        if(!v1_ok)
        {
            if(v1_comps[index] == "x")
                return +1;
        }
        else if(!v2_ok)
        {
            if(v2_comps[index] == "x")
                return -1;
        }

        if( v1_comp < v2_comp )
            return -1;

        if( v1_comp > v2_comp )
            return +1;

        // v1_comp == v2_comp
        ++index;
    }

    if( index < v2_comps.count() )
        return +1;

    if( index < v1_comps.count() )
        return -1;

    // Controversial return. I hope this never happens.
    return 0;
}

/**
\todo document this function
*/
QDomElement GCF::UpdateInfo::toDomElement(QDomDocument doc, bool forThisSystem) const
{
    QDomElement updateE = doc.createElement("Update");

    QDomElement nameE = doc.createElement("Name");
    nameE.appendChild( doc.createTextNode(d->name) );
    updateE.appendChild(nameE);

    QDomElement titleE = doc.createElement("Title");
    titleE.appendChild( doc.createTextNode(d->title) );
    updateE.appendChild(titleE);

    QDomElement sourceE = doc.createElement("Source");
    sourceE.appendChild( doc.createTextNode(d->source) );
    updateE.appendChild(sourceE);

    QDomElement iconE = doc.createElement("Icon");
    iconE.appendChild( doc.createTextNode(d->iconUrl.toString()) );
    updateE.appendChild(iconE);

    QDomElement typeE = doc.createElement("Type");
    typeE.appendChild( doc.createTextNode(d->type) );
    updateE.appendChild(typeE);

    QDomElement forceInstallE = doc.createElement("ForceInstall");
    forceInstallE.appendChild( doc.createTextNode( QVariant(d->forceInstall).toString() ) );
    updateE.appendChild(forceInstallE);

    QDomElement descE = doc.createElement("Description");
    descE.appendChild( doc.createCDATASection(d->description) );
    updateE.appendChild(descE);

    QDomElement releaseE = doc.createElement("Release");
    releaseE.appendChild( doc.createTextNode(d->release.toString()) );
    updateE.appendChild(releaseE);

    QDomElement versionE = doc.createElement("Version");
    versionE.appendChild( doc.createTextNode(d->version) );
    updateE.appendChild(versionE);

    QList<UpdateFileInfo> ufiList;
    if( forThisSystem )
    {
        UpdateFileInfo fileInfo = updateFileForThisSystem();
        ufiList.append(fileInfo);
    }
    else
        ufiList = d->updateFiles;

    for(int j=0; j<ufiList.count(); j++)
    {
        UpdateFileInfo ufi = ufiList[j];
        QDomElement updateFileE = doc.createElement("UpdateFile");
        updateE.appendChild(updateFileE);

        updateFileE.setAttribute("OS", ufi.OS);
        updateFileE.setAttribute("Arch", ufi.Arch);
        updateFileE.appendChild( doc.createTextNode(ufi.UpdateFileURL.toString()) );
    }

    const QList<UpdateInfo>& deps = d->dependencies;
    for(int j=0; j<deps.count(); j++)
    {
        UpdateInfo dep = deps[j];
        QString depStr = QString("%1 [%2]").arg( dep.name() ).arg( dep.version() );

        QDomElement dependsOnE = doc.createElement("DependsOn");
        dependsOnE.appendChild( doc.createTextNode(depStr) );
        updateE.appendChild(dependsOnE);
    }

    return updateE;
}

void GCF::UpdateInfo::setName(const QString& name)
{
    d->name = name;
}

void GCF::UpdateInfo::setTitle(const QString& title)
{
    d->title = title;
}

void GCF::UpdateInfo::setSource(const QString& src)
{
    d->source = src;
}

void GCF::UpdateInfo::setIconFromUrl(const QUrl& url)
{
    d->iconUrl = url;

    GCF::IconCache::instance().fetchIcon(url);
}

void GCF::UpdateInfo::setType(const QString& type)
{
    d->type = type;
}

void GCF::UpdateInfo::setForceInstall(bool fi)
{
    d->forceInstall = fi;
}

void GCF::UpdateInfo::setDescription(const QString& desc)
{
    d->description = desc;
}

void GCF::UpdateInfo::setRelease(const QDateTime& dt)
{
    d->release = dt;
}

void GCF::UpdateInfo::setVersion(const QString& version)
{
    d->version = version;
}

void GCF::UpdateInfo::setUpdateFiles(const QList<UpdateFileInfo>& ufList)
{
    d->updateFiles = ufList;
}

void GCF::UpdateInfo::setUpdateFile(const QString& arch, const QString& os, const QUrl& url)
{
    for(int i=0; i<d->updateFiles.count(); i++)
    {
        UpdateFileInfo& uFileInfo = d->updateFiles[i];
        if(uFileInfo.Arch == arch && uFileInfo.OS == os)
            uFileInfo.UpdateFileURL = url;
    }
}

void GCF::UpdateInfo::addDependency(const GCF::UpdateInfo& updateInfo)
{
    d->dependencies.append( updateInfo );
    ++(updateInfo.d->dependencyCount);
}

void GCF::UpdateInfo::setValid(bool val)
{
    d->valid = val;
}

/**
\internal
\ingroup updatergroup
\class GCF::UpdateInfoList GCF/UpdateInfoList
\brief Container class for UpdateInfo instances

This class is basically a QList<GCF::UpdateInfo>. It provides easy to use
mechanisms to add/search UpdateInfo structures based on their name and
version.
*/

/**
Constructor
*/
GCF::UpdateInfoList::UpdateInfoList()
{

}

/**
Destructor
*/
GCF::UpdateInfoList::~UpdateInfoList()
{

}

/**
Returns true of this list contains an UpdateInfo for (\c name, \c version).
*/
bool GCF::UpdateInfoList::containsUpdate(const QString& name, const QString& version) const
{
    int nr = this->count();
    for(int i=0; i<nr; i++)
    {
        GCF::UpdateInfo uInfo = this->at(i);
        if(uInfo.name() == name)
        {
            if(uInfo.version() == version)
                return true;

            if(UpdateInfo::CompareVersion(uInfo.version(), version) > 0)
                return true;
        }
    }

    return false;
}

/**
This function looks for an update-info whose name and version are as specified in the parameters.
If none was found an empty UpdateInfo structure is returned.

\note The returned update can be invalid and/or empty
*/
GCF::UpdateInfo GCF::UpdateInfoList::findUpdate(const QString& name, const QString& version) const
{
    int nr = this->count();
    for(int i=0; i<nr; i++)
    {
        GCF::UpdateInfo uInfo = this->at(i);
        if(uInfo.name() == name && uInfo.version() == version)
            return uInfo;
    }

    return GCF::UpdateInfo();
}

/**
This function looks for an update-info whose name and version are as specified in the parameters.
If none was found, then a new update-info instance is created and added to the list.

Either way the returned update-info has the same name and version number as those passed in the
parameter.

\note The returned update can be invalid
\note The returned update is never empty (ie name and version will surely be available)
*/
GCF::UpdateInfo GCF::UpdateInfoList::addIfNotFound(const QString& name, const QString& version)
{
    int nr = this->count();
    for(int i=0; i<nr; i++)
    {
        GCF::UpdateInfo uInfo = this->at(i);
        if(uInfo.name() == name && uInfo.version() == version)
            return uInfo;
    }

    GCF::UpdateInfo uInfo;
    uInfo.setName(name);
    uInfo.setVersion(version);
    this->append(uInfo);

    return uInfo;
}

/**
Sorts the list such that the update that is depended on the most comes first.
*/
void GCF::UpdateInfoList::sort()
{
    qSort( m_theList.begin(), m_theList.end() );
}

/**
Writes information about updates into a QDomDocument.

If \c forThisSystem is true, then only updates that can be installed on
this system will be populated into QDomDocument.
*/
QDomDocument GCF::UpdateInfoList::toDomDocument(bool forThisSystem) const
{
    QDomDocument doc;

    QDomElement rootE = doc.createElement("Updates");
    doc.appendChild(rootE);

    // We assume that the updates in this list can be applied to this version
    // of the application. Hence the baseline should be fine.
    rootE.setAttribute("Baseline", qApp->applicationVersion());

    int nr = this->count();
    for(int i=0; i<nr; i++)
    {
        GCF::UpdateInfo uInfo = this->at(i);

        QDomElement updateE = uInfo.toDomElement(doc, forThisSystem);
        rootE.appendChild(updateE);
    }

    return doc;
}
