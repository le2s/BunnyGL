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

#include "CVtkContourGrid.h"

DEFINE_VTK_OBJECT(CVtkContourGrid, CVtkPolyDataFilter, vtkContourGrid)
{
    pDesc->setNodeClassName("vtkContourGrid");
    pDesc->setNodeClassDescription("ContourGrid filter");

    pDesc->setPropertyInfo("NumberOfContours","SetNumberOfContours","GetNumberOfContours",QStringList() << "int", QStringList(),"void","int");
    pDesc->setPropertyInfo("ComputeNormals","SetComputeNormals","GetComputeNormals",QStringList() << "int", QStringList(), "void","int");    
    pDesc->setPropertyInfo("ComputeGradients","SetComputeGradients","GetComputeGradients",QStringList() << "int", QStringList(), "void","int");    
    pDesc->setPropertyInfo("ComputeScalars","SetComputeScalars","GetComputeScalars",QStringList() << "int", QStringList(), "void","int");    
    pDesc->setPropertyInfo("UseScalarTree","SetUseScalarTree","GetUseScalarTree",QStringList() << "int", QStringList(), "void","int");    
}

CVtkContourGrid::CVtkContourGrid() : m_vtkContourGrid(0)
{
    CVtkContourGrid::InitializeObjectDesc();
    setVtkObject(vtkContourGrid::New());
}

CVtkContourGrid::~CVtkContourGrid()
{

}

void CVtkContourGrid::setNumberOfContours(int number)
{
    m_vtkContourGrid->SetNumberOfContours(number);
}

int CVtkContourGrid::numberOfContours() const
{
    return m_vtkContourGrid->GetNumberOfContours();
}

void CVtkContourGrid::setComputeNormals(bool val)
{
    m_vtkContourGrid->SetComputeNormals(val);
}

bool CVtkContourGrid::isComputeNormals() const
{
    return m_vtkContourGrid->GetComputeNormals();
}

void CVtkContourGrid::setComputeGradients(bool val)
{
    m_vtkContourGrid->SetComputeGradients(val);
}

int CVtkContourGrid::isComputeGradients() const
{
    return m_vtkContourGrid->GetComputeGradients();
}

void CVtkContourGrid::setComputeScalars(bool val)
{
    m_vtkContourGrid->SetComputeScalars(val);
}

int CVtkContourGrid::isComputeScalars() const
{
    return m_vtkContourGrid->GetComputeScalars();
}

void CVtkContourGrid::setUseScalarTree(bool val)
{
    m_vtkContourGrid->SetUseScalarTree(val);
}

int CVtkContourGrid::isUseScalarTree() const
{
    return m_vtkContourGrid->GetUseScalarTree();
}
