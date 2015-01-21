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

#ifndef C_VTK_OBJECT_CONTROL_DATA_H
#define C_VTK_OBJECT_CONTROL_DATA_H

#include "IVisSystemNodeControlData.h"
#include <GCF/IContainer>

class CVtkObject;
class CVtkObjectControlData : virtual public IVisSystemNodeControlData,
                               virtual public IContainer
{
public:
    CVtkObjectControlData(CVtkObject* object=0);
    ~CVtkObjectControlData();

    void setObject(CVtkObject* object);
    CVtkObject* getVtkObject();

    // IQueryable implementation
    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();

    // IVisSystemNodeControlData implementation
    QString controlType();

    // IContainer implementation
    QObject* containerObject();

private:
    CVtkObject* m_CVtkObject;
};

#endif

