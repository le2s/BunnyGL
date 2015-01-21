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

#include "CVisSystemCanvasConnectionItem.h"
#include "CVisSystemCanvas.h"
#include "CVisSystemCanvasNodeItem.h"

#include "IVisNetwork.h"
#include "IConfigurable.h"
#include "IVisSystemNode.h"
#include "IPropertyEditor.h"
#include "IVisSystemNodeDesc.h"
#include "IVisSystemNodeIOData.h"
#include "IVisNetworkConnection.h"
#include "IVisSystemNodeConnectionPath.h"

#include <QMap>
#include <QMenu>
#include <QAction>
#include <QColor>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QPainterPathStroker>

#include <stdlib.h>

struct CVisSystemCanvasConnectionItemData
{
    CVisSystemCanvasConnectionItemData() : connection(0),
        senderNodeItem(0), receiverNodeItem(0),
        connectionConfigurable(0), connectionContainer(0) { }

    CVisSystemCanvas* canvas;
    IVisNetworkConnection* connection;
    CVisSystemCanvasNodeItem* senderNodeItem;
    CVisSystemCanvasNodeItem* receiverNodeItem;
    IConfigurable* connectionConfigurable;
    IContainer* connectionContainer;
	QPainterPath itemShape;
	QPainterPath drawShape;
	QRectF boundingRect;

    static QColor connectionItemColor(IVisNetworkConnection* connection);
};

CVisSystemCanvasConnectionItem::CVisSystemCanvasConnectionItem(IVisNetworkConnection* con, CVisSystemCanvas* canvas)
:QGraphicsItem(0, canvas->scene())
{
    d = new CVisSystemCanvasConnectionItemData;
    d->canvas = canvas;
    d->connection = con;

    IVisSystemNodeIOData* ioData = d->connection->connectionData();
    bool success = ioData && ioData->queryInterface("IContainer", (void**)(&d->connectionContainer));
    if( success )
    {
        QObject* conObj = d->connectionContainer->containerObject();
        d->connectionConfigurable = qobject_cast<IConfigurable*>(conObj);
    }

    setZValue(2.0f);
    setFlags(ItemIsSelectable|ItemIsFocusable);
    updateConnection();
}

CVisSystemCanvasConnectionItem::~CVisSystemCanvasConnectionItem()
{
    if(d->senderNodeItem)
        d->senderNodeItem->unregisterConnectionItem(this);
    if(d->receiverNodeItem)
        d->receiverNodeItem->unregisterConnectionItem(this);
    delete d;
}

IVisNetworkConnection* CVisSystemCanvasConnectionItem::connection()
{
    return d->connection;
}

IVisSystemNode* CVisSystemCanvasConnectionItem::senderNode()
{
    if(d->connection)
        return d->connection->senderNode();
    return 0;
}

int CVisSystemCanvasConnectionItem::senderPathIndex()
{
    if(d->connection)
        return d->connection->senderPathIndex();
    return -1;
}

IVisSystemNode* CVisSystemCanvasConnectionItem::receiverNode()
{
    if(d->connection)
        return d->connection->receiverNode();
    return 0;
}

int CVisSystemCanvasConnectionItem::receiverPathIndex()
{
    if(d->connection)
        return d->connection->receiverPathIndex();
    return -1;
}

IVisSystemNodeIOData* CVisSystemCanvasConnectionItem::connectionData()
{
    if(d->connection)
        return d->connection->connectionData();
    return 0;
}

CVisSystemCanvasNodeItem* CVisSystemCanvasConnectionItem::senderNodeItem()
{
    return d->senderNodeItem;
}

CVisSystemCanvasNodeItem* CVisSystemCanvasConnectionItem::receiverNodeItem()
{
    return d->receiverNodeItem;
}

QPainterPath CVisSystemCanvasConnectionItem::shape() const
{
    return d->itemShape;
}

QRectF CVisSystemCanvasConnectionItem::boundingRect() const
{
	return d->boundingRect;
}

void CVisSystemCanvasConnectionItem::paint(QPainter *paint, const QStyleOptionGraphicsItem* opt, QWidget *widget)
{
    Q_UNUSED(opt);
    Q_UNUSED(widget);

    QPen pen(d->connectionItemColor(d->connection), 2);
    QPen oldPen = paint->pen();
    if(isSelected())
        pen.setWidthF(pen.widthF()*2.0f);
    paint->setPen(pen);
	paint->drawPath(d->drawShape);
    paint->setPen(oldPen);
}

void CVisSystemCanvasConnectionItem::updateConnection()
{
    if(!d->connection)
        return;

    if(!d->senderNodeItem)
    {
        d->senderNodeItem = d->canvas->nodeItem(d->connection->senderNode());
        d->senderNodeItem->registerConnectionItem(this);
    }
    if(!d->receiverNodeItem)
    {
        d->receiverNodeItem = d->canvas->nodeItem(d->connection->receiverNode());
        d->receiverNodeItem->registerConnectionItem(this);
    }

    if(!d->senderNodeItem || !d->receiverNodeItem)
        return;

    QRectF senderRect = d->senderNodeItem->pathRect(senderPathIndex());
    QRectF receiverRect = d->receiverNodeItem->pathRect(receiverPathIndex());
	QPointF p1, p2;
    senderRect = d->senderNodeItem->mapToScene(senderRect).boundingRect();
    receiverRect = d->receiverNodeItem->mapToScene(receiverRect).boundingRect();
	p1 = senderRect.center();
	p2 = receiverRect.center();

    QPainterPath path;
    QPointF inputCP = p1 + QPointF(0.0f, 25.0f);
    QPointF outputCP = p2 - QPointF(0.0f, 25.0f);
    if(inputCP.x() - outputCP.x() < 25.0f)
        inputCP.setX(inputCP.x()+25.0f);
    path.moveTo(p1);
    path.cubicTo(inputCP, outputCP, p2);

	prepareGeometryChange();
	d->drawShape = path;

	QPainterPathStroker stroker;
	stroker.setWidth(10);
	stroker.setCapStyle(Qt::FlatCap);
	d->itemShape = stroker.createStroke(path);
	d->boundingRect = d->itemShape.boundingRect();
}

QVariant CVisSystemCanvasConnectionItem::itemChange(GraphicsItemChange change, const QVariant & value)
{
    if(change == ItemSelectedHasChanged)
    {
        emit itemSelected(value.toBool());

        if(value.toBool())
            d->canvas->connectionSelected(d->connection);
        else
            d->canvas->connectionUnselected(d->connection);

        if(value.toBool() && d->connectionContainer)
            d->canvas->propertyEditor()->setObject( d->connectionContainer->containerObject() );
    }
    return value;
}


void CVisSystemCanvasConnectionItem::mousePressEvent(QGraphicsSceneMouseEvent* me)
{
    QGraphicsItem::mousePressEvent(me);
}

void CVisSystemCanvasConnectionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* me)
{
    QGraphicsItem::mouseReleaseEvent(me);
    if(me->widget() == d->canvas || me->widget() == d->canvas->viewport())
        d->canvas->connectionClickedEvent(d->connection, me->scenePos().toPoint(), me->button(), me->modifiers());
}

void CVisSystemCanvasConnectionItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* me)
{
    QGraphicsItem::mouseDoubleClickEvent(me);

    if(me->widget() == d->canvas || me->widget() == d->canvas->viewport())
        emit d->canvas->connectionDoubleClickedEvent(d->connection, me->scenePos().toPoint(), me->button(), me->modifiers());

    if( d->connectionConfigurable )
        d->connectionConfigurable->showDefaultConfigurationDialog();
}

void CVisSystemCanvasConnectionItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* me)
{
    QGraphicsItem::contextMenuEvent(me);
    if(me->widget() == d->canvas || me->widget() == d->canvas->viewport())
    {
        QMenu menu;

        QList<QAction*> configActions;
        if( d->connectionConfigurable )
        {
            QStringList options = d->connectionConfigurable->configurationOptions();
            for(int i=0; i<options.count(); i++)
                configActions << menu.addAction( options[i] );
            if(options.count())
                menu.addSeparator();
        }

        // Remove connection option
        QAction* delCon = new QAction("Remove Connection", &menu);
        menu.addAction(delCon);

        // Allow others to insert their items into the menu.
        d->canvas->connectionContextMenuEvent(d->connection, &menu, me->modifiers());

        QAction* result = menu.exec(QCursor::pos());
        if(result == delCon)
        {
            IVisNetwork* visNetwork = d->canvas->visNetwork();
            if(visNetwork)
                visNetwork->disconnectNode(d->connection);
        }
        else if( d->connectionConfigurable && configActions.contains(result) )
            d->connectionConfigurable->showConfigurationDialog(result->text());
        me->accept();
    }
}

void CVisSystemCanvasConnectionItem::keyPressEvent(QKeyEvent* ke)
{
    QGraphicsItem::keyPressEvent(ke);
    emit d->canvas->connectionKeyEvent(d->connection, ke->key(), ke->text(), ke->modifiers());
}

double randomNumber(double min, double max)
{
    double rand_max = double(RAND_MAX);
    double r = double( rand() );
    double value = (max - min) / (rand_max) * r + min;
    return value;
}

QColor CVisSystemCanvasConnectionItemData::connectionItemColor(IVisNetworkConnection* connection)
{
    static QMap<QString, QColor> pathTypeColorMap;

    if(!connection)
        return Qt::black;

    int pathIndex = connection->senderPathIndex();
    IVisSystemNodeConnectionPath* path = connection->senderNode()->nodeDesc()->connectionPath(pathIndex);
    QString pathDataType;
    if(path)
        pathDataType = path->pathDataType();

    if(pathDataType.isEmpty())
        return Qt::black;

    if(pathTypeColorMap.contains(pathDataType))
        return pathTypeColorMap[pathDataType];

    QColor c;
    double r = randomNumber(0.0f, 1.0f);
    double g = randomNumber(0.0f, 1.0f);
    double b = randomNumber(0.0f, 1.0f);
    c.setRgbF(r, g, b);

    pathTypeColorMap[pathDataType] = c;
    return c;
}


