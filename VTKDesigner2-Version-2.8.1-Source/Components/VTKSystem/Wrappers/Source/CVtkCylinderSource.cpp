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

#include "CVtkCylinderSource.h"
#include "CVtkPolyData.h"

DEFINE_VTK_OBJECT(CVtkCylinderSource, CVtkPolyDataAlgorithm, vtkCylinderSource)
{
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassName("vtkCylinderSource");
    pDesc->setNodeClassDescription("Cylinder Source Algorithm");
    pDesc->setPropertyInfo("Radius", "SetRadius", "GetRadius", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Height", "SetHeight", "GetHeight", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Center", "SetCenter", "GetCenter", QStringList() << "double" << "double" << "double",
                           QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Capping", "SetCapping", "GetCapping", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Resolution", "SetResolution", "GetResolution", QStringList() << "int", QStringList(), "void", "int");
}

CVtkCylinderSource::CVtkCylinderSource() : m_vtkCylinderSource(0)
{
    CVtkCylinderSource::InitializeObjectDesc();
    setVtkObject(vtkCylinderSource::New());
}

CVtkCylinderSource::~CVtkCylinderSource()
{

}

double CVtkCylinderSource::radius() const
{
    return m_vtkCylinderSource->GetRadius();
}

void CVtkCylinderSource::setRadius(double r)
{
    m_vtkCylinderSource->SetRadius(r);
}

double CVtkCylinderSource::height() const
{
    return m_vtkCylinderSource->GetHeight();
}

void CVtkCylinderSource::setHeight(double h)
{
    m_vtkCylinderSource->SetHeight(h);
}

Point3D CVtkCylinderSource::center() const
{
    return Point3D(m_vtkCylinderSource->GetCenter());
}

void CVtkCylinderSource::setCenter(Point3D center)
{
    m_vtkCylinderSource->SetCenter(center.x, center.y, center.z);
}

bool CVtkCylinderSource::isCapping() const
{
    return m_vtkCylinderSource->GetCapping();
}

void CVtkCylinderSource::setCapping(bool val)
{
    m_vtkCylinderSource->SetCapping(val);
}

void CVtkCylinderSource::setResolution(int res)
{
    m_vtkCylinderSource->SetResolution(res);
}

int CVtkCylinderSource::resolution() const
{
    return m_vtkCylinderSource->GetResolution();
}

