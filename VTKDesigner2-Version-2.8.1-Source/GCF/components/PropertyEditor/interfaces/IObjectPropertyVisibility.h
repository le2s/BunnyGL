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

#ifndef I_OBJECT_PROPERTY_VISIBILITY_H
#define I_OBJECT_PROPERTY_VISIBILITY_H

#include <GCF/IContainer>

class IObjectPropertyVisibility : virtual public IContainer
{
public:
    virtual bool isQPropertyVisible(int qPropertyIndex) const = 0;
    virtual bool isEditablePropertyVisible(int ePropertyIndex) const = 0;
};

Q_DECLARE_INTERFACE(IObjectPropertyVisibility, "com.vcreatelogic.IObjectPropertyVisibility/1.0")

#endif


