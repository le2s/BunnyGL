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

#include "CVtkPointSet.h"

CVtkPointSet::CVtkPointSet(vtkPointSet* pointSet)
:m_vtkPointSet(pointSet)
{

}

CVtkPointSet::~CVtkPointSet()
{

}

void CVtkPointSet::setPointSet(vtkPointSet* pointSet)
{
    m_vtkPointSet = pointSet;
}

GCF_DEFINE_QUERY_TABLE(CVtkPointSet)
    GCF_IMPLEMENTS(IVtkDataObjectIOData)
    GCF_IMPLEMENTS(IVtkDataSetIOData)
    GCF_IMPLEMENTS(IVtkPointSetIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkPointSet)

void* CVtkPointSet::dataPointer()
{
    return (void*)m_vtkPointSet;
}

QString CVtkPointSet::dataTypeName()
{
    return "vtkPointSet";
}

vtkDataObject* CVtkPointSet::getVtkDataObject()
{
    return static_cast<vtkDataObject*>(m_vtkPointSet);
}

vtkDataSet* CVtkPointSet::getVtkDataSet()
{
    return static_cast<vtkDataSet*>(m_vtkPointSet);
}

vtkPointSet* CVtkPointSet::getVtkPointSet()
{
    return m_vtkPointSet;
}

vtkObject* CVtkPointSet::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkPointSet );
}



