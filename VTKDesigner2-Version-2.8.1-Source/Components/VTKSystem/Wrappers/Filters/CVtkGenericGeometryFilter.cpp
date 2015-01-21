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

#include "CVtkGenericGeometryFilter.h"

DEFINE_VTK_OBJECT(CVtkGenericGeometryFilter,CVtkPolyDataFilter,vtkGenericGeometryFilter)
{
    pDesc->setNodeClassName("vtkGenericGeometryFilter");
    pDesc->setNodeClassDescription("Extract geometry from data");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("PointClipping", "SetPointClipping", "GetPointClipping", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("CellClipping", "SetCellClipping", "GetCellClipping", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ExtentClipping", "SetExtentClipping", "GetExtentClipping", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("PointMinimum", "SetPointMinimum", "GetPointMinimum", QStringList() << "vtkIdType", QStringList(), "void", "vtkIdType");
    pDesc->setPropertyInfo("PointMaximum", "SetPointMaximum", "GetPointMaximum", QStringList() << "vtkIdType", QStringList(), "void", "vtkIdType");
    pDesc->setPropertyInfo("CellMaximum", "SetCellMaximum", "GetCellMaximum", QStringList() << "vtkIdType", QStringList(), "void", "vtkIdType");
    pDesc->setPropertyInfo("Merging", "SetMerging", "GetMerging", QStringList() << "int" , QStringList(), "void", "int");
}

CVtkGenericGeometryFilter::CVtkGenericGeometryFilter() : m_vtkGenericGeometryFilter(0)
{
    CVtkGenericGeometryFilter::InitializeObjectDesc();
    setVtkObject(vtkGenericGeometryFilter::New());
}

CVtkGenericGeometryFilter::~CVtkGenericGeometryFilter()
{

}

bool CVtkGenericGeometryFilter::isPointClipping() const
{
    return m_vtkGenericGeometryFilter->GetPointClipping();
}


void CVtkGenericGeometryFilter::setPointClipping(bool val)
{
    m_vtkGenericGeometryFilter->SetPointClipping(val);
}


bool CVtkGenericGeometryFilter::isCellClipping() const
{
    return m_vtkGenericGeometryFilter->GetCellClipping();
}


void CVtkGenericGeometryFilter::setCellClipping(bool val)
{
    m_vtkGenericGeometryFilter->SetCellClipping(val);
}


bool CVtkGenericGeometryFilter::isExtentClipping() const
{
    return m_vtkGenericGeometryFilter->GetExtentClipping();
}


void CVtkGenericGeometryFilter::setExtentClipping(bool val)
{
    m_vtkGenericGeometryFilter->SetExtentClipping(val);
}


int CVtkGenericGeometryFilter::pointMinimum() const
{
    return m_vtkGenericGeometryFilter->GetPointMinimum();
}


void CVtkGenericGeometryFilter::setPointMinimum(int val)
{
    m_vtkGenericGeometryFilter->SetPointMinimum(vtkIdType(val));
}


int CVtkGenericGeometryFilter::pointMaximum() const
{
    return m_vtkGenericGeometryFilter->GetPointMaximum();
}


void CVtkGenericGeometryFilter::setPointMaximum(int val)
{
    m_vtkGenericGeometryFilter->SetPointMaximum(vtkIdType(val));
}


int CVtkGenericGeometryFilter::cellMaximum() const
{
    return m_vtkGenericGeometryFilter->GetCellMaximum();
}


void CVtkGenericGeometryFilter::setCellMaximum(int val)
{
    m_vtkGenericGeometryFilter->SetCellMaximum(vtkIdType(val));
}


bool CVtkGenericGeometryFilter::isMerging() const
{
    return m_vtkGenericGeometryFilter->GetMerging();
}


void CVtkGenericGeometryFilter::setMerging(bool val)
{
    m_vtkGenericGeometryFilter->SetMerging(val);
}

