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
#include "CVtkCone.h"

DEFINE_VTK_OBJECT(CVtkCone,CVtkImplicitFunction,vtkCone)
{
    pDesc->setNodeClassName("vtkCone");
    pDesc->setNodeClassDescription("Implicit function for a cone");
    pDesc->setNodeClassCategory("Functions");

    pDesc->setPropertyInfo("Angle", "SetAngle", "GetAngle", QStringList() << "double", QStringList(), "void", "double");
}

CVtkCone::CVtkCone() : m_vtkCone(0)
{
    CVtkCone::InitializeObjectDesc();
    setVtkObject(vtkCone::New());
}

CVtkCone::~CVtkCone()
{

}

double CVtkCone::angle() const
{
    return m_vtkCone->GetAngle();
}


void CVtkCone::setAngle(double val)
{
    m_vtkCone->SetAngle(val);
}

