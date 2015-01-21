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

#include "CVtkCamera.h"
#include "CVtkMath.h"
#include "vtkMath.h"
#include <math.h>

DEFINE_VTK_OBJECT(CVtkCamera, CVtkObject, vtkCamera)
{
    pDesc->setNodeClassCategory("Rendering");
    pDesc->setNodeClassName("vtkCamera");
    pDesc->setNodeClassDescription("A camera for vtkRenderer");

    // Renderer Output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Camera", IVisSystemNodeConnectionPath::OutputPath, "vtkCamera", 0)
        );

    pDesc->setPropertyInfo("Position","SetPosition","GetPosition", QStringList()<<"double"<<"double"<<"double",
                        QStringList(), "void","double*");
    pDesc->setPropertyInfo("FocalPoint","SetFocalPoint","GetFocalPoint", QStringList()<< "double" << "double" << "double",
                        QStringList(), "void","double*");
    pDesc->setPropertyInfo("ViewUp","SetViewUp","GetViewUp", QStringList()<<"double"<<"double"<<"double",
                        QStringList(), "void","double*");
    pDesc->setPropertyInfo("Distance","SetDistance","GetDistance",QStringList()<<"double", QStringList(),"void","double");
    pDesc->setPropertyInfo("Roll","SetRoll","GetRoll",QStringList()<< "double",QStringList(), "void", "double");
    pDesc->setPropertyInfo("ParallelProjection","SetParallelProjection","GetParallelProjection",QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("UseHorizontalViewAngle", "SetUseHorizontalViewAngle","GetUseHorizontalViewAngle",QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("ViewAngle","SetViewAngle","GetViewAngle",QStringList()<<"double",QStringList(),"void","double");
    pDesc->setPropertyInfo("ParallelScale", "SetParallelScale", "GetParallelScale",QStringList()<<"double",QStringList(),"void","double");
    pDesc->setPropertyInfo("Thickness","SetThickness","GetThickness",QStringList() <<"double",QStringList(),"void","double");
    pDesc->setPropertyInfo("EyeAngle","SetEyeAngle","GetEyeAngle",QStringList()<<"double",QStringList(),"void","double");
    pDesc->setPropertyInfo("FocalDisk","SetFocalDisk","GetFocalDisk",QStringList()<<"double",QStringList(),"void","double");
    pDesc->setPropertyInfo("ViewPlaneNormal","SetViewPlaneNormal","GetViewPlaneNormal",QStringList()<<"double"<<"double"<<"double",
                            QStringList(),"void","double*");

}

static Vector3D computePerpendicular(Vector3D vec);

CVtkCamera::CVtkCamera(bool createVtkCamera) : m_vtkCamera(0)
{
    CVtkCamera::InitializeObjectDesc();
    if(createVtkCamera)
        setVtkObject(vtkCamera::New());
}

CVtkCamera::~CVtkCamera()
{

}

bool CVtkCamera::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Camera")
    {
        /*if(m_cameraData.getVtkCamera() != 0)
            return false;*/

        m_cameraData.setCamera(m_vtkCamera);
        *outputData = &m_cameraData;
        return true;
    }

    return CVtkObject::fetchOutput(path, outputData);
}

bool CVtkCamera::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Camera")
    {
        // m_cameraData.setCamera(0);
        return true;
    }

    return CVtkObject::outputDerefed(path, outputData);
}

Point3D CVtkCamera::position() const
{
    return m_vtkCamera->GetPosition();    
}

void CVtkCamera::setPosition(Point3D p)
{
    m_vtkCamera->SetPosition(p.x, p.y, p.z);
}

Point3D CVtkCamera::focalPoint() const
{
    return m_vtkCamera->GetFocalPoint();
}

void CVtkCamera::setFocalPoint(Point3D p)
{
    m_vtkCamera->SetFocalPoint(p.x,p.y,p.z);
}

Vector3D CVtkCamera::viewUp() const
{
    return m_vtkCamera->GetViewUp();    
}

void CVtkCamera::setViewUp(Vector3D v)
{
    m_vtkCamera->SetViewUp(v.x, v.y, v.z);
}


double CVtkCamera::distance() const
{
    return m_vtkCamera->GetDistance();
}

void CVtkCamera::setDistance(double d)
{
    m_vtkCamera->SetDistance(d);
}


double CVtkCamera::roll() const
{
    return m_vtkCamera->GetRoll();
}

void CVtkCamera::setRoll(double r)
{
    m_vtkCamera->SetRoll(r);
}


bool CVtkCamera::isParallelProjection() const
{
    return m_vtkCamera->GetParallelProjection();
}

void CVtkCamera::setParallelProjection(bool val)
{
    m_vtkCamera->SetParallelProjection(val);
}


bool CVtkCamera::isUseHorizontalViewAngle() const
{
    return m_vtkCamera->GetUseHorizontalViewAngle();
}

void CVtkCamera::setUseHorizontalViewAngle(bool val)
{
    m_vtkCamera->SetUseHorizontalViewAngle(val);
}


double CVtkCamera::viewAngle() const
{
    return m_vtkCamera->GetViewAngle();
}

void CVtkCamera::setViewAngle(double angle)
{
    m_vtkCamera->SetViewAngle(angle);
}


double CVtkCamera::parallelScale() const
{
    return m_vtkCamera->GetParallelScale();
}

void CVtkCamera::setParallelScale(double scale)
{
    m_vtkCamera->SetParallelScale(scale);
}


double CVtkCamera::thickness() const
{
    return m_vtkCamera->GetThickness();
}

void CVtkCamera::setThickness(double t)
{
    m_vtkCamera->SetThickness(t);
}

double CVtkCamera::eyeAngle() const
{
    return m_vtkCamera->GetEyeAngle();
}
 
void CVtkCamera::setEyeAngle(double angle)
{
    m_vtkCamera->SetEyeAngle(angle);
}


double CVtkCamera::focalDisk() const
{
    return m_vtkCamera->GetFocalDisk();
}

void CVtkCamera::setFocalDisk(double f)
{
    m_vtkCamera->SetFocalDisk(f);
}


Point3D CVtkCamera::viewPlaneNormal() const
{
    return m_vtkCamera->GetViewPlaneNormal();
}

void CVtkCamera::setViewPlaneNormal(Point3D p)
{
    m_vtkCamera->SetViewPlaneNormal(p.x,p.y,p.z);
}

void CVtkCamera::computeViewUp()
{
    Point3D pos = this->position();
    Point3D focus = this->focalPoint();

    // Compute the direction vector from position to focal point
    Vector3D vec;
    vec.x = focus.x - pos.x;
    vec.y = focus.y - pos.y;
    vec.z = focus.z - pos.z;

    // Compute the perpendicular to this point.
    Vector3D viewUpVec = CVtkMath::instance().perpendicularVector(vec);

    // Set this as the view up vector.
    this->setViewUp(viewUpVec);
}

Vector3D CVtkCamera::direction()
{
    return m_vtkCamera->GetDirectionOfProjection();
}

