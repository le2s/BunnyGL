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
#include "CVtkPointPicker.h"

DEFINE_VTK_OBJECT(CVtkPointPicker,CVtkPicker,vtkPointPicker)
{
    pDesc->setNodeClassName("vtkPointPicker");
    pDesc->setNodeClassDescription("Select a point by shooting a ray into a graphics window");
    pDesc->setNodeClassCategory("Locator");

}

CVtkPointPicker::CVtkPointPicker() : m_vtkPointPicker(0)
{
    CVtkPointPicker::InitializeObjectDesc();
    setVtkObject(vtkPointPicker::New());
}

CVtkPointPicker::~CVtkPointPicker()
{

}

int CVtkPointPicker::pickedPointId()
{
    return m_vtkPointPicker->GetPointId();
}

