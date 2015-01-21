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

#include "CVisSystemCanvasNodeLayout.h"
#include "CVisSystemCanvas.h"
#include "CVisSystemCanvasNodeItem.h"
#include "CVisSystemCanvasConnectionItem.h"
#include "IVisSystemNode.h"
#include "IVisNetworkConnection.h"
#include "IVisSystemNodeDesc.h"
#include "IVisSystemNodeConnectionPath.h"

#include <QDebug>
// #define SHOW_NODE_PLACEMENT_ANIMATION

#ifdef SHOW_NODE_PLACEMENT_ANIMATION
#include <QCoreApplication>
#include <QTime>
#endif

class TreeNode
{
public:
    static QList<TreeNode*> GlobalTreeNodeList;
    static TreeNode* FindTreeNode(CVisSystemCanvasNodeItem* item);

    TreeNode(CVisSystemCanvasNodeItem* item, TreeNode* parent=0);
    ~TreeNode();

    CVisSystemCanvasNodeItem* nodeItem() const;
    QRectF childrenBoundingRect() const;
    QRectF boundingRect() const;

    TreeNode* parent();
    QList<TreeNode*> childNodes();

    QRectF layout(const QRectF& occupiedRect=QRectF());
    void translate(qreal x, qreal y);
    void dumpTree(int indent=0);

private:
    QList<TreeNode*> m_childNodeList;
    CVisSystemCanvasNodeItem* m_item;
    TreeNode* m_parent;
    bool m_layoutDone;
};

QList<TreeNode*> TreeNode::GlobalTreeNodeList;

struct CVisSystemCanvasNodeLayoutData
{
    CVisSystemCanvasNodeLayoutData() : canvas(0) { }

    CVisSystemCanvas* canvas;
    QList<CVisSystemCanvasNodeItem*> visitedNodes;
    QList<CVisSystemCanvasNodeItem*> visitingNodes;
    QList<QRectF> occupiedRectList;

    QList<CVisSystemCanvasNodeItem*> getRootNodes();
    void layoutNodeItem(CVisSystemCanvasNodeItem* node);
    void centerNodeItem(CVisSystemCanvasNodeItem* node);
    QRectF adjustRect(QRectF rect);
    bool isControlPath(CVisSystemCanvasConnectionItem* conItem);
};

CVisSystemCanvasNodeLayout::CVisSystemCanvasNodeLayout()
{
    d = new CVisSystemCanvasNodeLayoutData;
}

CVisSystemCanvasNodeLayout::~CVisSystemCanvasNodeLayout()
{
    delete d;
}

void CVisSystemCanvasNodeLayout::setCanvas(CVisSystemCanvas* canvas)
{
    d->canvas = canvas;
}

CVisSystemCanvas* CVisSystemCanvasNodeLayout::canvas() const
{
    return d->canvas;
}

void CVisSystemCanvasNodeLayout::layoutNodes()
{
    #if 0
    if(!d->canvas)
        return;

    // Get Root Node
    QList<CVisSystemCanvasNodeItem*> rootNodes = d->getRootNodes();
    if(!rootNodes.count())
        return;

    // Delete the old tree
    qDeleteAll(TreeNode::GlobalTreeNodeList);
    TreeNode::GlobalTreeNodeList.clear();

    // Construct the new node tree
    QList<TreeNode*> topNodes;
    Q_FOREACH(CVisSystemCanvasNodeItem* rootNode, rootNodes)
        topNodes.append(new TreeNode(rootNode));

    // Layout the new node tree
    QRectF occupiedRect;
    Q_FOREACH(TreeNode* n, topNodes)
        occupiedRect |= n->layout(occupiedRect);

    // Delete all the tree nodes
    qDeleteAll(TreeNode::GlobalTreeNodeList);
    TreeNode::GlobalTreeNodeList.clear();
#endif

    QList<CVisSystemCanvasNodeItem*> nodeItemList = d->canvas->nodeItemList();
    if(!nodeItemList.count())
        return;

    d->visitedNodes.clear();
    d->visitingNodes.clear();

    for(int i=0; i<nodeItemList.count(); i++)
        d->layoutNodeItem(nodeItemList[i]);

    d->visitedNodes.clear();
    d->visitingNodes.clear();

    for(int i=0; i<nodeItemList.count(); i++)
        d->centerNodeItem(nodeItemList[i]);

    d->canvas->bringToCenter();
    d->canvas->zoomFit();

}

QList<CVisSystemCanvasNodeItem*> CVisSystemCanvasNodeLayoutData::getRootNodes()
{
    QList<CVisSystemCanvasNodeItem*> allNodes = this->canvas->nodeItemList();
    QList<CVisSystemCanvasNodeItem*> retList;

    Q_FOREACH(CVisSystemCanvasNodeItem* node, allNodes)
    {
        bool foundInput = false;
        for(int i=0; i<node->connectionItemCount(); i++)
        {
            CVisSystemCanvasConnectionItem* cItem = node->connectionItem(i);
            if(cItem->senderNodeItem() == node)
                continue;

            IVisSystemNode* senderNode = cItem->senderNode();
            IVisSystemNodeDesc* senderNodeDesc = senderNode->nodeDesc();
            IVisSystemNodeConnectionPath* cPath = senderNodeDesc->connectionPath(cItem->senderPathIndex());
            if(cPath->pathType() == IVisSystemNodeConnectionPath::OutputPath)
            {
                foundInput = true;
                break;
            }
        }

        if(!foundInput)
            retList.append(node);
    }

    return retList;
}

void CVisSystemCanvasNodeLayoutData::layoutNodeItem(CVisSystemCanvasNodeItem* nodeItem)
{
    QList<CVisSystemCanvasNodeItem*> inputNodes;
    QList<CVisSystemCanvasNodeItem*> controlNodes;

    if(visitedNodes.contains(nodeItem))
        return;

    if(visitingNodes.contains(nodeItem))
        return;

    // Mark that we are visiting this nodeItem right now.
    visitingNodes.append(nodeItem);

    // First place all the nodeItems from which we are taking input.
    for(int i=0; i<nodeItem->connectionItemCount(); i++)
    {
        CVisSystemCanvasConnectionItem* conItem = nodeItem->connectionItem(i);
        if(conItem->receiverNodeItem() == nodeItem && conItem->senderNodeItem() != nodeItem)
        {
            if(isControlPath(conItem))
            {
                if(visitingNodes.contains(conItem->senderNodeItem()) || visitedNodes.contains(conItem->senderNodeItem()))
                    continue;

                controlNodes.append(conItem->senderNodeItem());
                visitingNodes.append(conItem->senderNodeItem());
            }
            else
            {
                inputNodes.append(conItem->senderNodeItem());
                layoutNodeItem(conItem->senderNodeItem());
            }
        }
    }
    
    // Find out the bounding rectangle of all the input nodes.
    QRectF inputNodeRect;
    for(int i=0; i<inputNodes.count(); i++)
    {
        CVisSystemCanvasNodeItem* inputNode = inputNodes[i];
        QRectF itemRect = inputNode->boundingRect();
        itemRect = inputNode->mapToScene(itemRect).boundingRect();
        inputNodeRect = inputNodeRect.united(itemRect);
    }

    // nodeItem should surely be outside the boundary of inputNodeRect;
    QRectF nodeItemRect = nodeItem->boundingRect();
    nodeItemRect = nodeItem->mapToScene(nodeItemRect).boundingRect();
    QPointF nodeItemPos = QPointF(inputNodeRect.center().x(), inputNodeRect.bottom());
    nodeItemPos.setY(nodeItemPos.y() + inputNodeRect.height()/2 + nodeItemRect.height()/2);
    nodeItemRect.moveTopLeft( nodeItemRect.topLeft() + (nodeItemPos - nodeItemRect.center()) );
    nodeItemRect = adjustRect(nodeItemRect);
    nodeItemPos = nodeItemRect.center();

    // Move the node now.
    nodeItem->setPos(nodeItemPos);
    occupiedRectList.append(nodeItemRect);

    // Move control nodes
    for(int i=0; i<controlNodes.count(); i++)
    {
        CVisSystemCanvasNodeItem* controlNode = controlNodes[i];
        QRectF itemRect = controlNode->boundingRect();
        itemRect = controlNode->mapToScene(itemRect).boundingRect();
        itemRect.moveLeft(nodeItemRect.left() - 1.5f*itemRect.width());
        itemRect.moveTop(nodeItemRect.top());
        controlNode->setPos(itemRect.center());
        visitingNodes.removeAll(controlNode);
        visitedNodes.append(controlNode);
    }

    // Mark that we are leaving this nodeItem right now.
    visitingNodes.removeAll(nodeItem);

    // We have visited and positioned this nodeItem properly.
    visitedNodes.append(nodeItem);

#ifdef SHOW_NODE_PLACEMENT_ANIMATION
    // Wait for a little while and see the canvas being placed.
    QTime time = QTime::currentTime();
    while(1)
    {
        QCoreApplication::processEvents();
        QTime t = QTime::currentTime();
        if(qAbs(time.secsTo(t)) >= 1)
            break;
    }
#endif

}

void CVisSystemCanvasNodeLayoutData::centerNodeItem(CVisSystemCanvasNodeItem* nodeItem)
{
    QList<CVisSystemCanvasNodeItem*> outputNodes;

    if(visitedNodes.contains(nodeItem))
        return;
    if(visitingNodes.contains(nodeItem))
        return;

    visitingNodes.append(nodeItem);

    for(int i=0; i<nodeItem->connectionItemCount(); i++)
    {
        CVisSystemCanvasConnectionItem* conItem = nodeItem->connectionItem(i);
        if(conItem->senderNodeItem() == nodeItem && conItem->receiverNodeItem() != nodeItem)
        {
            if(isControlPath(conItem))
                continue;

            outputNodes.append(conItem->receiverNodeItem());
            centerNodeItem(conItem->receiverNodeItem());
        }
    }

    if(outputNodes.count() <= 1)
    {
        visitingNodes.removeAll(nodeItem);
        visitedNodes.append(nodeItem);
        return;
    }

    // Find out the bounding rectangle of all the output nodes.
    QRectF outputNodeRect;
    for(int i=0; i<outputNodes.count(); i++)
    {
        CVisSystemCanvasNodeItem* outputNode = outputNodes[i];
        QRectF itemRect = outputNode->boundingRect();
        itemRect = outputNode->mapToScene(itemRect).boundingRect();
        outputNodeRect = outputNodeRect.united(itemRect);
    }

    // node should be to the center of output nodes.
    QPointF itemPos = nodeItem->pos();
    itemPos.setX( outputNodeRect.center().x() );
    nodeItem->setPos(itemPos);

    // All done.
    visitingNodes.removeAll(nodeItem);
    visitedNodes.append(nodeItem);

#ifdef SHOW_NODE_PLACEMENT_ANIMATION
    // Wait for a little while and see the canvas being placed.
    QTime time = QTime::currentTime();
    while(1)
    {
        QCoreApplication::processEvents();
        QTime t = QTime::currentTime();
        if(qAbs(time.secsTo(t)) >= 1)
            break;
    }
#endif
}

QRectF CVisSystemCanvasNodeLayoutData::adjustRect(QRectF rect)
{
    QRectF intersectingRect;
    QRectF adjustedRect = rect;
    for(int i=0; i<occupiedRectList.count(); i++)
    {
        if(occupiedRectList[i].intersects(rect))
        {
            intersectingRect = occupiedRectList[i];
            break;
        }
    }

    if(intersectingRect.isNull())
        return adjustedRect;

    // Move the rectangle to the left.
    adjustedRect.moveLeft(intersectingRect.left() - adjustedRect.width() - 20);

    // Adjust again
    adjustedRect = adjustRect(adjustedRect);

    return adjustedRect;
}

bool CVisSystemCanvasNodeLayoutData::isControlPath(CVisSystemCanvasConnectionItem* conItem)
{
    if(!conItem)
        return false;

    IVisSystemNode* node = conItem->senderNode();
    int pathIndex = conItem->senderPathIndex();
    if(!node || pathIndex < 0)
        return false;

    IVisSystemNodeDesc* nodeDesc = node->nodeDesc();
    if(!nodeDesc)
        return false;

    IVisSystemNodeConnectionPath* path = nodeDesc->connectionPath(pathIndex);
    if(!path)
        return false;

    return path->pathType() == IVisSystemNodeConnectionPath::ControlPath;
}

///////////////////////////////////////////////////////////////////////////////
// TreeNode
///////////////////////////////////////////////////////////////////////////////

TreeNode* TreeNode::FindTreeNode(CVisSystemCanvasNodeItem* item)
{
    Q_FOREACH(TreeNode* n, TreeNode::GlobalTreeNodeList)
    {
        if(n->nodeItem() == item)
            return n;
    }

    return 0;
}

TreeNode::TreeNode(CVisSystemCanvasNodeItem* item, TreeNode* parent)
{
    m_parent = parent;
    if(m_parent)
        m_parent->m_childNodeList.append(this);
    m_item = item;
    m_layoutDone = false;

    // Register in the global tree node
    TreeNode::GlobalTreeNodeList.append(this);

    for(int i=0; i<item->connectionItemCount(); i++)
    {
        CVisSystemCanvasConnectionItem* cItem = item->connectionItem(i);
        if(cItem->receiverNodeItem() == item)
            continue;

        IVisSystemNode* receiverNode = cItem->receiverNode();
        IVisSystemNodeDesc* receiverNodeDesc = receiverNode->nodeDesc();
        IVisSystemNodeConnectionPath* cPath = receiverNodeDesc->connectionPath(cItem->receiverPathIndex());
        if(cPath->pathType() == IVisSystemNodeConnectionPath::InputPath)
        {
            TreeNode* nPtr = TreeNode::FindTreeNode(cItem->receiverNodeItem());
            if(nPtr)
                m_childNodeList.append(nPtr);
            else
                new TreeNode(cItem->receiverNodeItem(), this);
        }
    }
}

TreeNode::~TreeNode() 
{

}

CVisSystemCanvasNodeItem* TreeNode::nodeItem() const
{
    return m_item;
}

QRectF TreeNode::childrenBoundingRect() const 
{
    QRectF ret;

    for(int i=0; i<m_childNodeList.count(); i++)
    {
        TreeNode* childNode = m_childNodeList.at(i);
        if(i == 0)
            ret = childNode->boundingRect();
        else
            ret |= childNode->boundingRect();
    }

    return ret;
}

QRectF TreeNode::boundingRect() const 
{
    QRectF cbr = this->childrenBoundingRect();
    QRectF ret = m_item->mapToScene(m_item->boundingRect()).boundingRect();
    ret |= cbr;
    return ret;
}

TreeNode* TreeNode::parent()
{
    return m_parent;
}

QList<TreeNode*> TreeNode::childNodes()
{
    return m_childNodeList;
}

QRectF TreeNode::layout(const QRectF& occupiedRect)
{
    if(m_layoutDone)
        return boundingRect();

    // First layout the children
    QRectF localOccupiedRect;
    for(int i=0; i<m_childNodeList.count(); i++)
    {
        TreeNode* n = m_childNodeList.at(i);

        QRectF nRect = n->layout(localOccupiedRect);

        localOccupiedRect |= nRect;
    }

    // Now place this node above chilren, but
    // in the center of their bounding rectangle.
    qreal x = localOccupiedRect.center().x();
    qreal y = localOccupiedRect.top() -50;
    m_item->setPos(x, y);

    // Move this sub-tree such that it doesnt come within the already
    // occupied rectangle in the scene.
    localOccupiedRect |= m_item->mapToScene(m_item->boundingRect()).boundingRect();
    if(!occupiedRect.isNull() && localOccupiedRect.intersects(occupiedRect))
    {
        QPointF myCenter = localOccupiedRect.center();
        QPointF globalCenter = occupiedRect.center();
        qreal expectedDist = localOccupiedRect.width()/2 + occupiedRect.width()/2;
        qreal currentDist = myCenter.x() - globalCenter.x();
        this->translate(expectedDist-currentDist, 0);
        localOccupiedRect.translate(expectedDist-currentDist, 0);
    }

    m_layoutDone = true;

    return localOccupiedRect;
}

void TreeNode::translate(qreal x, qreal y)
{
    Q_FOREACH(TreeNode* n, m_childNodeList)
        n->translate(x, y);

    m_item->setPos( m_item->pos() + QPointF(x,y) );
}

void TreeNode::dumpTree(int indent)
{
    IVisSystemNode* node = m_item->visSystemNode();
    qDebug() << QString(indent, ' ') << node->nodeName() << " - [" 
             << node->nodeDesc()->nodeClassName() << "]";

    Q_FOREACH(TreeNode* n, m_childNodeList)
        n->dumpTree(indent+2);
}

