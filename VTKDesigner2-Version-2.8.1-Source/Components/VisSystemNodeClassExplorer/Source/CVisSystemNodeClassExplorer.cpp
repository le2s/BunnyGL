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

#include "CVisSystemNodeClassExplorer.h"
#include "CVisSystemNodeClassExplorerTreeWidget.h"

#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisSystemNodeDesc.h"

#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QLayoutItem>
#include <QTreeWidgetItemIterator>
#include <QHeaderView>
#include <QMetaObject>
#include <QAbstractItemModel>

struct CVisSystemNodeClassExplorerData
{
    CVisSystemNodeClassExplorerData() : visSystemNodeFactoryReg(0), treeWidget(0) { }

    IVisSystemNodeFactoryRegistry* visSystemNodeFactoryReg;
    QTreeWidget* treeWidget;
    QHBoxLayout* layout;
    int itemSize;
    IVisSystemNodeFactoryRegistryExplorer::ExploreMode exploreMode;

    QTreeWidgetItem* fetchParentItem(IVisSystemNodeDesc* desc);
	void filterItems(QTreeWidgetItem* parent, const QList<QTreeWidgetItem*>& items, bool showAll=false);
};

CVisSystemNodeClassExplorer::CVisSystemNodeClassExplorer(QWidget* parent)
:QWidget(parent)
{
    d = new CVisSystemNodeClassExplorerData;
    d->layout = new QHBoxLayout(this);
    d->layout->setMargin(0);
    d->layout->setSpacing(0);
    d->itemSize = 20;
    d->exploreMode = IVisSystemNodeFactoryRegistryExplorer::CategoryMode;

    setTreeWidget(new CVisSystemNodeClassExplorerTreeWidget(this));
}

CVisSystemNodeClassExplorer::~CVisSystemNodeClassExplorer()
{
    delete d;
}

void CVisSystemNodeClassExplorer::setTreeWidget(QTreeWidget* treeWidget)
{
    if(d->treeWidget == treeWidget)
        return;

    int twLocation = -1;

    if(d->treeWidget)
    {
        // find out the location where the old tree widget was
        // placed in the hbox layout.
        for(int i=0; i<d->layout->count(); i++)
        {
            QLayoutItem* item = d->layout->itemAt(i);
            if(item->widget() == treeWidget)
            {
                twLocation = i;
                break;
            }
        }

        d->layout->removeWidget(treeWidget);
        delete d->treeWidget;
        d->treeWidget = 0;
    }

    d->treeWidget = treeWidget;
    if(!d->treeWidget)
        d->treeWidget = new QTreeWidget(this);

    d->layout->insertWidget(twLocation, d->treeWidget);
    d->treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    d->treeWidget->setColumnCount(1);
    d->treeWidget->header()->hide();
    d->treeWidget->setRootIsDecorated(true);
    d->treeWidget->setDragEnabled(true);
}

QTreeWidget* CVisSystemNodeClassExplorer::treeWidget() const
{
    return d->treeWidget;
}

void CVisSystemNodeClassExplorer::setItemSize(int size)
{
    d->itemSize = size;
    if(d->itemSize < 10)
        d->itemSize = 10;

    QTreeWidgetItemIterator it(d->treeWidget);
    while( *it )
    {
        // if an item was not filtered out, then set its size to the
        // new item size.
        if((*it)->sizeHint(0) != QSize(0, 0))
            (*it)->setSizeHint(0, QSize(100, d->itemSize));
        ++it;
    }
}

int CVisSystemNodeClassExplorer::itemSize() const
{
    return d->itemSize;
}

void CVisSystemNodeClassExplorer::setExploreModeToCategoryMode()
{
    setExploreMode(IVisSystemNodeFactoryRegistryExplorer::CategoryMode);
}

void CVisSystemNodeClassExplorer::setExploreModeToClassHierarchyMode()
{
    setExploreMode(IVisSystemNodeFactoryRegistryExplorer::ClassHierarchyMode);
}

void CVisSystemNodeClassExplorer::setExploreModeToContextMode()
{
    setExploreMode(IVisSystemNodeFactoryRegistryExplorer::ContextMode);
}

QObject* CVisSystemNodeClassExplorer::containerObject()
{
    return this;
}

void CVisSystemNodeClassExplorer::updateClassList()
{
    // Force the change of explore mode so that the class list appears
    // to get updated.
    d->exploreMode = ContextMode;
    setExploreMode(CategoryMode);
}

void CVisSystemNodeClassExplorer::selectClassName(QString className)
{
    QList<QTreeWidgetItem*> items = d->treeWidget->findItems(className, Qt::MatchContains);
    if(items.count())
    {
        items.first()->setSelected(true);
        d->treeWidget->scrollToItem(items.first(), QAbstractItemView::EnsureVisible);
    }
}

QString CVisSystemNodeClassExplorer::selectedClassName()
{
    QList<QTreeWidgetItem*> items = d->treeWidget->selectedItems();
    if(items.count())
        return items.first()->text(0);
    return QString();
}

void CVisSystemNodeClassExplorer::setClassNameFilter(QString filter)
{
    QList<QTreeWidgetItem*> items = d->treeWidget->findItems(filter, Qt::MatchContains|Qt::MatchRecursive);

    /*
    if(items.count())
    {
        d->treeWidget->scrollToItem(items.first());
        d->treeWidget->setCurrentItem(items.first());
    }
    */

	for(int i=0; i<d->treeWidget->topLevelItemCount(); i++)
	{
		QTreeWidgetItem* item = d->treeWidget->topLevelItem(i);
		d->filterItems(item, items, !filter.length());

		if(items.count())
			item->setExpanded(true);
	}
}

void CVisSystemNodeClassExplorer::setExploreMode(IVisSystemNodeFactoryRegistryExplorer::ExploreMode mode)
{
    if(d->exploreMode == mode)
        return;

    d->exploreMode = mode;
    this->refresh();
}

IVisSystemNodeFactoryRegistryExplorer::ExploreMode CVisSystemNodeClassExplorer::exploreMode()
{
    return d->exploreMode;
}

void CVisSystemNodeClassExplorer::setNodeFactoryRegistry(IVisSystemNodeFactoryRegistry* reg)
{
    if(d->visSystemNodeFactoryReg)
        disconnect(d->visSystemNodeFactoryReg->containerObject(), 0, this, 0);
    d->visSystemNodeFactoryReg = reg;
    if(d->visSystemNodeFactoryReg)
        connect(d->visSystemNodeFactoryReg->containerObject(), SIGNAL(registryChanged()), this, SLOT(on_registryChanged()));
}

IVisSystemNodeFactoryRegistry* CVisSystemNodeClassExplorer::nodeFactoryRegistry()
{
    return d->visSystemNodeFactoryReg;
}

void CVisSystemNodeClassExplorer::refresh()
{
    d->treeWidget->clear();

    if(!d->visSystemNodeFactoryReg)
        return;

    QStringList classNameList = d->visSystemNodeFactoryReg->nodeClassNameList();
    int classCount = 0;
    for(int i=0; i<classNameList.count(); i++)
    {
        QString className = classNameList[i];
        IVisSystemNodeDesc* desc = d->visSystemNodeFactoryReg->nodeDesc(className);
        if(!desc || !d->visSystemNodeFactoryReg->canCreate(className))
            continue;

        QTreeWidgetItem* parentItem = d->fetchParentItem(desc);
        QTreeWidgetItem* item = 0;
        if(parentItem)
            item = new QTreeWidgetItem(parentItem);
        else
            item = new QTreeWidgetItem(d->treeWidget);
        item->setText(0, className);
        item->setSizeHint(0, QSize(100, d->itemSize));
        if(d->visSystemNodeFactoryReg->canCreate(className))
            item->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        else
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);

        item->setToolTip(0, desc->nodeClassDescription());
        item->setIcon(0, desc->nodeIcon());
        ++classCount;
    }

    if(d->exploreMode == IVisSystemNodeFactoryRegistryExplorer::CategoryMode)
    {
        for(int i=0; i<d->treeWidget->topLevelItemCount(); i++)
        {
            QTreeWidgetItem* item = d->treeWidget->topLevelItem(i);
            QString itemText = QString("%1 (%2)").arg(item->text(0)).arg(item->childCount());
            item->setText(0, itemText);
        }
    }

    qDebug("Node Class Explorer: %d classes registered", classCount);
}

void CVisSystemNodeClassExplorer::on_registryChanged()
{
    this->refresh();
}

QTreeWidgetItem* CVisSystemNodeClassExplorerData::fetchParentItem(IVisSystemNodeDesc* desc)
{
    if(!desc)
        return 0;

    QTreeWidgetItem* parentItem = 0;

    switch(exploreMode)
    {
    case IVisSystemNodeFactoryRegistryExplorer::ContextMode:
    case IVisSystemNodeFactoryRegistryExplorer::CategoryMode: {
            QString category = desc->nodeClassCategory();
            QList<QTreeWidgetItem*> parentItems = treeWidget->findItems(category, Qt::MatchFixedString);
            if(parentItems.count())
                parentItem = parentItems.first();
            else
            {
                parentItem = new QTreeWidgetItem(treeWidget);
                parentItem->setText(0, category);
                parentItem->setSizeHint(0, QSize(100, itemSize));
            }
        } break;
    case IVisSystemNodeFactoryRegistryExplorer::ClassHierarchyMode: {
            IVisSystemNodeDesc* parentDesc = desc->parentNodeDesc();
            if(!parentDesc)
                break;

            QString parentClassName = parentDesc->nodeClassName();
            QList<QTreeWidgetItem*> parentItems = treeWidget->findItems(parentClassName, Qt::MatchFixedString);
            if(parentItems.count())
                parentItem = parentItems.first();
            else
            {
                QTreeWidgetItem* parentItem2 = fetchParentItem(parentDesc);
                if(parentItem2)
                    parentItem = new QTreeWidgetItem(parentItem2);
                else
                    parentItem = new QTreeWidgetItem(treeWidget);
                parentItem->setText(0, parentClassName);
                parentItem->setSizeHint(0, QSize(100, itemSize));
            }

        } break;
    }

    return parentItem;
}

void CVisSystemNodeClassExplorerData::filterItems(QTreeWidgetItem* parent, const QList<QTreeWidgetItem*>& items, bool showAll)
{
	for(int i=0; i<parent->childCount(); i++)
	{
		QTreeWidgetItem* item = parent->child(i);
		if( !showAll && items.count() && !items.contains(item) )
			item->setHidden(true);
		else 
			item->setHidden(false);
	}
}

