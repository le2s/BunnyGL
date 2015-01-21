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

#include "CVtkStructuredGrid.h"

CVtkStructuredGrid::CVtkStructuredGrid(vtkStructuredGrid* sGrid)
:m_vtkStructuredGrid(sGrid)
{

}

CVtkStructuredGrid::~CVtkStructuredGrid()
{

}

void CVtkStructuredGrid::setStructuredGrid(vtkStructuredGrid* sGrid)
{
    m_vtkStructuredGrid = sGrid;
}

GCF_DEFINE_QUERY_TABLE(CVtkStructuredGrid)
    GCF_IMPLEMENTS(IVtkDataObjectIOData)
    GCF_IMPLEMENTS(IVtkDataSetIOData)
    GCF_IMPLEMENTS(IVtkPointSetIOData)
    GCF_IMPLEMENTS(IVtkStructuredGridIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkStructuredGrid)

void* CVtkStructuredGrid::dataPointer()
{
    return (void*)m_vtkStructuredGrid;
}

QString CVtkStructuredGrid::dataTypeName()
{
    return "vtkStructuredGrid";
}

vtkDataObject* CVtkStructuredGrid::getVtkDataObject()
{
    return static_cast<vtkDataObject*>(m_vtkStructuredGrid);
}

vtkDataSet* CVtkStructuredGrid::getVtkDataSet()
{
    return static_cast<vtkDataSet*>(m_vtkStructuredGrid);
}

vtkPointSet* CVtkStructuredGrid::getVtkPointSet()
{
    return static_cast<vtkPointSet*>(m_vtkStructuredGrid);
}

vtkStructuredGrid* CVtkStructuredGrid::getVtkStructuredGrid()
{
    return m_vtkStructuredGrid;
}

vtkObject* CVtkStructuredGrid::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkStructuredGrid );
}




