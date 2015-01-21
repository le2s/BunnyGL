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

#include "CVtkGenericContourFilter.h"

DEFINE_VTK_OBJECT(CVtkGenericContourFilter, CVtkPolyDataFilter, vtkGenericContourFilter)
{
    pDesc->setNodeClassName("vtkGenericContourFilter");
    pDesc->setNodeClassDescription("Generate isocontours from input dataset");

    pDesc->setPropertyInfo("NumberOfContours","SetNumberOfContours","GetNumberOfContours",QStringList() << "int", QStringList(),"void","int");
    pDesc->setPropertyInfo("ComputeNormals","SetComputeNormals","GetComputeNormals",QStringList() << "int", QStringList(), "void","int");    
    pDesc->setPropertyInfo("ComputeGradients","SetComputeGradients","GetComputeGradients",QStringList() << "int", QStringList(), "void","int");    
    pDesc->setPropertyInfo("ComputeScalars","SetComputeScalars","GetComputeScalars",QStringList() << "int", QStringList(), "void","int");    
}

CVtkGenericContourFilter::CVtkGenericContourFilter() : m_vtkGenericContourFilter(0)
{
    CVtkGenericContourFilter::InitializeObjectDesc();
    setVtkObject(vtkGenericContourFilter::New());
}

CVtkGenericContourFilter::~CVtkGenericContourFilter()
{

}

void CVtkGenericContourFilter::setNumberOfContours(int number)
{
    m_vtkGenericContourFilter->SetNumberOfContours(number);
}

int CVtkGenericContourFilter::numberOfContours() const
{
    return m_vtkGenericContourFilter->GetNumberOfContours();
}

void CVtkGenericContourFilter::setComputeNormals(bool val)
{
    m_vtkGenericContourFilter->SetComputeNormals(val);
}

bool CVtkGenericContourFilter::isComputeNormals() const
{
    return m_vtkGenericContourFilter->GetComputeNormals();
}

void CVtkGenericContourFilter::setComputeGradients(bool val)
{
    m_vtkGenericContourFilter->SetComputeGradients(val);
}

int CVtkGenericContourFilter::isComputeGradients() const
{
    return m_vtkGenericContourFilter->GetComputeGradients();
}

void CVtkGenericContourFilter::setComputeScalars(bool val)
{
    m_vtkGenericContourFilter->SetComputeScalars(val);
}

int CVtkGenericContourFilter::isComputeScalars() const
{
    return m_vtkGenericContourFilter->GetComputeScalars();
}

