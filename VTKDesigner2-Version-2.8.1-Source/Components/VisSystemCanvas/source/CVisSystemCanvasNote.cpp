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

#include "CVisSystemCanvasNote.h"
#include "CVisSystemCanvas.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QPainter>
#include <QMessageBox>

#include <QColor>
#include <QBrush>
#include <QPen>
#include <QApplication>
#include <QPalette>
#include <QGraphicsProxyWidget>
#include <QTextEdit>
#include <QPointer>
#include <QSvgRenderer>

const int borderSize = 10;
const int closeButtonRectSize = 12;

struct CVisSystemCanvasNoteData
{
    CVisSystemCanvasNoteData() : svgRenderer(0) { }

    CVisSystemCanvas* canvas;
    QTextDocument textDocument;
    QColor color;
    QSizeF minimumSize;
    QSvgRenderer* svgRenderer;
    QString svgElementId;

	Qt::CursorShape cursorShape;
	QPointF mousePos;
	bool resizing;
    bool mouseOver;

    QPointer<QTextEdit> textEdit;
    QPointer<QGraphicsProxyWidget> textEditProxyWidget;

    QRectF textRect(const QRectF& rect) {
        return rect.adjusted(20, 20, -20, -20);
    }

	QRectF closeButtonRect(const QRectF& rect) const {
		return QRectF(rect.right()-closeButtonRectSize, rect.top(), closeButtonRectSize, closeButtonRectSize);
	}

	QRectF resizeRect(const QRectF& rect) const {
		return QRectF(rect.right()-borderSize, rect.bottom()-borderSize, borderSize, borderSize);
	}
};

CVisSystemCanvasNote::CVisSystemCanvasNote(CVisSystemCanvas* parent)
:QGraphicsRectItem(0)
{
    d = new CVisSystemCanvasNoteData;
    d->canvas = parent;
    QObject::setParent(d->canvas);

    // Default font
    QFont font;
    font = QApplication::font();
    font.setStyleHint(QFont::Decorative);
    font.setPointSize(font.pointSize()+4);
    setNoteFont(font);

    // Default color
    d->color = Qt::black;

	d->cursorShape = Qt::ArrowCursor;
	d->resizing = false;
    d->mouseOver = false;

	setAcceptHoverEvents(true);
    d->minimumSize = QSizeF(100, 100);
    setRect(QRectF(-100, -100, 200, 200));
    qApp->installEventFilter(this);

    setFlags(QGraphicsRectItem::ItemIsFocusable|QGraphicsRectItem::ItemIsMovable|QGraphicsRectItem::ItemIsSelectable);

    connect(&d->textDocument, SIGNAL(contentsChanged()), this, SIGNAL(noteTextChanged()));
}

CVisSystemCanvasNote::~CVisSystemCanvasNote()
{
    delete d;
}

QObject* CVisSystemCanvasNote::containerObject()
{
    return this;
}

QGraphicsItem* CVisSystemCanvasNote::graphicsItem()
{
    return this;
}

void CVisSystemCanvasNote::setNoteRect(const QRectF& noteRect)
{
    QPointF pos = noteRect.center();
	QSizeF size = noteRect.size();
	QSizeF minSize = d->minimumSize;

	size = size.expandedTo(minSize);

    prepareGeometryChange();
    setRect(QRectF(-size.width()/2, -size.height()/2, size.width(), size.height()));
	setPos(pos);
    update();

    emit noteRectChanged();
	emit d->canvas->projectPartModified();
}

QRectF CVisSystemCanvasNote::noteRect() const
{
	QPointF p = this->pos();
    QRectF r = this->rect();
	QRectF geo = r.adjusted(p.x(), p.y(), p.x(), p.y());
	return geo;
}

void CVisSystemCanvasNote::setNoteSvgRenderer(QSvgRenderer* ren)
{
    d->svgRenderer = ren;
}

QSvgRenderer* CVisSystemCanvasNote::noteSvgRenderer() const
{
    return d->svgRenderer;
}

void CVisSystemCanvasNote::setNoteSvgElementId(const QString& id)
{
    d->svgElementId = id;
}

QString CVisSystemCanvasNote::noteSvgElementId() const
{
    return d->svgElementId;
}

void CVisSystemCanvasNote::setNoteText(const QString& text)
{
    d->textDocument.setPlainText(text);
    update();
}

QString CVisSystemCanvasNote::noteText() const
{
    return d->textDocument.toPlainText();
}

void CVisSystemCanvasNote::setNoteTextColor(const QColor& noteColor)
{
    d->color = noteColor;
    update();
}

QColor CVisSystemCanvasNote::noteTextColor() const
{
    return d->color;
}

void CVisSystemCanvasNote::setNoteFont(const QFont& font)
{
    d->textDocument.setDefaultFont(font);
    update();
}

QFont CVisSystemCanvasNote::noteFont() const
{
    return d->textDocument.defaultFont();
}

bool CVisSystemCanvasNote::eventFilter(QObject* obj, QEvent* event)
{
    if(!d->textEdit.isNull())
    {
        // If the user presses mouse anywhere but the text edit, stop editing
        if(event->type() == QEvent::MouseButtonPress && obj != d->textEdit && obj != d->textEdit->viewport())
            delete d->textEditProxyWidget;
        // If the text edit loses focus because of someother reason.
        else if(event->type() == QEvent::FocusOut && (obj == d->textEdit || obj == d->textEdit->viewport()))
            delete d->textEditProxyWidget;
    }

    return false;
}

void CVisSystemCanvasNote::paint(QPainter *p, const QStyleOptionGraphicsItem* opt, QWidget *widget)
{
    QRectF r = this->rect();

    if(d->svgRenderer && d->svgRenderer->isValid())
    {
        if (d->svgElementId.isEmpty())
            d->svgRenderer->render(p, r);
        else
            d->svgRenderer->render(p, d->svgElementId, r);
    }

    /*if (opt->state & QStyle::State_Selected)
        p->drawRect(r.adjusted(1,1,-1,-1));*/

    if(d->textEdit.isNull())
    {
        QRectF rect = d->textRect(this->rect());
        d->textDocument.setTextWidth(rect.width());
        p->save();
        p->translate(rect.topLeft());
        d->textDocument.drawContents(p, QRectF(0, 0, rect.width(), rect.height()));
        p->restore();
    }

    if(d->mouseOver)
    {
        // Draw the resize triangle
        QRectF rr = d->resizeRect(r);
        QPainterPath path;
        path.moveTo(rr.topRight());
        path.lineTo(rr.bottomLeft());
        path.lineTo(rr.bottomRight());
        path.closeSubpath();
        p->fillPath(path, Qt::black);

        // Draw the close button
        QRectF cr = d->closeButtonRect(r);
        static QPixmap pm(":/VisSystemCanvas/deleteNote.png");
        p->drawPixmap(cr, pm, QRectF());
    }
}

QVariant CVisSystemCanvasNote::itemChange(GraphicsItemChange change, const QVariant & value)
{
	if(change == QGraphicsRectItem::ItemPositionHasChanged)
		emit d->canvas->projectPartModified();

    return QGraphicsRectItem::itemChange(change, value);
}

void CVisSystemCanvasNote::mousePressEvent(QGraphicsSceneMouseEvent* me)
{
	if(me->button() == Qt::LeftButton && d->cursorShape == Qt::SizeFDiagCursor)
	{
		d->resizing = true;
		d->mousePos = me->scenePos();
	}
	else
    {
        QRectF cr = d->closeButtonRect(this->rect());
        if(cr.contains(me->pos()))
        {
            confirmAndDeleteSelf();
            me->accept();
        }
        else
		    QGraphicsRectItem::mousePressEvent(me);
    }
}

void CVisSystemCanvasNote::mouseMoveEvent(QGraphicsSceneMouseEvent* me)
{
	if(d->resizing)
	{
		QPointF dp = me->scenePos() - d->mousePos;

        prepareGeometryChange();
        QRectF r = noteRect();
		r.setBottomRight( r.bottomRight()+dp );
		if(d->minimumSize.width() > r.width() || d->minimumSize.height() > r.height())
			return;

        setNoteRect(r);
		d->mousePos = me->scenePos();
	}
	else
		QGraphicsRectItem::mouseMoveEvent(me);
}

void CVisSystemCanvasNote::mouseReleaseEvent(QGraphicsSceneMouseEvent* me)
{
	if(d->resizing)
		d->resizing = false;
	else
		QGraphicsRectItem::mouseReleaseEvent(me);
}

void CVisSystemCanvasNote::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* me)
{
    if(!d->textEdit.isNull())
        return;

	// Open a text editor on the canvas and let the user edit the text
    d->textEdit = new QTextEdit;
    d->textEdit->setAcceptRichText(false);
    d->textEdit->setDocument(&d->textDocument);
    d->textEdit->selectAll();
    d->textEdit->viewport()->setAutoFillBackground(false);
    d->textEdit->viewport()->setAttribute(Qt::WA_NoSystemBackground);
    d->textEdit->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    d->textEdit->setFrameShape(QFrame::NoFrame);
    d->textEditProxyWidget = scene()->addWidget(d->textEdit);
    d->textEditProxyWidget->setParentItem(this);

    QRectF rect = d->textRect(this->rect());
    d->textEditProxyWidget->setPos( rect.topLeft() );
    d->textEditProxyWidget->resize( rect.size() );
    d->textEditProxyWidget->show();

    d->textEdit->setFocus(Qt::MouseFocusReason);

    connect(d->textEdit, SIGNAL(contentsChanged()), this, SIGNAL(noteTextChanged()));
}

void CVisSystemCanvasNote::contextMenuEvent(QGraphicsSceneContextMenuEvent* me)
{
    QGraphicsRectItem::contextMenuEvent(me);
}

void CVisSystemCanvasNote::keyPressEvent(QKeyEvent* ke)
{
    QGraphicsRectItem::keyPressEvent(ke);
}

void CVisSystemCanvasNote::hoverEnterEvent(QGraphicsSceneHoverEvent* he)
{
    d->mouseOver = true;
	hoverMoveEvent(he);
    update();
}

void CVisSystemCanvasNote::hoverMoveEvent(QGraphicsSceneHoverEvent* he)
{
	QRectF rr = d->resizeRect(this->rect());
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

void CVisSystemCanvasNote::hoverLeaveEvent(QGraphicsSceneHoverEvent* he)
{
	if(d->cursorShape != Qt::ArrowCursor)
	{
		unsetCursor();
		d->cursorShape = Qt::ArrowCursor;
	}
    d->mouseOver = false;
    update();
	Q_UNUSED(he);
}

void CVisSystemCanvasNote::confirmAndDeleteSelf()
{
    QMessageBox::StandardButton result = QMessageBox::question(d->canvas, "Delete Note Confirmation",
        "Are you sure you want to delete this note?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    if(result == QMessageBox::Yes)
    {
        d->canvas->removeNote(this);
        deleteLater();
    }
}

