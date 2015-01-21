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

#include "CVtkDiskSource.h"

DEFINE_VTK_OBJECT(CVtkDiskSource, CVtkPolyDataAlgorithm, vtkDiskSource)
{
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassName("vtkDiskSource");
    pDesc->setNodeClassDescription("Disk Source Algorithm");
    pDesc->setPropertyInfo("InnerRadius", "SetInnerRadius", "GetInnerRadius", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("OuterRadius","SetOuterRadius","GetOuterRadius",QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("RadialResolution", "SetRadialResolution", "GetRadialResolution", QStringList() << "int",QStringList(), "void", "int");
    pDesc->setPropertyInfo("CircumferentialResolution", "SetCircumferentialResolution", "GetCircumferentialResolution", QStringList() << "double", QStringList(), "void", "int");
}

CVtkDiskSource::CVtkDiskSource() : m_vtkDiskSource(0)
{
    CVtkDiskSource::InitializeObjectDesc();
    setVtkObject(vtkDiskSource::New());
}

CVtkDiskSource::~CVtkDiskSource()
{

}
    
double CVtkDiskSource::innerRadius() const
{
    return m_vtkDiskSource->GetInnerRadius();
}

void CVtkDiskSource::setInnerRadius(double ir)
{
    m_vtkDiskSource->SetInnerRadius(ir);
}

double CVtkDiskSource::outerRadius() const
{
    return m_vtkDiskSource->GetOuterRadius();
}

void CVtkDiskSource::setOuterRadius(double rad)
{
    m_vtkDiskSource->SetOuterRadius(rad);
}

int CVtkDiskSource::radialResolution() const
{
    return m_vtkDiskSource->GetRadialResolution();
}

void CVtkDiskSource::setRadialResolution(int rr)
{
    m_vtkDiskSource->SetRadialResolution(rr);
}

int CVtkDiskSource::circumferentialResolution() const
{
    return m_vtkDiskSource->GetCircumferentialResolution();
}

void CVtkDiskSource::setCircumferentialResolution(int cr)
{
    m_vtkDiskSource->SetCircumferentialResolution(cr);
}

