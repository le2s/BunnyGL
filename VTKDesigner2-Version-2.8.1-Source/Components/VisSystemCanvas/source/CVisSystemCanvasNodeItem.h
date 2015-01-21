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

#ifndef C_VIS_SYSTEM_CANVAS_NODE_ITEM_H
#define C_VIS_SYSTEM_CANVAS_NODE_ITEM_H

#include <QObject>
#include <QLineEdit>
#include <QGraphicsRectItem>

class IVisSystemNode;
class CVisSystemCanvas;
class IVisSystemNodeConnectionPath;
class CVisSystemCanvasConnectionItem;

#define CVisSystemCanvasNodeItem_Type (QGraphicsItem::UserType+3)

struct CVisSystemCanvasNodeItemData;
class CVisSystemCanvasNodeItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    CVisSystemCanvasNodeItem(IVisSystemNode* node, CVisSystemCanvas* canvas);
    ~CVisSystemCanvasNodeItem();

    IVisSystemNode* visSystemNode() const;

    void registerConnectionItem(CVisSystemCanvasConnectionItem* item);
    void unregisterConnectionItem(CVisSystemCanvasConnectionItem* item);
    int connectionItemCount() const;
    CVisSystemCanvasConnectionItem* connectionItem(int index) const;

    QRectF pathRect(int pathIndex) const;
    QRectF pathRect(IVisSystemNodeConnectionPath* path) const;
    int pathIndexAt(const QPointF pos) const;
    IVisSystemNodeConnectionPath* pathAt(const QPointF pos) const;

signals:
    void itemSelected(bool val);

protected slots:
    void updateNode();
    void updateConnectionItems();

protected:
    void paint(QPainter *p, const QStyleOptionGraphicsItem* opt, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
    void mousePressEvent(QGraphicsSceneMouseEvent* me);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* me);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* me);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* me);
    void keyPressEvent(QKeyEvent* ke);
    QString toolTipAt(QPointF itemPos, QRectF& rect);
    int type () const;

private:
    friend class CVisSystemCanvasConnectionItem;
    friend class CVisSystemCanvas;
    friend class CNodeNameLineEdit;
    CVisSystemCanvasNodeItemData* d;
};

class CNodeNameLineEdit : public QLineEdit
{
public:
    CNodeNameLineEdit(QWidget* parent, IVisSystemNode* node, CVisSystemCanvasNodeItem* nodeItem);
    ~CNodeNameLineEdit();

protected:
    void keyPressEvent(QKeyEvent* ke);
    void closeEvent(QCloseEvent* ce);

private:
    IVisSystemNode* mNode;
    CVisSystemCanvasNodeItem* mNodeItem;
    bool mEscapePressed;
    QString mOldName;
};

#endif

