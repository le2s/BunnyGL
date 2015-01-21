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

#include "CVtkRuledSurfaceFilter.h"


DEFINE_VTK_OBJECT(CVtkRuledSurfaceFilter,CVtkPolyDataFilter,vtkRuledSurfaceFilter)
{
    pDesc->setNodeClassName("vtkRuledSurfaceFilter");
    pDesc->setNodeClassDescription("Generates a surface from a set of lines");

    pDesc->setPropertyInfo("DistanceFactor","SetDistanceFactor","GetDistanceFactor",QStringList()<<"double",QStringList(),"void","double");
    pDesc->setPropertyInfo("OnRatio","SetOnRatio","GetOnRatio",QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("Offset","SetOffset","GetOffset",QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("CloseSurface","SetCloseSurface","GetCloseSurface",QStringList()<<"bool",QStringList(),"void","bool");
    pDesc->setPropertyInfo("RuledMode","SetRuledMode","GetRuledMode",QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("PassLines","SetPassLines","GetPassLines",QStringList()<<"int",QStringList(),"void","int");
}

CVtkRuledSurfaceFilter::CVtkRuledSurfaceFilter() : m_vtkRuledSurfaceFilter(0)
{
    CVtkRuledSurfaceFilter::InitializeObjectDesc();
    setVtkObject(vtkRuledSurfaceFilter::New());
}

CVtkRuledSurfaceFilter::~CVtkRuledSurfaceFilter()
{

}

double CVtkRuledSurfaceFilter::distanceFactor() const
{
    return m_vtkRuledSurfaceFilter->GetDistanceFactor();
}

void CVtkRuledSurfaceFilter::setDistanceFactor(double val)
{
    m_vtkRuledSurfaceFilter->SetDistanceFactor(val);
}

int CVtkRuledSurfaceFilter::onRatio() const
{
    return m_vtkRuledSurfaceFilter->GetOnRatio();
}

void CVtkRuledSurfaceFilter::setOnRatio(int val)
{
    m_vtkRuledSurfaceFilter->SetOnRatio(val);
}

int CVtkRuledSurfaceFilter::offset() const
{
    return m_vtkRuledSurfaceFilter->GetOffset();
}

void CVtkRuledSurfaceFilter::setOffset(int val)
{
    m_vtkRuledSurfaceFilter->SetOffset(val);
}

bool CVtkRuledSurfaceFilter::isCloseSurface() const
{
    return m_vtkRuledSurfaceFilter->GetCloseSurface();
}

void CVtkRuledSurfaceFilter::setCloseSurface(bool val)
{
    m_vtkRuledSurfaceFilter->SetCloseSurface(val);
}

CVtkRuledSurfaceFilter::RuledMode CVtkRuledSurfaceFilter::ruledMode() const
{
    switch(m_vtkRuledSurfaceFilter->GetRuledMode())
    {
        case VTK_RULED_MODE_RESAMPLE: return ReSample;
        case VTK_RULED_MODE_POINT_WALK: return PointWalk;
        default: return ReSample;
    }
}

void CVtkRuledSurfaceFilter::setRuledMode(RuledMode val)
{
    switch(val)
    {
        case ReSample: m_vtkRuledSurfaceFilter->SetRuledMode(VTK_RULED_MODE_RESAMPLE);break;
        case PointWalk: m_vtkRuledSurfaceFilter->SetRuledMode(VTK_RULED_MODE_POINT_WALK);break;
        default:  m_vtkRuledSurfaceFilter->SetRuledMode(VTK_RULED_MODE_RESAMPLE);break;    
    }
}

bool CVtkRuledSurfaceFilter::isPassLines() const
{
    return m_vtkRuledSurfaceFilter->GetPassLines();
}

void CVtkRuledSurfaceFilter::setPassLines(bool val)
{
    m_vtkRuledSurfaceFilter->SetPassLines(val);
}





