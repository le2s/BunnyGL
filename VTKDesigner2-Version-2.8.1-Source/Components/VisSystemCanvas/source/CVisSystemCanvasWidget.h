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

#ifndef C_VIS_SYSTEM_CANVAS_WIDGET_H
#define C_VIS_SYSTEM_CANVAS_WIDGET_H

#include <QGraphicsRectItem>

#define CVisSystemCanvasWidget_Type (QGraphicsItem::UserType+10)

class CVisSystemCanvas;
struct CVisSystemCanvasWidgetData;
class CVisSystemCanvasWidget : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    CVisSystemCanvasWidget(CVisSystemCanvas* canvas);
    ~CVisSystemCanvasWidget();

	void setGeometry(const QRectF& geo);
	QRectF geometry() const;

    void setWidget(QWidget* widget);
    QWidget* widget() const;

protected:
    bool eventFilter(QObject* obj, QEvent* event);
    void paint(QPainter *p, const QStyleOptionGraphicsItem* opt, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
    void mousePressEvent(QGraphicsSceneMouseEvent* me);
	void mouseMoveEvent(QGraphicsSceneMouseEvent* me);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* me);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* me);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* me);
	void hoverEnterEvent(QGraphicsSceneHoverEvent* he);
	void hoverMoveEvent(QGraphicsSceneHoverEvent* he);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* he);
    void keyPressEvent(QKeyEvent* ke);
    int type () const { return CVisSystemCanvasWidget_Type; }

private:
    CVisSystemCanvasWidgetData* d;
};

#endif
