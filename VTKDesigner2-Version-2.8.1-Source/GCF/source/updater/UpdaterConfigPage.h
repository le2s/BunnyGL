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

#ifndef UPDATER_CONFIG_PAGE_H
#define UPDATER_CONFIG_PAGE_H

#include <QWidget>
#include <QTreeWidget>

namespace GCF
{

struct UpdaterConfigPageData;
class UpdaterConfigPage : public QWidget
{
    Q_OBJECT

public:
    UpdaterConfigPage(QWidget* parent=0);
    ~UpdaterConfigPage();

    QStringList thirdPartySites() const;
    bool enableUpdaterFlag() const;

protected slots:
    void on_cmdAddSite_clicked();
    void on_cmdRemoveSite_clicked();
    void on_tabWidget_currentChanged(int tab);

protected slots:
    void lstServerSites_selectionChanged();
    void init();

private:
    UpdaterConfigPageData* d;
};

class UpdatesView : public QTreeWidget
{
public:
    virtual ~UpdatesView() { }

protected:
    UpdatesView(const QString& fileName, QWidget* parent);
};

class AvailableUpdatesView : public UpdatesView
{
public:
    AvailableUpdatesView(QWidget* parent);
    ~AvailableUpdatesView() { }
};

class InstalledUpdatesView : public UpdatesView
{
public:
    InstalledUpdatesView(QWidget* parent);
    ~InstalledUpdatesView() { }
};
}

#endif
