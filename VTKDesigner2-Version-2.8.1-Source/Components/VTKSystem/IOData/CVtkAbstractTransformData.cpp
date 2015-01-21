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

#include "CVtkAbstractTransformData.h"

CVtkAbstractTransformData::CVtkAbstractTransformData(vtkAbstractTransform* abstractTransform)
:m_vtkAbstractTransform(abstractTransform)
{

}

CVtkAbstractTransformData::~CVtkAbstractTransformData()
{

}

void CVtkAbstractTransformData::setAbstractTransform(vtkAbstractTransform* abstractTransform)
{
    m_vtkAbstractTransform = abstractTransform;
}

void* CVtkAbstractTransformData::dataPointer()
{
    return m_vtkAbstractTransform;
}

QString CVtkAbstractTransformData::dataTypeName()
{
    return "vtkAbstractTransform";
}

GCF_BEGIN_QUERY_TABLE(CVtkAbstractTransformData)
    GCF_IMPLEMENTS(IVtkAbstractTransformIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkAbstractTransformData)

vtkAbstractTransform* CVtkAbstractTransformData::getVtkAbstractTransform()
{
    return m_vtkAbstractTransform;
}

vtkObject* CVtkAbstractTransformData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkAbstractTransform );
}
