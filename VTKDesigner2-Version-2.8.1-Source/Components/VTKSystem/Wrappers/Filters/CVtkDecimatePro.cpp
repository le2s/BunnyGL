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
#include "CVtkDecimatePro.h"

DEFINE_VTK_OBJECT(CVtkDecimatePro, CVtkPolyDataFilter, vtkDecimatePro)
{
    pDesc->setNodeClassName("vtkDecimatePro");
    pDesc->setNodeClassDescription("Reduce the number of triangles in a mesh");

    pDesc->setPropertyInfo("TargetReduction","SetTargetReduction","SetTargetReduction",QStringList()<<"double", QStringList(),"void","double");
    pDesc->setPropertyInfo("PreserveTopology","SetPreserveTopology","GetPreserveTopology",QStringList()<<"int", QStringList(),"void","int");
    pDesc->setPropertyInfo("FeatureAngle","SetFeatureAngle","GetFeatureAngle",QStringList()<<"double", QStringList(),"void","double");
    pDesc->setPropertyInfo("Splitting","SetSplitting","GetSplitting",QStringList()<<"int", QStringList(),"void","int");
    pDesc->setPropertyInfo("SplitAngle","SetSplittAngle","GetSplittAngle",QStringList()<<"double", QStringList(),"void","double");
    pDesc->setPropertyInfo("PreSplitMesh","SetPreSplitMesh","GetPreSplitMesh",QStringList()<<"int", QStringList(),"void","int");
    pDesc->setPropertyInfo("MaximumError","SetMaximumError","GetMaximumError",QStringList()<<"double", QStringList(),"void","double");
    pDesc->setPropertyInfo("AccumulateError","SetAccumulateError","GetAccumulateError",QStringList()<<"int", QStringList(),"void","int");
    pDesc->setPropertyInfo("ErrorIsAbsolute","SetErrorIsAbsolute","GetErrorIsAbsolute",QStringList()<<"int", QStringList(),"void","int");
    pDesc->setPropertyInfo("AbsoluteError","SetAbsoluteError","GetAbsoluteError",QStringList()<<"double", QStringList(),"void","double");
    pDesc->setPropertyInfo("BoundaryVertexDeletion","SetBoundaryVertexDeletion","GetBoundaryVertexDeletion",QStringList()<<"int", QStringList(),"void","int");
    pDesc->setPropertyInfo("Degree","SetDegree","GetDegree",QStringList()<<"int", QStringList(),"void","int");
    pDesc->setPropertyInfo("InflectionPointRatio","SetInflectionPointRatio","GetInflectionPointRatio",QStringList()<<"double", QStringList(),"void","double");
}

CVtkDecimatePro::CVtkDecimatePro() : m_vtkDecimatePro(0)
{
    CVtkDecimatePro::InitializeObjectDesc();
    setVtkObject(vtkDecimatePro::New());
}

CVtkDecimatePro::~CVtkDecimatePro()
{

}


double CVtkDecimatePro::targetReduction() const
{
    return m_vtkDecimatePro->GetTargetReduction();
}

void CVtkDecimatePro::setTargetReduction(double val)
{
    m_vtkDecimatePro->SetTargetReduction(val);
}


bool CVtkDecimatePro::isPreserveTopology() const
{
    return m_vtkDecimatePro->GetPreserveTopology();
}

void CVtkDecimatePro::setPreserveTopology(bool val)
{
    m_vtkDecimatePro->SetPreserveTopology(val);
}


double CVtkDecimatePro::featureAngle() const
{
    return m_vtkDecimatePro->GetFeatureAngle();
}

void CVtkDecimatePro::setFeatureAngle(double val)
{
    m_vtkDecimatePro->SetFeatureAngle(val);
}


bool CVtkDecimatePro::isSplitting() const
{
    return m_vtkDecimatePro->GetSplitting();
}

void CVtkDecimatePro::setSplitting(bool val)
{
    m_vtkDecimatePro->SetSplitting(val);
}


double CVtkDecimatePro::splitAngle() const
{
    return m_vtkDecimatePro->GetSplitAngle();
}

void CVtkDecimatePro::setSplitAngle(double val)
{
    m_vtkDecimatePro->SetSplitAngle(val);
}


bool CVtkDecimatePro::isPreSplitMesh() const
{
    return m_vtkDecimatePro->GetPreSplitMesh();
}

void CVtkDecimatePro::setPreSplitMesh(bool val)
{
    m_vtkDecimatePro->SetPreSplitMesh(val);
}


double CVtkDecimatePro::maximumError() const
{
    return m_vtkDecimatePro->GetMaximumError();
}

void CVtkDecimatePro::setMaximumError(double val)
{
    m_vtkDecimatePro->SetMaximumError(val);
}


bool CVtkDecimatePro::isAccumulateError() const
{
    return m_vtkDecimatePro->GetAccumulateError();
}

void CVtkDecimatePro::setAccumulateError(bool val)
{
    m_vtkDecimatePro->SetAccumulateError(val);
}


bool CVtkDecimatePro::isErrorIsAbsolute() const
{
    return m_vtkDecimatePro->GetErrorIsAbsolute();
}

void CVtkDecimatePro::setErrorIsAbsolute(bool val)
{
    m_vtkDecimatePro->SetErrorIsAbsolute(val);
}


double CVtkDecimatePro::absoluteError() const
{
    return m_vtkDecimatePro->GetAbsoluteError();
}

void CVtkDecimatePro::setAbsoluteError(double val)
{
    m_vtkDecimatePro->SetAbsoluteError(val);
}


bool CVtkDecimatePro::isBoundaryVertexDeletion() const
{
    return m_vtkDecimatePro->GetBoundaryVertexDeletion();
}

void CVtkDecimatePro::setBoundaryVertexDeletion(bool val)
{
    m_vtkDecimatePro->SetBoundaryVertexDeletion(val);
}


int CVtkDecimatePro::degree() const
{
    return m_vtkDecimatePro->GetDegree();
}

void CVtkDecimatePro::setDegree(int val)
{
    m_vtkDecimatePro->SetDegree(val);
}


double CVtkDecimatePro::inflectionPointRatio() const
{
    return m_vtkDecimatePro->GetInflectionPointRatio();
}

void CVtkDecimatePro::setInflectionPointRatio(double val)
{
    m_vtkDecimatePro->SetInflectionPointRatio(val);
}



