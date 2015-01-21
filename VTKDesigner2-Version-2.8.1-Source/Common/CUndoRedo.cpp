/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore. Details of those terms
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

#include "CUndoRedo.h"
#include <QMap>
#include <QStack>

struct CUndoRedoStackData
{
    QStack<QUndoCommand*> undoStack;
    QStack<QUndoCommand*> redoStack;
    QMap< QObject*, QList<CUndoRedoCmdBase*> > objectCmdMap;
    QList< QObject* > invalidPtrList;
};

CUndoRedoStack::CUndoRedoStack(QObject* parent)
:QObject(parent)
{
    d = new CUndoRedoStackData;
}

CUndoRedoStack::~CUndoRedoStack()
{
    clear();
    delete d;
}

void CUndoRedoStack::push(QUndoCommand* cmd)
{
    if(!cmd)
        return;

    // First register the undo command into the undo stack.
    QUndoCommand* topCmd = d->undoStack.count() ? d->undoStack.top() : 0;
    if(!topCmd)
    {
        d->undoStack.push(cmd);
        CUndoRedoCmdBase* cmd2 = dynamic_cast<CUndoRedoCmdBase*>(cmd);
        if(cmd2)
            addCommand(cmd2);
    }
    else
    {
        if(topCmd->id() == cmd->id())
        {
            bool success = topCmd->mergeWith(cmd);
            if(!success)
            {
                d->undoStack.push(cmd);
                CUndoRedoCmdBase* cmd2 = dynamic_cast<CUndoRedoCmdBase*>(cmd);
                if(cmd2)
                    addCommand(cmd2);
            }
            else
                delete cmd;
        }
    }

    // Now clear the redo stack.
    for(int i=0; i<d->redoStack.count(); i++)
    {
        QUndoCommand* cmd = d->redoStack.pop();
        CUndoRedoCmdBase* cmd2 = dynamic_cast<CUndoRedoCmdBase*>(cmd);
        if(cmd2)
            removeCommand(cmd2);
        delete cmd;
    }

    // emit change notifications.
    emit canUndoChanged(d->undoStack.count());
    emit canRedoChanged(d->redoStack.count());
    emit countChanged(count());
}

void CUndoRedoStack::remove(QUndoCommand* cmd)
{
    if(!cmd)
        return;

    int inUndo = d->undoStack.indexOf(cmd);
    int inRedo = inUndo >= 0 ? -1 : d->redoStack.indexOf(cmd);

    CUndoRedoCmdBase* cmd2 = dynamic_cast<CUndoRedoCmdBase*>(cmd);
    if(cmd2)
        removeCommand(cmd2);

    if(inUndo >= 0)
    {
        d->undoStack.remove(inUndo);
        emit canUndoChanged(d->undoStack.count());
    }
    else if(inRedo >= 0)
    {
        d->redoStack.remove(inRedo);
        emit canRedoChanged(d->redoStack.count());
    }

    emit countChanged(count());
}

int CUndoRedoStack::count() const
{
    return d->undoStack.count() + d->redoStack.count();
}

bool CUndoRedoStack::canUndo() const
{
    return d->undoStack.count() >= 0;
}

bool CUndoRedoStack::canRedo() const
{
    return d->redoStack.count() >= 0;
}

QString CUndoRedoStack::undoText() const
{
    QUndoCommand* topCmd = d->undoStack.count() ? d->undoStack.top() : 0;
    if(topCmd)
        return topCmd->text();

    return QString();
}

QString CUndoRedoStack::redoText() const
{
    QUndoCommand* topCmd = d->redoStack.count() ? d->redoStack.top() : 0;
    if(topCmd)
        return topCmd->text();

    return QString();
}

void CUndoRedoStack::clear()
{
    qDeleteAll(d->undoStack);
    qDeleteAll(d->redoStack);
    d->undoStack.clear();
    d->redoStack.clear();

    QList<QObject*> objects = d->objectCmdMap.keys();
    for(int i=0; i<objects.count(); i++)
    {
        if(d->invalidPtrList.contains( objects[i] ) )
            continue;
        disconnect(objects[i], 0, this, 0);
    }
    d->objectCmdMap.clear();

    emit stackCleared();
    emit canUndoChanged(false);
    emit canRedoChanged(false);
    emit countChanged(count());
}

void CUndoRedoStack::undo()
{
    QUndoCommand* topCmd = d->undoStack.count() ? d->undoStack.pop() : 0;
    if(!topCmd)
        return;

    QString cmdText = topCmd->text();
    topCmd->undo();
    d->redoStack.push(topCmd);
    qWarning("Undo: %s", qPrintable(cmdText));

    // emit change notifications.
    emit undone(cmdText);
    emit canUndoChanged(d->undoStack.count());
    emit canRedoChanged(d->redoStack.count());
}

void CUndoRedoStack::redo()
{
    QUndoCommand* topCmd = d->redoStack.count() ? d->redoStack.pop() : 0;
    if(!topCmd)
        return;

    QString cmdText = topCmd->text();
    topCmd->redo();
    d->undoStack.push(topCmd);
    qWarning("Redo: %s", qPrintable(cmdText));

    // emit change notifications.
    emit redone(cmdText);
    emit canUndoChanged(d->undoStack.count());
    emit canRedoChanged(d->redoStack.count());
}

void CUndoRedoStack::addCommand(CUndoRedoCmdBase* cmd)
{
    if(!cmd)
        return;

    cmd->setUndoRedoStack(this);

    QList<QObject*> objects = cmd->referencedObjects();
    for(int i=0; i<objects.count(); i++)
    {
        QObject* obj = objects[i];
        if(!obj)
            continue;

        bool connected = d->objectCmdMap.contains(obj);
        d->objectCmdMap[obj].append( cmd );
        if(!connected)
            connect(obj, SIGNAL(destroyed(QObject*)), this, SLOT(on_object_destroyed(QObject*)));
    }
}

void CUndoRedoStack::removeCommand(CUndoRedoCmdBase* cmd)
{
    QMap< QObject*, QList<CUndoRedoCmdBase*> >::iterator it = d->objectCmdMap.begin();
    QMap< QObject*, QList<CUndoRedoCmdBase*> >::iterator end = d->objectCmdMap.begin();
    QList<QObject*> toRemoveList;
    while( it != end )
    {
        if( it.value().contains(cmd) )
        {
            QList<CUndoRedoCmdBase*> & cmdList = d->objectCmdMap[ it.key() ];
            cmdList.removeAll(cmd);
            if(!cmdList.count())
            {
                disconnect( it.key(), 0, this, 0 );
                toRemoveList.append( it.key() );
            }
        }
        ++it;
    }

    for(int i=0; i<toRemoveList.count(); i++)
        d->objectCmdMap.remove( toRemoveList[i] );

    cmd->setUndoRedoStack(0);
}

void CUndoRedoStack::notifyObjectPointerChange(QObject* oldPtr, QObject* newPtr)
{
    if(!oldPtr || !newPtr)
        return;

    if(!d->objectCmdMap.contains(oldPtr))
        return;

    QList<CUndoRedoCmdBase*> cmdList = d->objectCmdMap[oldPtr];
    for(int i=0; i<cmdList.count(); i++)
        cmdList[i]->referenceObjectChanged( oldPtr, newPtr );

    d->objectCmdMap.remove(oldPtr);
    d->invalidPtrList.removeAll(oldPtr);

    bool connected = d->objectCmdMap.contains(newPtr);
    if(connected)
        d->objectCmdMap[newPtr] += cmdList;
    else
        d->objectCmdMap[newPtr] = cmdList;
    if(!connected)
        connect(newPtr, SIGNAL(destroyed(QObject*)), this, SLOT(on_object_destroyed(QObject*)));
}

void CUndoRedoStack::on_object_destroyed(QObject* obj)
{
    if(!d->objectCmdMap.contains(obj))
        return;

    const QList<CUndoRedoCmdBase*>& cmdList = d->objectCmdMap[obj];
    for(int i=0; i<cmdList.count(); i++)
        cmdList[i]->referenceObjectDestroyed( obj );

    d->invalidPtrList.append(obj);
}

