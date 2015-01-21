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
#include "CVtkCellLocator.h"

DEFINE_VTK_OBJECT(CVtkCellLocator,CVtkLocator,vtkCellLocator)
{
    pDesc->setNodeClassName("vtkCellLocator");
    pDesc->setNodeClassDescription("Octree-based spatial search object to quickly locate cells");
    pDesc->setNodeClassCategory("Locator");

    pDesc->setPropertyInfo("NumberOfCellsPerBucket", "SetNumberOfCellsPerBucket", "GetNumberOfCellsPerBucket", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("CacheCellBounds", "SetCacheCellBounds", "GetCacheCellBounds", QStringList() << "int", QStringList(), "void", "int");
}

CVtkCellLocator::CVtkCellLocator() : m_vtkCellLocator(0)
{
    CVtkCellLocator::InitializeObjectDesc();
    setVtkObject(vtkCellLocator::New());
}

CVtkCellLocator::~CVtkCellLocator()
{

}

int CVtkCellLocator::numberOfCellsPerBucket() const
{
    return m_vtkCellLocator->GetNumberOfCellsPerBucket();
}


void CVtkCellLocator::setNumberOfCellsPerBucket(int val)
{
    m_vtkCellLocator->SetNumberOfCellsPerBucket(val);
}


bool CVtkCellLocator::isCacheCellBounds() const
{
    return m_vtkCellLocator->GetCacheCellBounds();
}


void CVtkCellLocator::setCacheCellBounds(bool val)
{
    m_vtkCellLocator->SetCacheCellBounds(val);
}

