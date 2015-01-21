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

#include "CVtkRibbonFilter.h"


DEFINE_VTK_OBJECT(CVtkRibbonFilter, CVtkPolyDataFilter, vtkRibbonFilter)
{
    pDesc->setNodeClassName("vtkRibbonFilter");
    pDesc->setNodeClassDescription("Create oriented ribbons from lines defined in polygonal dataset");

    pDesc->setPropertyInfo("Width","SetWidth","GetWidth", QStringList()<<"double", QStringList(), "void","double");
    pDesc->setPropertyInfo("Angle","SetAngle","GetAngle", QStringList()<<"double", QStringList(), "void","double");
    pDesc->setPropertyInfo("VaryWidth","SetVaryWidth","GetVaryWidth", QStringList() <<"int", QStringList(), "void","int");
    pDesc->setPropertyInfo("WidthFactor","SetWidthFactor","GetWidthFactor", QStringList() << "double", QStringList(), "void","double");
    pDesc->setPropertyInfo("DefaultNormal","SetDefaultNormal","GetDefaultNormal", QStringList() << "double" << "double" << "double", QStringList(), "void","double*");    
    pDesc->setPropertyInfo("UseDefaultNormal","SetUseDefaultNormal","GetUseDefaultNormal", QStringList() << "int", QStringList(), "void","int");
    pDesc->setPropertyInfo("GenerateTCoords","SetGenerateTCoords","GetGenerateTCoords", QStringList() << "int", QStringList(), "void","int");
    pDesc->setPropertyInfo("TextureLength","SetTextureLength","GetTextureLength", QStringList() << "double", QStringList(), "void","double");
}


CVtkRibbonFilter::CVtkRibbonFilter() : m_vtkRibbonFilter()
{
    CVtkRibbonFilter::InitializeObjectDesc();
    setVtkObject(vtkRibbonFilter::New());
}

CVtkRibbonFilter::~CVtkRibbonFilter()
{

}

double CVtkRibbonFilter::width() const
{
    return m_vtkRibbonFilter->GetWidth();    
}

void CVtkRibbonFilter::setWidth(double val)
{
    m_vtkRibbonFilter->SetWidth(val);
}


double CVtkRibbonFilter::angle() const
{
    return m_vtkRibbonFilter->GetAngle();    
}

void CVtkRibbonFilter::setAngle(double ang)
{
    m_vtkRibbonFilter->SetAngle(ang);
}

bool CVtkRibbonFilter::isVaryWidth() const
{
    return m_vtkRibbonFilter->GetVaryWidth();
}

void CVtkRibbonFilter::setVaryWidth(bool val)
{
    m_vtkRibbonFilter->SetVaryWidth(val);
}

double CVtkRibbonFilter::widthFactor() const
{
    return m_vtkRibbonFilter->GetWidthFactor();
}

void CVtkRibbonFilter::setWidthFactor(double val)
{
    m_vtkRibbonFilter->SetWidthFactor(val);
}

Point3D CVtkRibbonFilter::defaultNormal() const
{
    return m_vtkRibbonFilter->GetDefaultNormal();
}

void CVtkRibbonFilter::setDefaultNormal(Point3D df)
{
    m_vtkRibbonFilter->SetDefaultNormal(df.x, df.y, df.z);
}


bool CVtkRibbonFilter::isUseDefaultNormal() const
{
    return m_vtkRibbonFilter->GetUseDefaultNormal();
}

void CVtkRibbonFilter::setUseDefaultNormal(bool val)
{
    m_vtkRibbonFilter->SetUseDefaultNormal(val);
}

CVtkRibbonFilter::TCoords CVtkRibbonFilter::generateTCoords() const
{
    switch(m_vtkRibbonFilter->GetGenerateTCoords())
    {
        case  VTK_TCOORDS_OFF: return Off2;
        case  VTK_TCOORDS_FROM_NORMALIZED_LENGTH: return NormalizedLength;
        case  VTK_TCOORDS_FROM_LENGTH: return Length;
        case  VTK_TCOORDS_FROM_SCALARS: return Scalars;
        default: return Off2;
    }
}

void CVtkRibbonFilter::setGenerateTCoords(CVtkRibbonFilter::TCoords val)
{
    switch(val)
    {    
        case Off2: m_vtkRibbonFilter->SetGenerateTCoords(VTK_TCOORDS_OFF); break;
        case NormalizedLength: m_vtkRibbonFilter->SetGenerateTCoords(VTK_TCOORDS_FROM_NORMALIZED_LENGTH); break;
        case Length: m_vtkRibbonFilter->SetGenerateTCoords(VTK_TCOORDS_FROM_LENGTH); break;
        case Scalars: m_vtkRibbonFilter->SetGenerateTCoords(VTK_TCOORDS_FROM_SCALARS); break;
        default: m_vtkRibbonFilter->SetGenerateTCoords(VTK_TCOORDS_OFF); break;
    }
}

double CVtkRibbonFilter::textureLength() const
{
    return m_vtkRibbonFilter->GetTextureLength();
}

void CVtkRibbonFilter::setTextureLength(double val)
{
    m_vtkRibbonFilter->SetTextureLength(val);
}