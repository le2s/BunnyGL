/****************************************************************************
**
** Copyright (C) UNO and VCreate Logic
**
** Use of this file is limited according to the terms specified by
** UNO and VCreate Logic.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "InfoVisOutputViewer.h"
#include "IVisSystemNode.h"
#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"

#include <GCF/AbstractComponent>

#include <QStackedWidget>
#include <QListWidget>
#include <QHBoxLayout>

#include "QAbstractItemViewVisNode.h"

struct InfoVisOutputViewerData
{
    QListWidget* nodeListWidget;
    QStackedWidget* widgetStack;

    QList<IVisSystemNode*> nodeList;
    QList<QObject*> widgetObjectList;
};

InfoVisOutputViewer::InfoVisOutputViewer(QWidget* parent)
: QWidget(parent)
{
    d = new InfoVisOutputViewerData;

    d->nodeListWidget = new QListWidget(this);
    d->nodeListWidget->setMaximumWidth(200);

    d->widgetStack = new QStackedWidget(this);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(d->nodeListWidget);
    layout->addWidget(d->widgetStack);
    layout->setMargin(0);

    connect(d->nodeListWidget, SIGNAL(currentRowChanged(int)),
            this, SLOT(slotCurrentRowChanged(int)));
    
    IVisNetwork* network = GCF::findObject<IVisNetwork*>();
    if(network)
    {
        connect(network->containerObject(),
                SIGNAL(nodeAdded(IVisSystemNode*)),
                this,
                SLOT(slotNodeAdded(IVisSystemNode*)));
    }

    IVisNetworkCanvas* canvas = GCF::findObject<IVisNetworkCanvas*>();
    if(canvas)
    {
        connect(canvas->containerObject(),
                SIGNAL(nodeSelected(IVisSystemNode*)),
                this, 
                SLOT(slotNodeSelected(IVisSystemNode*)));
    }
}

InfoVisOutputViewer::~InfoVisOutputViewer()
{
    QList<IVisSystemNode*> copy = d->nodeList;
    Q_FOREACH(IVisSystemNode* node, copy)
        this->removeViewNode(node);

    delete d;
}

void InfoVisOutputViewer::addViewNode(IVisSystemNode* node, QWidget* widget)
{
    if(!node || !widget || d->nodeList.contains(node))
        return;

    d->nodeList.append(node);
    d->nodeListWidget->addItem(node->nodeName());
    connect(node->containerObject(), SIGNAL(nodeNameChanged()), 
            this, SLOT(slotNodeNameChanged()));

    d->widgetStack->addWidget(widget);
    d->widgetObjectList.append( (QObject*)widget );
    connect(widget, SIGNAL(destroyed(QObject*)), 
        this, SLOT(slotNodeWidgetDestroyed(QObject*)));

    d->nodeListWidget->setCurrentRow(d->nodeListWidget->count());
}

void InfoVisOutputViewer::removeViewNode(IVisSystemNode* node)
{
    if(!node || !d->nodeList.contains(node))
        return;

    int index = d->nodeList.indexOf(node);
    if(index < 0)
        return;

    QWidget* widget = d->widgetStack->widget(index);
    disconnect(widget, 0, this, 0);

    disconnect(node->containerObject(), 0, this, 0);

    d->widgetStack->removeWidget(widget);
    d->widgetObjectList.removeAt(index);
    delete d->nodeListWidget->item(index);
    d->nodeList.removeAt(index);
}

int InfoVisOutputViewer::viewNodeCount() const
{
    return d->nodeList.count();
}

IVisSystemNode* InfoVisOutputViewer::viewNodeAt(int index) const
{
    if(index < 0 || index >= d->nodeList.count())
        return 0;

    return d->nodeList.at(index);
}

QWidget* InfoVisOutputViewer::viewWidgetAt(int index) const
{
    return d->widgetStack->widget(index);
}

void InfoVisOutputViewer::slotCurrentRowChanged(int index)
{
    d->widgetStack->setCurrentIndex(index);
}

void InfoVisOutputViewer::slotNodeAdded(IVisSystemNode* node)
{
    // Check to see if the node is a QAbstractItemViewVisNode
    QAbstractItemViewVisNode* itemViewNode 
        = qobject_cast<QAbstractItemViewVisNode*>(node->containerObject());
    if(itemViewNode)
    {
        this->addViewNode( node, itemViewNode->view() );
        return;
    }

    // TODO: Check for other node types
}

void InfoVisOutputViewer::slotNodeWidgetDestroyed(QObject* obj)
{
    int index = d->widgetObjectList.indexOf(obj);
    if(index < 0)
        return;

    IVisSystemNode* node = d->nodeList.at(index);
    this->removeViewNode(node);
}

void InfoVisOutputViewer::slotNodeSelected(IVisSystemNode* node)
{
    int index = d->nodeList.indexOf(node);
    if(index < 0)
        return;

    d->nodeListWidget->setCurrentRow(index);
}

void InfoVisOutputViewer::slotNodeNameChanged()
{
    IVisSystemNode* node = qobject_cast<IVisSystemNode*>(sender());
    if(!node || !d->nodeList.contains(node))
        return;

    int index = d->nodeList.indexOf(node);
    d->nodeListWidget->item(index)->setText( node->nodeName() );
}
