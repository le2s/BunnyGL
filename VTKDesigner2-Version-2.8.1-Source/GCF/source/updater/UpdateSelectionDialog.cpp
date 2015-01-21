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

#include "UpdateSelectionDialog.h"

#include <QPushButton>
#include <QHeaderView>

#include "ui_UpdateSelectionDialog.h"

namespace GCF
{
    struct UpdateSelectionDialogData
    {
        UpdateSelectionDialogData() : inItemChanged(false) { }

        Ui::UpdateSelectionDialog ui;
        bool inItemChanged;

        GCF::UpdateInfoList updateInfoList;
        GCF::UpdateInfoList selectedUpdates;

        QMap<GCF::UpdateInfo, QTreeWidgetItem*> updateInfoItemMap;
        QMap<GCF::UpdateInfo, QList<UpdateInfo> > dependingList;
        QList<QTreeWidgetItem*> itemChangedList; // used to avoid recursion in on_lstUpdates_itemChanged
    };
}

GCF::UpdateSelectionDialog::UpdateSelectionDialog(const GCF::UpdateInfoList& list, QWidget* parent)
:QDialog(parent)
{
    d = new UpdateSelectionDialogData;

    d->ui.setupUi(this);
    d->updateInfoList = list;
    d->selectedUpdates = list;

    // Populate lstUpdates (QTreeWidget) with information about the
    // Updates
    QMap<QString, QTreeWidgetItem*> sourceItemMap;
    QFont sourceItemFont = d->ui.lstUpdates->font();
    sourceItemFont.setBold(true);
    sourceItemFont.setPointSize( sourceItemFont.pointSize()+2 );

    // Hide the header view in lstUpdates
    d->ui.lstUpdates->header()->hide();
    d->ui.lstUpdates->setColumnCount(1);

    for(int i=0; i<list.count(); i++)
    {
        GCF::UpdateInfo info = list.at(i);
        QString source = info.source();
        QTreeWidgetItem* parentItem = 0;

        if( sourceItemMap.contains(source) )
            parentItem = sourceItemMap[source];
        else
        {
            parentItem = new QTreeWidgetItem(d->ui.lstUpdates);
            parentItem->setText(0, source);
            parentItem->setFont(0, sourceItemFont);
            parentItem->setExpanded(true);
            parentItem->setBackground(0, palette().button());
            parentItem->setFlags( Qt::ItemIsEnabled );
            sourceItemMap[source] = parentItem;
        }

        QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
        item->setText( 0, QString("%1 [%2]").arg(info.title()).arg(info.version()) );
        item->setIcon( 0, info.icon() );
        item->setData( 0, Qt::UserRole, qVariantFromValue<GCF::UpdateInfo>(info) );
        item->setFlags( Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsUserCheckable );
        item->setCheckState( 0, Qt::Checked );

        d->updateInfoItemMap[info] = item;

        QList<UpdateInfo> deps = info.dependencies();
        for(int i=0; i<deps.count(); i++)
        {
            GCF::UpdateInfo depInfo = deps.at(i);
            d->dependingList[depInfo].append(info);
        }
    }

    if(d->ui.lstUpdates->topLevelItemCount() == 0)
        d->ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(d->ui.lstUpdates, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
              this, SLOT(slotItemChanged(QTreeWidgetItem*)));
}

GCF::UpdateSelectionDialog::~UpdateSelectionDialog()
{
    delete d;
}

GCF::UpdateInfoList GCF::UpdateSelectionDialog::listedUpdates() const
{
    return d->updateInfoList;
}

GCF::UpdateInfoList GCF::UpdateSelectionDialog::selectedUpdates() const
{
    return d->selectedUpdates;
}

void GCF::UpdateSelectionDialog::slotItemChanged(QTreeWidgetItem* item)
{
    if(!item || d->itemChangedList.contains(item))
        return;

    d->itemChangedList.append(item);

    GCF::UpdateInfo uInfo = item->data(0, Qt::UserRole).value<GCF::UpdateInfo>();
    if(item->checkState(0) == Qt::Unchecked)
    {
        d->selectedUpdates.removeAll(uInfo);

        QList<GCF::UpdateInfo> toUncheck = d->dependingList[uInfo];
        Q_FOREACH(GCF::UpdateInfo uInfo2, toUncheck)
        {
            QTreeWidgetItem* item2 = d->updateInfoItemMap[uInfo2];
            item2->setCheckState( 0, Qt::Unchecked ); // this will recursively uncheck others as well..
        }
    }
    else if(item->checkState(0) == Qt::Checked)
    {
        if(!d->selectedUpdates.contains(uInfo))
            d->selectedUpdates.append(uInfo);

        QList<GCF::UpdateInfo> toCheck = uInfo.dependencies();
        Q_FOREACH(GCF::UpdateInfo uInfo2, toCheck)
        {
            QTreeWidgetItem* item2 = d->updateInfoItemMap[uInfo2];
            item2->setCheckState( 0, Qt::Checked ); // this will recursively uncheck others as well..
        }
    }

    d->itemChangedList.removeAll(item);
}

