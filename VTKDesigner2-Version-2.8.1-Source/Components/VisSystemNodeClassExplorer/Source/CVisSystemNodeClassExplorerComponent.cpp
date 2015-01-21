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

#include "CVisSystemNodeClassExplorerComponent.h"
#include "CVisSystemNodeClassExplorer.h"

#include "IVisSystemNodeFactoryRegistryExplorer.h"

#include "IVisSystemNodeFactory.h"
#include "IVisSystemNode.h"


#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTreeWidget>
#include <QCompleter>
#include <QAbstractItemModel>

struct CVisSystemNodeClassExplorerComponentData
{
    CVisSystemNodeClassExplorerComponentData() : explorer(0),
        explorerContainer(0), ecLayout(0), searchForWidget(0) { }

    IVisSystemNodeFactoryRegistryExplorer* explorer;
    QWidget* explorerContainer;
    QVBoxLayout* ecLayout;
    QWidget* searchForWidget;

    // NodeFactoryRegistry stuff.
    QList<IVisSystemNodeFactory*> nodeFactoryList;
};

GCF_DEFINE_COMPONENT(CVisSystemNodeClassExplorerComponent)

CVisSystemNodeClassExplorerComponent& CVisSystemNodeClassExplorerComponent::instance()
{
    static CVisSystemNodeClassExplorerComponent* theInstance = GCF_CREATE_COMPONENT(CVisSystemNodeClassExplorerComponent);
    return *theInstance;
}

CVisSystemNodeClassExplorerComponent::CVisSystemNodeClassExplorerComponent()
{
    d = new CVisSystemNodeClassExplorerComponentData;

    d->explorerContainer = new QWidget;
    d->ecLayout = new QVBoxLayout(d->explorerContainer);
    d->ecLayout->setMargin(0);
    d->ecLayout->setSpacing(2);

    CVisSystemNodeClassExplorer* explorerWidget = new CVisSystemNodeClassExplorer(d->explorerContainer);
    d->explorer = explorerWidget;
    d->explorer->setNodeFactoryRegistry(this);
    d->ecLayout->addWidget(explorerWidget);

    setSearchForWidget(0); // creates a default search for widget.
}

CVisSystemNodeClassExplorerComponent::~CVisSystemNodeClassExplorerComponent()
{
    delete d;
}

void CVisSystemNodeClassExplorerComponent::setSearchForWidget(QWidget* widget)
{
    if(d->searchForWidget && d->searchForWidget == widget)
        return;

    if(d->searchForWidget)
        delete d->searchForWidget;

    d->searchForWidget = widget;
    if(!d->searchForWidget)
    {
        QObject* ec = d->explorer->containerObject();
        CVisSystemNodeClassExplorer* ncExp = qobject_cast<CVisSystemNodeClassExplorer*>(ec);
        if(!ncExp)
        {
            // Dont know what to do without ncExp
            return;
        }

        QWidget* searchFor = new QWidget(d->explorerContainer);
        QVBoxLayout* layout = new QVBoxLayout(searchFor);
        QLabel* label = new QLabel("&Look for", searchFor);
        QLineEdit* lineEdit = new QLineEdit(searchFor);
        layout->addWidget(label);
        layout->addWidget(lineEdit);
        label->setBuddy(lineEdit);
        layout->setMargin(2);
        QCompleter* completer = new QCompleter(lineEdit);
        lineEdit->setCompleter(completer);
        completer->setModel(ncExp->treeWidget()->model());
        completer->setCompletionMode(QCompleter::InlineCompletion);
        d->searchForWidget = searchFor;
        connect(lineEdit, SIGNAL(textChanged(const QString&)), ncExp, SLOT(setFilter(const QString&)));
    }

    d->ecLayout->insertWidget(0, d->searchForWidget);
    d->searchForWidget->show();
}

QWidget* CVisSystemNodeClassExplorerComponent::searchForWidget() const
{
    return d->searchForWidget;
}

QWidget* CVisSystemNodeClassExplorerComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    if(comps.last() == "explorerContainer")
        return d->explorerContainer;

    if(comps.last() == "explorer")
    {
        QObject* ec = d->explorer->containerObject();
        QWidget* widget = qobject_cast<QWidget*>(ec);
        return widget;
    }

    if(comps.last() == "searchForWidget")
        return d->searchForWidget;

    if(comps.last() == "explorerTree")
    {
        QObject* ec = d->explorer->containerObject();
        CVisSystemNodeClassExplorer* ncExp = qobject_cast<CVisSystemNodeClassExplorer*>(ec);
        return ncExp->treeWidget();
    }

    return 0;
}

void CVisSystemNodeClassExplorerComponent::addChildWidget(QWidget* parent, QWidget* child, const QString& hint)
{
    /**
    We can welcome the following kinds of child widget into the component

    1. Search For Widget (which is a line edit by default)
    2. Tree widget that should show up in the node class explorer.
    3. Any other widget that can be added at the end of the explorerContainer widget.
    */

    if(parent == d->explorerContainer)
    {
        // If the child widget being added is a "FilterWidgetHint" widget then we set it
        // as a search widget.
        if(hint == "FilterWidgetHint")
        {
            setSearchForWidget(child);
            return;
        }

        // At this moment we really have no idea about the kind of child widget that is being
        // added to the container. So lets just add the child to the layout and hope that
        // child knows what to do.
        d->ecLayout->addWidget(child);
    }
    else
    {
        // Now lets check if the child is a tree widget. If the child is a tree widget and the
        // explorer we are currently using is a CVisSystemNodeClassExplorer instance, then
        // we can ask it use the new tree widget as explorer tree.
        QObject* ec = d->explorer->containerObject();
        CVisSystemNodeClassExplorer* ncExp = qobject_cast<CVisSystemNodeClassExplorer*>(ec);
        if(ncExp && parent == ncExp)
        {
            QTreeWidget* tw = qobject_cast<QTreeWidget*>(child);
            if(tw)
            {
                ncExp->setTreeWidget(tw);
                return;
            }
        }
    }

    // The child widget is trying to parent into some place that we cannot allow.
    child->setParent(0);
    child->show();
}

void CVisSystemNodeClassExplorerComponent::removeChildWidget(QWidget* parent, QWidget* child, const QString& hint)
{
    if(parent == d->explorerContainer)
    {
        if(hint == "FilterWidgetHint")
        {
            setSearchForWidget(0);
            return;
        }

        d->ecLayout->removeWidget(child);
    }
    else
    {
        QObject* ec = d->explorer->containerObject();
        CVisSystemNodeClassExplorer* ncExp = qobject_cast<CVisSystemNodeClassExplorer*>(ec);
        if(ncExp && parent == ncExp)
        {
            QTreeWidget* tw = qobject_cast<QTreeWidget*>(child);
            if(tw)
            {
                ncExp->setTreeWidget(0);
                return;
            }
        }
    }

    child->setParent(0);
    Q_UNUSED(hint);
}

QObject* CVisSystemNodeClassExplorerComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    
    CVisSystemNodeClassExplorerComponent* that = const_cast<CVisSystemNodeClassExplorerComponent*>(this);
    
    if(comps.last() == "controller")
        return that;

    return 0;
}

void CVisSystemNodeClassExplorerComponent::initializeComponent()
{
    // Do nothing
}

void CVisSystemNodeClassExplorerComponent::finalizeComponent()
{
    // Do nothing
}

QObject* CVisSystemNodeClassExplorerComponent::containerObject()
{
    return this;
}

void CVisSystemNodeClassExplorerComponent::registerNodeFactory(IVisSystemNodeFactory* factory)
{
    if(!factory || d->nodeFactoryList.contains(factory))
        return;
    d->nodeFactoryList.append(factory);
    emit registryChanged();
}

void CVisSystemNodeClassExplorerComponent::unregisterNodeFactory(IVisSystemNodeFactory* factory)
{
    if(!factory || !d->nodeFactoryList.contains(factory))
        return;
    d->nodeFactoryList.removeAll(factory);
    emit registryChanged();
}

int CVisSystemNodeClassExplorerComponent::nodeFactoryCount()
{
    return d->nodeFactoryList.count();
}

IVisSystemNodeFactory* CVisSystemNodeClassExplorerComponent::nodeFactory(int index)
{
    if(index < 0 || index >= d->nodeFactoryList.count())
        return 0;
    return d->nodeFactoryList[index];
}

QStringList CVisSystemNodeClassExplorerComponent::nodeClassNameList()
{
    QStringList ret;
    for(int i=0; i<d->nodeFactoryList.count(); i++)
    {
        IVisSystemNodeFactory* factory = d->nodeFactoryList[i];
        ret += factory->nodeClassNameList();
    }
    return ret;
}

IVisSystemNodeDesc* CVisSystemNodeClassExplorerComponent::nodeDesc(QString nodeClassName)
{
    for(int i=0; i<d->nodeFactoryList.count(); i++)
    {
        IVisSystemNodeFactory* factory = d->nodeFactoryList[i];
        if(factory->nodeClassNameList().contains(nodeClassName))
            return factory->nodeDesc(nodeClassName);
    }
    return 0;
}

bool CVisSystemNodeClassExplorerComponent::canCreate(QString nodeClassName)
{
    for(int i=0; i<d->nodeFactoryList.count(); i++)
    {
        IVisSystemNodeFactory* factory = d->nodeFactoryList[i];
        if(factory->nodeClassNameList().contains(nodeClassName))
            return factory->canCreate(nodeClassName);
    }
    return false;
}

IVisSystemNode* CVisSystemNodeClassExplorerComponent::createNode(QString nodeClassName)
{
    for(int i=0; i<d->nodeFactoryList.count(); i++)
    {
        IVisSystemNodeFactory* factory = d->nodeFactoryList[i];
        if(factory->nodeClassNameList().contains(nodeClassName))
            return factory->createNode(nodeClassName);
    }
    return 0;
}

void CVisSystemNodeClassExplorerComponent::deleteNode(IVisSystemNode* node)
{
    if(!node)
        return;

    IVisSystemNodeFactory* factory = node->nodeFactory();
    if(factory)
        factory->deleteNode(node);
    else
        node->finalizeNode();
}

QString CVisSystemNodeClassExplorerComponent::productName() const
{
    return "Visualization Node Class Explorer";
}

QString CVisSystemNodeClassExplorerComponent::organization() const
{
    return "VCreate Logic (P) Ltd";
}

QImage CVisSystemNodeClassExplorerComponent::programLogo() const
{
    return QImage();
}

QString CVisSystemNodeClassExplorerComponent::version() const
{
    return "2.0";
}

QString CVisSystemNodeClassExplorerComponent::shortDescription() const
{
    return "Explorer for all visualization system node classes in VTK Designer";
}

QString CVisSystemNodeClassExplorerComponent::homepage() const
{
    return "http://www.vcreatelogic.com/oss/vtkdesigner";
}

QString CVisSystemNodeClassExplorerComponent::bugAddress() const
{
    return "http://www.vcreatelogic.com/oss/vtkdesigner/feedback/bugs";
}

const QList<GCF::AboutPerson> CVisSystemNodeClassExplorerComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    if(retList.count() == 0)
    {
        retList << GCF::AboutPerson("Prashanth N Udupa", "prashanth@vcreatelogic.com", "http://www.vcreatelogic.com", "Founder, Vision, Design, Development");
        retList << GCF::AboutPerson("Lohith D S", "NA", "http://www.vcreatelogic.com", "VTK System Development");
    }
    return retList;
}

const QList<GCF::AboutPerson> CVisSystemNodeClassExplorerComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString CVisSystemNodeClassExplorerComponent::license() const
{
    return "Dual License: GPL Version 2 for FOSS development, Commercial for others.";
}

QString CVisSystemNodeClassExplorerComponent::copyrightStatement() const
{
    return QString();
}

GCF_EXPORT_COMPONENT_PLUGIN(CVisSystemNodeClassExplorerComponent)



