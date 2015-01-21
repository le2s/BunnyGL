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

#include "BookmarkComponent.h"
#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>
#include <GCF/ComponentSettings>

#include "IBrowserWindow.h"

#include <QUrl>
#include <QMenu>
#include <QAction>
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QMessageBox>
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QListView>
#include <QStringListModel>
#include <QDockWidget>
#include <QPointer>

GCF_DEFINE_COMPONENT(BookmarkComponent)

struct BookmarkComponentData
{
    BookmarkComponentData() : browserWindowComp(0), browserWindow(0),
        bookmarksAction(0), bookmarksMenu(0), bookmarksModel(0) { }

    GCF::AbstractComponent* browserWindowComp;
    IBrowserWindow* browserWindow;
    QStringList bookmarks;
    QAction* bookmarksAction;
    QMenu* bookmarksMenu;
    QStringListModel* bookmarksModel;
    QPointer<QDockWidget> dockWidget;
};

BookmarkComponent& BookmarkComponent::instance()
{
    static BookmarkComponent* theInstance = GCF_CREATE_COMPONENT(BookmarkComponent);
    return *theInstance;
}

BookmarkComponent::BookmarkComponent()
{
    d = new BookmarkComponentData;
}

BookmarkComponent::~BookmarkComponent()
{
    delete d;
}

QObject* BookmarkComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split(".");

    if(comps.last() == "controller")
        return const_cast<BookmarkComponent*>(this);

    return 0;
}

QAction* BookmarkComponent::fetchAction(const QString& completeName) const
{
    QStringList comps = completeName.split(".");

    if(comps.last() == "bookmarks")
    {
        if(!d->bookmarksAction)
        {
            d->bookmarksMenu = new QMenu("Bookmarks");
            d->bookmarksAction = d->bookmarksMenu->menuAction();
        }

        return d->bookmarksAction;
    }

    return GCF::AbstractComponent::fetchAction(completeName);
}

void BookmarkComponent::initializeComponent()
{
    // Do nothing
}

void BookmarkComponent::finalizeComponent()
{
    settings()["Bookmarks"] = d->bookmarks;
}

void BookmarkComponent::creationAnnounced()
{
    QString name;
    QObject* obj = findObject("IBrowserWindow", name, &(d->browserWindowComp));
    if(obj)
        d->browserWindow = qobject_cast<IBrowserWindow*>(obj);

    d->bookmarks = settings()["Bookmarks"].toStringList();
    Q_FOREACH(QString bookmark, d->bookmarks)
    {
        QAction* action = d->bookmarksMenu->addAction(bookmark);
        connect(action, SIGNAL(triggered(bool)), this, SLOT(slotShowBookmarkFromQAction()));
    }
}

void BookmarkComponent::slotShowBookmarkFromQAction()
{
    if(!d->browserWindow)
        return;

    QAction* sa = qobject_cast<QAction*>(sender());
    if( !sa )
        return;

    QString urlStr = sa->text();

    d->browserWindow->urlEdit()->setText(urlStr);
    d->browserWindow->goButton()->click();
}

void BookmarkComponent::bookmarkCurrentPage()
{
    if(!d->browserWindow)
        return;

    QWebView* webView = d->browserWindow->browserControl();
    QUrl url = webView->url();
    QString urlStr = url.toString();
    if(urlStr.isEmpty())
        return;

    if(d->bookmarks.contains(urlStr))
        return;

    d->bookmarks.append(urlStr);
    QAction* action = d->bookmarksMenu->addAction(urlStr);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(slotShowBookmarkFromQAction()));
    if(d->bookmarksModel)
        d->bookmarksModel->setStringList(d->bookmarks);
}

void BookmarkComponent::showBookmarksPanel()
{
    if(!d->browserWindow)
        return;

    QMainWindow* mainWin = GCF::AbstractComponent::mainWindow();
    if(!mainWin)
        return;

    if(!d->bookmarksModel)
        d->bookmarksModel = new QStringListModel(d->bookmarks, this);

    if(d->dockWidget.isNull())
    {
        d->dockWidget = new QDockWidget(mainWin);
        QListView* bookmarksView = new QListView(d->dockWidget);
        d->dockWidget->setWidget(bookmarksView);
        bookmarksView->setModel(d->bookmarksModel);

        mainWin->addDockWidget(Qt::LeftDockWidgetArea, d->dockWidget, Qt::Horizontal);
        d->dockWidget->setWindowTitle("Bookmarks");
    }

    d->dockWidget->show();
}

GCF_EXPORT_COMPONENT_PLUGIN(BookmarkComponent)

