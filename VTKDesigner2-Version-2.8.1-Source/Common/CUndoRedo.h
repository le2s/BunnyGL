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

#ifndef C_UNDO_REDO_H
#define C_UNDO_REDO_H

#include <QUndoCommand>

class CUndoRedoCmdBase;

struct CUndoRedoStackData;
class CUndoRedoStack : public QObject
{
    Q_OBJECT

public:
    CUndoRedoStack(QObject* parent=0);
    ~CUndoRedoStack();

    void push(QUndoCommand* cmd);
    void remove(QUndoCommand* cmd);
    int count() const;

    bool canUndo() const;
    bool canRedo() const;
    QString undoText() const;
    QString redoText() const;

public slots:
    void clear();
    void undo();
    void redo();

signals:
    void canUndoChanged(bool val);
    void canRedoChanged(bool val);
    void stackCleared();
    void undone(const QString& text);
    void redone(const QString& text);
    void countChanged(int);

protected:
    void addCommand(CUndoRedoCmdBase* cmd);
    void removeCommand(CUndoRedoCmdBase* cmd);
    void notifyObjectPointerChange(QObject* oldPtr, QObject* newPtr);

protected slots:
    void on_object_destroyed(QObject* obj);

private:
    friend class CUndoRedoCmdBase;
    CUndoRedoStackData* d;
};

class CUndoRedoCmdBase : public QUndoCommand
{
protected:
    CUndoRedoCmdBase() { mUndoRedoStack = 0; }
    virtual ~CUndoRedoCmdBase() { mUndoRedoStack = 0; }

    QList<QObject*> referencedObjects() { return mRefObjList; }
    void setReferencedObjects(const QList<QObject*>& refObjList) { mRefObjList = refObjList; }

    virtual void referenceObjectDestroyed(QObject* obj) { Q_UNUSED(obj); }
    virtual void referenceObjectChanged(QObject* oldPtr, QObject* newPtr) { Q_UNUSED(oldPtr); Q_UNUSED(newPtr); }

protected:
    void setUndoRedoStack(CUndoRedoStack* stack) { mUndoRedoStack = stack; }
    CUndoRedoStack* undoRedoStack() const { return mUndoRedoStack; }
    void notifyObjectPointerChange(QObject* oldPtr, QObject* newPtr) {
        if(mUndoRedoStack)
            mUndoRedoStack->notifyObjectPointerChange(oldPtr, newPtr);
    }

private:
    friend class CUndoRedoStack;
    QList<QObject*> mRefObjList;
    CUndoRedoStack* mUndoRedoStack;
};

#endif
