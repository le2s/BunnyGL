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

#include "CVisSystemCanvasThumbnailView.h"
#include <QScrollBar>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <math.h>

struct CVisSystemCanvasThumbnailViewData
{
    CVisSystemCanvasThumbnailViewData() : panMode(false) { }

    bool panMode;
    QPoint mousePos;
};

CVisSystemCanvasThumbnailView::CVisSystemCanvasThumbnailView(QWidget* parent)
: QGraphicsView(parent)
{
    d = new CVisSystemCanvasThumbnailViewData;

    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);
}

CVisSystemCanvasThumbnailView::~CVisSystemCanvasThumbnailView()
{
    delete d;
}

void CVisSystemCanvasThumbnailView::setScene(QGraphicsScene* scene)
{
    QGraphicsView::setScene(scene);
    zoomFit();
}

void CVisSystemCanvasThumbnailView::zoomFit()
{
    fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void CVisSystemCanvasThumbnailView::mousePressEvent(QMouseEvent* me)
{
    if( !itemAt(me->pos()) )
    {
        d->panMode = true;
        d->mousePos = me->pos();
    }
    else
        QGraphicsView::mousePressEvent(me);
}

void CVisSystemCanvasThumbnailView::mouseMoveEvent(QMouseEvent* me)
{
    if(d->panMode)
    {
        QPoint dp = d->mousePos - me->pos();
        d->mousePos = me->pos();

        horizontalScrollBar()->setValue(horizontalScrollBar()->value()+dp.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value()+dp.y());
    }
    else
        me->accept();
}

void CVisSystemCanvasThumbnailView::mouseReleaseEvent(QMouseEvent* me)
{
    if(!d->panMode)
        QGraphicsView::mouseReleaseEvent(me);

    d->panMode = false;
}

void CVisSystemCanvasThumbnailView::keyPressEvent(QKeyEvent* ke)
{
    ke->setAccepted(true); // do nothing
}

void CVisSystemCanvasThumbnailView::keyReleaseEvent(QKeyEvent* ke)
{
    ke->setAccepted(true); // do nothing
}

void CVisSystemCanvasThumbnailView::wheelEvent(QWheelEvent* we)
{
    int delta = int(we->delta()) / 120;
    qreal s = 1;

    if(delta < 0)
        s = 0.9;
    else
        s = 1.1;

    scale(s, s);
    we->accept();
}

void CVisSystemCanvasThumbnailView::contextMenuEvent(QContextMenuEvent* me)
{
    if( !itemAt(me->pos()) )
    {
        QMenu menu;
        QAction* fitInViewAct = menu.addAction("View All");
        if( menu.exec(QCursor::pos()) == fitInViewAct )
            zoomFit();
        me->accept();
    }
    else
        QGraphicsView::contextMenuEvent(me);
}


