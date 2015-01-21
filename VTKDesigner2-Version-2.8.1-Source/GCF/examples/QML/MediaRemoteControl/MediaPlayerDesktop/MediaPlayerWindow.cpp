#include "MediaPlayerWindow.h"
#include "ui_MediaPlayerWindow.h"

#include <QtDebug>
#include <QDeclarativeContext>
#include <GCF/DeclarativeView>

struct MediaPlayerWindowData
{
    Ui::MediaPlayerWindow ui;
};

MediaPlayerWindow::MediaPlayerWindow(QWidget *parent) :
    QMainWindow(parent)
{
    d = new MediaPlayerWindowData;
    d->ui.setupUi(this);

    d->ui.audioLibraryView->setSourceLater(QUrl("qrc:///QML/AudioLibrary.qml"));
    d->ui.videoLibraryView->setSourceLater(QUrl("qrc:///QML/VideoLibrary.qml"));

    connect(d->ui.mediaPlayer, SIGNAL(fullScreenRequest()), this, SLOT(onFullScreenRequest()));
    connect(d->ui.mediaPlayer, SIGNAL(normalScreenRequest()), this, SLOT(onNormalScreenRequest()));
}

MediaPlayerWindow::~MediaPlayerWindow()
{
    delete d;
}

MediaPlayer* MediaPlayerWindow::mediaPlayer() const
{
    return d->ui.mediaPlayer;
}

void MediaPlayerWindow::showEvent(QShowEvent *)
{
    int width = this->width()-this->layout()->margin() - 10;
    int width1 = qMin(int(qreal(width)*0.20), 200);
    int width2 = width - width1;
    d->ui.splitter->setSizes(QList<int>() << width1 << width2);
}

void MediaPlayerWindow::onFullScreenRequest()
{
    d->ui.tabWidget->hide();
    d->ui.menubar->hide();
    d->ui.statusbar->hide();
    d->ui.centralwidget->layout()->setMargin(0);
    this->raise();
    this->setWindowState(Qt::WindowFullScreen);
}

void MediaPlayerWindow::onNormalScreenRequest()
{
    d->ui.tabWidget->show();
    d->ui.menubar->show();
    d->ui.statusbar->show();
    d->ui.centralwidget->layout()->setMargin(10);
    this->raise();
    this->setWindowState(Qt::WindowMaximized);
}
