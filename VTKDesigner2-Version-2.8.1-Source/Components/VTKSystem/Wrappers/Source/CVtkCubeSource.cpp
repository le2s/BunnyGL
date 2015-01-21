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

#include "CVtkCubeSource.h"
#include "CVtkPolyData.h"

DEFINE_VTK_OBJECT(CVtkCubeSource, CVtkPolyDataAlgorithm, vtkCubeSource)
{
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassName("vtkCubeSource");
    pDesc->setNodeClassDescription("Cube Source Algorithm");
    pDesc->setPropertyInfo("XLength", "SetXLength", "GetXLength", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("YLength","SetYLength","GetYLength",QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("ZLength", "SetZLength", "GetZLength", QStringList() << "double",QStringList(), "void", "double");
    pDesc->setPropertyInfo("Center", "SetCenter", "GetCenter", QStringList() << "double" << "double" << "double", QStringList(), "void", "Point3D");
}

CVtkCubeSource::CVtkCubeSource() : m_vtkCubeSource(0)
{
    CVtkCubeSource::InitializeObjectDesc();
    setVtkObject(vtkCubeSource::New());
}

CVtkCubeSource::~CVtkCubeSource()
{

}

double CVtkCubeSource::xLength() const
{
    return m_vtkCubeSource->GetXLength();
}

void CVtkCubeSource::setXLength(double xl)
{
    m_vtkCubeSource    ->SetXLength(xl);
}

double CVtkCubeSource::yLength() const
{
    return m_vtkCubeSource->GetYLength();
}

void CVtkCubeSource::setYLength(double yl)
{
    m_vtkCubeSource    ->SetYLength(yl);
}

double CVtkCubeSource::zLength() const
{
    return m_vtkCubeSource->GetZLength();
}

void CVtkCubeSource::setZLength(double zl)
{
    m_vtkCubeSource    ->SetZLength(zl);
}

Point3D CVtkCubeSource::center() const
{
    return Point3D(m_vtkCubeSource->GetCenter());
}

void CVtkCubeSource::setCenter(Point3D p)
{
    m_vtkCubeSource->SetCenter(p.x, p.y, p.z);
}

