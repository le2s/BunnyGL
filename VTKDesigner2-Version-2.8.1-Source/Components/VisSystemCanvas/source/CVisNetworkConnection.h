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


#ifndef C_VIS_NETWORK_CONNECTION_H
#define C_VIS_NETWORK_CONNECTION_H

#include "IVisNetworkConnection.h"

struct CVisNetworkConnectionData;
class CVisNetworkConnection : virtual public IVisNetworkConnection
{
public:
    CVisNetworkConnection();
    ~CVisNetworkConnection();

    void setSenderNode(IVisSystemNode* node);
    void setReceiverNode(IVisSystemNode* node);
    void setSenderPathIndex(int index);
    void setReceiverPathIndex(int index);
    void setConnectionData(IVisSystemNodeIOData* data);
    void setSenderControlData(IVisSystemNodeControlData* data);
    void setReceiverControlData(IVisSystemNodeControlData* data);

    // IQueryable implementation
    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();

    // IVisNetworkConnection implementation
    IVisSystemNode* senderNode();
    int senderPathIndex();
    IVisSystemNode* receiverNode();
    int receiverPathIndex();
    IVisSystemNodeIOData* connectionData();
    IVisSystemNodeControlData* senderControlData();
    IVisSystemNodeControlData* receiverControlData();

private:
    CVisNetworkConnectionData* d;
};

#endif
