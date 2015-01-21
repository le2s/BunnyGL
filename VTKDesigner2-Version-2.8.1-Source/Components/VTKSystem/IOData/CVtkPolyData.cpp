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

#include "CVtkPolyData.h"

CVtkPolyData::CVtkPolyData(vtkPolyData* pointSet)
:m_vtkPolyData(pointSet)
{

}

CVtkPolyData::~CVtkPolyData()
{

}

void CVtkPolyData::setPolyData(vtkPolyData* polyData)
{
    m_vtkPolyData = polyData;
}

GCF_DEFINE_QUERY_TABLE(CVtkPolyData)
    GCF_IMPLEMENTS(IVtkDataObjectIOData)
    GCF_IMPLEMENTS(IVtkDataSetIOData)
    GCF_IMPLEMENTS(IVtkPointSetIOData)
    GCF_IMPLEMENTS(IVtkPolyDataIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkPolyData)

void* CVtkPolyData::dataPointer()
{
    return (void*)m_vtkPolyData;
}

QString CVtkPolyData::dataTypeName()
{
    return "vtkPolyData";
}

vtkDataObject* CVtkPolyData::getVtkDataObject()
{
    return static_cast<vtkDataObject*>(m_vtkPolyData);
}

vtkDataSet* CVtkPolyData::getVtkDataSet()
{
    return static_cast<vtkDataSet*>(m_vtkPolyData);
}

vtkPointSet* CVtkPolyData::getVtkPointSet()
{
    return static_cast<vtkPointSet*>(m_vtkPolyData);
}

vtkPolyData* CVtkPolyData::getVtkPolyData()
{
    return m_vtkPolyData;
}

vtkObject* CVtkPolyData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkPolyData );
}

