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

#ifndef C_VIS_SYSTEM_CANVAS_CONNECTION_ITEM_H
#define C_VIS_SYSTEM_CANVAS_CONNECTION_ITEM_H

#include <QObject>
#include <QGraphicsLineItem>

class IVisSystemNode;
class IVisNetworkConnection;
class IVisSystemNodeIOData;
class CVisSystemCanvas;
class CVisSystemCanvasNodeItem;

struct CVisSystemCanvasConnectionItemData;
class CVisSystemCanvasConnectionItem : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    CVisSystemCanvasConnectionItem(IVisNetworkConnection* con, CVisSystemCanvas* canvas);
    ~CVisSystemCanvasConnectionItem();

    IVisNetworkConnection* connection();

    IVisSystemNode* senderNode();
    int senderPathIndex();
    IVisSystemNode* receiverNode();
    int receiverPathIndex();
    IVisSystemNodeIOData* connectionData();

    CVisSystemCanvasNodeItem* senderNodeItem();
    CVisSystemCanvasNodeItem* receiverNodeItem();

    // QGraphicsItem implementation
    QPainterPath shape() const;
    QRectF boundingRect() const;
    void paint(QPainter *paint, const QStyleOptionGraphicsItem* opt, QWidget *widget);

signals:
    void itemSelected(bool val);

protected slots:
    void updateConnection();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
    void mousePressEvent(QGraphicsSceneMouseEvent* me);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* me);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* me);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* me);
    void keyPressEvent(QKeyEvent* ke);

private:
    friend class CVisSystemCanvasNodeItem;
    friend class CVisSystemCanvas;
    CVisSystemCanvasConnectionItemData* d;
};

#endif


