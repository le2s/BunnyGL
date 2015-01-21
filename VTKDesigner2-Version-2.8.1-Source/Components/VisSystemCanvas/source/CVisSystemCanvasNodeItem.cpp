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

#include "CVisSystemCanvasNodeItem.h"
#include "CVisSystemCanvas.h"
#include "CVisSystemCanvasConnectionItem.h"
#include "CVisSystemCanvasCmds.h"

#include "IConfigurable.h"
#include "IPropertyEditor.h"
#include "IVisSystemNode.h"
#include "IVisSystemNodeDesc.h"
#include "IVisSystemNodeConnectionPath.h"

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QPainter>
#include <QLinearGradient>
#include <QStyleOptionGraphicsItem>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QStyle>
#include <QStyleOptionButton>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>

struct CVisSystemCanvasNodeItemData
{
    CVisSystemCanvasNodeItemData() : node(0), canvas(0), configurable(0),
        updateConnectionItemsScheduled(false) { }

    IVisSystemNode* node;
    CVisSystemCanvas* canvas;
    IConfigurable* configurable;
    QMap<IVisSystemNodeConnectionPath*, QRectF> pathRectMap;
    QList<CVisSystemCanvasConnectionItem*> connectionList;
    QPointF oldPos, newPos;
    bool updateConnectionItemsScheduled;
};

CVisSystemCanvasNodeItem::CVisSystemCanvasNodeItem(IVisSystemNode* node, CVisSystemCanvas* canvas)
:QGraphicsRectItem(0, canvas->scene())
{
    d = new CVisSystemCanvasNodeItemData;
    d->node = node;
    d->canvas = canvas;
    if(d->node)
    {
        QObject* object = d->node->containerObject();
        d->configurable = qobject_cast<IConfigurable*>(object);
    }

    setZValue(1.0f);
    updateNode();
    QObject* object = d->node->containerObject();
    if(object)
        connect(object, SIGNAL(nodeNameChanged()), this, SLOT(updateNode()));

#if QT_VERSION > 0x040600
    setFlags(ItemIsSelectable|ItemIsMovable|ItemIsFocusable|ItemSendsGeometryChanges|ItemSendsScenePositionChanges);
#else
    setFlags(ItemIsSelectable|ItemIsMovable|ItemIsFocusable);
#endif
}

CVisSystemCanvasNodeItem::~CVisSystemCanvasNodeItem()
{
    delete d;
}

IVisSystemNode* CVisSystemCanvasNodeItem::visSystemNode() const
{
    return d->node;
}

void CVisSystemCanvasNodeItem::registerConnectionItem(CVisSystemCanvasConnectionItem* item)
{
    if(!item)
        return;

    if(item->senderNode() == d->node || item->receiverNode() == d->node)
        d->connectionList.append(item);
}

void CVisSystemCanvasNodeItem::unregisterConnectionItem(CVisSystemCanvasConnectionItem* item)
{
    d->connectionList.removeAll(item);
}

int CVisSystemCanvasNodeItem::connectionItemCount() const
{
    return d->connectionList.count();
}

CVisSystemCanvasConnectionItem* CVisSystemCanvasNodeItem::connectionItem(int index) const
{
    if(index < 0 || index >= d->connectionList.count())
        return 0;
    return d->connectionList[index];
}

QRectF CVisSystemCanvasNodeItem::pathRect(int pathIndex) const
{
    if(!d->node)
        return QRectF();

    IVisSystemNodeConnectionPath* path = d->node->nodeDesc()->connectionPath(pathIndex);
    if(!path)
        return QRectF();

    if(d->pathRectMap.contains(path))
        return d->pathRectMap[path];

    return d->node->pathRect(path);
}

QRectF CVisSystemCanvasNodeItem::pathRect(IVisSystemNodeConnectionPath* path) const
{
    if(!d->node)
        return QRectF();

    if(path)
        d->node->pathRect(path);

    if(d->pathRectMap.contains(path))
        return d->pathRectMap[path];

    return d->node->pathRect(path);
}

int CVisSystemCanvasNodeItem::pathIndexAt(const QPointF pos) const
{
    QList<QRectF> pathRects = d->pathRectMap.values();
    for(int i=0; i<pathRects.count(); i++)
    {
        QRectF rect = pathRects[i];
        if(rect.contains(pos))
        {
            IVisSystemNodeConnectionPath* path = d->pathRectMap.key(rect);
            return path->pathIndex();
        }
    }

    return -1;
}

IVisSystemNodeConnectionPath* CVisSystemCanvasNodeItem::pathAt(const QPointF pos) const
{
    QList<QRectF> pathRects = d->pathRectMap.values();
    for(int i=0; i<pathRects.count(); i++)
    {
        QRectF rect = pathRects[i];
        if(rect.contains(pos))
        {
            IVisSystemNodeConnectionPath* path = d->pathRectMap.key(rect);
            return path;
        }
    }

    return 0;
}

void CVisSystemCanvasNodeItem::updateNode()
{
    if(!d->node)
        return;

    QRectF r = d->node->nodeRect();

#ifndef USE_SYSTEM_STYLE
    r = r.adjusted(0, 0, 5, 5);
#endif

    setRect(r);
    d->pathRectMap.clear();
    for(int i=0; i<d->node->nodeDesc()->connectionPathCount(); i++)
    {
        IVisSystemNodeConnectionPath* path = d->node->nodeDesc()->connectionPath(i);
        if(!path)
            continue;
        QRectF pathRect = d->node->pathRect(path);
        d->pathRectMap[path] = pathRect;
    }
    update();

    updateConnectionItems();
}

void CVisSystemCanvasNodeItem::updateConnectionItems()
{
    for(int i=0; i<d->connectionList.count(); i++)
        d->connectionList[i]->updateConnection();
    d->updateConnectionItemsScheduled = false;
}

#define MEAN_COLOR(c1, c2) QColor( (c1.red()+c2.red())/2, (c1.green()+c2.green())/2, (c1.blue()+c2.blue())/2 );

void CVisSystemCanvasNodeItem::paint(QPainter *p, const QStyleOptionGraphicsItem* opt, QWidget* widget)
{
    Q_UNUSED(widget);

    if(!d->node)
        return;

    QRectF r = this->rect();
    QPen pen = p->pen();

#ifdef USE_SYSTEM_STYLE
    QStyleOptionButton hopt;
    hopt.rect = r.toRect();
    hopt.palette = opt->palette;
    hopt.state = QStyle::State_Active|QStyle::State_Enabled|QStyle::State_Horizontal|QStyle::State_Enabled|QStyle::State_Raised;
    if(isSelected())
        hopt.features = QStyleOptionButton::DefaultButton;
    if(widget)
        widget->style()->drawControl(QStyle::CE_PushButtonBevel, &hopt, p, 0);
    else
        QApplication::style()->drawControl(QStyle::CE_PushButtonBevel, &hopt, p, 0);

    p->setPen(pen);
    p->drawText(r2, Qt::AlignCenter, d->node->nodeName());
#else
    QRectF r2 = r.adjusted(0, 0, -5, -5);

    // Draw the node shadow
	if(opt->levelOfDetail >= 0.75)
    {
        QColor color1 = opt->palette.shadow().color();
        color1.setAlphaF(this->isSelected() ? 0.4 : 0.3);

        int shadowSize = this->isSelected() ? 5 : 3;
        QPainterPath path;
        path.addRoundRect(r2.adjusted(shadowSize,shadowSize,shadowSize,shadowSize), 10, 10);
        p->fillPath(path, color1);
    }

    // Draw the node rectangle.
    double alpha = 0.60;
    if(this->isSelected())
        alpha = 0.95;

    if(this->isSelected())
        p->setPen( QPen(opt->palette.highlight().color(), 2) );
    else
    {
		QColor penColor = opt->palette.highlight().color();
        penColor.setAlphaF(0.85);
        p->setPen( QPen(penColor, 2) );
    }

	if(opt->levelOfDetail >= 0.75)
    {
        QColor topColor = opt->palette.highlight().color();
        QColor midColor = opt->palette.light().color();
        QColor bottomColor = topColor;

        topColor.setAlphaF(alpha);
        midColor.setAlphaF(alpha);
        bottomColor.setAlphaF(alpha);

        QLinearGradient grad(r2.topLeft(), r2.bottomLeft());
        grad.setColorAt(0, topColor);
        grad.setColorAt(0.2, midColor);
        grad.setColorAt(0.8, midColor);
        grad.setColorAt(1, bottomColor);

        QPainterPath path;
        path.addRoundRect(r2.adjusted(1,1,-1,-1), 10, 10);
        p->fillPath(path, grad);
        p->drawPath(path);
    }
	else
	{
		QColor fillColor = opt->palette.light().color();
		fillColor.setAlphaF(alpha);

        QPainterPath path;
        path.addRoundRect(r2.adjusted(1,1,-1,-1), 10, 10);
		p->fillPath(path, fillColor);
		p->drawPath(path);
	}

    // Draw the node icon
	QRectF textRect;
	if(opt->levelOfDetail >= 0.75)
	{
		QPixmap nodePm = d->node->nodeDesc()->nodeIcon().pixmap(30, 30);
		QRectF iconRect( r2.left()+10, r2.top()+7, nodePm.width(), nodePm.height() );
		iconRect.moveTop( r2.center().y() - nodePm.height()/2 );
		p->drawPixmap( iconRect, nodePm, QRectF(0,0,nodePm.width(),nodePm.height()) );
		textRect = QRectF ( iconRect.right()+2, r2.top()+10, r2.width(), r2.height()-20 );
		textRect.setRight( r2.right() );
	}
	else
		textRect = r2;

    // Draw the node text
    p->setPen(pen);

	if(opt->levelOfDetail >= 0.75)
	{
		// First draw the node name
		textRect.setBottom( r2.center().y() );
		p->drawText(textRect, Qt::AlignCenter, d->node->nodeName());
	    
		// Now draw the node class name in a smaller font
		QFont font = p->font();
		QFont newFont = font;
		newFont.setPointSize( font.pointSize()-1 );
		p->setFont( newFont );
		textRect.moveTop( r2.center().y()+1 );
		p->drawText(textRect, Qt::AlignCenter, d->node->nodeDesc()->nodeClassName());
		p->setFont(font);
	}
	else
		p->drawText(textRect, Qt::AlignCenter, d->node->nodeDesc()->nodeClassName());

#endif

	if(opt->levelOfDetail >= 0.75)
		d->node->paintNode(p, r, *opt);

    QBrush brush = opt->palette.mid();
    QColor color = brush.color();
    color.setAlphaF(0.75f);
    brush.setColor(color);
    color = opt->palette.shadow().color();
    color.setAlphaF(0.75f);

    QMap<IVisSystemNodeConnectionPath*, QRectF>::iterator it = d->pathRectMap.begin();
    QMap<IVisSystemNodeConnectionPath*, QRectF>::iterator end = d->pathRectMap.end();
    while(it != end)
    {
        p->setPen(color);
        p->setBrush(brush);
        p->drawRect(it.value());
        p->setPen(pen);
        d->node->paintConnectionPath(it.key(), p, it.value(), *opt);
        ++it;
    }
}

QVariant CVisSystemCanvasNodeItem::itemChange(GraphicsItemChange change, const QVariant & value)
{
    if(change == ItemPositionHasChanged)
    {
        if(!d->updateConnectionItemsScheduled)
        {
            d->updateConnectionItemsScheduled = true;
            QMetaObject::invokeMethod(this, "updateConnectionItems", Qt::QueuedConnection);
        }
    }
    else if(change == ItemSelectedHasChanged)
    {
        emit itemSelected(value.toBool());
        if(value.toBool())
            setFocus();
        else
            clearFocus();

        if(value.toBool())
            emit d->canvas->nodeSelected(d->node);
        else
            emit d->canvas->nodeUnselected(d->node);
    }
    return value;
}

void CVisSystemCanvasNodeItem::mousePressEvent(QGraphicsSceneMouseEvent* me)
{
    d->oldPos = this->pos();
    QGraphicsRectItem::mousePressEvent(me);
}

void CVisSystemCanvasNodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* me)
{
    QGraphicsRectItem::mouseReleaseEvent(me);
    if(me->widget() == d->canvas || me->widget() == d->canvas->viewport())
        emit d->canvas->nodeClickedEvent(d->node, me->scenePos().toPoint(), me->button(), me->modifiers());

    d->newPos = this->pos();
    if(d->oldPos != d->newPos)
    {
        CNodeMoveCmd* cmd = new CNodeMoveCmd(this, d->oldPos, d->newPos);
        d->canvas->undoRedoStack()->push(cmd);
        d->oldPos = QPointF();
        d->newPos = QPointF();
   }
}

void CVisSystemCanvasNodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* me)
{
    QGraphicsRectItem::mouseDoubleClickEvent(me);
    if(me->widget() == d->canvas || me->widget() == d->canvas->viewport())
    {
        bool success = false;
        if(d->configurable)
            success = d->configurable->showDefaultConfigurationDialog();
        
        if(!success)
        {
            if( d->canvas->isDoubleClickSignalConnected() )
            {
                emit d->canvas->nodeDoubleClickedEvent(d->node, me->scenePos().toPoint(), me->button(), me->modifiers());
                return;
            }

            IPropertyEditor* propEditor = d->canvas->getNewPropertyEditorInstance();
            if(!propEditor)
                return;

            QDialog dialog( d->canvas );
            QWidget* propEditorWidget = (QWidget*)(propEditor->containerObject());
            QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
            QVBoxLayout* layout = new QVBoxLayout(&dialog);
            
            layout->addWidget(propEditorWidget);
            layout->addWidget(buttonBox);

            propEditor->setObject(d->node->containerObject());

            dialog.setWindowTitle( QString("%1 properties").arg(d->node->nodeName()) );

            connect(buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
            dialog.resize(320, 400);
            dialog.exec();
        }
    }
}

void CVisSystemCanvasNodeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* me)
{
    QGraphicsRectItem::contextMenuEvent(me);
    if(me->widget() == d->canvas || me->widget() == d->canvas->viewport())
    {
        // Show the context sensitive menu
        QMenu menu;

        // First show configurable options.
        QList<QAction*> configActions;
        if(d->configurable)
        {
            QStringList configOptions = d->configurable->configurationOptions();
            for(int i=0; i<configOptions.count(); i++)
            {
                QString option = configOptions[i];
                QAction* action = new QAction(option, &menu);
                menu.addAction(action);
                configActions.append(action);
            }
            if(configOptions.count())
                menu.addSeparator();
        }

        // Now show command actions.
        QList<QAction*> commandActions;
        QStringList commandNames = d->node->nodeDesc()->commandNames();
        for(int i=0; i<commandNames.count(); i++)
        {
            QString command = commandNames[i];
            QAction* action = new QAction(command, &menu);
            menu.addAction(action);
            commandActions.append(action);
        }
        if(commandNames.count())
            menu.addSeparator();

        // Now show default options
        QAction* disconnectNode = new QAction("Disconnect Node", &menu);
        disconnectNode->setEnabled( d->canvas->hasConnections(d->node) );
        menu.addAction(disconnectNode);
        QAction* delNode = new QAction("Remove node", &menu);
        menu.addAction(delNode);

        // Allow others to insert their items into the menu.
        emit d->canvas->nodeContextMenuEvent(d->node, &menu, me->modifiers());

        QAction* result = menu.exec(QCursor::pos());
        if(result == delNode)
        {
            IVisNetwork* visNetwork = d->canvas->visNetwork();
            if(visNetwork)
                visNetwork->removeNode(d->node);
        }
        else if(result == disconnectNode)
        {
            IVisNetwork* visNetwork = d->canvas->visNetwork();
            if(visNetwork)
                visNetwork->disconnectNode(d->node);
        }
        else if(configActions.contains(result))
        {
            QString configOption = result->text();
            d->configurable->showConfigurationDialog(configOption);
        }
        else if(commandActions.contains(result))
        {
            QString command = result->text();
            QString result;
            d->node->executeCommand(command, result);
            qDebug(qPrintable(result));
        }
        me->accept();
    }    
}

void CVisSystemCanvasNodeItem::keyPressEvent(QKeyEvent* ke)
{
    if(ke->key() == Qt::Key_F2)
    {
        QRectF nodeRect = mapToScene(boundingRect()).boundingRect();
        QRect nodeRect2 = d->canvas->mapFromScene(nodeRect).boundingRect();
        nodeRect2 = nodeRect2.adjusted(10, 10, -10, -10);
        CNodeNameLineEdit* nameLineEdit = new CNodeNameLineEdit(d->canvas->viewport(), d->node, this);
        nameLineEdit->setGeometry(nodeRect2);
        nameLineEdit->show();
        nameLineEdit->setFocus();
        ke->accept();
        return;
    }
    
    if(ke->key() == Qt::Key_Up && ke->modifiers()&Qt::AltModifier)
    {
        d->canvas->selectPrevious(this);
        return;
    }

    if(ke->key() == Qt::Key_Down && ke->modifiers()&Qt::AltModifier)
    {
        d->canvas->selectNext(this);
        return;
    }

    QGraphicsRectItem::keyPressEvent(ke);

    emit d->canvas->nodeKeyEvent(d->node, ke->key(), ke->text(), ke->modifiers());
}

QString CVisSystemCanvasNodeItem::toolTipAt(QPointF itemPos, QRectF& rect)
{
    IVisSystemNodeConnectionPath* path = pathAt(itemPos);
    if(!path)
    {
        IVisSystemNode* node = d->node;
        rect = node->pathRect(path);
        return QString("%1: %2").arg(node->nodeDesc()->nodeClassName()).arg(node->nodeDesc()->nodeClassDescription());
    }

    rect = boundingRect();
    return QString("%1 (%2)").arg(path->pathName()).arg(path->pathIndex());
}

int CVisSystemCanvasNodeItem::type() const
{
    return CVisSystemCanvasNodeItem_Type;
}

// CNodeNameLineEdit

CNodeNameLineEdit::CNodeNameLineEdit(QWidget* parent, IVisSystemNode* node, CVisSystemCanvasNodeItem* nodeItem)
:QLineEdit(parent), mNode(node), mNodeItem(nodeItem), mEscapePressed(false)
{
    connect(this, SIGNAL(editingFinished()), this, SLOT(close()));
    setText(node->nodeName());
    mOldName = node->nodeName();
    setFocus();
    grabKeyboard();
    setFrame(false);
    setAlignment(Qt::AlignCenter);
    setAutoFillBackground(false);
    selectAll();
    setMinimumHeight(sizeHint().height());
}

CNodeNameLineEdit::~CNodeNameLineEdit()
{

}

void CNodeNameLineEdit::keyPressEvent(QKeyEvent* ke)
{
    mEscapePressed = false;
    if(ke->key() == Qt::Key_Escape)
        mEscapePressed = true;
    if(mEscapePressed)
        close();
    else
        QLineEdit::keyPressEvent(ke);
}

void CNodeNameLineEdit::closeEvent(QCloseEvent* ce)
{
    releaseKeyboard();
    mNodeItem->setFocus();
    parentWidget()->setFocus();
    if(!mEscapePressed)
        mNode->setNodeName(this->text());
    else
        mNode->setNodeName(mOldName);

    CNodeNameChangeCmd* cmd = new CNodeNameChangeCmd(mNode, mOldName, this->text());
    mNodeItem->d->canvas->undoRedoStack()->push(cmd);

    deleteLater();
    Q_UNUSED(ce);
}


