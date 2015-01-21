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

#include "CVtkRegularPolygonSource.h"

DEFINE_VTK_OBJECT(CVtkRegularPolygonSource, CVtkPolyDataAlgorithm, vtkRegularPolygonSource)
{
    pDesc->setNodeClassName("vtkRegularPolygonSource");
    pDesc->setNodeClassDescription("Regular Polygone Source Algorithm");
    pDesc->setNodeClassCategory("PolyDataSource");

    pDesc->setPropertyInfo("NumberOfSides","SetNumberOfSides","GetNumberOfSides",QStringList() << "int",QStringList(), "void", "int");
    pDesc->setPropertyInfo("Center", "SetCenter", "GetCenter", QStringList() << "double"<<"double"<<"double",QStringList(),"void","double*");
    pDesc->setPropertyInfo("Normal", "SetNormal","GetNormal",QStringList() << "double"<<"double"<<"double",QStringList(),"void","double*");
    pDesc->setPropertyInfo("Radius", "SetRadius","GetRadius",QStringList() << "double",QStringList(),"void","double");
    pDesc->setPropertyInfo("GeneratePolygon", "SetGeneratePolygon","GetGeneratePolygon",QStringList() <<"int", QStringList(),"void","int");
}

CVtkRegularPolygonSource::CVtkRegularPolygonSource() : m_vtkRegularPolygonSource(0)
{
    CVtkRegularPolygonSource::InitializeObjectDesc();
    setVtkObject(vtkRegularPolygonSource::New());
}

CVtkRegularPolygonSource::~CVtkRegularPolygonSource()
{

}

int CVtkRegularPolygonSource::numberOfSides() const
{
    return m_vtkRegularPolygonSource->GetNumberOfSides();
}

void CVtkRegularPolygonSource::setNumberOfSides(int n)
{    
    m_vtkRegularPolygonSource->SetNumberOfSides(n);
}

Point3D CVtkRegularPolygonSource::center() const
{
    return (Point3D(m_vtkRegularPolygonSource->GetCenter()));
}

void CVtkRegularPolygonSource::setCenter(Point3D c)
{
    m_vtkRegularPolygonSource->SetCenter(c.x, c.y, c.z);
}


Point3D CVtkRegularPolygonSource::normal() const
{
    return (Point3D(m_vtkRegularPolygonSource->GetNormal()));
}

void CVtkRegularPolygonSource::setNormal(Point3D n)
{
    m_vtkRegularPolygonSource->SetNormal(n.x, n.y, n.z);
}


double CVtkRegularPolygonSource::radius() const
{
    return m_vtkRegularPolygonSource->GetRadius();
}

void CVtkRegularPolygonSource::setRadius(double r)
{
    m_vtkRegularPolygonSource->SetRadius(r);
}


bool CVtkRegularPolygonSource::isGeneratePolygon() const
{
    return m_vtkRegularPolygonSource->GetGeneratePolygon();
}

void CVtkRegularPolygonSource::setGeneratePolygon(bool val)
{
    m_vtkRegularPolygonSource->SetGeneratePolygon(val);
}




