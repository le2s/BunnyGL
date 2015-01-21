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

#ifndef I_VIS_NETWORK_CONNECTION_H
#define I_VIS_NETWORK_CONNECTION_H

#include <GCF/DataStore>
#include <GCF/IQueryable>

class IVisSystemNode;
class IVisSystemNodeIOData;
class IVisSystemNodeControlData;

class IVisNetworkConnection : public virtual IQueryable
{
public:
    virtual IVisSystemNode*     senderNode() = 0;
    virtual int                 senderPathIndex() = 0;
    virtual IVisSystemNode*     receiverNode() = 0;
    virtual int                 receiverPathIndex() = 0;
    virtual IVisSystemNodeIOData*  connectionData() = 0;
    virtual IVisSystemNodeControlData* senderControlData() = 0;
    virtual IVisSystemNodeControlData* receiverControlData() = 0;
};

Q_DECLARE_INTERFACE(IVisNetworkConnection, "com.vcreatelogic.IVisNetworkConnection/1.0")

#endif


