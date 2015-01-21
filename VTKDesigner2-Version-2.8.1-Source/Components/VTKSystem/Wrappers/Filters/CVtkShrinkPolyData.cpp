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
#include "CVtkShrinkPolyData.h"

DEFINE_VTK_OBJECT(CVtkShrinkPolyData, CVtkPolyDataFilter, vtkShrinkPolyData)
{
    pDesc->setNodeClassName("vtkShrinkPolyData");
    pDesc->setNodeClassDescription("ShrinkPolyData");

    pDesc->setPropertyInfo("ShrinkFactor","SetShrinkFactor","GetShrinkFactor",QStringList() <<"double", QStringList(), "void", "double");
}



CVtkShrinkPolyData::CVtkShrinkPolyData() : m_vtkShrinkPolyData(0)
{
    CVtkShrinkPolyData::InitializeObjectDesc();
    setVtkObject(vtkShrinkPolyData::New());
}

CVtkShrinkPolyData::~CVtkShrinkPolyData()
{

}

double CVtkShrinkPolyData::shrinkFactor() const
{
    return m_vtkShrinkPolyData->GetShrinkFactor();    
}

void CVtkShrinkPolyData::setShrinkFactor(double val)
{
    m_vtkShrinkPolyData->SetShrinkFactor(val);    
}
