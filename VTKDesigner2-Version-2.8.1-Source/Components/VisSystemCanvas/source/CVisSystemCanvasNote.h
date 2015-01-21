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

#ifndef C_VIS_SYSTEM_CANVAS_NOTE_H
#define C_VIS_SYSTEM_CANVAS_NOTE_H

#include <QObject>
#include <QGraphicsRectItem>
#include "IVisNetworkCanvasNote.h"

#define CVisSystemCanvasNote_Type (QGraphicsRectItem::UserType+11)

class QSvgRenderer;
class CVisSystemCanvas;
struct CVisSystemCanvasNoteData;
class CVisSystemCanvasNote : public QObject, public QGraphicsRectItem,
                             virtual public IVisNetworkCanvasNote
{
    Q_OBJECT
    Q_INTERFACES(IVisNetworkCanvasNote)

public:
    CVisSystemCanvasNote(CVisSystemCanvas* parent=0);
    ~CVisSystemCanvasNote();

    // IContainer implementation
    QObject* containerObject();

    // IVisNetworkCanvasNote implementation
    QGraphicsItem* graphicsItem();
    void setNoteRect(const QRectF& rect);
    QRectF noteRect() const;
    void setNoteSvgRenderer(QSvgRenderer* ren);
    QSvgRenderer* noteSvgRenderer() const;
    void setNoteSvgElementId(const QString& id);
    QString noteSvgElementId() const;
    void setNoteText(const QString& text);
    QString noteText() const;
    void setNoteTextColor(const QColor& noteColor);
    QColor noteTextColor() const;
    void setNoteFont(const QFont& font);
    QFont noteFont() const;

signals:
    // IVisNetworkCanvasNote signals
    void noteRectChanged();
    void noteTextChanged();
    void noteTextEdited();

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
    int type () const { return CVisSystemCanvasNote_Type; }
    void confirmAndDeleteSelf();

private:
    CVisSystemCanvasNoteData* d;
};

#endif
