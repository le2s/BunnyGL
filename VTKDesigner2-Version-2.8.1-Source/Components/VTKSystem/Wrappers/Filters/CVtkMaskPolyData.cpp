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

#include "CVtkMaskPolyData.h"

DEFINE_VTK_OBJECT(CVtkMaskPolyData, CVtkPolyDataFilter,vtkMaskPolyData)
{
    pDesc->setNodeClassName("vtkMaskPolyData");    
    pDesc->setNodeClassDescription("Sample subset of input polygonal data cells");

    pDesc->setPropertyInfo("OnRatio","SetOnRatio","GetOnRatio",QStringList() << "int",QStringList(),"void","int");
    pDesc->setPropertyInfo("Offset","SetOffset","GetOffset",QStringList() << "vtkIdType",QStringList(),"void","vtkIdType");
}

CVtkMaskPolyData::CVtkMaskPolyData() : m_vtkMaskPolyData(0)
{
    CVtkMaskPolyData::InitializeObjectDesc();
    setVtkObject(vtkMaskPolyData::New());
}

CVtkMaskPolyData::~CVtkMaskPolyData()
{

}

int CVtkMaskPolyData::onRatio() const
{
    return m_vtkMaskPolyData->GetOnRatio();
}

void CVtkMaskPolyData::setOnRatio(int val)
{
    m_vtkMaskPolyData->SetOnRatio(val);
}

int CVtkMaskPolyData::offset() const
{
    return m_vtkMaskPolyData->GetOffset();
}

void CVtkMaskPolyData::setOffset(int val)
{
    m_vtkMaskPolyData->SetOffset((int)val);
}

