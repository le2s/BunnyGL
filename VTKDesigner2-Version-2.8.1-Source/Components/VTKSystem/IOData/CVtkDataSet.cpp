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

#include "CVtkDataSet.h"

CVtkDataSet::CVtkDataSet(vtkDataSet* dataSet)
:m_vtkDataSet(dataSet)
{

}

CVtkDataSet::~CVtkDataSet()
{

}

void CVtkDataSet::setDataSet(vtkDataSet* dataSet)
{
    m_vtkDataSet = dataSet;
}

GCF_DEFINE_QUERY_TABLE(CVtkDataSet)
    GCF_IMPLEMENTS(IVtkDataObjectIOData)
    GCF_IMPLEMENTS(IVtkDataSetIOData)
GCF_END_QUERY_TABLE(CVtkDataSet)

void* CVtkDataSet::dataPointer()
{
    return (void*)m_vtkDataSet;
}

QString CVtkDataSet::dataTypeName()
{
    return "vtkDataSet";
}

vtkDataObject* CVtkDataSet::getVtkDataObject()
{
    return static_cast<vtkDataObject*>(m_vtkDataSet);
}

vtkDataSet* CVtkDataSet::getVtkDataSet()
{
    return m_vtkDataSet;
}



