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

#include "CVisNetworkConnection.h"

struct CVisNetworkConnectionData
{
    CVisNetworkConnectionData() :
        senderNode(0), receiverNode(0), senderPathIdx(-1),
        receiverPathIdx(-1), ioData(0), senderControlData(0),
        receiverControlData(0) { }

    IVisSystemNode* senderNode;
    IVisSystemNode* receiverNode;
    int senderPathIdx;
    int receiverPathIdx;
    IVisSystemNodeIOData* ioData;
    IVisSystemNodeControlData* senderControlData;
    IVisSystemNodeControlData* receiverControlData;
};

CVisNetworkConnection::CVisNetworkConnection()
{
    d = new CVisNetworkConnectionData;
}

CVisNetworkConnection::~CVisNetworkConnection()
{
    delete d;
}

void CVisNetworkConnection::setSenderNode(IVisSystemNode* node)
{
    d->senderNode = node;
}

void CVisNetworkConnection::setReceiverNode(IVisSystemNode* node)
{
    d->receiverNode = node;
}

void CVisNetworkConnection::setSenderPathIndex(int index)
{
    d->senderPathIdx = index;
}

void CVisNetworkConnection::setReceiverPathIndex(int index)
{
    d->receiverPathIdx = index;
}

void CVisNetworkConnection::setConnectionData(IVisSystemNodeIOData* data)
{
    d->ioData = data;
}

void CVisNetworkConnection::setSenderControlData(IVisSystemNodeControlData* data)
{
    d->senderControlData = data;
}

void CVisNetworkConnection::setReceiverControlData(IVisSystemNodeControlData* data)
{
    d->receiverControlData = data;
}


GCF_BEGIN_QUERY_TABLE(CVisNetworkConnection)
    GCF_IMPLEMENTS(IVisNetworkConnection)
GCF_END_QUERY_TABLE(CVisNetworkConnection)

IVisSystemNode* CVisNetworkConnection::senderNode()
{
    return d->senderNode;
}

int CVisNetworkConnection::senderPathIndex()
{
    return d->senderPathIdx;
}

IVisSystemNode* CVisNetworkConnection::receiverNode()
{
    return d->receiverNode;
}

int CVisNetworkConnection::receiverPathIndex()
{
    return d->receiverPathIdx;
}

IVisSystemNodeIOData* CVisNetworkConnection::connectionData()
{
    return d->ioData;
}

IVisSystemNodeControlData* CVisNetworkConnection::senderControlData()
{
    return d->senderControlData;
}

IVisSystemNodeControlData* CVisNetworkConnection::receiverControlData()
{
    return d->receiverControlData;
}

