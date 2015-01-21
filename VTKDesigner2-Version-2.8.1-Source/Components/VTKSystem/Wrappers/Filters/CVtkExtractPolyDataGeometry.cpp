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
#include "CVtkExtractPolyDataGeometry.h"

DEFINE_VTK_OBJECT(CVtkExtractPolyDataGeometry, CVtkPolyDataFilter, vtkExtractPolyDataGeometry)
{
    pDesc->setNodeClassName("vtkExtractPolyDataGeometry");
    pDesc->setNodeClassDescription("ExtractPolyDataGeometry");

    pDesc->setPropertyInfo("ExtractInside", "SetExtractInside", "GetExtractInside", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("ExtractBoundaryCells", "SetExtractBoundaryCells", "GetExtractBoundaryCells", QStringList() << "bool", QStringList(), "void", "bool");
}



CVtkExtractPolyDataGeometry::CVtkExtractPolyDataGeometry() : m_vtkExtractPolyDataGeometry(0)
{
    CVtkExtractPolyDataGeometry::InitializeObjectDesc();
    setVtkObject(vtkExtractPolyDataGeometry::New());
}

CVtkExtractPolyDataGeometry::~CVtkExtractPolyDataGeometry()
{

}

bool CVtkExtractPolyDataGeometry::isExtractInside() const
{
    return m_vtkExtractPolyDataGeometry->GetExtractInside();
}

void CVtkExtractPolyDataGeometry::setExtractInside(bool val)
{
    m_vtkExtractPolyDataGeometry->SetExtractInside(val);
}

bool CVtkExtractPolyDataGeometry::isExtractBoundaryCells() const
{
    return m_vtkExtractPolyDataGeometry->GetExtractBoundaryCells();    
}

void CVtkExtractPolyDataGeometry::setExtractBoundaryCells(bool val)
{
    m_vtkExtractPolyDataGeometry->SetExtractBoundaryCells(val);
}

