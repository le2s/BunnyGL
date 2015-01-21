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

#include "CVtkPicker.h"
#include "vtkPoints.h"

DEFINE_VTK_OBJECT(CVtkPicker,CVtkAbstractPropPicker,vtkPicker)
{
    pDesc->setNodeClassName("vtkPicker");
    pDesc->setNodeClassDescription("Superclass for 3D geometric pickers (uses ray cast)");
    pDesc->setNodeClassCategory("Locator");
    pDesc->setCreateFunction(0);

    pDesc->setPropertyInfo("Tolerance", "SetTolerance", "GetTolerance", QStringList() << "double", QStringList(), "void", "double");
}

CVtkPicker::CVtkPicker() : m_vtkPicker(0)
{
    CVtkPicker::InitializeObjectDesc();
}

CVtkPicker::~CVtkPicker()
{

}

double CVtkPicker::tolerance() const
{
    return m_vtkPicker->GetTolerance();
}


void CVtkPicker::setTolerance(double val)
{
    m_vtkPicker->SetTolerance(val);
}

Point3D CVtkPicker::mapperPosition() const
{
    return m_vtkPicker->GetMapperPosition();
}

int CVtkPicker::pickedPositionCount() const
{
    vtkPoints* points = m_vtkPicker->GetPickedPositions();
    if(points)
        return points->GetNumberOfPoints();
    return 0;
}

Point3D CVtkPicker::pickedPosition(int index) const
{
    if(index < 0 || index >= pickedPositionCount())
        return Point3D();

    return m_vtkPicker->GetPickedPositions()->GetPoint( vtkIdType(index) );
}

