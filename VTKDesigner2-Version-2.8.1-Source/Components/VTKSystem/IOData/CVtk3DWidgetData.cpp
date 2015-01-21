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

#include "CVtk3DWidgetData.h"

CVtk3DWidgetData::CVtk3DWidgetData(vtk3DWidget* widget)
:m_vtk3DWidget(widget)
{

}

CVtk3DWidgetData::~CVtk3DWidgetData()
{

}

void CVtk3DWidgetData::set3DWidget(vtk3DWidget* widget)
{
    m_vtk3DWidget = widget;
}


void* CVtk3DWidgetData::dataPointer()
{
    return m_vtk3DWidget;
}

QString CVtk3DWidgetData::dataTypeName()
{
    return "vtk3DWidget";
}

GCF_BEGIN_QUERY_TABLE(CVtk3DWidgetData)
    GCF_IMPLEMENTS(IVtk3DWidgetIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtk3DWidgetData)

vtk3DWidget* CVtk3DWidgetData::getVtk3DWidget()
{
    return m_vtk3DWidget;
}

vtkObject* CVtk3DWidgetData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtk3DWidget );
}


