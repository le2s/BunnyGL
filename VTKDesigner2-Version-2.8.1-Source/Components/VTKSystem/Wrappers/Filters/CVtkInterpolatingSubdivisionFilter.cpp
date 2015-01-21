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

#include "CVtkInterpolatingSubdivisionFilter.h"

DEFINE_VTK_OBJECT(CVtkInterpolatingSubdivisionFilter, CVtkPolyDataFilter, vtkInterpolatingSubdivisionFilter)
{
    pDesc->setNodeClassName("vtkInterpolatingSubdivisionFilter");
    pDesc->setNodeClassDescription("Triangle Filter");
    pDesc->setPropertyInfo("NumberOfSubdivisions", "SetNumberOfSubdivisions", "GetNumberOfSubdivisions", QStringList() << "int", QStringList(), "void", "int");

    pDesc->setCreateFunction(0);
}

CVtkInterpolatingSubdivisionFilter::CVtkInterpolatingSubdivisionFilter() : m_vtkInterpolatingSubdivisionFilter(0)
{
    CVtkInterpolatingSubdivisionFilter::InitializeObjectDesc();
    // setVtkObject(vtkInterpolatingSubdivisionFilter::New());
}

CVtkInterpolatingSubdivisionFilter::~CVtkInterpolatingSubdivisionFilter()
{

}

void CVtkInterpolatingSubdivisionFilter::setNumberOfSubdivisions(int val)
{
    m_vtkInterpolatingSubdivisionFilter->SetNumberOfSubdivisions(val);
}

int CVtkInterpolatingSubdivisionFilter::numberOfSubdivisions() const
{
    return m_vtkInterpolatingSubdivisionFilter->GetNumberOfSubdivisions();
}



