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

#ifndef C_VIS_SYSTEM_NODE_CLASS_EXPLORER_H
#define C_VIS_SYSTEM_NODE_CLASS_EXPLORER_H

#include <QTreeWidget>
#include "IVisSystemNodeFactoryRegistryExplorer.h"

class QTreeWidget;

struct CVisSystemNodeClassExplorerData;
class CVisSystemNodeClassExplorer : public QWidget, virtual public IVisSystemNodeFactoryRegistryExplorer 
{
    Q_OBJECT
    Q_INTERFACES(IVisSystemNodeFactoryRegistryExplorer)
    Q_PROPERTY(int ItemSize WRITE setItemSize READ itemSize)

public:
    CVisSystemNodeClassExplorer(QWidget* parent=0);
    ~CVisSystemNodeClassExplorer();

    void setTreeWidget(QTreeWidget* treeWidget);
    QTreeWidget* treeWidget() const;

    void setItemSize(int size);
    int itemSize() const;

public slots:
    void setExploreModeToCategoryMode();
    void setExploreModeToClassHierarchyMode();
    void setExploreModeToContextMode();
    void setFilter(const QString & filter) { setClassNameFilter(filter); }

public:
    // Implemented from IContainer
    QObject* containerObject();

    // Implemented from IVisSystemNodeFactoryRegistryExplorer
    void updateClassList();
    void selectClassName(QString className);
    QString selectedClassName();
    void setClassNameFilter(QString filter);
    void setExploreMode(IVisSystemNodeFactoryRegistryExplorer::ExploreMode mode);
    IVisSystemNodeFactoryRegistryExplorer::ExploreMode exploreMode();
    void setNodeFactoryRegistry(IVisSystemNodeFactoryRegistry* reg);
    IVisSystemNodeFactoryRegistry* nodeFactoryRegistry();

protected slots:
    void refresh();
    void on_registryChanged();

private:
    CVisSystemNodeClassExplorerData* d;
};

#endif
