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

#include "CVtkActor2DData.h"
#include "vtkActor2D.h"

CVtkActor2DData::CVtkActor2DData(vtkActor2D* actor)
:m_vtkActor2D(actor)
{

}

CVtkActor2DData::~CVtkActor2DData()
{

}

void CVtkActor2DData::setActor2D(vtkActor2D* actor)
{
    m_vtkActor2D = actor;
}

GCF_DEFINE_QUERY_TABLE(CVtkActor2DData)
    GCF_IMPLEMENTS(IVtkActor2DIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkActor2DData)

void* CVtkActor2DData::dataPointer()
{
    return (void*)(m_vtkActor2D);
}

QString CVtkActor2DData::dataTypeName()
{
    return "vtkActor2D";
}

vtkActor2D* CVtkActor2DData::getVtkActor2D()
{
    return m_vtkActor2D;
}

vtkObject* CVtkActor2DData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkActor2D );
}

