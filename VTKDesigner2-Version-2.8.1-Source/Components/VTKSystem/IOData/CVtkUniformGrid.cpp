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

#include "CVtkUniformGrid.h"

CVtkUniformGrid::CVtkUniformGrid(vtkUniformGrid* uniformGrid)
:m_vtkUniformGrid(uniformGrid)
{

}

CVtkUniformGrid::~CVtkUniformGrid()
{

}

void CVtkUniformGrid::setUniformGrid(vtkUniformGrid* uniformGrid)
{
    m_vtkUniformGrid = uniformGrid;
}

GCF_DEFINE_QUERY_TABLE(CVtkUniformGrid)
    GCF_IMPLEMENTS(IVtkDataObjectIOData)
    GCF_IMPLEMENTS(IVtkDataSetIOData)
    GCF_IMPLEMENTS(IVtkImageDataIOData)
    GCF_IMPLEMENTS(IVtkUniformGridIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkUniformGrid)

void* CVtkUniformGrid::dataPointer()
{
    return (void*)m_vtkUniformGrid;
}

QString CVtkUniformGrid::dataTypeName()
{
    return "vtkUniformGrid";
}

vtkDataObject* CVtkUniformGrid::getVtkDataObject()
{
    return static_cast<vtkDataObject*>(m_vtkUniformGrid);
}

vtkDataSet* CVtkUniformGrid::getVtkDataSet()
{
    return static_cast<vtkDataSet*>(m_vtkUniformGrid);
}

vtkImageData* CVtkUniformGrid::getVtkImageData()
{
    return static_cast<vtkImageData*>(m_vtkUniformGrid);
}

vtkUniformGrid* CVtkUniformGrid::getVtkUniformGrid()
{
    return m_vtkUniformGrid;
}

vtkObject* CVtkUniformGrid::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkUniformGrid );
}




