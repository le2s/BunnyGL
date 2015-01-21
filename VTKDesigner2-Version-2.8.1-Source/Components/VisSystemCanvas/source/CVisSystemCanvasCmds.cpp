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

#include "CVisSystemCanvasCmds.h"
#include "IEditableProperties.h"
#include "IPropertyEditor.h"
#include "IVisNetwork.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IQObjectLoadSave.h"
#include "IVisSystemNode.h"
#include "IVisSystemNodeDesc.h"
#include "CVisSystemCanvas.h"

#include <QMetaObject>

CVisSystemCanvas* CCanvasUndoRedoCmd::Canvas = 0;
IVisNetworkCanvas* CCanvasUndoRedoCmd::VisNetworkCanvas = 0;
IVisNetwork* CCanvasUndoRedoCmd::VisNetwork = 0;
IVisSystemNodeFactoryRegistry* CCanvasUndoRedoCmd::NodeFactoryRegistry = 0;
IQObjectLoadSave* CCanvasUndoRedoCmd::QObjectLoadSave = 0;
IPropertyEditor* CCanvasUndoRedoCmd::PropertyEditor = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNodeMoveCmd implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CVisSystemCanvasNodeItem.h"
#include "IVisSystemNode.h"

CNodeMoveCmd::CNodeMoveCmd(CVisSystemCanvasNodeItem* item, QPointF oldPos, QPointF newPos)
: mItemPtr(item), mOldPtr(item), mOldPos(oldPos), mNewPos(newPos)
{
    setReferencedObjects( QList<QObject*>() << mOldPtr );

    setText( QString("%1 Node movement").arg(item->visSystemNode()->nodeName()) );
}

CNodeMoveCmd::~CNodeMoveCmd()
{

}

int CNodeMoveCmd::id()
{
    return 1;
}

bool CNodeMoveCmd::mergeWith(const QUndoCommand* cmd)
{
    const CNodeMoveCmd* cmd2 = dynamic_cast<const CNodeMoveCmd*>(cmd);
    if(!cmd2)
        return false;

    if(cmd2->mItemPtr == mItemPtr)
    {
        mNewPos = cmd2->mNewPos;
        return true;
    }

    return false;
}

void CNodeMoveCmd::undo()
{
    if(!mItemPtr)
        return;

    mItemPtr->setPos( mOldPos );
}

void CNodeMoveCmd::redo()
{
    if(!mItemPtr)
        return;

    mItemPtr->setPos( mNewPos );
}

void CNodeMoveCmd::referenceObjectDestroyed(QObject* obj)
{
    if(!mItemPtr || mOldPtr != obj)
        return;

    mItemPtr = 0;
}

void CNodeMoveCmd::referenceObjectChanged(QObject* oldPtr, QObject* newPtr)
{
    if(mItemPtr)
        return;

    if(mOldPtr != oldPtr)
        return;

    mItemPtr = qobject_cast<CVisSystemCanvasNodeItem*>(newPtr);
    mOldPtr = newPtr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNodePropertyChangeCmd implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNodePropertyChangeCmd::CNodePropertyChangeCmd(QObject* object, QString propName, QVariant oldValue, QVariant newValue)
: mObjectPtr(object), mOldPtr(object), mPropName(propName), mOldValue(oldValue), mNewValue(newValue), mPropIndex(-1)
{
    cachePropertyInfo();
    setReferencedObjects( QList<QObject*>() << mOldPtr );

    setText( QString("%1 Property Change").arg(object->objectName()) );
}

CNodePropertyChangeCmd::~CNodePropertyChangeCmd()
{

}

int CNodePropertyChangeCmd::id()
{
    return 2;
}

bool CNodePropertyChangeCmd::mergeWith(const QUndoCommand* cmd)
{
    const CNodePropertyChangeCmd* cmd2 = dynamic_cast<const CNodePropertyChangeCmd*>(cmd);
    if(!cmd2)
        return false;

    if(cmd2->mObjectPtr == mObjectPtr && cmd2->mPropName == mPropName)
    {
        mNewValue = cmd2->mNewValue;
        return true;
    }

    return false;
}

void CNodePropertyChangeCmd::undo()
{
    setPropertyValue( mOldValue );
}

void CNodePropertyChangeCmd::redo()
{
    setPropertyValue( mNewValue );
}

void CNodePropertyChangeCmd::referenceObjectDestroyed(QObject* obj)
{
    if(!mObjectPtr || mOldPtr != obj)
        return;

    mObjectPtr = 0;
}

void CNodePropertyChangeCmd::referenceObjectChanged(QObject* oldPtr, QObject* newPtr)
{
    if(mObjectPtr)
        return;

    if(mOldPtr != oldPtr)
        return;

    mObjectPtr = newPtr;
    mOldPtr = newPtr;

    cachePropertyInfo();
}

void CNodePropertyChangeCmd::cachePropertyInfo()
{
    mPropIndex = -1;

    const QMetaObject* mo = mObjectPtr->metaObject();
    int index = mo->indexOfProperty( qPrintable(mPropName) );
    if(index >= 0)
        return;

    IEditableProperties* eProps = qobject_cast<IEditableProperties*>(mObjectPtr);
    if(!eProps)
        return;

    for(int i=0; i<eProps->propertyCount(); i++)
    {
        if(eProps->propertyName(i) == mPropName)
        {
            mPropIndex = i;
            break;
        }
    }
}

void CNodePropertyChangeCmd::setPropertyValue(QVariant value)
{
    if(!mObjectPtr)
        return;

    if(mPropIndex >= 0)
    {
        IEditableProperties* eProps = qobject_cast<IEditableProperties*>(mObjectPtr);
        eProps->setPropertyValue(mPropIndex, value);
    }
    else
    {
        mObjectPtr->setProperty( qPrintable(mPropName), value );
    }

    if(CCanvasUndoRedoCmd::PropertyEditor && CCanvasUndoRedoCmd::PropertyEditor->object() == mObjectPtr)
    {
        CCanvasUndoRedoCmd::PropertyEditor->setObject(0);
        CCanvasUndoRedoCmd::PropertyEditor->setObject(mObjectPtr);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNodeNameChangeCmd implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNodeNameChangeCmd::CNodeNameChangeCmd(IVisSystemNode* node, QString oldName, QString newName)
:mNodePtr(node), mOldName(oldName), mNewName(newName)
{
    mOldPtr = node->containerObject();
    setReferencedObjects( QList<QObject*>() << mOldPtr );
}

CNodeNameChangeCmd::~CNodeNameChangeCmd()
{

}

int CNodeNameChangeCmd::id()
{
    return 3;
}

bool CNodeNameChangeCmd::mergeWith(const QUndoCommand* cmd)
{
    const CNodeNameChangeCmd* cmd2 = dynamic_cast<const CNodeNameChangeCmd*>(cmd);
    if(!cmd2)
        return false;

    if(cmd2->mNodePtr == mNodePtr)
    {
        mNewName = cmd2->mNewName;
        return true;
    }

    return false;  
}

void CNodeNameChangeCmd::undo()
{
    if(!mNodePtr)
        return;

    mNodePtr->setNodeName(mOldName);
}

void CNodeNameChangeCmd::redo()
{
    if(!mNodePtr)
        return;

    mNodePtr->setNodeName(mNewName);
}

void CNodeNameChangeCmd::referenceObjectDestroyed(QObject* obj)
{
    if(!mNodePtr || mOldPtr != obj)
        return;

    mNodePtr = 0;
}

void CNodeNameChangeCmd::referenceObjectChanged(QObject* oldPtr, QObject* newPtr)
{
    if(mNodePtr)
        return;

    if(mOldPtr != oldPtr)
        return;

    mNodePtr = qobject_cast<IVisSystemNode*>(newPtr);
    if(mNodePtr)
        mOldPtr = newPtr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNodeConnectionCmd implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNodeConnectionCmd::CNodeConnectionCmd(IVisSystemNode* sender, int outLine, IVisSystemNode* receiver, int inLine)
:mSenderNode(sender), mOutLine(outLine), mReceiverNode(receiver), mInLine(inLine)
{
    mSenderObj = sender->containerObject();
    mReceiverObj = receiver->containerObject();

    setReferencedObjects( QList<QObject*>() << mSenderObj << mReceiverObj );
}

CNodeConnectionCmd::~CNodeConnectionCmd()
{

}

int CNodeConnectionCmd::id()
{
    return 4;
}

bool CNodeConnectionCmd::mergeWith(const QUndoCommand* cmd)
{
    Q_UNUSED(cmd);
    return false;
}

void CNodeConnectionCmd::undo()
{
    if(!CCanvasUndoRedoCmd::VisNetwork || !mSenderNode || !mReceiverNode || mOutLine < 0 || mInLine < 0)
        return;

    CCanvasUndoRedoCmd::VisNetwork->disconnectNode(mSenderNode, mOutLine, mReceiverNode, mInLine);
}

void CNodeConnectionCmd::redo()
{
    if(!CCanvasUndoRedoCmd::VisNetwork || !mSenderNode || !mReceiverNode || mOutLine < 0 || mInLine < 0)
        return;

    IVisNetworkConnection* con = 0;
    CCanvasUndoRedoCmd::VisNetwork->connectNode(mSenderNode, mOutLine, mReceiverNode, mInLine, &con);
}

void CNodeConnectionCmd::referenceObjectDestroyed(QObject* obj)
{
    if( obj == mSenderObj )
        mSenderNode = 0;
    else if( obj == mReceiverObj )
        mReceiverNode = 0;
}

void CNodeConnectionCmd::referenceObjectChanged(QObject* oldPtr, QObject* newPtr)
{
    if(oldPtr == mSenderObj)
    {
        mSenderNode = qobject_cast<IVisSystemNode*>(newPtr);
        if(mSenderNode)
            mSenderObj = newPtr;
    }
    else if(oldPtr == mReceiverObj)
    {
        mReceiverNode = qobject_cast<IVisSystemNode*>(newPtr);
        if(mReceiverNode)
            mReceiverObj = newPtr;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNodeDisconnectionCmd implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNodeDisconnectionCmd::CNodeDisconnectionCmd(IVisSystemNode* sender, int outLine, IVisSystemNode* receiver, int inLine)
:mSenderNode(sender), mOutLine(outLine), mReceiverNode(receiver), mInLine(inLine)
{
    mSenderObj = sender->containerObject();
    mReceiverObj = receiver->containerObject();

    setReferencedObjects( QList<QObject*>() << mSenderObj << mReceiverObj );
}

CNodeDisconnectionCmd::~CNodeDisconnectionCmd()
{

}

int CNodeDisconnectionCmd::id()
{
    return 4;
}

bool CNodeDisconnectionCmd::mergeWith(const QUndoCommand* cmd)
{
    Q_UNUSED(cmd);
    return false;
}

void CNodeDisconnectionCmd::undo()
{
    if(!CCanvasUndoRedoCmd::VisNetwork || !mSenderNode || !mReceiverNode || mOutLine < 0 || mInLine < 0)
        return;

    IVisNetworkConnection* con = 0;
    CCanvasUndoRedoCmd::VisNetwork->connectNode(mSenderNode, mOutLine, mReceiverNode, mInLine, &con);
}

void CNodeDisconnectionCmd::redo()
{
    if(!CCanvasUndoRedoCmd::VisNetwork || !mSenderNode || !mReceiverNode || mOutLine < 0 || mInLine < 0)
        return;

    CCanvasUndoRedoCmd::VisNetwork->disconnectNode(mSenderNode, mOutLine, mReceiverNode, mInLine);
}

void CNodeDisconnectionCmd::referenceObjectDestroyed(QObject* obj)
{
    if( obj == mSenderObj )
        mSenderNode = 0;
    else if( obj == mReceiverObj )
        mReceiverNode = 0;
}

void CNodeDisconnectionCmd::referenceObjectChanged(QObject* oldPtr, QObject* newPtr)
{
    if(oldPtr == mSenderObj)
    {
        mSenderNode = qobject_cast<IVisSystemNode*>(newPtr);
        if(mSenderNode)
            mSenderObj = newPtr;
    }
    else if(oldPtr == mReceiverObj)
    {
        mReceiverNode = qobject_cast<IVisSystemNode*>(newPtr);
        if(mReceiverNode)
            mReceiverObj = newPtr;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNodeDeleteCmd implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNodeDeleteCmd::CNodeDeleteCmd(IVisSystemNode* node)
:mNode(0)
{
    // The node would get deleted immediately after this constructor returns.
    // So we need to save everything about this object right now.
    mNodeObject = node->containerObject();
    mNode = 0;

    mNodeClassName = node->nodeDesc()->nodeClassName();

    if(CCanvasUndoRedoCmd::QObjectLoadSave)
    {
        QDomDocument doc;
        QDomElement rootE = doc.createElement("NodeInfo");
        doc.appendChild(rootE);
        CCanvasUndoRedoCmd::QObjectLoadSave->saveProperties(mNodeObject, doc, rootE);

        CVisSystemCanvasNodeItem* nodeItem = CCanvasUndoRedoCmd::Canvas->nodeItem(node);
        if(nodeItem)
        {
            rootE.setAttribute("X", nodeItem->pos().x());
            rootE.setAttribute("Y", nodeItem->pos().y());
        }

        mNodeXml = doc.toString();
    }
}

CNodeDeleteCmd::~CNodeDeleteCmd()
{

}

int CNodeDeleteCmd::id()
{
    return 5;
}

bool CNodeDeleteCmd::mergeWith(const QUndoCommand* cmd)
{
    Q_UNUSED(cmd);
    return false;
}

void CNodeDeleteCmd::undo()
{
    if(mNode)
        return;

    if(CCanvasUndoRedoCmd::NodeFactoryRegistry)
    {
        mNode = CCanvasUndoRedoCmd::NodeFactoryRegistry->createNode(mNodeClassName);
        if(!mNode)
            return;

        QObject* nodeObj = mNode->containerObject();
        QDomDocument doc;
        doc.setContent( mNodeXml );

        if(CCanvasUndoRedoCmd::QObjectLoadSave)
            CCanvasUndoRedoCmd::QObjectLoadSave->loadProperties(nodeObj, doc, doc.documentElement());

        if(CCanvasUndoRedoCmd::VisNetwork)
            CCanvasUndoRedoCmd::VisNetwork->addNode(mNode);

        CVisSystemCanvasNodeItem* nodeItem = CCanvasUndoRedoCmd::Canvas->nodeItem(mNode);
        if(nodeItem)
        {
            QDomElement rootE = doc.documentElement();
            QPointF pos( rootE.attribute("X").toDouble(), rootE.attribute("Y").toDouble() );
            nodeItem->setPos( pos );
        }

        notifyObjectPointerChange(mNodeObject, nodeObj);
        mNodeObject = nodeObj;
    }
}

void CNodeDeleteCmd::redo()
{
    if(!mNode)
        return;

    if( CCanvasUndoRedoCmd::VisNetwork )
    {
        CCanvasUndoRedoCmd::VisNetwork->removeNode(mNode);
        mNode = 0;
    }
}

void CNodeDeleteCmd::referenceObjectDestroyed(QObject* obj)
{
    Q_UNUSED(obj);
}

void CNodeDeleteCmd::referenceObjectChanged(QObject* oldPtr, QObject* newPtr)
{
    Q_UNUSED(oldPtr);
    Q_UNUSED(newPtr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNodeDeleteCmd implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNodeCreateCmd::CNodeCreateCmd(IVisSystemNode* node)
:mNode(node)
{
    mNodeObject = node->containerObject();
    mNodeClassName = node->nodeDesc()->nodeClassName();

    CVisSystemCanvasNodeItem* nodeItem = CCanvasUndoRedoCmd::Canvas->nodeItem(node);
    mNodePos = nodeItem ? nodeItem->pos() : QPointF();
}

CNodeCreateCmd::~CNodeCreateCmd()
{

}

int CNodeCreateCmd::id()
{
    return 6;
}

bool CNodeCreateCmd::mergeWith(const QUndoCommand* cmd)
{
    Q_UNUSED(cmd);
    return false;
}

void CNodeCreateCmd::undo()
{
    if(!mNode)
        return;

    if(CCanvasUndoRedoCmd::VisNetwork)
    {
        CCanvasUndoRedoCmd::VisNetwork->removeNode(mNode);
        mNode = 0;
    }
}

void CNodeCreateCmd::redo()
{
    if(mNode)
        return;

    mNode = CCanvasUndoRedoCmd::NodeFactoryRegistry->createNode(mNodeClassName);
    if(mNode)
    {
        CCanvasUndoRedoCmd::VisNetwork->addNode(mNode);

        CVisSystemCanvasNodeItem* nodeItem = CCanvasUndoRedoCmd::Canvas->nodeItem(mNode);
        if(nodeItem)
            nodeItem->setPos(mNodePos);
    }
}

void CNodeCreateCmd::referenceObjectDestroyed(QObject* obj)
{
    Q_UNUSED(obj);
}

void CNodeCreateCmd::referenceObjectChanged(QObject* oldPtr, QObject* newPtr)
{
    Q_UNUSED(oldPtr);
    Q_UNUSED(newPtr);
}

