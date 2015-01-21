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

#include "SnapshotCapturerComponent.h"
#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>

#include "IBrowserWindow.h"

#include <QPainter>
#include <QPixmap>
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QMessageBox>
#include <QMainWindow>
#include <QFileDialog>

GCF_DEFINE_COMPONENT(SnapshotCapturerComponent)

struct SnapshotCapturerComponentData
{
    SnapshotCapturerComponentData() : browserWindowComp(0), browserWindow(0) { }

    GCF::AbstractComponent* browserWindowComp;
    IBrowserWindow* browserWindow;
};

SnapshotCapturerComponent& SnapshotCapturerComponent::instance()
{
    static SnapshotCapturerComponent* theInstance = GCF_CREATE_COMPONENT(SnapshotCapturerComponent);
    return *theInstance;
}

SnapshotCapturerComponent::SnapshotCapturerComponent()
{
    d = new SnapshotCapturerComponentData;
}

SnapshotCapturerComponent::~SnapshotCapturerComponent()
{
    delete d;
}

QObject* SnapshotCapturerComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split(".");

    if(comps.last() == "controller")
        return const_cast<SnapshotCapturerComponent*>(this);

    return 0;
}

void SnapshotCapturerComponent::initializeComponent()
{
    // Do nothing
}

void SnapshotCapturerComponent::finalizeComponent()
{
    // Do nothing
}

void SnapshotCapturerComponent::creationAnnounced()
{
    QString name;
    QObject* obj = findObject("IBrowserWindow", name, &(d->browserWindowComp));
    if(obj)
        d->browserWindow = qobject_cast<IBrowserWindow*>(obj);
}

void SnapshotCapturerComponent::captureSnapshot()
{
    if( !d->browserWindow )
    {
        QMessageBox::information( mainWindow(), "Cannot find browser",
            "Cannot find the browser component. There was some error in the example code");

        return;
    }

    QWebView* webView = d->browserWindow->browserControl();
    QWebPage* page    = webView->page();
    QWebFrame* frame  = page->mainFrame();
    QPixmap   pm(640, 640);
    pm.fill(Qt::white);

    QPainter paint;
    paint.begin( &pm );
    frame->render(&paint);
    paint.end();

    // Find out from the user where to save the snapshot.
     QString fileName = QFileDialog::getSaveFileName(mainWindow(), tr("Save File"),
                                QDir::homePath(),
                                "Images (*.jpg)");
    if(fileName.isEmpty())
        return;

    pm.save(fileName, "JPG");

    QMessageBox::information( mainWindow(), "Snapshot saved",
        QString("Snapshot saved into %1").arg(fileName));
}

GCF_EXPORT_COMPONENT_PLUGIN(SnapshotCapturerComponent)

