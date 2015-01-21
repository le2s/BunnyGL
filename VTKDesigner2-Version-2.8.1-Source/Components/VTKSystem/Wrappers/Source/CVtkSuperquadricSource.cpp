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

#include "CVtkSuperquadricSource.h"


DEFINE_VTK_OBJECT(CVtkSuperquadricSource, CVtkPolyDataAlgorithm, vtkSuperquadricSource)
{
    pDesc->setNodeClassName("vtkSuperquadricSource");
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassDescription("Superquadric Polydata source");    

    pDesc->setPropertyInfo("Center", "SetCenter", "GetCenter",QStringList() << "double" << "double" << "double" ,
                            QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Scale", "SetScale", "GetScale",QStringList() <<"double"<<"double"<<"double",
                            QStringList(), "void", "double*");
    pDesc->setPropertyInfo("ThetaResolution", "SetThetaResolution", "GetThetaResolution",QStringList() <<"int",QStringList(), "void", "int");    
    pDesc->setPropertyInfo("PhiResolution", "SetPhiResolution", "GetPhiResolution",QStringList() <<"int",QStringList(), "void", "int");        
    pDesc->setPropertyInfo("Thickness", "SetThickness", "GetThickness",QStringList() <<"double",QStringList(), "void", "double");                
    pDesc->setPropertyInfo("PhiRoundness", "SetPhiRoundness", "GetPhiRoundness",QStringList() <<"double",QStringList(), "void", "double");                
    pDesc->setPropertyInfo("ThetaRoundness ", "SetThetaRoundness ", "GetThetaRoundness ",QStringList() <<"double",QStringList(), "void", "double");            
    pDesc->setPropertyInfo("Size", "SetSize", "GetSize",QStringList() <<"double",QStringList(), "void", "double");                
    pDesc->setPropertyInfo("Toroidal", "SetToroidal", "GetToroidal",QStringList() <<"int",QStringList(), "void", "int");                    
}

CVtkSuperquadricSource::CVtkSuperquadricSource() : m_vtkSuperquadricSource(0)
{
    CVtkSuperquadricSource::InitializeObjectDesc();
    setVtkObject(vtkSuperquadricSource::New());
}

CVtkSuperquadricSource::~CVtkSuperquadricSource()
{
        
}

void CVtkSuperquadricSource::setCenter(Point3D c)
{
    m_vtkSuperquadricSource->SetCenter(c.x, c.y, c.z);
}

Point3D CVtkSuperquadricSource::center() const
{
    return m_vtkSuperquadricSource->GetCenter();
}

void CVtkSuperquadricSource::setScale(Point3D s)
{
    m_vtkSuperquadricSource->SetScale(s.x, s.y, s.z);
}

Point3D CVtkSuperquadricSource::scale() const
{
    return m_vtkSuperquadricSource->GetScale();
}

void CVtkSuperquadricSource::setThetaResolution(int tr)
{
    m_vtkSuperquadricSource->SetThetaResolution(tr);
}

int CVtkSuperquadricSource::thetaResolution() const
{
    return m_vtkSuperquadricSource->GetThetaResolution();
}


void CVtkSuperquadricSource::setPhiResolution(int ps)
{
    m_vtkSuperquadricSource->SetPhiResolution(ps);
}

int CVtkSuperquadricSource::phiResolution() const
{
    return m_vtkSuperquadricSource->GetPhiResolution();
}

void CVtkSuperquadricSource::setThetaRoundness(double st)
{
    m_vtkSuperquadricSource->SetThetaRoundness(st);
}

double CVtkSuperquadricSource::thetaRoundness() const
{
    return m_vtkSuperquadricSource->GetThetaRoundness();
}

void CVtkSuperquadricSource::setThickness(double t)
{
    m_vtkSuperquadricSource->SetThickness(t);
}

double CVtkSuperquadricSource::thickness() const
{
    return m_vtkSuperquadricSource->GetThickness();
}

void CVtkSuperquadricSource::setPhiRoundness(double pr)
{
    m_vtkSuperquadricSource->SetPhiRoundness(pr);
}

double CVtkSuperquadricSource::phiRoundness() const
{
    return m_vtkSuperquadricSource->GetPhiRoundness();
}

void CVtkSuperquadricSource::setSize(double s)
{
    m_vtkSuperquadricSource->SetSize(s);
}

double CVtkSuperquadricSource::size() const
{
    return m_vtkSuperquadricSource->GetSize();
}

void CVtkSuperquadricSource::setToroidal(bool val)
{
    m_vtkSuperquadricSource->SetToroidal(val);
}

bool  CVtkSuperquadricSource::isToroidal() const
{
    return m_vtkSuperquadricSource->GetToroidal();
}

