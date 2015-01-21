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

#include "CVtkPolyDataNormals.h"

DEFINE_VTK_OBJECT(CVtkPolyDataNormals, CVtkPolyDataFilter, vtkPolyDataNormals)
{
    pDesc->setNodeClassName("vtkPolyDataNormals");
    pDesc->setNodeClassDescription("Compute normals for polygonal mesh");

    pDesc->setPropertyInfo("FeatureAngle", "SetFeatureAngle", "GetFeatureAngle", QStringList()<<"double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Splitting", "SetSplitting", "GetSplitting", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Consistency", "SetConsistency", "GetConsistency", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("AutoOrientNormals", "SetAutoOrientNormals", "GetAutoOrientNormals", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ComputePointNormals", "SetComputePointNormals", "GetComputePointNormals", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ComputeCellNormals", "SetComputeCellNormals", "GetComputeCellNormals", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("FlipNormals", "SetFlipNormals", "GetFlipNormals", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("NonManifoldTraversal", "SetNonManifoldTraversal", "GetNonManifoldTraversal", QStringList() << "int", QStringList(), "void", "int");
}

CVtkPolyDataNormals::CVtkPolyDataNormals() : m_vtkPolyDataNormals(0)
{
    CVtkPolyDataNormals::InitializeObjectDesc();
    setVtkObject(vtkPolyDataNormals::New());
}

CVtkPolyDataNormals::~CVtkPolyDataNormals()
{

}

void CVtkPolyDataNormals::setFeatureAngle(double val)
{
    m_vtkPolyDataNormals->SetFeatureAngle(val);    
}

double CVtkPolyDataNormals::featureAngle() const
{
    return (m_vtkPolyDataNormals->GetFeatureAngle());
}

void CVtkPolyDataNormals::setSplitting(bool val)
{
    m_vtkPolyDataNormals->SetSplitting(val);    
}

bool CVtkPolyDataNormals::isSplitting() const
{
    return (m_vtkPolyDataNormals->GetSplitting());
}


void CVtkPolyDataNormals::setConsistency(bool val)
{
    m_vtkPolyDataNormals->SetConsistency(val);
}

bool CVtkPolyDataNormals::isConsistency() const
{
    return (m_vtkPolyDataNormals->GetConsistency());
}


void CVtkPolyDataNormals::setAutoOrientNormals(bool val)
{
    m_vtkPolyDataNormals->SetAutoOrientNormals(val);
}

bool CVtkPolyDataNormals::isAutoOrientNormals() const
{
    return (m_vtkPolyDataNormals->GetAutoOrientNormals());
}


void CVtkPolyDataNormals::setComputePointNormals(bool val)
{
    m_vtkPolyDataNormals->SetComputePointNormals(val);
}

bool CVtkPolyDataNormals::isComputePointNormals() const
{
    return (m_vtkPolyDataNormals->GetComputePointNormals());
}

void CVtkPolyDataNormals::setComputeCellNormals(bool val)
{
    m_vtkPolyDataNormals->SetComputeCellNormals(val);
}

bool CVtkPolyDataNormals::isComputeCellNormals() const
{
    return (m_vtkPolyDataNormals->GetComputeCellNormals());
}


void CVtkPolyDataNormals::setFlipNormals(bool val)
{
    m_vtkPolyDataNormals->SetFlipNormals(val);
}

bool CVtkPolyDataNormals::isFlipNormals() const
{
    return (m_vtkPolyDataNormals->GetFlipNormals());
}

void CVtkPolyDataNormals::setNonManifoldTraversal(bool val)
{
    m_vtkPolyDataNormals->SetNonManifoldTraversal(val);
}

bool CVtkPolyDataNormals::isNonManifoldTraversal() const
{
    return (m_vtkPolyDataNormals->GetNonManifoldTraversal());
}

