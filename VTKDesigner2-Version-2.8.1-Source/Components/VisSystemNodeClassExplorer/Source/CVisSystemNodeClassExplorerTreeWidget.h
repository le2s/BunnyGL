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

#ifndef C_VIS_SYSTEM_NODE_CLASS_EXPLORER_TREE_WIDGET_H
#define C_VIS_SYSTEM_NODE_CLASS_EXPLORER_TREE_WIDGET_H

#include <QTreeWidget>
#include <QItemDelegate>

struct CVisSystemNodeClassExplorerTreeWidgetData;
class CVisSystemNodeClassExplorerTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    CVisSystemNodeClassExplorerTreeWidget(QWidget* widget=0);
    ~CVisSystemNodeClassExplorerTreeWidget();

protected:
    void mousePressEvent(QMouseEvent* me);
    void mouseMoveEvent(QMouseEvent* me);
    void drawBranches(QPainter* paint, const QRect & rect, const QModelIndex & index) const;

private slots:
    void on_itemClicked(QTreeWidgetItem* item, int col);
    void on_itemRightClicked(QTreeWidgetItem* item);

private:
    CVisSystemNodeClassExplorerTreeWidgetData* d;
};

class CVisSystemNodeClassExplorerTreeWidgetDelegate : public QItemDelegate
{
public:
    CVisSystemNodeClassExplorerTreeWidgetDelegate(QAbstractItemDelegate* dd, QTreeWidget* tw, QObject* parent);
    ~CVisSystemNodeClassExplorerTreeWidgetDelegate() { }

    QSize sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& index) const;
    void paint(QPainter* paint, const QStyleOptionViewItem& opt, const QModelIndex& index) const;

private:
    QAbstractItemDelegate* defaultDelegate;
    QTreeWidget* treeWidget;
};


#endif

