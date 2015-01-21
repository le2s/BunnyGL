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

#ifndef C_VTK_PROPERTY_DATA_H
#define C_VTK_PROPERTY_DATA_H

#include "IVtkPropertyIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkProperty.h"

class CVtkPropertyData : virtual public IVtkPropertyIOData,
                         virtual public IVtkObjectIOData
{
public:
    CVtkPropertyData(vtkProperty* property=0);
    ~CVtkPropertyData();

    void setProperty(vtkProperty* property);

    // IQueryable implementation
    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IVtkPropertyIOData implementation
    vtkProperty* getVtkProperty();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkProperty* m_vtkProperty;
};

#endif

