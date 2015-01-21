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

#include "CVtkSphereSource.h"

DEFINE_VTK_OBJECT(CVtkSphereSource, CVtkPolyDataAlgorithm, vtkSphereSource)
{
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassName("vtkSphereSource");
    pDesc->setNodeClassDescription("Creates a sphere polydata");

    pDesc->setPropertyInfo("Radius", "SetRadius", "GetRadius", QStringList() << "double",
                           QStringList(), "void", "double");
    pDesc->setPropertyInfo("Center", "SetCenter", "GetCenter", QStringList() << "double" << "double" << "double",
                           QStringList(), "void", "double*");
    pDesc->setPropertyInfo("ThetaResolution", "SetThetaResolution", "GetThetaResolution", QStringList() << "int",
                           QStringList(), "void", "int");
    pDesc->setPropertyInfo("PhiResolution", "SetPhiResolution", "GetPhiResolution", QStringList() << "int",
                           QStringList(), "void", "int");
    pDesc->setPropertyInfo("StartTheta", "SetStartTheta", "GetStartTheta", QStringList() << "double",
                           QStringList(), "void", "double");
    pDesc->setPropertyInfo("EndTheta", "SetEndTheta", "GetEndTheta", QStringList() << "double",
                           QStringList(), "void", "double");
    pDesc->setPropertyInfo("StartPhi", "SetStartPhi", "GetStartPhi", QStringList() << "double",
                           QStringList(), "void", "double");
    pDesc->setPropertyInfo("EndPhi", "SetEndPhi", "GetEndPhi", QStringList() << "double",
                           QStringList(), "void", "double");
    pDesc->setPropertyInfo("LatLongTessellation", "SetLatLongTessellation", "GetLatLongTessellation", QStringList() << "int",
                           QStringList(), "void", "int");
}

CVtkSphereSource::CVtkSphereSource() : m_vtkSphereSource(0)
{
    CVtkSphereSource::InitializeObjectDesc();
    setVtkObject(vtkSphereSource::New());
}

CVtkSphereSource::~CVtkSphereSource()
{

}

void CVtkSphereSource::setRadius(double r)
{
    m_vtkSphereSource->SetRadius(r);
}

double CVtkSphereSource::radius() const
{
    return m_vtkSphereSource->GetRadius();
}

void CVtkSphereSource::setCenter(Point3D p)
{
    m_vtkSphereSource->SetCenter(p.x, p.y, p.z);
}

Point3D CVtkSphereSource::center() const
{
    double* center = m_vtkSphereSource->GetCenter();
    return Point3D(center);
}

void CVtkSphereSource::setThetaResolution(int res)
{
    m_vtkSphereSource->SetThetaResolution(res);
}

int CVtkSphereSource::thetaResolution() const
{
    return m_vtkSphereSource->GetThetaResolution();
}

void CVtkSphereSource::setPhiResolution(int res)
{
    m_vtkSphereSource->SetPhiResolution(res);
}

int CVtkSphereSource::phiResolution() const
{
    return m_vtkSphereSource->GetPhiResolution();
}

void CVtkSphereSource::setStartTheta(double val)
{
    m_vtkSphereSource->SetStartTheta(val);
}

double CVtkSphereSource::startTheta() const
{
    return m_vtkSphereSource->GetStartTheta();
}

void CVtkSphereSource::setStartPhi(double val)
{
    m_vtkSphereSource->SetStartPhi(val);
}

double CVtkSphereSource::startPhi() const
{
    return m_vtkSphereSource->GetStartPhi();
}

void CVtkSphereSource::setEndTheta(double val)
{
    m_vtkSphereSource->SetEndTheta(val);
}

double CVtkSphereSource::endTheta() const
{
    return m_vtkSphereSource->GetEndTheta();
}

void CVtkSphereSource::setEndPhi(double val)
{
    m_vtkSphereSource->SetEndPhi(val);
}

double CVtkSphereSource::endPhi() const
{
    return m_vtkSphereSource->GetEndPhi();
}

void CVtkSphereSource::setLatLongTessellation(bool val)
{
    m_vtkSphereSource->SetLatLongTessellation(val);
}

bool CVtkSphereSource::latLongTessellation() const
{
    return m_vtkSphereSource->GetLatLongTessellation();
}
