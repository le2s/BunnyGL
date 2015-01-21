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

#include "UpdaterConfigPage.h"

#include <GCF/UpdaterComponent>
#include <QTreeWidgetItem>
#include <QItemSelectionModel>

#include <QHeaderView>

#include <GCF/UpdateInfo>
#include "UpdateXMLParser.h"

#include "ui_UpdaterConfigPage.h"

namespace GCF
{
    struct UpdaterConfigPageData
    {
        Ui::UpdaterConfigPage ui;
    };
}

GCF::UpdaterConfigPage::UpdaterConfigPage(QWidget* parent)
:QWidget(parent)
{
    d = new UpdaterConfigPageData;
    d->ui.setupUi(this);

    connect(d->ui.lstServerSites->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(lstServerSites_selectionChanged()));

    QMetaObject::invokeMethod(this, "init", Qt::QueuedConnection);
    QMetaObject::invokeMethod(this, "lstServerSites_selectionChanged", Qt::QueuedConnection);
}

GCF::UpdaterConfigPage::~UpdaterConfigPage()
{
    delete d;
}

QStringList GCF::UpdaterConfigPage::thirdPartySites() const
{
    QStringList ret;
    for(int i=0; i<d->ui.lstServerSites->count(); i++)
    {
        QListWidgetItem* item = d->ui.lstServerSites->item(i);
        ret.append( item->text() );
    }

    return ret;
}

bool GCF::UpdaterConfigPage::enableUpdaterFlag() const
{
    return d->ui.chkEnableUpdater->isChecked();
}

void GCF::UpdaterConfigPage::on_cmdAddSite_clicked()
{
    QListWidgetItem* item = new QListWidgetItem(d->ui.lstServerSites);
    item->setText("Enter server site URL here...");
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
    d->ui.lstServerSites->editItem(item);
}

void GCF::UpdaterConfigPage::on_cmdRemoveSite_clicked()
{
    QListWidgetItem* item = d->ui.lstServerSites->currentItem();
    if(!item)
        return;

    delete item;
}

void GCF::UpdaterConfigPage::on_tabWidget_currentChanged(int tab)
{
    Q_UNUSED(tab)
}

void GCF::UpdaterConfigPage::lstServerSites_selectionChanged()
{
    QListWidgetItem* item = d->ui.lstServerSites->currentItem();
    if(item && GCF::UpdaterComponent::instance().isAllowThirdPartyServerSites())
        d->ui.cmdRemoveSite->setEnabled(true);
    else
        d->ui.cmdRemoveSite->setEnabled(false);
}

void GCF::UpdaterConfigPage::init()
{
    d->ui.chkEnableUpdater->setChecked(true);

    // Show the primary server site
    d->ui.txtPrimaryServerSite->setText( GCF::UpdaterComponent::instance().primaryServerSite() );

    // Show the other server sites
    QStringList sites = GCF::UpdaterComponent::instance().serverSiteList();
    Q_FOREACH(QString site, sites)
    {
        QListWidgetItem* item = new QListWidgetItem(d->ui.lstServerSites);
        item->setText(site);
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
    }

    // Disable "Add Site" button if third party sites are not enabled
    if(!GCF::UpdaterComponent::instance().isAllowThirdPartyServerSites())
        d->ui.cmdAddSite->setEnabled(false);
}

///////////////////////////////////////////////////////////////////////////////
// GCF::UpdatesView
///////////////////////////////////////////////////////////////////////////////

GCF::UpdatesView::UpdatesView(const QString& fileName, QWidget* parent)
:QTreeWidget(parent)
{
    GCF::UpdateInfoList updates;
    GCF::UpdateXMLParser().parse( fileName, updates );

    setIndentation(0);
    setRootIsDecorated(false);

    QFont sourceItemFont = this->font();
    sourceItemFont.setBold(true);
    sourceItemFont.setPointSize( sourceItemFont.pointSize()+2 );
    this->header()->hide();

    QMap<QString, QTreeWidgetItem*> sourceItemMap;

    int h = fontMetrics().height() + fontMetrics().ascent() + fontMetrics().descent();
    for(int i=0; i<updates.count(); i++)
    {
        GCF::UpdateInfo uInfo = updates[i];

        QString source = uInfo.source();
        QTreeWidgetItem* parentItem = 0;

        if( sourceItemMap.contains(source) )
            parentItem = sourceItemMap[source];
        else
        {
            parentItem = new QTreeWidgetItem(this);
            parentItem->setText(0, source);
            parentItem->setFont(0, sourceItemFont);
            parentItem->setExpanded(true);
            parentItem->setBackground(0, palette().button());
            parentItem->setSizeHint(0, QSize(h,h));
            parentItem->setFlags( Qt::ItemIsEnabled );
            sourceItemMap[source] = parentItem;
        }

        QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
        item->setText( 0, QString("%1 [Version %2]").arg(uInfo.title()).arg(uInfo.version()) );
        item->setToolTip( 0, uInfo.description().simplified() );
        item->setIcon( 0, uInfo.icon() );
        item->setSizeHint(0, QSize(h,h));
        item->setFlags( Qt::ItemIsEnabled );
    }
}

///////////////////////////////////////////////////////////////////////////////
// GCF::AvailableUpdatesView
///////////////////////////////////////////////////////////////////////////////

GCF::AvailableUpdatesView::AvailableUpdatesView(QWidget* parent)
:UpdatesView(GCF::UpdaterComponent::instance().resolveFileName("${AVAIL_UPDATES_XML}"), parent)
{

}

///////////////////////////////////////////////////////////////////////////////
// GCF::InstalledUpdatesView
///////////////////////////////////////////////////////////////////////////////

GCF::InstalledUpdatesView::InstalledUpdatesView(QWidget* parent)
:UpdatesView(GCF::UpdaterComponent::instance().resolveFileName("${INST_UPDATES_XML}"), parent)
{

}
