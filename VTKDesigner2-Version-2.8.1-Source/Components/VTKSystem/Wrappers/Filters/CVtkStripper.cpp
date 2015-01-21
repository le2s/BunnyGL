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

#include "CVtkStripper.h"

DEFINE_VTK_OBJECT(CVtkStripper,CVtkPolyDataFilter,vtkStripper)
{
    pDesc->setNodeClassName("vtkStripper");
    pDesc->setNodeClassDescription("Create triangle strips and/or poly-lines");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("MaximumLength", "SetMaximumLength", "GetMaximumLength", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("PassCellDataAsFieldData", "SetPassCellDataAsFieldData", "GetPassCellDataAsFieldData", QStringList() << "int" , QStringList(), "void", "int");
}

CVtkStripper::CVtkStripper() : m_vtkStripper(0)
{
    CVtkStripper::InitializeObjectDesc();
    setVtkObject(vtkStripper::New());
}

CVtkStripper::~CVtkStripper()
{

}

int CVtkStripper::maximumLength() const
{
    return m_vtkStripper->GetMaximumLength();
}


void CVtkStripper::setMaximumLength(int val)
{
    m_vtkStripper->SetMaximumLength(val);
}


bool CVtkStripper::isPassCellDataAsFieldData() const
{
    return m_vtkStripper->GetPassCellDataAsFieldData();
}


void CVtkStripper::setPassCellDataAsFieldData(bool val)
{
    m_vtkStripper->SetPassCellDataAsFieldData(val);
}

