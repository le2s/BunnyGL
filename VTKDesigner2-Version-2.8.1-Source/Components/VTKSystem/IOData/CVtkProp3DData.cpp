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

#include "CVtkProp3DData.h"
#include "vtkProp3D.h"

CVtkProp3DData::CVtkProp3DData(vtkProp3D* actor)
:m_vtkProp3D(actor)
{

}

CVtkProp3DData::~CVtkProp3DData()
{

}

void CVtkProp3DData::setProp3D(vtkProp3D* actor)
{
    m_vtkProp3D = actor;
}

GCF_DEFINE_QUERY_TABLE(CVtkProp3DData)
    GCF_IMPLEMENTS(IVtkProp3DIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkProp3DData)

void* CVtkProp3DData::dataPointer()
{
    return (void*)(m_vtkProp3D);
}

QString CVtkProp3DData::dataTypeName()
{
    return "vtkProp3D";
}

vtkProp3D* CVtkProp3DData::getVtkProp3D()
{
    return m_vtkProp3D;
}

vtkObject* CVtkProp3DData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkProp3D );
}


