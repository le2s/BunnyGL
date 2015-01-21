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

#include "CVtkMapper2DData.h"
#include "vtkMapper2D.h"

CVtkMapper2DData::CVtkMapper2DData(vtkMapper2D* mapper)
:m_vtkMapper2D(mapper)
{

}

CVtkMapper2DData::~CVtkMapper2DData()
{

}

void CVtkMapper2DData::setMapper2D(vtkMapper2D* mapper)
{
    m_vtkMapper2D = mapper;
}

GCF_DEFINE_QUERY_TABLE(CVtkMapper2DData)
    GCF_IMPLEMENTS(IVtkMapper2DIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkMapper2DData)

void* CVtkMapper2DData::dataPointer()
{
    return (void*)(m_vtkMapper2D);
}

QString CVtkMapper2DData::dataTypeName()
{
    return "vtkMapper2D";
}

vtkMapper2D* CVtkMapper2DData::getVtkMapper2D()
{
    return m_vtkMapper2D;
}

vtkObject* CVtkMapper2DData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkMapper2D );
}


