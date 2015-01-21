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

#include "CVisSystemCanvasWidget.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QPainter>

#include <QColor>
#include <QBrush>
#include <QPen>
#include <QApplication>
#include <QPalette>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>

#include "CVisSystemCanvas.h"

const int borderSize = 5;
const int titleBarSize = 25;

struct CVisSystemCanvasWidgetData
{
    CVisSystemCanvasWidgetData() : widget(0), pWidget(0) { }

	CVisSystemCanvas* canvas;
    QWidget* widget;
    QGraphicsProxyWidget* pWidget;
	QSizeF minimumSize;

	Qt::CursorShape cursorShape;
	QPointF mousePos;
	bool resizing;

    QRectF titleTextRect(const QRectF& rect) const {
        return QRectF(rect.x(), rect.y(), rect.width(), titleBarSize).adjusted(2*borderSize, borderSize, -borderSize, -borderSize);
    }

    QRectF titleBarRect(const QRectF& rect) const {
        return QRectF(rect.x(), rect.y(), rect.width(), titleBarSize);
    }

    QRectF widgetRect(const QRectF& rect) const {
        return rect.adjusted(borderSize, titleBarSize, -borderSize, -borderSize);
    }

	QRectF resizeRect(const QRectF& rect) const {
		return QRectF(rect.right()-borderSize, rect.bottom()-borderSize, borderSize, borderSize);
	}
};

CVisSystemCanvasWidget::CVisSystemCanvasWidget(CVisSystemCanvas* canvas)
:QGraphicsRectItem(0, 0)
{
    d = new CVisSystemCanvasWidgetData;
	d->canvas = canvas;
	d->cursorShape = Qt::ArrowCursor;
	d->resizing = false;
	setAcceptHoverEvents(true);

    setFlags(QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable);

    QPalette pal = QApplication::palette();
	setBrush( pal.highlight() );
    setPen( pal.highlightedText().color() );

    canvas->scene()->addItem(this);
}

CVisSystemCanvasWidget::~CVisSystemCanvasWidget()
{
	if(d->pWidget)
	{
		d->pWidget->setWidget(0);
		delete d->pWidget;
	}
    delete d;
}

void CVisSystemCanvasWidget::setGeometry(const QRectF& geo)
{
	QPointF pos = geo.center();
	QSizeF size = geo.size();
	QSizeF minSize = d->minimumSize;
	QRectF rect;

	size = size.expandedTo(minSize);
	rect = QRectF(-size.width()/2, -size.height()/2, size.width(), size.height());

	setRect(rect);
	setPos(pos);

	if(d->pWidget)
	{
		QRectF wRect = d->widgetRect(rect);
		d->pWidget->resize(wRect.size());
		d->pWidget->setPos(wRect.topLeft());
	}

	emit d->canvas->projectPartModified();
}

QRectF CVisSystemCanvasWidget::geometry() const
{
	QPointF p = this->pos();
	QRectF r = this->rect();
	QRectF geo = r.adjusted(p.x(), p.y(), p.x(), p.y());
	return geo;
}

void CVisSystemCanvasWidget::setWidget(QWidget* widget)
{
    if(d->widget == widget)
        return;

    if(d->pWidget)
    {
        if(d->widget)
            d->widget->removeEventFilter(this);
        delete d->pWidget;
    }

    d->widget = widget;
    d->pWidget = 0;
	d->minimumSize = QSizeF(0,0);

    if(!d->widget)
    {
        update();
        return;
    }

	d->minimumSize = d->widget ? d->widget->sizeHint() + QSizeF(titleBarSize*2, titleBarSize*2+titleBarSize) : d->minimumSize;
    d->pWidget = scene()->addWidget(widget);
    d->pWidget->setParentItem(this);

	QPointF pos = this->pos();
	setGeometry( QRectF(pos.x(), pos.y(), 1, 1) );

    update();
}

QWidget* CVisSystemCanvasWidget::widget() const
{
    return d->widget;
}

bool CVisSystemCanvasWidget::eventFilter(QObject* obj, QEvent* event)
{
    if(obj == d->widget && event->type() == QEvent::WindowTitleChange)
        update();

    return false;
}

void CVisSystemCanvasWidget::paint(QPainter *p, const QStyleOptionGraphicsItem* opt, QWidget *widget)
{
	p->fillRect(rect(), opt->palette.highlight());

    if(d->widget)
	{
		if(opt->levelOfDetail >= 0.75)
		{
			QRectF titleBarRect = d->titleBarRect(rect());
			QLinearGradient grad(titleBarRect.topLeft(), titleBarRect.bottomLeft());
			grad.setColorAt(0, opt->palette.highlight().color());
			grad.setColorAt(1, opt->palette.highlight().color().dark());
			p->fillRect(titleBarRect, grad);

			QFont oldFont = p->font();
			QFont font = oldFont;
			font.setBold(true);
			p->setFont(font);

			QPen oldPen = p->pen();
			p->setPen(opt->palette.highlightedText().color());
			p->drawText(d->titleTextRect(rect()), Qt::AlignLeft|Qt::AlignVCenter, d->widget->windowTitle());

			p->setFont(oldFont);
			p->setPen(oldPen);
		}
	}
    else
        p->fillRect( d->widgetRect(rect()), opt->palette.window() );
}

QVariant CVisSystemCanvasWidget::itemChange(GraphicsItemChange change, const QVariant & value)
{
	if(change == QGraphicsItem::ItemPositionHasChanged)
		emit d->canvas->projectPartModified();

    return QGraphicsRectItem::itemChange(change, value);
}

void CVisSystemCanvasWidget::mousePressEvent(QGraphicsSceneMouseEvent* me)
{
	if(me->button() == Qt::LeftButton && d->cursorShape == Qt::SizeFDiagCursor)
	{
		d->resizing = true;
		d->mousePos = me->scenePos();
	}
	else
		QGraphicsRectItem::mousePressEvent(me);
}

void CVisSystemCanvasWidget::mouseMoveEvent(QGraphicsSceneMouseEvent* me)
{
	if(d->resizing)
	{
        QPointF mp = me->scenePos();
		QPointF dp = mp - d->mousePos;
        qDebug("dp = %.2f, %.2f = (%.2f, %.2f) - (%.2f, %.2f)", dp.x(), dp.y(),
               mp.x(), mp.y(), d->mousePos.x(), d->mousePos.y());

		prepareGeometryChange();
		QRectF r = this->geometry();
		r.setBottomRight( r.bottomRight()+dp );
		if(d->minimumSize.width() > r.width() || d->minimumSize.height() > r.height())
			return;

		setGeometry(r);
		d->mousePos = mp;
	}
	else
		QGraphicsRectItem::mouseMoveEvent(me);
}

void CVisSystemCanvasWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent* me)
{
	if(d->resizing)
    {
		d->resizing = false;
        d->mousePos = me->scenePos();
    }
	else
		QGraphicsRectItem::mouseReleaseEvent(me);
}

void CVisSystemCanvasWidget::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* me)
{
	QGraphicsRectItem::mouseDoubleClickEvent(me);
}

void CVisSystemCanvasWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent* me)
{
    QGraphicsRectItem::contextMenuEvent(me);
}

void CVisSystemCanvasWidget::keyPressEvent(QKeyEvent* ke)
{
    QGraphicsRectItem::keyPressEvent(ke);
}

void CVisSystemCanvasWidget::hoverEnterEvent(QGraphicsSceneHoverEvent* he)
{
	hoverMoveEvent(he);
}

void CVisSystemCanvasWidget::hoverMoveEvent(QGraphicsSceneHoverEvent* he)
{
	QRectF rr = d->resizeRect(rect());
	if(rr.contains(he->pos()))
	{
		if(d->cursorShape != Qt::SizeFDiagCursor)
		{
			setCursor(Qt::SizeFDiagCursor);
			d->cursorShape = Qt::SizeFDiagCursor;
		}
	}
	else
	{
		if(d->cursorShape != Qt::ArrowCursor)
		{
			unsetCursor();
			d->cursorShape = Qt::ArrowCursor;
		}
	}
}

void CVisSystemCanvasWidget::hoverLeaveEvent(QGraphicsSceneHoverEvent* he)
{
	if(d->cursorShape != Qt::ArrowCursor)
	{
		unsetCursor();
		d->cursorShape = Qt::ArrowCursor;
	}
	Q_UNUSED(he);
}



