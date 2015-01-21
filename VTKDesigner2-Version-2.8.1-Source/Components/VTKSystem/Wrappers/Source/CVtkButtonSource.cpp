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

#include "CVtkButtonSource.h"

DEFINE_VTK_OBJECT(CVtkButtonSource, CVtkPolyDataAlgorithm, vtkButtonSource)
{
	pDesc->setNodeClassCategory("PolyDataSource");
	pDesc->setNodeClassName("vtkButtonSource");
	pDesc->setNodeClassDescription("Creates a Button polydata");
	pDesc->setPropertyInfo("TwoSided", "SetTwoSided", "GetTwoSided", QStringList() << "int",
						   QStringList(), "void", "int");
}

CVtkButtonSource::CVtkButtonSource() : m_vtkButtonSource(0)
{
	CVtkButtonSource::InitializeObjectDesc();
	setVtkObject(vtkButtonSource::New());
}

CVtkButtonSource::~CVtkButtonSource()
{

}

void CVtkButtonSource::setTwoSided(bool val)
{
	m_vtkButtonSource->SetTwoSided(val);	
}

bool CVtkButtonSource::isTwoSided() const
{
	return m_vtkButtonSource->GetTwoSided();
}
