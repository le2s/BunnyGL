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
#include "CVtkTextureMapToPlane.h"

DEFINE_VTK_OBJECT(CVtkTextureMapToPlane,CVtkDataSetFilter,vtkTextureMapToPlane)
{
    pDesc->setNodeClassName("vtkTextureMapToPlane");
    pDesc->setNodeClassDescription("Generate texture coordinates by mapping points to Plane");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("Point1", "SetPoint1", "GetPoint1", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Point2", "SetPoint2", "GetPoint2", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Origin", "SetOrigin", "GetOrigin", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Normal", "SetNormal", "GetNormal", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("SRange", "SetSRange", "GetSRange", QStringList() << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("TRange", "SetTRange", "GetTRange", QStringList() << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("AutomaticPlaneGeneration", "SetAutomaticPlaneGeneration", "GetAutomaticPlaneGeneration", QStringList() << "int" , QStringList(), "void", "int");
}

CVtkTextureMapToPlane::CVtkTextureMapToPlane() : m_vtkTextureMapToPlane(0)
{
    CVtkTextureMapToPlane::InitializeObjectDesc();
    setVtkObject(vtkTextureMapToPlane::New());
}

CVtkTextureMapToPlane::~CVtkTextureMapToPlane()
{

}

Point3D CVtkTextureMapToPlane::point1() const
{
    return Point3D(m_vtkTextureMapToPlane->GetPoint1());
}


void CVtkTextureMapToPlane::setPoint1(Point3D val)
{
    m_vtkTextureMapToPlane->SetPoint1(val.x, val.y, val.z);
}


Point3D CVtkTextureMapToPlane::point2() const
{
    return Point3D(m_vtkTextureMapToPlane->GetPoint2());
}


void CVtkTextureMapToPlane::setPoint2(Point3D val)
{
    m_vtkTextureMapToPlane->SetPoint2(val.x, val.y, val.z);
}


Point3D CVtkTextureMapToPlane::origin() const
{
    return Point3D(m_vtkTextureMapToPlane->GetOrigin());
}


void CVtkTextureMapToPlane::setOrigin(Point3D val)
{
    m_vtkTextureMapToPlane->SetOrigin(val.x, val.y, val.z);
}


Vector3D CVtkTextureMapToPlane::normal() const
{
    return Vector3D(m_vtkTextureMapToPlane->GetNormal());
}


void CVtkTextureMapToPlane::setNormal(Vector3D val)
{
    m_vtkTextureMapToPlane->SetNormal(val.x, val.y, val.z);
}


ValueRange CVtkTextureMapToPlane::sRange() const
{
    return ValueRange(m_vtkTextureMapToPlane->GetSRange());
}


void CVtkTextureMapToPlane::setSRange(ValueRange val)
{
    m_vtkTextureMapToPlane->SetSRange(val.min, val.max);
}


ValueRange CVtkTextureMapToPlane::tRange() const
{
    return ValueRange(m_vtkTextureMapToPlane->GetTRange());
}


void CVtkTextureMapToPlane::setTRange(ValueRange val)
{
    m_vtkTextureMapToPlane->SetTRange(val.min, val.max);
}


bool CVtkTextureMapToPlane::isAutomaticPlaneGeneration() const
{
    return m_vtkTextureMapToPlane->GetAutomaticPlaneGeneration();
}


void CVtkTextureMapToPlane::setAutomaticPlaneGeneration(bool val)
{
    m_vtkTextureMapToPlane->SetAutomaticPlaneGeneration(val);
}

