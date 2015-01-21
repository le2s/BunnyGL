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

#include "CVtkRendererData.h"

CVtkRendererData::CVtkRendererData(vtkRenderer* renderer)
:m_vtkRenderer(renderer)
{

}

CVtkRendererData::~CVtkRendererData()
{

}

void CVtkRendererData::setRenderer(vtkRenderer* renderer)
{
    m_vtkRenderer = renderer;
}

void* CVtkRendererData::dataPointer()
{
    return m_vtkRenderer;
}

QString CVtkRendererData::dataTypeName()
{
    return "vtkRenderer";
}

GCF_BEGIN_QUERY_TABLE(CVtkRendererData)
    GCF_IMPLEMENTS(IVtkRendererIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkRendererData)

vtkRenderer* CVtkRendererData::getVtkRenderer()
{
    return m_vtkRenderer;
}

vtkObject* CVtkRendererData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkRenderer );
}

