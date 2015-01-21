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

#include "CVtkDataObject.h"
#include "vtkDataObject.h"

CVtkDataObject::CVtkDataObject(vtkDataObject* dataObject)
:m_vtkDataObject(dataObject)
{

}

CVtkDataObject::~CVtkDataObject()
{

}

void CVtkDataObject::setDataObject(vtkDataObject* dataObject)
{
    m_vtkDataObject = dataObject;
}

GCF_DEFINE_QUERY_TABLE(CVtkDataObject)
    GCF_IMPLEMENTS(IVtkDataObjectIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkDataObject)

void* CVtkDataObject::dataPointer()
{
    return (void*)(m_vtkDataObject);
}

QString CVtkDataObject::dataTypeName()
{
    return "vtkDataObject";
}

vtkDataObject* CVtkDataObject::getVtkDataObject()
{
    return m_vtkDataObject;
}

vtkObject* CVtkDataObject::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkDataObject );
}

