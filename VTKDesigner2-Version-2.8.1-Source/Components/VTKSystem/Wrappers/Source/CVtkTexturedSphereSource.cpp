/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
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

#include "CVtkTexturedSphereSource.h"

DEFINE_VTK_OBJECT(CVtkTexturedSphereSource, CVtkPolyDataAlgorithm, vtkTexturedSphereSource)
{
    pDesc->setNodeClassName("vtkTexturedSphereSource");
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassDescription("TextureSphereSource Algorithm");

    pDesc->setPropertyInfo("Radius", "SetRadius", "GetRadius", QStringList() << "double",
                           QStringList(), "void", "double");
    pDesc->setPropertyInfo("ThetaResolution", "SetThetaResolution", "GetThetaResolution", QStringList() << "int",
                           QStringList(), "void", "int");
    pDesc->setPropertyInfo("PhiResolution", "SetPhiResolution", "GetPhiResolution", QStringList() << "int",
                           QStringList(), "void", "int");
    pDesc->setPropertyInfo("Theta", "SetTheta", "GetTheta", QStringList() << "double",
                           QStringList(), "void", "double");
    pDesc->setPropertyInfo("Phi", "SetPhi", "GetPhi", QStringList() << "double",
                           QStringList(), "void", "double");
    
}

CVtkTexturedSphereSource::CVtkTexturedSphereSource() : m_vtkTexturedSphereSource(0)
{
    CVtkTexturedSphereSource::InitializeObjectDesc();
    setVtkObject(vtkTexturedSphereSource::New());
}

CVtkTexturedSphereSource::~CVtkTexturedSphereSource()
{

}
void CVtkTexturedSphereSource::setRadius(double val)
{
    m_vtkTexturedSphereSource->SetRadius(val);
}

double CVtkTexturedSphereSource::radius() const
{
    return m_vtkTexturedSphereSource->GetRadius();
}

void CVtkTexturedSphereSource::setThetaResolution(int res)
{
    m_vtkTexturedSphereSource->SetThetaResolution(res);
}

int CVtkTexturedSphereSource::thetaResolution() const
{
    return m_vtkTexturedSphereSource->GetThetaResolution();
}

void CVtkTexturedSphereSource::setPhiResolution(int res)
{
    m_vtkTexturedSphereSource->SetPhiResolution(res);
}

int CVtkTexturedSphereSource::phiResolution() const
{
    return m_vtkTexturedSphereSource->GetPhiResolution();
}

void CVtkTexturedSphereSource::setTheta(double val)
{
    m_vtkTexturedSphereSource->SetTheta(val);
}

double CVtkTexturedSphereSource::theta() const
{
    return m_vtkTexturedSphereSource->GetTheta();
}

void CVtkTexturedSphereSource::setPhi(double val)
{
    m_vtkTexturedSphereSource->SetPhi(val);
}

double CVtkTexturedSphereSource::phi() const
{
    return m_vtkTexturedSphereSource->GetPhi();
}
