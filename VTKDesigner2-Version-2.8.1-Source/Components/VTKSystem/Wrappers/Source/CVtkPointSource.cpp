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

#include "CVtkPointSource.h"
#include "CVtkPolyData.h"

DEFINE_VTK_OBJECT(CVtkPointSource, CVtkPolyDataAlgorithm, vtkPointSource)
{
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassName("vtkPointSource");
    pDesc->setNodeClassDescription("Create a random cloud of points");
    pDesc->setPropertyInfo("PointCount", "SetNumberOfPoints", "GetNumberOfPoints", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Radius", "SetRadius", "GetRadius", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Center", "SetCenter", "GetCenter", QStringList() << "double" << "double" << "double",
                           QStringList(), "void", "double*");    
    pDesc->setPropertyInfo("Distribution", "SetDistribution", "GetDistribution", QStringList() << "int", QStringList(), "void", "int");
}

CVtkPointSource::CVtkPointSource() : m_vtkPointSource(0)
{
    CVtkPointSource::InitializeObjectDesc();
    setVtkObject(vtkPointSource::New());
}

CVtkPointSource::~CVtkPointSource()
{

}

int CVtkPointSource::pointCount() const
{
    return int(m_vtkPointSource->GetNumberOfPoints());
}

void CVtkPointSource::setPointCount(int c)
{
    m_vtkPointSource->SetNumberOfPoints(vtkIdType(c));
}

double CVtkPointSource::radius() const
{
    return m_vtkPointSource->GetRadius();
}

void CVtkPointSource::setRadius(double r)
{
    m_vtkPointSource->SetRadius(r);
}

Point3D CVtkPointSource::center() const
{
    return Point3D(m_vtkPointSource->GetCenter());
}

void CVtkPointSource::setCenter(Point3D center)
{
    m_vtkPointSource->SetCenter(center.x, center.y, center.z);
}
int CVtkPointSource::distribution() const
{
    return m_vtkPointSource->GetDistribution();
}
void CVtkPointSource::setDistribution(int d)
{
    m_vtkPointSource->SetDistribution(d);
}

