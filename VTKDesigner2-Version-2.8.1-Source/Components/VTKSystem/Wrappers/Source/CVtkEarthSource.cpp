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

#include "CVtkEarthSource.h"

DEFINE_VTK_OBJECT(CVtkEarthSource, CVtkPolyDataAlgorithm, vtkEarthSource)
{
    pDesc->setNodeClassName("vtkEarthSource");
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassDescription("Create the continents of the Earth as a sphere");

    pDesc->setPropertyInfo("Radius","SetRadius","GetRadius",QStringList() << "double",QStringList(), "void","double");
    pDesc->setPropertyInfo("OnRatio","SetOnRatio","GetOnRatio",QStringList() <<"int",QStringList(), "void","int");
    pDesc->setPropertyInfo("Outline","SetOutline","GetOutline",QStringList() <<"int",QStringList(),"void","int");
}

CVtkEarthSource::CVtkEarthSource() : m_vtkEarthSource(0)
{
    CVtkEarthSource::InitializeObjectDesc();
    setVtkObject(vtkEarthSource::New());
}

CVtkEarthSource::~CVtkEarthSource()
{

}

double CVtkEarthSource::radius() const
{
    return m_vtkEarthSource->GetRadius();
}

void CVtkEarthSource::setRadius(double r)
{
    m_vtkEarthSource->SetRadius(r);
}

int CVtkEarthSource::onRatio() const
{
    return m_vtkEarthSource->GetOnRatio();
}

void CVtkEarthSource::setOnRatio(int ratio)
{
    m_vtkEarthSource->SetOnRatio(ratio);
}

bool CVtkEarthSource::isOutline() const
{
    return m_vtkEarthSource->GetOutline();
}

void CVtkEarthSource::setOutline(bool ol)
{
    m_vtkEarthSource->SetOutline(ol);
}
