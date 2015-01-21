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
** Contact info@vfetchlogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef C_VIS_SYSTEM_NODE_CLASS_EXPLORER_COMPONENT_H
#define C_VIS_SYSTEM_NODE_CLASS_EXPLORER_COMPONENT_H

#include <GCF/AbstractComponent>
#include <GCF/IAboutData>

#include "IVisSystemNodeFactoryRegistry.h"


class IVisSystemNodeFactoryRegistryExplorer;

struct CVisSystemNodeClassExplorerComponentData;
class CVisSystemNodeClassExplorerComponent : public GCF::AbstractComponent,
                                             virtual public IVisSystemNodeFactoryRegistry,
                                             virtual public IAboutData
{
    Q_OBJECT
    Q_INTERFACES(IVisSystemNodeFactoryRegistry IAboutData)
    GCF_DECLARE_COMPONENT(CVisSystemNodeClassExplorerComponent)

public:
    static CVisSystemNodeClassExplorerComponent& instance();
    ~CVisSystemNodeClassExplorerComponent();

    void setSearchForWidget(QWidget* widget);
    QWidget* searchForWidget() const;

protected:
    CVisSystemNodeClassExplorerComponent();
    QObject* fetchObject(const QString& completeName) const;
    QWidget* fetchWidget(const QString& completeName) const;
    void addChildWidget(QWidget* parent, QWidget* child, const QString& hint);
    void removeChildWidget(QWidget* parent, QWidget* child, const QString& hint);
    void initializeComponent();
    void finalizeComponent();

public:
    // IContainer implementation
    QObject* containerObject();

    // IVisSystemNodeFactoryRegistry implementation
    void registerNodeFactory(IVisSystemNodeFactory* factory);
    void unregisterNodeFactory(IVisSystemNodeFactory* factory);
    int nodeFactoryCount();
    IVisSystemNodeFactory* nodeFactory(int index);
    QStringList nodeClassNameList();
    IVisSystemNodeDesc* nodeDesc(QString nodeClassName);
    bool canCreate(QString nodeClassName);
    IVisSystemNode* createNode(QString nodeClassName);
    void deleteNode(IVisSystemNode* node);

signals:
    void registryChanged();

public:
    QString productName() const;
    QString organization() const;
    QImage programLogo() const;
    QString version() const;
    QString shortDescription() const;
    QString homepage() const;
    QString bugAddress() const;
    const QList<GCF::AboutPerson> authors() const;
    const QList<GCF::AboutPerson> credits() const;
    QString license() const;
    QString copyrightStatement() const;

private:
    CVisSystemNodeClassExplorerComponentData* d;
};

#endif
