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

#include "CVtkTubeFilter.h"


DEFINE_VTK_OBJECT(CVtkTubeFilter, CVtkPolyDataFilter, vtkTubeFilter)
{
    pDesc->setNodeClassName("vtkTubeFilter");
    pDesc->setNodeClassDescription("TubeFilter algorithm");

    pDesc->setPropertyInfo("Radius","SetRadius","GetRadius", QStringList() <<"double", QStringList(), "void","double");
    pDesc->setPropertyInfo("VaryRadius","SetVaryRadius","GetVaryRadius", QStringList() <<"int", QStringList(), "void","int");
    pDesc->setPropertyInfo("NumberOfSides","SetNumberOfSides","GetNumberOfSides", QStringList() << "int", QStringList(), "void","int");
    pDesc->setPropertyInfo("RadiusFactor","SetRadiusFactor","GetRadiusFactor", QStringList() << "double", QStringList(), "void","double");
    pDesc->setPropertyInfo("DefaultNormal","SetDefaultNormal","GetDefaultNormal", QStringList() << "double" << "double" << "double", QStringList(), "void","double*");
    pDesc->setPropertyInfo("UseDefaultNormal","SetUseDefaultNormal","GetUseDefaultNormal", QStringList() << "int", QStringList(), "void","int");
    pDesc->setPropertyInfo("SidesShareVertices","SetSidesShareVertices","GetSidesShareVertices", QStringList() << "int", QStringList(), "void","int");
    pDesc->setPropertyInfo("Capping","SetCapping","GetCapping", QStringList() << "int", QStringList(), "void","int");
    pDesc->setPropertyInfo("OnRatio","SetOnRatio","GetOnRatio", QStringList() << "int", QStringList(), "void","int");
    pDesc->setPropertyInfo("Offset","SetOffset","GetOffset", QStringList() << "int", QStringList(), "void","int");
    pDesc->setPropertyInfo("GenerateTCoords","SetGenerateTCoords","GetGenerateTCoords", QStringList() << "int", QStringList(), "void","int");
}


CVtkTubeFilter::CVtkTubeFilter() : m_vtkTubeFilter()
{
    CVtkTubeFilter::InitializeObjectDesc();
    setVtkObject(vtkTubeFilter::New());
}

CVtkTubeFilter::~CVtkTubeFilter()
{

}

double CVtkTubeFilter::radius() const
{
    return m_vtkTubeFilter->GetRadius();
}

void CVtkTubeFilter::setRadius(double r)
{
    m_vtkTubeFilter->SetRadius(r);
}

CVtkTubeFilter::VaryRadius CVtkTubeFilter::varyRadius() const
{
    switch(m_vtkTubeFilter->GetVaryRadius())
    {
        case VTK_VARY_RADIUS_OFF:    return (Off);
        case VTK_VARY_RADIUS_BY_SCALAR: return (Scalar);
        case VTK_VARY_RADIUS_BY_VECTOR: return (Vector);
        case VTK_VARY_RADIUS_BY_ABSOLUTE_SCALAR: return (AbsoluteScalar);
        default:    return (Off);
    }
}

void CVtkTubeFilter::setVaryRadius(CVtkTubeFilter::VaryRadius vr)
{
    switch(vr)
    {
        case Off: m_vtkTubeFilter->SetVaryRadius(VTK_VARY_RADIUS_OFF); 
                break;
        case Scalar: m_vtkTubeFilter->SetVaryRadius(VTK_VARY_RADIUS_BY_SCALAR);
                break;
        case Vector: m_vtkTubeFilter->SetVaryRadius(VTK_VARY_RADIUS_BY_VECTOR);
                break;
        case AbsoluteScalar: m_vtkTubeFilter->SetVaryRadius(VTK_VARY_RADIUS_BY_ABSOLUTE_SCALAR);
                break;
        default:m_vtkTubeFilter->SetVaryRadius(VTK_VARY_RADIUS_OFF);
    }
}


int CVtkTubeFilter::numberOfSides() const
{
    return m_vtkTubeFilter->GetNumberOfSides();
}

void CVtkTubeFilter::setNumberOfSides(int nos)
{
    m_vtkTubeFilter->SetNumberOfSides(nos);
}


double CVtkTubeFilter::radiusFactor() const
{
    return m_vtkTubeFilter->GetRadiusFactor();
}

void CVtkTubeFilter::setRadiusFactor(double rf)
{
    m_vtkTubeFilter->SetRadiusFactor(rf);
}


Point3D CVtkTubeFilter::defaultNormal() const
{
    return m_vtkTubeFilter->GetDefaultNormal();
}

void CVtkTubeFilter::setDefaultNormal(Point3D df)
{
    m_vtkTubeFilter->SetDefaultNormal(df.x, df.y, df.z);
}


bool CVtkTubeFilter::isUseDefaultNormal() const
{
    return m_vtkTubeFilter->GetUseDefaultNormal();
}

void CVtkTubeFilter::setUseDefaultNormal(bool udn)
{
    m_vtkTubeFilter->SetUseDefaultNormal(udn);
}

bool CVtkTubeFilter::isSidesShareVertices() const
{
    return m_vtkTubeFilter->GetSidesShareVertices();
}

void CVtkTubeFilter::setSidesShareVertices(bool ssv)
{
    m_vtkTubeFilter->SetSidesShareVertices(ssv);
}

bool CVtkTubeFilter::isCapping() const
{
    return m_vtkTubeFilter->GetCapping();
}

void CVtkTubeFilter::setCapping(bool val)
{
    m_vtkTubeFilter->SetCapping(val);
}


int CVtkTubeFilter::onRatio() const
{
    return m_vtkTubeFilter->GetOnRatio();
}

void CVtkTubeFilter::setOnRatio(int val)
{
    m_vtkTubeFilter->SetOnRatio(val);
}


int CVtkTubeFilter::offset() const
{
    return m_vtkTubeFilter->GetOffset();
}

void CVtkTubeFilter::setOffset(int val)
{
    m_vtkTubeFilter->SetOffset(val);
}


CVtkTubeFilter::TCoords CVtkTubeFilter::generateTCoords() const
{
    switch(m_vtkTubeFilter->GetGenerateTCoords())
    {
        case  VTK_TCOORDS_OFF: return Off2;
        case  VTK_TCOORDS_FROM_NORMALIZED_LENGTH: return NormalizedLength;
        case  VTK_TCOORDS_FROM_LENGTH: return Length;
        case  VTK_TCOORDS_FROM_SCALARS: return Scalars;
        default: return Off2;
    }
}

void CVtkTubeFilter::setGenerateTCoords(CVtkTubeFilter::TCoords val)
{
    switch(val)
    {    
        case Off2: m_vtkTubeFilter->SetGenerateTCoords(VTK_TCOORDS_OFF); break;
        case NormalizedLength: m_vtkTubeFilter->SetGenerateTCoords(VTK_TCOORDS_FROM_NORMALIZED_LENGTH); break;
        case Length: m_vtkTubeFilter->SetGenerateTCoords(VTK_TCOORDS_FROM_LENGTH); break;
        case Scalars: m_vtkTubeFilter->SetGenerateTCoords(VTK_TCOORDS_FROM_SCALARS); break;
        default: m_vtkTubeFilter->SetGenerateTCoords(VTK_TCOORDS_OFF); break;
    }
}


