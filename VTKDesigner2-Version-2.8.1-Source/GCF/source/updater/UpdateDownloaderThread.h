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

#ifndef UPDATE_DOWNLOADER_THREAD_H
#define UPDATE_DOWNLOADER_THREAD_H

#include <QThread>

namespace GCF
{
struct UpdateDownloaderThreadData;
class UpdateDownloaderThread : public QThread
{
    Q_OBJECT

public:
    UpdateDownloaderThread(QObject* parent=0);
    ~UpdateDownloaderThread();

public slots:
    void stop();

signals:
    void progress(int pc);
    void finishedDownloadingUpdates(int count);

protected:
    void run();

protected slots:
    void downloadNextUpdate();
    void downloadCompleted();
    void downloadFailure();
    void downloadProgress(int);
    void allUpdatesDownloaded();

private:
    UpdateDownloaderThreadData* d;
};
}

#endif
