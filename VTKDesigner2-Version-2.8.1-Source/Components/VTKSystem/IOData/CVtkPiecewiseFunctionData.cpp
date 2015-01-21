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

#include "CVtkPiecewiseFunctionData.h"

CVtkPiecewiseFunctionData::CVtkPiecewiseFunctionData(vtkPiecewiseFunction* pwFn)
:m_vtkPiecewiseFunction(pwFn)
{

}

CVtkPiecewiseFunctionData::~CVtkPiecewiseFunctionData()
{

}

void CVtkPiecewiseFunctionData::setPiecewiseFunction(vtkPiecewiseFunction* pwFn)
{
    m_vtkPiecewiseFunction = pwFn;
}

GCF_DEFINE_QUERY_TABLE(CVtkPiecewiseFunctionData)
    GCF_IMPLEMENTS(IVtkDataObjectIOData)
    GCF_IMPLEMENTS(IVtkPiecewiseFunctionIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkPiecewiseFunctionData)

void* CVtkPiecewiseFunctionData::dataPointer()
{
    return (void*)m_vtkPiecewiseFunction;
}

QString CVtkPiecewiseFunctionData::dataTypeName()
{
    return "vtkPiecewiseFunction";
}

vtkDataObject* CVtkPiecewiseFunctionData::getVtkDataObject()
{
    return static_cast<vtkDataObject*>(m_vtkPiecewiseFunction);
}

vtkPiecewiseFunction* CVtkPiecewiseFunctionData::getVtkPiecewiseFunction()
{
    return m_vtkPiecewiseFunction;
}

vtkObject* CVtkPiecewiseFunctionData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkPiecewiseFunction );
}


