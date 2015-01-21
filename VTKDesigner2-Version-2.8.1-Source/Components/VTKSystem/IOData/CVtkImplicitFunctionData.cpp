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

#include "CVtkImplicitFunctionData.h"
#include "vtkImplicitFunction.h"

CVtkImplicitFunctionData::CVtkImplicitFunctionData(vtkImplicitFunction* function)
:m_vtkImplicitFunction(function)
{

}

CVtkImplicitFunctionData::~CVtkImplicitFunctionData()
{

}

void CVtkImplicitFunctionData::setImplicitFunction(vtkImplicitFunction* function)
{
    m_vtkImplicitFunction = function;
}

GCF_DEFINE_QUERY_TABLE(CVtkImplicitFunctionData)
    GCF_IMPLEMENTS(IVtkImplicitFunctionIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkImplicitFunctionData)

void* CVtkImplicitFunctionData::dataPointer()
{
    return (void*)(m_vtkImplicitFunction);
}

QString CVtkImplicitFunctionData::dataTypeName()
{
    return "vtkImplicitFunction";
}

vtkImplicitFunction* CVtkImplicitFunctionData::getVtkImplicitFunction()
{
    return m_vtkImplicitFunction;
}

vtkObject* CVtkImplicitFunctionData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkImplicitFunction );
}

