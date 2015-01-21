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

#include "CVtkPropertyData.h"
#include "vtkProperty.h"

CVtkPropertyData::CVtkPropertyData(vtkProperty* property)
:m_vtkProperty(property)
{

}

CVtkPropertyData::~CVtkPropertyData()
{

}

void CVtkPropertyData::setProperty(vtkProperty* property)
{
    m_vtkProperty = property;
}

GCF_DEFINE_QUERY_TABLE(CVtkPropertyData)
    GCF_IMPLEMENTS(IVtkPropertyIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkPropertyData)

void* CVtkPropertyData::dataPointer()
{
    return (void*)(m_vtkProperty);
}

QString CVtkPropertyData::dataTypeName()
{
    return "vtkProperty";
}

vtkProperty* CVtkPropertyData::getVtkProperty()
{
    return m_vtkProperty;
}

vtkObject* CVtkPropertyData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkProperty );
}




