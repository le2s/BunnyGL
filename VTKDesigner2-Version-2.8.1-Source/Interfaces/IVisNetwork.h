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

#ifndef I_VIS_NETWORK_H
#define I_VIS_NETWORK_H

#include <GCF/IContainer>

class IVisSystemNode;
class IVisNetworkConnection;
class IVisNetworkEventListener;

class IVisNetwork : virtual public IContainer
{
public:
    // Node query methods.
    virtual void                addNode(IVisSystemNode* node) = 0;
    virtual void                removeNode(IVisSystemNode* node) = 0;
    virtual void                removeAllNodes() = 0;
    virtual int                 nodeCount() = 0;
    virtual IVisSystemNode*     node(int index) = 0;
    virtual bool                hasNode(IVisSystemNode* node) = 0;

    // Node connection methods.
    virtual bool                connectNode(IVisSystemNode* sender, IVisSystemNode* receiver) = 0;
    virtual bool                connectNode(IVisSystemNode* sender, int outLine, IVisSystemNode* receiver, int inLine, IVisNetworkConnection** connection) = 0;
    virtual bool                disconnectNode(IVisSystemNode* sender, int outLine, IVisSystemNode* receiver, int inLine) = 0;
    virtual bool                disconnectNode(IVisNetworkConnection* connection) = 0;
    virtual bool                disconnectNode(IVisSystemNode* sender, IVisSystemNode* receiver) = 0;
    virtual bool                disconnectNode(IVisSystemNode* node) = 0;
    virtual int                 connectionCount() = 0;
    virtual IVisNetworkConnection* connection(int index) = 0;
    virtual bool                hasConnection(IVisNetworkConnection* con) = 0;

    // Must be implemented as signals.
    virtual void                nodeAdded(IVisSystemNode* node) = 0;
    virtual void                nodeRemoved(IVisSystemNode* node) = 0;
    virtual void                connectionCreated(IVisNetworkConnection* connection) = 0;
    virtual void                connectionDestroyed(IVisNetworkConnection* connection) = 0;
};

Q_DECLARE_INTERFACE(IVisNetwork, "com.vcreatelogic.IVisNetwork/1.0")

#endif
