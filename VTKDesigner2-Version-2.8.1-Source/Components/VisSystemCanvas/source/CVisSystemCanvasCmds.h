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

#ifndef C_VIS_SYSTEM_CANVAS_CMDS_H
#define C_VIS_SYSTEM_CANVAS_CMDS_H

#include "CUndoRedo.h"
#include <QPointF>
#include <QVariant>

// Common stuff
class IVisNetwork;
class IVisSystemNodeFactoryRegistry;
class IQObjectLoadSave;
class IPropertyEditor;
class IVisNetworkCanvas;
class CVisSystemCanvas;

class CCanvasUndoRedoCmd
{
public:
    static CVisSystemCanvas* Canvas;
    static IVisNetworkCanvas* VisNetworkCanvas;
    static IVisNetwork* VisNetwork;
    static IVisSystemNodeFactoryRegistry* NodeFactoryRegistry;
    static IQObjectLoadSave* QObjectLoadSave;
    static IPropertyEditor* PropertyEditor;
};

// Node move command.
class CVisSystemCanvasNodeItem;
class CNodeMoveCmd : public CUndoRedoCmdBase
{
public:
    CNodeMoveCmd(CVisSystemCanvasNodeItem* item, QPointF oldPos, QPointF newPos);
    ~CNodeMoveCmd();

    int id();
    bool mergeWith(const QUndoCommand* cmd);
    void undo();
    void redo();

protected:
    void referenceObjectDestroyed(QObject* obj);
    void referenceObjectChanged(QObject* oldPtr, QObject* newPtr);

private:
    CVisSystemCanvasNodeItem* mItemPtr;
    QObject* mOldPtr;
    QPointF mOldPos;
    QPointF mNewPos;
};

// Property Change command
class CNodePropertyChangeCmd : public CUndoRedoCmdBase
{
public:
    CNodePropertyChangeCmd(QObject* object, QString propName, QVariant oldValue, QVariant newValue);
    ~CNodePropertyChangeCmd();

    int id();
    bool mergeWith(const QUndoCommand* cmd);
    void undo();
    void redo();

protected:
    void referenceObjectDestroyed(QObject* obj);
    void referenceObjectChanged(QObject* oldPtr, QObject* newPtr);
    void cachePropertyInfo();
    void setPropertyValue(QVariant value);

private:
    QObject* mObjectPtr;
    QObject* mOldPtr;
    QString mPropName;
    QVariant mOldValue;
    QVariant mNewValue;
    int mPropIndex;
};

// Node name change event.
class IVisSystemNode;
class CNodeNameChangeCmd : public CUndoRedoCmdBase
{
public:
    CNodeNameChangeCmd(IVisSystemNode* node, QString oldName, QString newName);
    ~CNodeNameChangeCmd();

    int id();
    bool mergeWith(const QUndoCommand* cmd);
    void undo();
    void redo();

protected:
    void referenceObjectDestroyed(QObject* obj);
    void referenceObjectChanged(QObject* oldPtr, QObject* newPtr);

private:
    QObject* mOldPtr;
    IVisSystemNode* mNodePtr;
    QString mOldName;
    QString mNewName;
};

// Node connect/disconnect event
class CNodeConnectionCmd : public CUndoRedoCmdBase
{
public:
    CNodeConnectionCmd(IVisSystemNode* sender, int outLine, IVisSystemNode* receiver, int inLine);
    ~CNodeConnectionCmd();

    int id();
    bool mergeWith(const QUndoCommand* cmd);
    void undo();
    void redo();

protected:
    void referenceObjectDestroyed(QObject* obj);
    void referenceObjectChanged(QObject* oldPtr, QObject* newPtr);

private:
    IVisSystemNode* mSenderNode;
    int mOutLine;
    IVisSystemNode* mReceiverNode;
    int mInLine;
    QObject* mSenderObj;
    QObject* mReceiverObj;
};

// Node disconnect/connect event
class CNodeDisconnectionCmd : public CUndoRedoCmdBase
{
public:
    CNodeDisconnectionCmd(IVisSystemNode* sender, int outLine, IVisSystemNode* receiver, int inLine);
    ~CNodeDisconnectionCmd();

    int id();
    bool mergeWith(const QUndoCommand* cmd);
    void undo();
    void redo();

protected:
    void referenceObjectDestroyed(QObject* obj);
    void referenceObjectChanged(QObject* oldPtr, QObject* newPtr);

private:
    IVisSystemNode* mSenderNode;
    int mOutLine;
    IVisSystemNode* mReceiverNode;
    int mInLine;
    QObject* mSenderObj;
    QObject* mReceiverObj;
};

// Node deletion event
class CNodeDeleteCmd : public CUndoRedoCmdBase
{
public:
    CNodeDeleteCmd(IVisSystemNode* node);
    ~CNodeDeleteCmd();

    int id();
    bool mergeWith(const QUndoCommand* cmd);
    void undo();
    void redo();

protected:
    void referenceObjectDestroyed(QObject* obj);
    void referenceObjectChanged(QObject* oldPtr, QObject* newPtr);

private:
    IVisSystemNode* mNode;
    QObject* mNodeObject;
    QString mNodeXml;
    QString mNodeClassName;
};

// Node creation event
class CNodeCreateCmd : public CUndoRedoCmdBase
{
public:
    CNodeCreateCmd(IVisSystemNode* node);
    ~CNodeCreateCmd();

    int id();
    bool mergeWith(const QUndoCommand* cmd);
    void undo();
    void redo();

protected:
    void referenceObjectDestroyed(QObject* obj);
    void referenceObjectChanged(QObject* oldPtr, QObject* newPtr);

private:
    IVisSystemNode* mNode;
    QObject* mNodeObject;
    QString mNodeClassName;
    QPointF mNodePos;
};


#endif
