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

#ifndef GCF_ICON_CACHE_H
#define GCF_ICON_CACHE_H

#include <QUrl>
#include <QIcon>
#include <QObject>

namespace GCF
{
struct IconCacheData;
class IconCache : public QObject
{
    Q_OBJECT

public:
    static IconCache& instance();
    ~IconCache();

    void fetchIcon(const QUrl& url, bool forceFetch=false);
    QIcon icon(const QUrl& url);

protected:
    IconCache(QObject* parent=0);

protected slots:
    void iconDownloaded();
    void iconDownloadAborted();
    void fetchIcon2(const QUrl& url);

signals:
    void iconFetched(const QUrl& url);

private:
    IconCacheData* d;
};
}

#endif
