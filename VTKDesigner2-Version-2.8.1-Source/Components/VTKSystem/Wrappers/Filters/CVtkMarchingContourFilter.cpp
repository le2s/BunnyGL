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

#include "CVtkMarchingContourFilter.h"

DEFINE_VTK_OBJECT(CVtkMarchingContourFilter, CVtkPolyDataFilter, vtkMarchingContourFilter)
{
    pDesc->setNodeClassName("vtkMarchingContourFilter");
    pDesc->setNodeClassDescription("Generate isosurfaces/isolines from scalar values");

    pDesc->setPropertyInfo("NumberOfContours","SetNumberOfContours","GetNumberOfContours",QStringList() << "int", QStringList(),"void","int");
    pDesc->setPropertyInfo("ComputeNormals","SetComputeNormals","GetComputeNormals",QStringList() << "int", QStringList(), "void","int");    
    pDesc->setPropertyInfo("ComputeGradients","SetComputeGradients","GetComputeGradients",QStringList() << "int", QStringList(), "void","int");    
    pDesc->setPropertyInfo("ComputeScalars","SetComputeScalars","GetComputeScalars",QStringList() << "int", QStringList(), "void","int");    
}

CVtkMarchingContourFilter::CVtkMarchingContourFilter() : m_vtkMarchingContourFilter(0)
{
    CVtkMarchingContourFilter::InitializeObjectDesc();
    setVtkObject(vtkMarchingContourFilter::New());
}

CVtkMarchingContourFilter::~CVtkMarchingContourFilter()
{

}

void CVtkMarchingContourFilter::setNumberOfContours(int number)
{
    m_vtkMarchingContourFilter->SetNumberOfContours(number);
}

int CVtkMarchingContourFilter::numberOfContours() const
{
    return m_vtkMarchingContourFilter->GetNumberOfContours();
}

void CVtkMarchingContourFilter::setComputeNormals(bool val)
{
    m_vtkMarchingContourFilter->SetComputeNormals(val);
}

bool CVtkMarchingContourFilter::isComputeNormals() const
{
    return m_vtkMarchingContourFilter->GetComputeNormals();
}

void CVtkMarchingContourFilter::setComputeGradients(bool val)
{
    m_vtkMarchingContourFilter->SetComputeGradients(val);
}

int CVtkMarchingContourFilter::isComputeGradients() const
{
    return m_vtkMarchingContourFilter->GetComputeGradients();
}

void CVtkMarchingContourFilter::setComputeScalars(bool val)
{
    m_vtkMarchingContourFilter->SetComputeScalars(val);
}

int CVtkMarchingContourFilter::isComputeScalars() const
{
    return m_vtkMarchingContourFilter->GetComputeScalars();
}

