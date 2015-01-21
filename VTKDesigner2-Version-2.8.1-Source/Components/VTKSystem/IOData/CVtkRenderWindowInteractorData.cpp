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

#include "CVtkRenderWindowInteractorData.h"

CVtkRenderWindowInteractorData::CVtkRenderWindowInteractorData(vtkRenderWindowInteractor* RenderWindowInteractor)
{
    m_vtkRenderWindowInteractor = RenderWindowInteractor;
}

CVtkRenderWindowInteractorData::~CVtkRenderWindowInteractorData()
{

}

void CVtkRenderWindowInteractorData::setRenderWindowInteractor(vtkRenderWindowInteractor* RenderWindowInteractor)
{
    m_vtkRenderWindowInteractor = RenderWindowInteractor;
}

GCF_DEFINE_QUERY_TABLE(CVtkRenderWindowInteractorData)
    GCF_IMPLEMENTS(IVtkRenderWindowInteractorIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkRenderWindowInteractorData)

void* CVtkRenderWindowInteractorData::dataPointer()
{
    return m_vtkRenderWindowInteractor;
}

QString CVtkRenderWindowInteractorData::dataTypeName()
{
    return "vtkRenderWindowInteractor";
}

vtkRenderWindowInteractor* CVtkRenderWindowInteractorData::getVtkRenderWindowInteractor()
{
    return m_vtkRenderWindowInteractor;
}

vtkObject* CVtkRenderWindowInteractorData::getVtkObject()
{
    return vtkObject::SafeDownCast(m_vtkRenderWindowInteractor);
}


