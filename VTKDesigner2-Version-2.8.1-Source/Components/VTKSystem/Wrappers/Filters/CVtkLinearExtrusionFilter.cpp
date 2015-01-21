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

#include "CVtkLinearExtrusionFilter.h"

DEFINE_VTK_OBJECT(CVtkLinearExtrusionFilter, CVtkPolyDataFilter, vtkLinearExtrusionFilter)
{
    pDesc->setNodeClassName("vtkLinearExtrusionFilter");
    pDesc->setNodeClassDescription("Sweep polygonal data creating a \"skirt\" from free edges and lines, and lines from vertices");

    pDesc->setPropertyInfo("ExtrusionType", "SetExtrusionType", "GetExtrusionType", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Capping", "SetCapping", "GetCapping", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("ScaleFactor", "SetScaleFactor", "GetScaleFactor", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Vector", "SetVector", "GetVector", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("ExtrusionPoint", "SetExtrusionPoint", "GetExtrusionPoint", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    
    // Q_ENUMS(ExtrusionType)
    // Q_PROPERTY(ExtrusionType ExtrusionType READ extrusionType WRITE setExtrusionType)
    // Q_PROPERTY(bool Capping READ isCapping WRITE setCapping)
    // Q_PROPERTY(double ScaleFactor READ scaleFactor WRITE setScaleFactor)
    // Q_PROPERTY(Point Vector READ vector WRITE setVector)
    // Q_PROPERTY(Point ExtrusionPoint READ extrusionPoint WRITE setExtrusionPoint)
}

CVtkLinearExtrusionFilter::CVtkLinearExtrusionFilter() : m_vtkLinearExtrusionFilter(0)
{
    CVtkLinearExtrusionFilter::InitializeObjectDesc();
    setVtkObject(vtkLinearExtrusionFilter::New());
}

CVtkLinearExtrusionFilter::~CVtkLinearExtrusionFilter()
{

}

void CVtkLinearExtrusionFilter::setExtrusionType(CVtkLinearExtrusionFilter::ExtrusionType type)
{
    switch(type)
    {
    case Vector:
        m_vtkLinearExtrusionFilter->SetExtrusionType(VTK_VECTOR_EXTRUSION);
        break;
    case Normal:
        m_vtkLinearExtrusionFilter->SetExtrusionType(VTK_NORMAL_EXTRUSION);
        break;
    case Point:
    default:
        m_vtkLinearExtrusionFilter->SetExtrusionType(VTK_POINT_EXTRUSION);
        break;
    }
}

CVtkLinearExtrusionFilter::ExtrusionType CVtkLinearExtrusionFilter::extrusionType() const
{
    switch(m_vtkLinearExtrusionFilter->GetExtrusionType())
    {
    case VTK_VECTOR_EXTRUSION: return Vector;
    case VTK_NORMAL_EXTRUSION: return Normal;
    case VTK_POINT_EXTRUSION: return Point;
    }

    return Point;
}


void CVtkLinearExtrusionFilter::setCapping(bool val)
{
    m_vtkLinearExtrusionFilter->SetCapping(val);
}

bool CVtkLinearExtrusionFilter::isCapping() const
{
    return m_vtkLinearExtrusionFilter->GetCapping();
}


void CVtkLinearExtrusionFilter::setScaleFactor(double factor)
{
    m_vtkLinearExtrusionFilter->SetScaleFactor(factor);
}

double CVtkLinearExtrusionFilter::scaleFactor() const
{
    return m_vtkLinearExtrusionFilter->GetScaleFactor();
}

void CVtkLinearExtrusionFilter::setVector(Point3D vec)
{
    m_vtkLinearExtrusionFilter->SetVector(vec.x, vec.y, vec.z);
}

Point3D CVtkLinearExtrusionFilter::vector() const
{
    return Point3D(m_vtkLinearExtrusionFilter->GetVector());
}

void CVtkLinearExtrusionFilter::setExtrusionPoint(Point3D ep)
{
    m_vtkLinearExtrusionFilter->SetExtrusionPoint(ep.x, ep.y, ep.z);
}

Point3D CVtkLinearExtrusionFilter::extrusionPoint() const
{
    return Point3D(m_vtkLinearExtrusionFilter->GetExtrusionPoint());
}


