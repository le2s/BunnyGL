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

#include "CVtkTriangleFilter.h"
                  
DEFINE_VTK_OBJECT(CVtkTriangleFilter, CVtkPolyDataFilter, vtkTriangleFilter)
{
    pDesc->setNodeClassName("vtkTriangleFilter");
    pDesc->setNodeClassDescription("Triangle Filter");
    pDesc->setPropertyInfo("PassVerts", "SetPassVerts", "GetPassVerts", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("PassLines", "SetPassLines", "GetPassLines", QStringList() << "int", QStringList(), "void", "int");
}

CVtkTriangleFilter::CVtkTriangleFilter() : m_vtkTriangleFilter(0)
{
    CVtkTriangleFilter::InitializeObjectDesc();
    setVtkObject(vtkTriangleFilter::New());
}

CVtkTriangleFilter::~CVtkTriangleFilter()
{

}

void CVtkTriangleFilter::setPassVerts(bool val)
{
    m_vtkTriangleFilter->SetPassVerts(val);    
}

bool CVtkTriangleFilter::isPassVerts() const
{
    return (m_vtkTriangleFilter->GetPassVerts());
}


void CVtkTriangleFilter::setPassLines(bool val)
{
    m_vtkTriangleFilter->SetPassLines(val);
}

bool CVtkTriangleFilter::isPassLines() const
{
    return (m_vtkTriangleFilter->GetPassLines());
}

