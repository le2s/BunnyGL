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
#include "CVtkBox.h"

DEFINE_VTK_OBJECT(CVtkBox,CVtkImplicitFunction,vtkBox)
{
    pDesc->setNodeClassName("vtkBox");
    pDesc->setNodeClassDescription("Implicit function for a bounding box");
    pDesc->setNodeClassCategory("Functions");

    pDesc->setPropertyInfo("XMin", "SetXMin", "GetXMin", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("XMax", "SetXMax", "GetXMax", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Bounds", "SetBounds", "GetBounds", QStringList() << "double" << "double" << "double" << "double" << "double" << "double", QStringList(), "void", "double*");
}

CVtkBox::CVtkBox() : m_vtkBox(0)
{
    CVtkBox::InitializeObjectDesc();
    setVtkObject(vtkBox::New());
}

CVtkBox::~CVtkBox()
{

}

Point3D CVtkBox::xMin() const
{
    Point3D ret;
    m_vtkBox->GetXMin(ret.x, ret.y, ret.z);
    return ret;
}


void CVtkBox::setXMin(Point3D val)
{
    m_vtkBox->SetXMin(val.x, val.y, val.z);
}


Point3D CVtkBox::xMax() const
{
    Point3D ret;
    m_vtkBox->GetXMax(ret.x, ret.y, ret.z);
    return ret;
}


void CVtkBox::setXMax(Point3D val)
{
    m_vtkBox->SetXMax(val.x, val.y, val.z);
}


BoundingBox3D CVtkBox::bounds() const
{
    BoundingBox3D box;
    m_vtkBox->GetBounds(box.xMin,box.xMax, box.yMin, box.yMax,box.zMin ,box.zMax);
    return BoundingBox3D(box);
}


void CVtkBox::setBounds(BoundingBox3D val)
{
    m_vtkBox->SetBounds(val.xMin,val.xMax,val.yMin,val.yMax,val.zMin,val.zMax);
}

