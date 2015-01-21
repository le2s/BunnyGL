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

#include "CVtkActorData.h"
#include "vtkActor.h"

CVtkActorData::CVtkActorData(vtkActor* actor)
:m_vtkActor(actor)
{

}

CVtkActorData::~CVtkActorData()
{

}

void CVtkActorData::setActor(vtkActor* actor)
{
    m_vtkActor = actor;
}

GCF_DEFINE_QUERY_TABLE(CVtkActorData)
    GCF_IMPLEMENTS(IVtkActorIOData)
    GCF_IMPLEMENTS(IVtkProp3DIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkActorData)

void* CVtkActorData::dataPointer()
{
    return (void*)(m_vtkActor);
}

QString CVtkActorData::dataTypeName()
{
    return "vtkActor";
}

vtkActor* CVtkActorData::getVtkActor()
{
    return m_vtkActor;
}

vtkProp3D* CVtkActorData::getVtkProp3D()
{
    return dynamic_cast<vtkProp3D*>(m_vtkActor);
}

vtkObject* CVtkActorData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkActor );
}

