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

#include "CVtkRectilinearGrid.h"

CVtkRectilinearGrid::CVtkRectilinearGrid(vtkRectilinearGrid* rGrid)
:m_vtkRectilinearGrid(rGrid)
{

}

CVtkRectilinearGrid::~CVtkRectilinearGrid()
{

}

void CVtkRectilinearGrid::setRectilinearGrid(vtkRectilinearGrid* rGrid)
{
    m_vtkRectilinearGrid = rGrid;
}

GCF_DEFINE_QUERY_TABLE(CVtkRectilinearGrid)
    GCF_IMPLEMENTS(IVtkDataObjectIOData)
    GCF_IMPLEMENTS(IVtkDataSetIOData)
    GCF_IMPLEMENTS(IVtkRectilinearGridIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkRectilinearGrid)

void* CVtkRectilinearGrid::dataPointer()
{
    return (void*)m_vtkRectilinearGrid;
}

QString CVtkRectilinearGrid::dataTypeName()
{
    return "vtkRectilinearGrid";
}

vtkDataObject* CVtkRectilinearGrid::getVtkDataObject()
{
    return static_cast<vtkDataObject*>(m_vtkRectilinearGrid);
}

vtkDataSet* CVtkRectilinearGrid::getVtkDataSet()
{
    return static_cast<vtkDataSet*>(m_vtkRectilinearGrid);
}

vtkRectilinearGrid* CVtkRectilinearGrid::getVtkRectilinearGrid()
{
    return m_vtkRectilinearGrid;
}

vtkObject* CVtkRectilinearGrid::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkRectilinearGrid );
}


