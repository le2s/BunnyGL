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

#include "CVtkDuplicatePolyData.h"

DEFINE_VTK_OBJECT(CVtkDuplicatePolyData,CVtkPolyDataFilter,vtkDuplicatePolyData)
{
	pDesc->setNodeClassName("vtkDuplicatePolyData");
	pDesc->setNodeClassDescription("For distributed tiled displays");
	pDesc->setNodeClassCategory("PolyData Filters");

	pDesc->setPropertyInfo("Synchronous", "SetSynchronous", "GetSynchronous", QStringList() << "int" , QStringList(), "void", "int");
	pDesc->setPropertyInfo("ClientFlag", "SetClientFlag", "GetClientFlag", QStringList() << "int", QStringList(), "void", "int");
}

CVtkDuplicatePolyData::CVtkDuplicatePolyData() : m_vtkDuplicatePolyData(0)
{
	CVtkDuplicatePolyData::InitializeObjectDesc();
//	setVtkObject(vtkDuplicatePolyData::New());
}

CVtkDuplicatePolyData::~CVtkDuplicatePolyData()
{

}

bool CVtkDuplicatePolyData::isSynchronous() const
{
	return m_vtkDuplicatePolyData->GetSynchronous();
}


void CVtkDuplicatePolyData::setSynchronous(bool val)
{
	m_vtkDuplicatePolyData->SetSynchronous(val);
}


int CVtkDuplicatePolyData::clientFlag() const
{
	return m_vtkDuplicatePolyData->GetClientFlag();
}


void CVtkDuplicatePolyData::setClientFlag(int val)
{
	m_vtkDuplicatePolyData->SetClientFlag(val);
}

