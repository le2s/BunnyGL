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
#include "CVtkGeometryFilter.h"

DEFINE_VTK_OBJECT(CVtkGeometryFilter, CVtkPolyDataFilter, vtkGeometryFilter)
{
    pDesc->setNodeClassName("vtkGeometryFilter");
    pDesc->setNodeClassDescription("Extract geometry from data (or convert data to polygonal type)");

    pDesc->setPropertyInfo("PointClipping","SetPointClipping","GetPointClipping", QStringList() <<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("CellClipping","SetCellClipping","GetCellClipping", QStringList() <<"int",QStringList(),"void","int");    
    pDesc->setPropertyInfo("ExtentClipping","SetExtentClipping","GetExtentClipping", QStringList() <<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("PointMinimum","SetPointMinimum","GetPointMinimum", QStringList() <<"vtkIdType",QStringList(),"void","vtkIdType");
    pDesc->setPropertyInfo("PointMaximum","SetPointMaximum","GetPointMaximum", QStringList() <<"vtkIdType",QStringList(),"void","vtkIdType");
    pDesc->setPropertyInfo("CellMinimum","SetCellMinimum","GetCellMinimum", QStringList() <<"vtkIdType",QStringList(),"void","vtkIdType");
    pDesc->setPropertyInfo("CellMaximum","SetCellMaximum","GetCellMaximum", QStringList() <<"vtkIdType",QStringList(),"void","vtkIdType");
    pDesc->setPropertyInfo("Merging","SetMerging","GetMerging", QStringList() <<"int",QStringList(),"void","int");
}


CVtkGeometryFilter::CVtkGeometryFilter() : m_vtkGeometryFilter(0)
{
    CVtkGeometryFilter::InitializeObjectDesc();
    setVtkObject(vtkGeometryFilter::New());
}

CVtkGeometryFilter::~CVtkGeometryFilter()
{

}


void     CVtkGeometryFilter::setPointClipping(bool val)
{
    m_vtkGeometryFilter->SetPointClipping(val);
}

bool     CVtkGeometryFilter::isPointClipping() const
{
    return m_vtkGeometryFilter->GetPointClipping();
}


void     CVtkGeometryFilter::setCellClipping(bool val)
{
    m_vtkGeometryFilter->SetCellClipping(val);
}

bool     CVtkGeometryFilter::isCellClipping() const
{
    return m_vtkGeometryFilter->GetCellClipping();
}


void     CVtkGeometryFilter::setExtentClipping(bool val)
{
    m_vtkGeometryFilter->SetExtentClipping(val);
}

int     CVtkGeometryFilter::isExtentClipping() const
{
    return m_vtkGeometryFilter->GetExtentClipping();
}


void     CVtkGeometryFilter::setPointMinimum(int t)
{
    m_vtkGeometryFilter->SetPointMinimum(t);
}

int     CVtkGeometryFilter::pointMinimum() const
{
    return m_vtkGeometryFilter->GetPointMinimum();
}


void     CVtkGeometryFilter::setPointMaximum(int t)
{
    m_vtkGeometryFilter->SetPointMaximum((int)t);
}

int     CVtkGeometryFilter::pointMaximum() const
{
    return int(m_vtkGeometryFilter->GetPointMaximum());
}


void     CVtkGeometryFilter::setCellMinimum(int t)
{
    m_vtkGeometryFilter->SetCellMinimum(t);
}

int     CVtkGeometryFilter::cellMinimum() const
{
    return m_vtkGeometryFilter->GetCellMinimum();
}

void     CVtkGeometryFilter::setCellMaximum(int t)
{
    m_vtkGeometryFilter->SetCellMaximum(t);
}

int     CVtkGeometryFilter::cellMaximum() const
{
    return m_vtkGeometryFilter->GetCellMaximum();
}


void     CVtkGeometryFilter::setMerging(bool val)
{
    m_vtkGeometryFilter->SetMerging(val);
}

int     CVtkGeometryFilter::isMerging() const
{
    return m_vtkGeometryFilter->GetMerging();
}

