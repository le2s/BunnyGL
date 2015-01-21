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

#include "CVtkLineSource.h"
#include "CVtkPolyData.h"

DEFINE_VTK_OBJECT(CVtkLineSource, CVtkPolyDataAlgorithm, vtkLineSource)
{
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassName("vtkLineSource");
    pDesc->setNodeClassDescription("Line Source Algorithm");
    pDesc->setPropertyInfo("Point1", "SetPoint1", "GetPoint1", QStringList() << "double" << "double" << "double",
                           QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Point2", "SetPoint2", "GetPoint2", QStringList() << "double" << "double" << "double",
                           QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Resolution", "SetResolution", "GetResolution", QStringList() << "int", QStringList(), "void", "int");
}

CVtkLineSource::CVtkLineSource() : m_vtkLineSource(0)
{
    CVtkLineSource::InitializeObjectDesc();
    setVtkObject(vtkLineSource::New());
}

CVtkLineSource::~CVtkLineSource()
{

}

Point3D CVtkLineSource::point1() const
{
    return Point3D(m_vtkLineSource->GetPoint1());
}

void CVtkLineSource::setPoint1(Point3D point1)
{
    m_vtkLineSource->SetPoint1(point1.x, point1.y, point1.z);
}

Point3D CVtkLineSource::point2() const
{
    return Point3D(m_vtkLineSource->GetPoint2());
}

void CVtkLineSource::setPoint2(Point3D point2)
{
    m_vtkLineSource->SetPoint2(point2.x, point2.y, point2.z);
}

void CVtkLineSource::setResolution(int res)
{
    m_vtkLineSource->SetResolution(res);
}

int CVtkLineSource::resolution() const
{
    return m_vtkLineSource->GetResolution();
}
