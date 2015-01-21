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
#include "CVtkGridSynchronizedTemplates3D.h"

DEFINE_VTK_OBJECT(CVtkGridSynchronizedTemplates3D, CVtkPolyDataFilter, vtkGridSynchronizedTemplates3D)
{
    pDesc->setNodeClassName("vtkGridSynchronizedTemplates");
    pDesc->setNodeClassDescription("Generate isosurface from structured grids");

    pDesc->setPropertyInfo("NumberOfContours","SetNumberOfContours","GetNumberOfContours",QStringList() << "int", QStringList(),"void","int");
    pDesc->setPropertyInfo("ComputeNormals","SetComputeNormals","GetComputeNormals",QStringList() << "int", QStringList(), "void","int");    
    pDesc->setPropertyInfo("ComputeGradients","SetComputeGradients","GetComputeGradients",QStringList() << "int", QStringList(), "void","int");    
    pDesc->setPropertyInfo("ComputeScalars","SetComputeScalars","GetComputeScalars",QStringList() << "int", QStringList(), "void","int");    
    
}

CVtkGridSynchronizedTemplates3D::CVtkGridSynchronizedTemplates3D() : m_vtkGridSynchronizedTemplates3D(0)
{
    CVtkGridSynchronizedTemplates3D::InitializeObjectDesc();
    setVtkObject(vtkGridSynchronizedTemplates3D::New());
}

CVtkGridSynchronizedTemplates3D::~CVtkGridSynchronizedTemplates3D()
{

}

void CVtkGridSynchronizedTemplates3D::setNumberOfContours(int number)
{
    m_vtkGridSynchronizedTemplates3D->SetNumberOfContours(number);
}

int CVtkGridSynchronizedTemplates3D::numberOfContours() const
{
    return m_vtkGridSynchronizedTemplates3D->GetNumberOfContours();
}

void CVtkGridSynchronizedTemplates3D::setComputeNormals(bool val)
{
    m_vtkGridSynchronizedTemplates3D->SetComputeNormals(val);
}

bool CVtkGridSynchronizedTemplates3D::isComputeNormals() const
{
    return m_vtkGridSynchronizedTemplates3D->GetComputeNormals();
}

void CVtkGridSynchronizedTemplates3D::setComputeGradients(bool val)
{
    m_vtkGridSynchronizedTemplates3D->SetComputeGradients(val);
}

int CVtkGridSynchronizedTemplates3D::isComputeGradients() const
{
    return m_vtkGridSynchronizedTemplates3D->GetComputeGradients();
}

void CVtkGridSynchronizedTemplates3D::setComputeScalars(bool val)
{
    m_vtkGridSynchronizedTemplates3D->SetComputeScalars(val);
}

int CVtkGridSynchronizedTemplates3D::isComputeScalars() const
{
    return m_vtkGridSynchronizedTemplates3D->GetComputeScalars();
}

