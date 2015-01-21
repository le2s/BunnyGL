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

#include "CVtkPlaneSource.h"
#include "CVtkPolyData.h"

DEFINE_VTK_OBJECT(CVtkPlaneSource, CVtkPolyDataAlgorithm, vtkPlaneSource)
{
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassName("vtkPlaneSource");
    pDesc->setNodeClassDescription("Create an array of quadrilaterals located in a plane");

    pDesc->setPropertyInfo("XResolution", "SetXResolution", "GetXResolution", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("YResolution", "SetYResolution", "GetYResolution", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Origin", "SetOrigin", "GetOrigin", QStringList() << "double" << "double" << "double",
                   QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Origin", "SetOrigin", "GetOrigin", QStringList() << "double" << "double" << "double",
                           QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Point1", "SetPoint1", "GetPoint1", QStringList() << "double" << "double" << "double",
                           QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Point2", "SetPoint2", "GetPoint2", QStringList() << "double" << "double" << "double",
                           QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Center", "SetCenter", "GetCenter", QStringList() << "double" << "double" << "double",
                           QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Normal", "SetNormal", "GetNormal", QStringList() << "double" << "double" << "double",
                           QStringList(), "void", "double*");

}

CVtkPlaneSource::CVtkPlaneSource() : m_vtkPlaneSource(0)
{
    CVtkPlaneSource::InitializeObjectDesc();
    setVtkObject(vtkPlaneSource::New());
}

CVtkPlaneSource::~CVtkPlaneSource()
{

}

int CVtkPlaneSource::xResolution() const
{
    return (m_vtkPlaneSource->GetXResolution());
}

void CVtkPlaneSource::setXResolution(int x)
{
    m_vtkPlaneSource->SetXResolution(x);
}

int CVtkPlaneSource::yResolution() const
{
    return (m_vtkPlaneSource->GetYResolution());
}

void CVtkPlaneSource::setYResolution(int y)
{
    m_vtkPlaneSource->SetYResolution(y);
}

Point3D CVtkPlaneSource::origin() const
{
    return Point3D(m_vtkPlaneSource->GetOrigin());
}

void CVtkPlaneSource::setOrigin(Point3D p)
{
    m_vtkPlaneSource->SetOrigin(p.x, p.y, p.z);
}

Point3D CVtkPlaneSource::point1() const
{
    return Point3D(m_vtkPlaneSource->GetPoint1());
}

void CVtkPlaneSource::setPoint1(Point3D point1)
{
    m_vtkPlaneSource->SetPoint1(point1.x, point1.y, point1.z);
}

Point3D CVtkPlaneSource::point2() const
{
    return Point3D(m_vtkPlaneSource->GetPoint2());
}

void CVtkPlaneSource::setPoint2(Point3D point2)
{
    m_vtkPlaneSource->SetPoint2(point2.x, point2.y, point2.z);
}

Point3D CVtkPlaneSource::center() const
{
    return Point3D(m_vtkPlaneSource->GetCenter());
}

void CVtkPlaneSource::setCenter(Point3D center)
{
    m_vtkPlaneSource->SetCenter(center.x, center.y, center.z);
}

Point3D CVtkPlaneSource::normal() const
{
    return Point3D(m_vtkPlaneSource->GetNormal());
}

void CVtkPlaneSource::setNormal(Point3D norm)
{
    m_vtkPlaneSource->SetNormal(norm.x, norm.y, norm.z);
}

