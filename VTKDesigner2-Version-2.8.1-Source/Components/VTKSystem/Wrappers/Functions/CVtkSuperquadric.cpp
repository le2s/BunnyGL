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
#include "CVtkSuperquadric.h"

DEFINE_VTK_OBJECT(CVtkSuperquadric,CVtkImplicitFunction,vtkSuperquadric)
{
    pDesc->setNodeClassName("vtkSuperquadric");
    pDesc->setNodeClassDescription("Implicit function for a Superquadric");
    pDesc->setNodeClassCategory("Functions");

    pDesc->setPropertyInfo("Center", "SetCenter", "GetCenter", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Scale", "SetScale", "GetScale", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Thickness", "SetThickness", "GetThickness", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("PhiRoundness ", "SetPhiRoundness ", "GetPhiRoundness ", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("ThetaRoundness", "SetThetaRoundness", "GetThetaRoundness", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Size", "SetSize", "GetSize", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Toroidal", "SetToroidal", "GetToroidal", QStringList() << "int" , QStringList(), "void", "int");
}

CVtkSuperquadric::CVtkSuperquadric() : m_vtkSuperquadric(0)
{
    CVtkSuperquadric::InitializeObjectDesc();
    setVtkObject(vtkSuperquadric::New());
}

CVtkSuperquadric::~CVtkSuperquadric()
{

}

Point3D CVtkSuperquadric::center() const
{
    return Point3D(m_vtkSuperquadric->GetCenter());
}


void CVtkSuperquadric::setCenter(Point3D val)
{
    m_vtkSuperquadric->SetCenter(val.x, val.y, val.z);
}


Point3D CVtkSuperquadric::scale() const
{
    return Point3D(m_vtkSuperquadric->GetScale());
}


void CVtkSuperquadric::setScale(Point3D val)
{
    m_vtkSuperquadric->SetScale(val.x, val.y, val.z);
}


double CVtkSuperquadric::thickness() const
{
    return m_vtkSuperquadric->GetThickness();
}


void CVtkSuperquadric::setThickness(double val)
{
    m_vtkSuperquadric->SetThickness(val);
}


double CVtkSuperquadric::phiRoundness () const
{
    return m_vtkSuperquadric->GetPhiRoundness ();
}


void CVtkSuperquadric::setPhiRoundness (double val)
{
    m_vtkSuperquadric->SetPhiRoundness (val);
}


double CVtkSuperquadric::thetaRoundness() const
{
    return m_vtkSuperquadric->GetThetaRoundness();
}


void CVtkSuperquadric::setThetaRoundness(double val)
{
    m_vtkSuperquadric->SetThetaRoundness(val);
}


double CVtkSuperquadric::size() const
{
    return m_vtkSuperquadric->GetSize();
}


void CVtkSuperquadric::setSize(double val)
{
    m_vtkSuperquadric->SetSize(val);
}


bool CVtkSuperquadric::isToroidal() const
{
    return m_vtkSuperquadric->GetToroidal();
}


void CVtkSuperquadric::setToroidal(bool val)
{
    m_vtkSuperquadric->SetToroidal(val);
}

