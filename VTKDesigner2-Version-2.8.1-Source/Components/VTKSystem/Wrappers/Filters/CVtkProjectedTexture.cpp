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
#include "CVtkProjectedTexture.h"

DEFINE_VTK_OBJECT(CVtkProjectedTexture,CVtkDataSetFilter,vtkProjectedTexture)
{
    pDesc->setNodeClassName("vtkProjectedTexture");
    pDesc->setNodeClassDescription("Assign texture coordinates for a projected texture");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("Up", "SetUp", "GetUp", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Position", "SetPosition", "GetPosition", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("FocalPoint", "SetFocalPoint", "GetFocalPoint", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("MirrorSeparation", "SetMirrorSeparation", "GetMirrorSeparation", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("SRange", "SetSRange", "GetSRange", QStringList() << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("TRange", "SetTRange", "GetTRange", QStringList() << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("AspectRatio", "SetAspectRatio", "GetAspectRatio", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("CameraMode", "SetCameraMode", "GetCameraMode", QStringList() << "int", QStringList(), "void", "int");
}

CVtkProjectedTexture::CVtkProjectedTexture() : m_vtkProjectedTexture(0)
{
    CVtkProjectedTexture::InitializeObjectDesc();
    setVtkObject(vtkProjectedTexture::New());
}

CVtkProjectedTexture::~CVtkProjectedTexture()
{

}

Vector3D CVtkProjectedTexture::up() const
{
    return Vector3D(m_vtkProjectedTexture->GetUp());
}


void CVtkProjectedTexture::setUp(Vector3D val)
{
    m_vtkProjectedTexture->SetUp(val.x, val.y, val.z);
}


Point3D CVtkProjectedTexture::position() const
{
    return Point3D(m_vtkProjectedTexture->GetPosition());
}


void CVtkProjectedTexture::setPosition(Point3D val)
{
    m_vtkProjectedTexture->SetPosition(val.x, val.y, val.z);
}


Point3D CVtkProjectedTexture::focalPoint() const
{
    return Point3D(m_vtkProjectedTexture->GetFocalPoint());
}


void CVtkProjectedTexture::setFocalPoint(Point3D val)
{
    m_vtkProjectedTexture->SetFocalPoint(val.x, val.y, val.z);
}


double CVtkProjectedTexture::mirrorSeparation() const
{
    return m_vtkProjectedTexture->GetMirrorSeparation();
}


void CVtkProjectedTexture::setMirrorSeparation(double val)
{
    m_vtkProjectedTexture->SetMirrorSeparation(val);
}


ValueRange CVtkProjectedTexture::sRange() const
{
    return ValueRange(m_vtkProjectedTexture->GetSRange());
}


void CVtkProjectedTexture::setSRange(ValueRange val)
{
    m_vtkProjectedTexture->SetSRange(val.min, val.max);
}


ValueRange CVtkProjectedTexture::tRange() const
{
    return ValueRange(m_vtkProjectedTexture->GetTRange());
}


void CVtkProjectedTexture::setTRange(ValueRange val)
{
    m_vtkProjectedTexture->SetTRange(val.min, val.max);
}


Vector3D CVtkProjectedTexture::aspectRatio() const
{
    return Vector3D(m_vtkProjectedTexture->GetAspectRatio());
}


void CVtkProjectedTexture::setAspectRatio(Vector3D val)
{
    m_vtkProjectedTexture->SetAspectRatio(val.x, val.y, val.z);
}


CVtkProjectedTexture::CameraMode CVtkProjectedTexture::cameraMode() const
{
    switch(m_vtkProjectedTexture->GetCameraMode())
    {
        case VTK_PROJECTED_TEXTURE_USE_PINHOLE:return (PinHole);
        case VTK_PROJECTED_TEXTURE_USE_TWO_MIRRORS:return (TwoMirrors);
        default :return (PinHole);
    }
}

void CVtkProjectedTexture::setCameraMode(CameraMode val)
{
    switch(val)
    {
        case PinHole:
            m_vtkProjectedTexture->SetCameraMode(VTK_PROJECTED_TEXTURE_USE_PINHOLE);
            break;
        case TwoMirrors:
            m_vtkProjectedTexture->SetCameraMode(VTK_PROJECTED_TEXTURE_USE_TWO_MIRRORS);
            break;
        default :
            m_vtkProjectedTexture->SetCameraMode(VTK_PROJECTED_TEXTURE_USE_PINHOLE);
            break;
     }
}

