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
#include "CVtkSimpleElevationFilter.h"

DEFINE_VTK_OBJECT(CVtkSimpleElevationFilter,CVtkDataSetFilter,vtkSimpleElevationFilter)
{
    pDesc->setNodeClassName("vtkSimpleElevationFilter");
    pDesc->setNodeClassDescription("Generate scalars along a specified direction");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("Vector", "SetVector", "GetVector", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
}

CVtkSimpleElevationFilter::CVtkSimpleElevationFilter() : m_vtkSimpleElevationFilter(0)
{
    CVtkSimpleElevationFilter::InitializeObjectDesc();
    setVtkObject(vtkSimpleElevationFilter::New());
}

CVtkSimpleElevationFilter::~CVtkSimpleElevationFilter()
{

}

Vector3D CVtkSimpleElevationFilter::vector() const
{
    return Vector3D(m_vtkSimpleElevationFilter->GetVector());
}


void CVtkSimpleElevationFilter::setVector(Vector3D val)
{
    m_vtkSimpleElevationFilter->SetVector(val.x, val.y, val.z);
}

