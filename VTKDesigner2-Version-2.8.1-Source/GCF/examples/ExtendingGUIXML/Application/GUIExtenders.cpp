#include "GUIExtenders.h"
#include "ApplicationContainerComponent.h"
#include <QToolBar>
#include <QTableWidget>
#include <QIcon>
#include <QAction>
#include <QStyle>
#include <QtDebug>

void MenuWidgetTabExtender::parseElement(QDomElement e, GCF::ComponentGui gui)
{
    /*
      This function is called when <menutab> XML element is found in the GUIXML file.
      We process the <menutab> XML element by

      a) Creating a new node (GCF::ComponentGuiNode) of type MenuWidgetTabExtender::Type
         and adding it to the component-gui.
      b) Creating one node each of type MenuWidgetTabItemExtender::Type for every
         <menutabitem> XML element found under <menutab>
      */
    GCF::ComponentGuiNode node;
    node.setType(this->type());
    node.setName(e.attribute("name"));
    storeAttributes(e, node.nodeData());
    gui.addNode(node);

    QDomElement groupE = e.firstChildElement("menutabitem");
    while( !groupE.isNull() )
    {
        GCF::ComponentGuiNode tabItemNode;
        tabItemNode.setType(MenuWidgetTabItemExtender::Type);
        tabItemNode.setName(groupE.attribute("name"));
        storeAttributes(groupE, tabItemNode.nodeData());
        tabItemNode["parentTab"] = node.completeName();
        gui.addNode(tabItemNode);

        groupE = groupE.nextSiblingElement("menutabitem");
    }
}

void MenuWidgetTabExtender::initializeNode(GCF::ComponentGuiNode node)
{
    /*
      This function is called with a gui-node of type MenuWidgetTabExtender::Type is to
      be initialized. In this function we either merge the toolbar with already
      existing toolbar or create a new one.
      */
    QToolBar* toolBar = 0;

    if(node.hasKey("mergeWith"))
    {
        QString mergeWith = node["mergeWith"].toString();
        GCF::ComponentGuiNode mergeNode = node.gui().findNode(mergeWith);
        toolBar = mergeNode.hasKey("QToolBar") ? (QToolBar*)mergeNode["QToolBar"].value<QObject*>() : 0;
        node["usingMergedToolBar"] = toolBar ? true : false;
    }

    if(!toolBar)
    {
        QString title = node["title"].toString();

        toolBar = new QToolBar;
        toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolBar->setFloatable(false);
        ApplicationContainerComponent::instance().menuWidget()->addTab(toolBar, title);
        node["QToolBar"] = QVariant::fromValue( (QObject*)toolBar );
        node["usingMergedToolBar"] = false;
    }
}

void MenuWidgetTabExtender::cleanupNode(GCF::ComponentGuiNode node)
{
    if(node["usingMergedToolBar"].toBool())
        return;

    QToolBar* toolBar = node.hasKey("QToolBar") ? (QToolBar*)node["QToolBar"].value<QObject*>() : 0;
    if(toolBar)
        delete toolBar;
}

void MenuWidgetTabExtender::showNode(GCF::ComponentGuiNode node)
{
    Q_UNUSED(node);
}

void MenuWidgetTabExtender::hideNode(GCF::ComponentGuiNode node)
{
    Q_UNUSED(node);
}

/*
  Implementation of MenuWidgetTabItemExtender follows
  */

void MenuWidgetTabItemExtender::parseElement(QDomElement e, GCF::ComponentGui gui)
{
    GCF::ComponentGuiNode tabItemNode;
    tabItemNode.setType(MenuWidgetTabItemExtender::Type);
    tabItemNode.setName(e.attribute("name"));
    storeAttributes(e, tabItemNode.nodeData());
    gui.addNode(tabItemNode);
}

void MenuWidgetTabItemExtender::initializeNode(GCF::ComponentGuiNode node)
{
    QToolBar* toolBar = 0;

    QString parentTab = node["parentTab"].toString();
    if(!parentTab.isEmpty())
    {
        GCF::ComponentGuiNode parentTabNode = node.gui().findNode(parentTab);
        toolBar = parentTabNode.hasKey("QToolBar") ? (QToolBar*)parentTabNode["QToolBar"].value<QObject*>() : 0;
    }

    if(!toolBar)
    {
        if(ApplicationContainerComponent::instance().menuWidget()->count())
            toolBar = (QToolBar*)ApplicationContainerComponent::instance().menuWidget()->widget(0);
        else
        {
            toolBar = new QToolBar;
            toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            toolBar->setFloatable(false);
            ApplicationContainerComponent::instance().menuWidget()->insertTab(0, toolBar, "Default");
        }
    }

    if(!toolBar)
        return; // should never happen

    QString actionName = node["actionName"].toString();
    if(actionName.isEmpty())
        return;

    QAction* action = qobject_cast<QAction*>(node.gui().findNode(actionName).object());
    if(!action)
        return;

    if(action->icon().isNull())
        action->setIcon( qApp->windowIcon() );
    if(action->icon().isNull())
        action->setIcon( qApp->style()->standardIcon(QStyle::SP_DesktopIcon) );

    toolBar->addAction(action);

    node["parentToolBar"] = QVariant::fromValue( (QObject*)toolBar );
    node["QAction"] = QVariant::fromValue(action);
}

void MenuWidgetTabItemExtender::cleanupNode(GCF::ComponentGuiNode node)
{
    QAction* action = node.hasKey("QAction") ? node["QAction"].value<QAction*>() : 0;
    if(!action)
        return;

    QToolBar* toolBar = node.hasKey("parentToolBar") ? (QToolBar*)node["parentToolBar"].value<QObject*>() : 0;
    if(toolBar)
        toolBar->removeAction(action);

    node.removeKey("parentToolBar");
    node.removeKey("QAction");
}

void MenuWidgetTabItemExtender::showNode(GCF::ComponentGuiNode node)
{
    Q_UNUSED(node);
}

void MenuWidgetTabItemExtender::hideNode(GCF::ComponentGuiNode node)
{
    Q_UNUSED(node);
}


