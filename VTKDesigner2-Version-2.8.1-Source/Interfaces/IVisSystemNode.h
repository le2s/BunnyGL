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

#ifndef I_VIS_SYSTEM_NODE_H
#define I_VIS_SYSTEM_NODE_H

#include <GCF/DataStore>
#include <GCF/IContainer>
#include <QStyleOption>

class IVisSystemNodeFactory;
class IVisSystemNodeDesc;
class IVisSystemNodeIOData;
class IVisSystemNodeControlData;
class IVisSystemNodeConnectionPath;

class IVisSystemNode : public virtual IContainer
{
public:
    // Meta information about the node
    virtual IVisSystemNodeDesc* nodeDesc() = 0;
    virtual void                setNodeName(QString name) = 0;
    virtual QString             nodeName() = 0;

    // Connection with the node factory that created this node.
    virtual IVisSystemNodeFactory* nodeFactory() = 0;

    // Generic Data Store for this node.
    virtual GCF::DataStore          nodeData() = 0;

    // Methods to establish connections.
    virtual bool                hasInput(IVisSystemNodeConnectionPath* path) = 0;
    virtual bool                setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData) = 0;
    virtual bool                removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData) = 0;
    virtual bool                fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData) = 0;
    virtual bool                outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData) = 0;

    // Methods to establish control path connections.
    virtual bool                fetchControlData(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData** ctrlData) = 0;
    virtual bool                connectControl(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData* ctrlData) = 0;
    virtual bool                disconnectControl(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData* ctrlData) = 0;

    // Methods to execute commands
    virtual bool                executeCommand(QString cmdName, QString& result) = 0;

    // Visual Display Hints and Mechanisms.
    virtual QRectF              nodeRect() = 0;
    virtual QRectF              pathRect(IVisSystemNodeConnectionPath* path) = 0;
    virtual void                paintNode(QPainter* p, QRectF rect, const QStyleOption& opt) = 0;
    virtual void                paintConnectionPath(IVisSystemNodeConnectionPath* path, QPainter* p, QRectF rect, const QStyleOption& opt) = 0;

    // Methods to initialize and destroy the node
    virtual void                initializeNode() = 0;
    virtual void                finalizeNode() = 0;

    // Must be implemented as signal
    virtual void                nodeNameChanged() = 0;
    virtual void                propertyChanged(const QString& name) = 0;
};

Q_DECLARE_INTERFACE(IVisSystemNode, "com.vcreatelogic.IVisSystemNode/1.0")

#endif
