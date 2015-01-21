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
#include "CVtkPlane.h"

DEFINE_VTK_OBJECT(CVtkPlane,CVtkImplicitFunction,vtkPlane)
{
    pDesc->setNodeClassName("vtkPlane");
    pDesc->setNodeClassDescription("Perform various plane computations");
    pDesc->setNodeClassCategory("Functions");

    pDesc->setPropertyInfo("Normal", "SetNormal", "GetNormal", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Origin", "SetOrigin", "GetOrigin", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
}

CVtkPlane::CVtkPlane() : m_vtkPlane(0)
{
    CVtkPlane::InitializeObjectDesc();
    setVtkObject(vtkPlane::New());
}

CVtkPlane::~CVtkPlane()
{

}

Vector3D CVtkPlane::normal() const
{
    return Vector3D(m_vtkPlane->GetNormal());
}


void CVtkPlane::setNormal(Vector3D val)
{
    m_vtkPlane->SetNormal(val.x, val.y, val.z);
}


Point3D CVtkPlane::origin() const
{
    return Point3D(m_vtkPlane->GetOrigin());
}


void CVtkPlane::setOrigin(Point3D val)
{
    m_vtkPlane->SetOrigin(val.x, val.y, val.z);
}

