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

#include "CScriptableVtkDataSet.h"

CScriptableVtkDataSet::CScriptableVtkDataSet(vtkDataSet* dataSet, CVtkObject* source)
:CScriptableVtkDataObject(dataSet, source), m_vtkDataSet(dataSet)
{

}

CScriptableVtkDataSet::~CScriptableVtkDataSet()
{

}

void CScriptableVtkDataSet::computeBounds()
{
    m_vtkDataSet->ComputeBounds();
}

BoundingBox3D CScriptableVtkDataSet::boundingBox() const
{
    // m_vtkDataSet->ComputeBounds();
    BoundingBox3D bbox(m_vtkDataSet->GetBounds());
    return bbox;
}

Point3D CScriptableVtkDataSet::center() const
{
    Point3D point(m_vtkDataSet->GetCenter());
    return point;
}

double CScriptableVtkDataSet::length() const
{
    return m_vtkDataSet->GetLength();
}

ValueRange CScriptableVtkDataSet::scalarRange() const
{
    ValueRange range;
    range.min = m_vtkDataSet->GetScalarRange()[0];
    range.max = m_vtkDataSet->GetScalarRange()[0];
    return range;
}

