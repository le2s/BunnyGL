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

#include "CVtkDataSetSurfaceFilter.h"


DEFINE_VTK_OBJECT(CVtkDataSetSurfaceFilter, CVtkPolyDataFilter, vtkDataSetSurfaceFilter)
{
    pDesc->setNodeClassName("vtkDataSetSurfaceFilter");
    pDesc->setNodeClassDescription("Extracts outer (polygonal) surface");

    pDesc->setPropertyInfo("UseStrips","SetUseStrips", "GetUseStrips", QStringList() << "int",QStringList(), "void", "int");
    pDesc->setPropertyInfo("PieceInvariant","SetPieceInvariant", "GetPieceInvariant", QStringList() << "int",QStringList(), "void", "int");
}

CVtkDataSetSurfaceFilter::CVtkDataSetSurfaceFilter() : m_vtkDataSetSurfaceFilter(0)
{
    CVtkDataSetSurfaceFilter::InitializeObjectDesc();
    setVtkObject(vtkDataSetSurfaceFilter::New());
}

CVtkDataSetSurfaceFilter::~CVtkDataSetSurfaceFilter()
{

}

bool CVtkDataSetSurfaceFilter::isUseStrips() const
{
    return m_vtkDataSetSurfaceFilter->GetUseStrips();
}

void CVtkDataSetSurfaceFilter::setUseStrips(bool val)
{
    m_vtkDataSetSurfaceFilter->SetUseStrips(val);
}

int CVtkDataSetSurfaceFilter::pieceInvariant() const
{
    return m_vtkDataSetSurfaceFilter->GetPieceInvariant();
}

void CVtkDataSetSurfaceFilter::setPieceInvariant(int pv)
{
    m_vtkDataSetSurfaceFilter->SetPieceInvariant(pv);
}
