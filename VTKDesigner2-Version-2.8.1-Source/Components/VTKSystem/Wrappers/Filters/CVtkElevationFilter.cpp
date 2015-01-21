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

#include "CVtkElevationFilter.h"

DEFINE_VTK_OBJECT(CVtkElevationFilter,CVtkDataSetFilter,vtkElevationFilter)
{
    pDesc->setNodeClassName("vtkElevationFilter");
    pDesc->setNodeClassDescription("Generate scalars along a specified direction");
       pDesc->setPropertyInfo("LowPoint", "SetLowPoint", "GetLowPoint", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("HighPoint", "SetHighPoint", "GetHighPoint", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("ScalarRange", "SetScalarRange", "GetScalarRange", QStringList() << "double" << "double", QStringList(), "void", "double*");
}

CVtkElevationFilter::CVtkElevationFilter() : m_vtkElevationFilter(0)
{
    CVtkElevationFilter::InitializeObjectDesc();
    setVtkObject(vtkElevationFilter::New());
}

CVtkElevationFilter::~CVtkElevationFilter()
{

}

Point3D CVtkElevationFilter::lowPoint() const
{
    return Point3D(m_vtkElevationFilter->GetLowPoint());
}

void CVtkElevationFilter::setLowPoint(Point3D val)
{
    m_vtkElevationFilter->SetLowPoint(val.x, val.y, val.z);
}

Point3D CVtkElevationFilter::highPoint() const
{
    return Point3D(m_vtkElevationFilter->GetHighPoint());
}

void CVtkElevationFilter::setHighPoint(Point3D val)
{
    m_vtkElevationFilter->SetHighPoint(val.x, val.y, val.z);
}


ValueRange CVtkElevationFilter::scalarRange() const
{
    return ValueRange(m_vtkElevationFilter->GetScalarRange());
}

void CVtkElevationFilter::setScalarRange(ValueRange val)
{
    m_vtkElevationFilter->SetScalarRange(val.min, val.max);
}



