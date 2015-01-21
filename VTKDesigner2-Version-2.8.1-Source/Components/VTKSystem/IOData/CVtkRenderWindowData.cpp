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

#include "CVtkRenderWindowData.h"

CVtkRenderWindowData::CVtkRenderWindowData(vtkRenderWindow* renderWindow)
:m_vtkRenderWindow(renderWindow)
{

}

CVtkRenderWindowData::~CVtkRenderWindowData()
{

}

void CVtkRenderWindowData::setRenderWindow(vtkRenderWindow* renderWindow)
{
    m_vtkRenderWindow = renderWindow;
}

void* CVtkRenderWindowData::dataPointer()
{
    return m_vtkRenderWindow;
}

QString CVtkRenderWindowData::dataTypeName()
{
    return "vtkRenderWindow";
}

GCF_BEGIN_QUERY_TABLE(CVtkRenderWindowData)
    GCF_IMPLEMENTS(IVtkRenderWindowIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkRenderWindowData)

vtkRenderWindow* CVtkRenderWindowData::getVtkRenderWindow()
{
    return m_vtkRenderWindow;
}

vtkObject* CVtkRenderWindowData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkRenderWindow );
}




