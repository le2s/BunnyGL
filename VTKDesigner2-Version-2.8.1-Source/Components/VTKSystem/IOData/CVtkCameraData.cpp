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

#include "CVtkCameraData.h"
#include "vtkCamera.h"

CVtkCameraData::CVtkCameraData(vtkCamera* camera)
:m_vtkCamera(camera)
{

}

CVtkCameraData::~CVtkCameraData()
{

}

void CVtkCameraData::setCamera(vtkCamera* camera)
{
    m_vtkCamera = camera;
}

GCF_DEFINE_QUERY_TABLE(CVtkCameraData)
    GCF_IMPLEMENTS(IVtkCameraIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkCameraData)

void* CVtkCameraData::dataPointer()
{
    return (void*)(m_vtkCamera);
}

QString CVtkCameraData::dataTypeName()
{
    return "vtkCamera";
}

vtkCamera* CVtkCameraData::getVtkCamera()
{
    return m_vtkCamera;
}

vtkObject* CVtkCameraData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkCamera );
}



