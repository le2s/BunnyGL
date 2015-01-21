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

#include "CVtkConeSource.h"
#include "CVtkPolyData.h"

DEFINE_VTK_OBJECT(CVtkConeSource, CVtkPolyDataAlgorithm, vtkConeSource)
{
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassName("vtkConeSource");
    pDesc->setNodeClassDescription("Cone Source Algorithm");

    pDesc->setPropertyInfo("Height", "SetHeight", "GetHeight", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Radius", "SetRadius", "GetRadius", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Center", "SetCenter", "GetCenter", QStringList() << "double" << "double" << "double",
                           QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Direction", "SetDirection", "GetDirection", QStringList() << "double" << "double" << "double",
                           QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Angle", "SetAngle", "GetAngle", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Capping", "SetCapping", "GetCapping", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Resolution", "SetResolution", "GetResolution", QStringList() << "int", QStringList(), "void", "int");
}

CVtkConeSource::CVtkConeSource() : m_vtkConeSource(0)
{
    CVtkConeSource::InitializeObjectDesc();
    setVtkObject(vtkConeSource::New());
}

CVtkConeSource::~CVtkConeSource()
{

}

double CVtkConeSource::height() const
{
    return m_vtkConeSource->GetHeight();    
}

void CVtkConeSource::setHeight(double h)
{
    m_vtkConeSource->SetHeight(h);
}

double CVtkConeSource::radius() const
{
    return m_vtkConeSource->GetRadius();
}

void CVtkConeSource::setRadius(double r)
{
    m_vtkConeSource->SetRadius(r);
}

Point3D CVtkConeSource::center() const
{
    return Point3D(m_vtkConeSource->GetCenter());
}

void CVtkConeSource::setCenter(Point3D center)
{
    m_vtkConeSource->SetCenter(center.x, center.y, center.z);
}

Vector3D CVtkConeSource::direction() const
{
    return Vector3D(m_vtkConeSource->GetDirection());
}

void CVtkConeSource::setDirection(Vector3D direction)
{
    m_vtkConeSource->SetDirection(direction.x, direction.y, direction.z);
}

bool CVtkConeSource::isCapping() const
{
    return m_vtkConeSource->GetCapping();
}

void CVtkConeSource::setCapping(bool val)
{
    m_vtkConeSource->SetCapping(val);
}

double CVtkConeSource::angle() const
{
    return m_vtkConeSource->GetAngle();
}

void CVtkConeSource::setAngle(double angle)
{
    m_vtkConeSource->SetAngle(angle);
}

void CVtkConeSource::setResolution(int res)
{
    m_vtkConeSource->SetResolution(res);
}

int CVtkConeSource::resolution() const
{
    return m_vtkConeSource->GetResolution();
}

