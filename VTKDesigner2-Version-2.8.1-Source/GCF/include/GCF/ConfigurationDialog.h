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

#ifndef CONFIGURATION_DIALOG_H
#define CONFIGURATION_DIALOG_H

#include "Common.h"

#include <QDialog>
#include <QIcon>

class QPushButton;
class QTreeWidgetItem;

namespace GCF
{
struct ConfigDialogData;
class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    ConfigDialog(QWidget* parent=0);
    ~ConfigDialog();

    void addPage(QWidget* page, const QString& title=QString(), const QIcon& icon=QIcon());
    void removePage(QWidget* page);
    int pageCount() const;
    QWidget* pageAt(int index) const;
    int indexOfPage(QWidget* widget) const;

    QWidget* currentPage() const;
    int currentPageIndex() const;

public slots:
    void setCurrentPage(QWidget* page);
    void setCurrentPageFromIndex(int index);

signals:
    void currentPageIndexChanged(int index);
    void currentPageChanged(QWidget* page);
    void pageAdded(int index);
    void pageRemoved(int index);

protected:
    void childEvent(QChildEvent* ce);

protected slots:
    void resizeTimeLineChanged(qreal value);

private:
    ConfigDialogData* d;
};

struct ConfigDialogToolbarData;
class ConfigDialogToolbar : public QWidget
{
    Q_OBJECT

public:
    ConfigDialogToolbar(QWidget* parent=0);
    ~ConfigDialogToolbar();

    void addToolButton(const QString& title, const QIcon& icon);
    void removeToolButton(const QString& title);
    int toolButtonCount() const;
    QString toolButtonText(int index) const;
    QIcon toolButtonIcon(int index) const;
    int indexOfToolButton(const QString& title);

    bool isReadOnly() const;
    int currentToolButton() const;

    QSize sizeHint() const;

public slots:
    void setReadOnly(bool val);
    void setCurrentToolButton(int index);

signals:
    void currentToolButtonChanged(int index);
    void toolButtonAdded(int index, const QString& title);
    void toolButtonRemoved(int index, const QString& title);

protected:
    void updateToolButtonWidth();
    void resizeEvent(QResizeEvent* re);
    void paintEvent(QPaintEvent* pe);
    void mousePressEvent(QMouseEvent* me);
    void mouseReleaseEvent(QMouseEvent* me);

private:
    ConfigDialogToolbarData* d;
};

struct ConfigurationDialogData;
class GCF_EXPORT_CLASS ConfigurationDialog : public ConfigDialog
{
    Q_OBJECT

public:
    ConfigurationDialog(QWidget* parent=0);
    ~ConfigurationDialog();

    // void addPage(QWidget* child);

    QWidget* pageContainer() const;
    QPushButton* okButton() const;
    QPushButton* cancelButton() const;

protected:
    void showEvent(QShowEvent* se);
    void closeEvent(QCloseEvent* ce);

protected:
    ConfigurationDialogData* d;
};

struct ComponentPropertyEditorPageData;
class GCF_EXPORT_CLASS ComponentPropertyEditorPage : public QWidget
{
    Q_OBJECT

public:
    ComponentPropertyEditorPage(QWidget* parent=0);
    ~ComponentPropertyEditorPage();

public slots:
    void loadProperties();
    void saveProperties();

protected slots:
    void on_componentTree_itemClicked(QTreeWidgetItem* item);
    void on_cmdAddProperty_clicked();
    void on_cmdRemoveProperty_clicked();
    void on_propertyEditor_itemChanged(QTreeWidgetItem* item, int col);

private:
    ComponentPropertyEditorPageData* d;
};

}
#endif
