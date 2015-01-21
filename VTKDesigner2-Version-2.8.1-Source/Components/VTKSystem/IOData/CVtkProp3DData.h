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

#ifndef C_VTK_PROP3D_DATA_H
#define C_VTK_PROP3D_DATA_H

#include "IVtkProp3DIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkProp3D.h"

class CVtkProp3DData : virtual public IVtkProp3DIOData,
                       virtual public IVtkObjectIOData
{
public:
    CVtkProp3DData(vtkProp3D* actor=0);
    ~CVtkProp3DData();

    void setProp3D(vtkProp3D* actor);

    // IQueryable implementation
    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IVtkProp3DIOData implementation
    vtkProp3D* getVtkProp3D();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkProp3D* m_vtkProp3D;
};

#endif

