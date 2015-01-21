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
#include "CVtkFeatureEdges.h"

DEFINE_VTK_OBJECT(CVtkFeatureEdges, CVtkPolyDataFilter, vtkFeatureEdges)
{
    pDesc->setNodeClassName("vtkFeatureEdges");
    pDesc->setNodeClassDescription("Extract boundary, non-manifold, and/or sharp edges from polygonal data");

    pDesc->setPropertyInfo("BoundaryEdges","SetBoundaryEdges","GetBoundaryEdges", QStringList() <<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("FeatureEdges","SetFeatureEdges","GetFeatureEdges", QStringList() <<"int",QStringList(),"void","int");    
    pDesc->setPropertyInfo("FeatureAngle","SetFeatureAngle","GetFeatureAngle", QStringList() <<"double",QStringList(),"void","double");
    pDesc->setPropertyInfo("NonManifoldEdges","SetNonManifoldEdges","GetNonManifoldEdges", QStringList() <<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("ManifoldEdges","SetManifoldEdges","GetManifoldEdges", QStringList() <<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("Coloring","SetColoring","GetColoring", QStringList() <<"int",QStringList(),"void","int");
}


CVtkFeatureEdges::CVtkFeatureEdges() : m_vtkFeatureEdges(0)
{
    CVtkFeatureEdges::InitializeObjectDesc();
    setVtkObject(vtkFeatureEdges::New());
}

CVtkFeatureEdges::~CVtkFeatureEdges()
{

}

void CVtkFeatureEdges::setBoundaryEdges(bool val)
{
    m_vtkFeatureEdges->SetBoundaryEdges(val);
}

bool CVtkFeatureEdges::isBoundaryEdges() const
{
    return m_vtkFeatureEdges->GetBoundaryEdges();
}


void CVtkFeatureEdges::setFeatureEdges(bool val)
{
    m_vtkFeatureEdges->SetFeatureEdges(val);
}

bool CVtkFeatureEdges::isFeatureEdges() const
{
    return m_vtkFeatureEdges->GetFeatureEdges();
}


void CVtkFeatureEdges::setFeatureAngle (double val)
{
    m_vtkFeatureEdges->SetFeatureAngle(val);
}

double CVtkFeatureEdges::featureAngle() const
{
    return m_vtkFeatureEdges->GetFeatureAngle();
}


void CVtkFeatureEdges::setNonManifoldEdges(bool val)
{
    m_vtkFeatureEdges->SetNonManifoldEdges(val);
}

bool CVtkFeatureEdges::isNonManifoldEdges() const
{
    return     m_vtkFeatureEdges->GetNonManifoldEdges();
}

void CVtkFeatureEdges::setManifoldEdges(bool val)
{
    m_vtkFeatureEdges->SetManifoldEdges(val);

}

bool CVtkFeatureEdges::isManifoldEdges() const
{
    return m_vtkFeatureEdges->GetManifoldEdges();
}

void CVtkFeatureEdges::setColoring(bool val)
{
    m_vtkFeatureEdges->SetColoring(val);
}

bool CVtkFeatureEdges::isColoring() const
{
    return m_vtkFeatureEdges->GetColoring();
}
