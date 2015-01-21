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
#include "CVtkCylinder.h"

DEFINE_VTK_OBJECT(CVtkCylinder,CVtkImplicitFunction,vtkCylinder)
{
    pDesc->setNodeClassName("vtkCylinder");
    pDesc->setNodeClassDescription("Implicit function for a cylinder");
    pDesc->setNodeClassCategory("Functions");

    pDesc->setPropertyInfo("Radius", "SetRadius", "GetRadius", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Center", "SetCenter", "GetCenter", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
}

CVtkCylinder::CVtkCylinder() : m_vtkCylinder(0)
{
    CVtkCylinder::InitializeObjectDesc();
    setVtkObject(vtkCylinder::New());
}

CVtkCylinder::~CVtkCylinder()
{

}

double CVtkCylinder::radius() const
{
    return m_vtkCylinder->GetRadius();
}


void CVtkCylinder::setRadius(double val)
{
    m_vtkCylinder->SetRadius(val);
}


Point3D CVtkCylinder::center() const
{
    return Point3D(m_vtkCylinder->GetCenter());
}


void CVtkCylinder::setCenter(Point3D val)
{
    m_vtkCylinder->SetCenter(val.x, val.y, val.z);
}

