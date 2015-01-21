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
#include "CVtkWarpTo.h"

DEFINE_VTK_OBJECT(CVtkWarpTo,CVtkPointSetFilter,vtkWarpTo)
{
    pDesc->setNodeClassName("vtkWarpTo");
    pDesc->setNodeClassDescription("Deform geometry by warping towards a point");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("ScaleFactor", "SetScaleFactor", "GetScaleFactor", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Position", "SetPosition", "GetPosition", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Absolute", "SetAbsolute", "GetAbsolute", QStringList() << "int" , QStringList(), "void", "int");
}

CVtkWarpTo::CVtkWarpTo() : m_vtkWarpTo(0)
{
    CVtkWarpTo::InitializeObjectDesc();
    setVtkObject(vtkWarpTo::New());
}

CVtkWarpTo::~CVtkWarpTo()
{

}

double CVtkWarpTo::scaleFactor() const
{
    return m_vtkWarpTo->GetScaleFactor();
}


void CVtkWarpTo::setScaleFactor(double val)
{
    m_vtkWarpTo->SetScaleFactor(val);
}


Point3D CVtkWarpTo::position() const
{
    return Point3D(m_vtkWarpTo->GetPosition());
}


void CVtkWarpTo::setPosition(Point3D val)
{
    m_vtkWarpTo->SetPosition(val.x, val.y, val.z);
}


bool CVtkWarpTo::isAbsolute() const
{
    return m_vtkWarpTo->GetAbsolute();
}


void CVtkWarpTo::setAbsolute(bool val)
{
    m_vtkWarpTo->SetAbsolute(val);
}

