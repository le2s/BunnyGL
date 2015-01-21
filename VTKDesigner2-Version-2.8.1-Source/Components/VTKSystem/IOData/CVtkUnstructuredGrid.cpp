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

#include "CVtkUnstructuredGrid.h"

CVtkUnstructuredGrid::CVtkUnstructuredGrid(vtkUnstructuredGrid* usGrid)
:m_vtkUnstructuredGrid(usGrid)
{

}

CVtkUnstructuredGrid::~CVtkUnstructuredGrid()
{

}

void CVtkUnstructuredGrid::setUnstructuredGrid(vtkUnstructuredGrid* usGrid)
{
    m_vtkUnstructuredGrid = usGrid;
}

GCF_DEFINE_QUERY_TABLE(CVtkUnstructuredGrid)
    GCF_IMPLEMENTS(IVtkDataObjectIOData)
    GCF_IMPLEMENTS(IVtkDataSetIOData)
    GCF_IMPLEMENTS(IVtkPointSetIOData)
    GCF_IMPLEMENTS(IVtkUnstructuredGridIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkUnstructuredGrid)

void* CVtkUnstructuredGrid::dataPointer()
{
    return (void*)m_vtkUnstructuredGrid;
}

QString CVtkUnstructuredGrid::dataTypeName()
{
    return "vtkUnstructuredGrid";
}

vtkDataObject* CVtkUnstructuredGrid::getVtkDataObject()
{
    return static_cast<vtkDataObject*>(m_vtkUnstructuredGrid);
}

vtkDataSet* CVtkUnstructuredGrid::getVtkDataSet()
{
    return static_cast<vtkDataSet*>(m_vtkUnstructuredGrid);
}

vtkPointSet* CVtkUnstructuredGrid::getVtkPointSet()
{
    return static_cast<vtkPointSet*>(m_vtkUnstructuredGrid);
}

vtkUnstructuredGrid* CVtkUnstructuredGrid::getVtkUnstructuredGrid()
{
    return m_vtkUnstructuredGrid;
}

vtkObject* CVtkUnstructuredGrid::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkUnstructuredGrid );
}




