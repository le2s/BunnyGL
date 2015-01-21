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
#include "CVtkSphere.h"

DEFINE_VTK_OBJECT(CVtkSphere,CVtkImplicitFunction,vtkSphere)
{
    pDesc->setNodeClassName("vtkSphere");
    pDesc->setNodeClassDescription("Implicit function for a sphere");
    pDesc->setNodeClassCategory("Functions");

    pDesc->setPropertyInfo("Radius", "SetRadius", "GetRadius", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Center", "SetCenter", "GetCenter", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
}

CVtkSphere::CVtkSphere() : m_vtkSphere(0)
{
    CVtkSphere::InitializeObjectDesc();
    setVtkObject(vtkSphere::New());
}

CVtkSphere::~CVtkSphere()
{

}

double CVtkSphere::radius() const
{
    return m_vtkSphere->GetRadius();
}


void CVtkSphere::setRadius(double val)
{
    m_vtkSphere->SetRadius(val);
}


Point3D CVtkSphere::center() const
{
    return Point3D(m_vtkSphere->GetCenter());
}


void CVtkSphere::setCenter(Point3D val)
{
    m_vtkSphere->SetCenter(val.x, val.y, val.z);
}

