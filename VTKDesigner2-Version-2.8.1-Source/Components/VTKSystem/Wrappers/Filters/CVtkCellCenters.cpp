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

#include "CVtkCellCenters.h"

DEFINE_VTK_OBJECT(CVtkCellCenters,CVtkPolyDataFilter,vtkCellCenters)
{
    pDesc->setNodeClassName("vtkCellCenters");
    pDesc->setNodeClassDescription("Generate points at center of cells");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("VertexCells", "SetVertexCells", "GetVertexCells", QStringList() << "int" , QStringList(), "void", "int");
}

CVtkCellCenters::CVtkCellCenters() : m_vtkCellCenters(0)
{
    CVtkCellCenters::InitializeObjectDesc();
    setVtkObject(vtkCellCenters::New());
}

CVtkCellCenters::~CVtkCellCenters()
{

}

bool CVtkCellCenters::isVertexCells() const
{
    return m_vtkCellCenters->GetVertexCells();
}


void CVtkCellCenters::setVertexCells(bool val)
{
    m_vtkCellCenters->SetVertexCells(val);
}

