/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this flie is limited according to the terms specified by
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

#include "CVtkPolyDataSourceWidget.h"

DEFINE_VTK_OBJECT(CVtkPolyDataSourceWidget, CVtk3DWidget, vtkPolyDataSourceWidget)
{
    pDesc->setNodeClassName("vtkPolyDataSourceWidget");
    pDesc->setNodeClassCategory("Interactor Observers");
    pDesc->setNodeClassDescription("Provides a PolyDataSource widget");
};

CVtkPolyDataSourceWidget::CVtkPolyDataSourceWidget()
{
    m_vtkPolyDataSourceWidget = 0;
    setVtkObject(vtkPolyDataSourceWidget::New());
}

CVtkPolyDataSourceWidget::~CVtkPolyDataSourceWidget()
{

}