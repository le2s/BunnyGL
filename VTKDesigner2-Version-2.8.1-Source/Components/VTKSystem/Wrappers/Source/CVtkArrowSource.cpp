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

#include "CVtkArrowSource.h"
#include "CVtkPolyData.h"

DEFINE_VTK_OBJECT(CVtkArrowSource, CVtkPolyDataAlgorithm, vtkArrowSource)
{
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassName("vtkArrowSource");
    pDesc->setNodeClassDescription("Arrow Source Algorithm");
    pDesc->setPropertyInfo("TipLength", "SetTipLength", "GetTipLength", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("TipRadius","SetTipRadius","GetTipRadius",QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("TipResolution", "SetTipResolution", "GetTipResolution", QStringList() << "int",QStringList(), "void", "int");

    pDesc->setPropertyInfo("ShaftRadius", "SetShaftRadius", "GetShaftRadius", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("ShaftResolution", "SetShaftResolution", "GetShaftResolution", QStringList() << "int", QStringList(), "void", "int");
}

CVtkArrowSource::CVtkArrowSource() : m_vtkArrowSource(0)
{
    CVtkArrowSource::InitializeObjectDesc();
    setVtkObject(vtkArrowSource::New());
}

CVtkArrowSource::~CVtkArrowSource()
{

}

double CVtkArrowSource::tipLength() const
{
    return m_vtkArrowSource->GetTipLength();
}

void CVtkArrowSource::setTipLength(double tl)
{
    m_vtkArrowSource->SetTipLength(tl);
}

double CVtkArrowSource::tipRadius() const
{
    return m_vtkArrowSource->GetTipRadius();
}

void CVtkArrowSource::setTipRadius(double tiprad)
{
    m_vtkArrowSource->SetTipRadius(tiprad);
}

int CVtkArrowSource::tipResolution() const
{
    return m_vtkArrowSource->GetTipResolution();
}

void CVtkArrowSource::setTipResolution(int tipres)
{
    m_vtkArrowSource->SetTipResolution(tipres);
}

double CVtkArrowSource::shaftRadius() const
{
    return m_vtkArrowSource->GetShaftRadius();
}

void CVtkArrowSource::setShaftRadius(double shrad)
{
    m_vtkArrowSource->SetShaftRadius(shrad);
}

int CVtkArrowSource::shaftResolution() const
{
    return m_vtkArrowSource->GetShaftResolution();
}

void CVtkArrowSource::setShaftResolution(int shres)
{
    m_vtkArrowSource->SetShaftResolution(shres);
}

