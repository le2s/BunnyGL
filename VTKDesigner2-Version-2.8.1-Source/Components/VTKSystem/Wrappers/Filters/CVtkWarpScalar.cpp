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
#include "CVtkWarpScalar.h"

DEFINE_VTK_OBJECT(CVtkWarpScalar,CVtkPointSetFilter,vtkWarpScalar)
{
    pDesc->setNodeClassName("vtkWarpScalar");
    pDesc->setNodeClassDescription("Deform geometry with scalar data");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("ScaleFactor", "SetScaleFactor", "GetScaleFactor", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("UseNormal", "SetUseNormal", "GetUseNormal", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("Normal", "SetNormal", "GetNormal", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("XYPlane", "SetXYPlane", "GetXYPlane", QStringList() << "int" , QStringList(), "void", "int");
}

CVtkWarpScalar::CVtkWarpScalar() : m_vtkWarpScalar(0)
{
    CVtkWarpScalar::InitializeObjectDesc();
    setVtkObject(vtkWarpScalar::New());
}

CVtkWarpScalar::~CVtkWarpScalar()
{

}

double CVtkWarpScalar::scaleFactor() const
{
    return m_vtkWarpScalar->GetScaleFactor();
}


void CVtkWarpScalar::setScaleFactor(double val)
{
    m_vtkWarpScalar->SetScaleFactor(val);
}


bool CVtkWarpScalar::isUseNormal() const
{
    return m_vtkWarpScalar->GetUseNormal();
}


void CVtkWarpScalar::setUseNormal(bool val)
{
    m_vtkWarpScalar->SetUseNormal(val);
}


Vector3D CVtkWarpScalar::normal() const
{
    return Vector3D(m_vtkWarpScalar->GetNormal());
}


void CVtkWarpScalar::setNormal(Vector3D val)
{
    m_vtkWarpScalar->SetNormal(val.x, val.y, val.z);
}


bool CVtkWarpScalar::isXYPlane() const
{
    return m_vtkWarpScalar->GetXYPlane();
}


void CVtkWarpScalar::setXYPlane(bool val)
{
    m_vtkWarpScalar->SetXYPlane(val);
}

