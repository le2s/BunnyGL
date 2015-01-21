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

#include "CVtkRotationalExtrusionFilter.h"



DEFINE_VTK_OBJECT(CVtkRotationalExtrusionFilter,CVtkPolyDataFilter,vtkRotationalExtrusionFilter)
{
    pDesc->setNodeClassName("vtkRotationalExtrusionFilter");
    pDesc->setNodeClassDescription("Sweep polygonal data creating \"skirt\" from free edges and lines, and lines from vertices");

    pDesc->setPropertyInfo("Resolution","SetResolution","GetResolution",QStringList()<<"int",QStringList(),"void","int");    
    pDesc->setPropertyInfo("Capping","SetCapping","GetCapping",QStringList()<<"int",QStringList(),"void","int");    
    pDesc->setPropertyInfo("Angle","SetAngle","GetAngle",QStringList()<<"double",QStringList(),"void","double");    
    pDesc->setPropertyInfo("Translation","SetTranslation","GetTranslation",QStringList()<<"double",QStringList(),"void","double");    
    pDesc->setPropertyInfo("DeltaRadius","SetDeltaRadius","GetDeltaRadius",QStringList()<<"double",QStringList(),"void","double");    
}
    
CVtkRotationalExtrusionFilter::CVtkRotationalExtrusionFilter() : m_vtkRotationalExtrusionFilter(0)
{
    CVtkRotationalExtrusionFilter::InitializeObjectDesc();
    setVtkObject(vtkRotationalExtrusionFilter::New());
}

CVtkRotationalExtrusionFilter::~CVtkRotationalExtrusionFilter()
{

}

int CVtkRotationalExtrusionFilter::resolution() const
{
    return m_vtkRotationalExtrusionFilter->GetResolution();
}

void CVtkRotationalExtrusionFilter::setResolution(int r)
{
    m_vtkRotationalExtrusionFilter->SetResolution(r);
}


bool CVtkRotationalExtrusionFilter::isCapping() const
{
    return m_vtkRotationalExtrusionFilter->GetCapping();
}

void CVtkRotationalExtrusionFilter::setCapping(bool val)
{
    m_vtkRotationalExtrusionFilter->SetCapping(val);
}


double CVtkRotationalExtrusionFilter::angle() const
{
    return m_vtkRotationalExtrusionFilter->GetAngle();
}

void CVtkRotationalExtrusionFilter::setAngle(double val)
{
    m_vtkRotationalExtrusionFilter->SetAngle(val);
}


double CVtkRotationalExtrusionFilter::translation() const
{
    return m_vtkRotationalExtrusionFilter->GetTranslation();
}

void CVtkRotationalExtrusionFilter::setTranslation(double val)
{
    m_vtkRotationalExtrusionFilter->SetTranslation(val);
}


double CVtkRotationalExtrusionFilter::deltaRadius() const
{
    return m_vtkRotationalExtrusionFilter->GetDeltaRadius();
}

void CVtkRotationalExtrusionFilter::setDeltaRadius(double r)
{
    m_vtkRotationalExtrusionFilter->SetDeltaRadius(r);
}
