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

#ifndef C_VTK_CAMERA_DATA_H
#define C_VTK_CAMERA_DATA_H

#include "IVtkCameraIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkCamera.h"

class CVtkCameraData : virtual public IVtkCameraIOData,
                       virtual public IVtkObjectIOData
{
public:
    CVtkCameraData(vtkCamera* camera=0);
    ~CVtkCameraData();

    void setCamera(vtkCamera* camera);

    // IQueryable implementation
    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IVtkCameraIOData implementation
    vtkCamera* getVtkCamera();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkCamera* m_vtkCamera;
};

#endif

