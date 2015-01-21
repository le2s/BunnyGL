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
#include "CVtkCellPicker.h"

DEFINE_VTK_OBJECT(CVtkCellPicker,CVtkPicker,vtkCellPicker)
{
    pDesc->setNodeClassName("vtkCellPicker");
    pDesc->setNodeClassDescription("Select a cell by shooting a ray into graphics window");
    pDesc->setNodeClassCategory("Locator");

}

CVtkCellPicker::CVtkCellPicker() : m_vtkCellPicker(0)
{
    CVtkCellPicker::InitializeObjectDesc();
    setVtkObject(vtkCellPicker::New());
}

CVtkCellPicker::~CVtkCellPicker()
{

}

int CVtkCellPicker::pickedCellId() const
{
    return m_vtkCellPicker->GetCellId();
}

int CVtkCellPicker::pickedSubId() const
{
    return m_vtkCellPicker->GetSubId();
}

Point3D CVtkCellPicker::pickedPCoords() const
{
    return m_vtkCellPicker->GetPCoords();
}



