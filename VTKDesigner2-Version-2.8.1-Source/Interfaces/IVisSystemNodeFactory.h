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

#ifndef I_VIS_SYSTEM_NODE_FACTORY_H
#define I_VIS_SYSTEM_NODE_FACTORY_H

#include <GCF/IContainer>

class IVisSystemNode;
class IVisSystemNodeDesc;
class IVisSystemNodeFactory : virtual public IContainer
{
public:
    // Information about the node classes supported by the visualization system.
    virtual QStringList         nodeClassNameList() = 0;
    virtual IVisSystemNodeDesc* nodeDesc(QString nodeClassName) = 0;

    // Factory methods.
    virtual bool                canCreate(QString nodeClassName) = 0;
    virtual IVisSystemNode*     createNode(QString nodeClassName) = 0;
    virtual void                deleteNode(IVisSystemNode* node) = 0;
};

Q_DECLARE_INTERFACE(IVisSystemNodeFactory, "com.vcreatelogic.IVisSystemNodeFactory/1.0")

#endif

