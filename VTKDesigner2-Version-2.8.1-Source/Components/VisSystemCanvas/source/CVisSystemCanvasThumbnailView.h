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

#ifndef C_VIS_SYSTEM_CANVAS_THUMBNAIL_VIEW_H
#define C_VIS_SYSTEM_CANVAS_THUMBNAIL_VIEW_H

#include <QGraphicsView>

struct CVisSystemCanvasThumbnailViewData;
class CVisSystemCanvasThumbnailView : public QGraphicsView
{
    Q_OBJECT

public:
    CVisSystemCanvasThumbnailView(QWidget* parent=0);
    ~CVisSystemCanvasThumbnailView();

    void setScene(QGraphicsScene* scene);

protected slots:
    void zoomFit();

protected:
    void mousePressEvent(QMouseEvent* me);
    void mouseMoveEvent(QMouseEvent* me);
    void mouseReleaseEvent(QMouseEvent* me);
    void keyPressEvent(QKeyEvent* ke);
    void keyReleaseEvent(QKeyEvent* ke);
    void wheelEvent(QWheelEvent* we);
    void contextMenuEvent(QContextMenuEvent* me);

protected:
    CVisSystemCanvasThumbnailViewData* d;
};

#endif
