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

#include "CVtkSplineFilter.h"

DEFINE_VTK_OBJECT(CVtkSplineFilter,CVtkPolyDataFilter,vtkSplineFilter)
{
    pDesc->setNodeClassName("vtkSplineFilter");
    pDesc->setNodeClassDescription("Generate uniformly subdivided polylines from a set of input polyline using a vtkSpline");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("MaximumNumberOfSubdivisions", "SetMaximumNumberOfSubdivisions", "GetMaximumNumberOfSubdivisions", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Subdivide", "SetSubdivide", "GetSubdivide", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("NumberOfSubdivisions", "SetNumberOfSubdivisions", "GetNumberOfSubdivisions", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Length", "SetLength", "GetLength", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("GenerateTCoords", "SetGenerateTCoords", "GetGenerateTCoords", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("TextureLength", "SetTextureLength", "GetTextureLength", QStringList() << "double", QStringList(), "void", "double");
}

CVtkSplineFilter::CVtkSplineFilter() : m_vtkSplineFilter(0)
{
    CVtkSplineFilter::InitializeObjectDesc();
    setVtkObject(vtkSplineFilter::New());
}

CVtkSplineFilter::~CVtkSplineFilter()
{

}

int CVtkSplineFilter::maximumNumberOfSubdivisions() const
{
    return m_vtkSplineFilter->GetMaximumNumberOfSubdivisions();
}


void CVtkSplineFilter::setMaximumNumberOfSubdivisions(int val)
{
    m_vtkSplineFilter->SetMaximumNumberOfSubdivisions(val);
}


CVtkSplineFilter::Subdivide CVtkSplineFilter::subdivide() const
{
    switch(m_vtkSplineFilter->GetSubdivide())
    {
        case VTK_SUBDIVIDE_SPECIFIED:
            return (Specified);
        case VTK_SUBDIVIDE_LENGTH:
            return (Length);
        default :
            return (Specified);
    }
}

void CVtkSplineFilter::setSubdivide(Subdivide val)
{
    switch(val)
    {
        case Specified:
            m_vtkSplineFilter->SetSubdivide(VTK_SUBDIVIDE_SPECIFIED);
            break;
        case Length:
            m_vtkSplineFilter->SetSubdivide(VTK_SUBDIVIDE_LENGTH);
            break;
        default :
            m_vtkSplineFilter->SetSubdivide(VTK_SUBDIVIDE_SPECIFIED);
            break;
    }
}


int CVtkSplineFilter::numberOfSubdivisions() const
{
    return m_vtkSplineFilter->GetNumberOfSubdivisions();
}


void CVtkSplineFilter::setNumberOfSubdivisions(int val)
{
    m_vtkSplineFilter->SetNumberOfSubdivisions(val);
}


double CVtkSplineFilter::length() const
{
    return m_vtkSplineFilter->GetLength();
}


void CVtkSplineFilter::setLength(double val)
{
    m_vtkSplineFilter->SetLength(val);
}


CVtkSplineFilter::GenerateTCoords CVtkSplineFilter::generateTCoords() const
{
    switch(m_vtkSplineFilter->GetGenerateTCoords())
    {
        case VTK_TCOORDS_OFF:
            return (Off);
        case VTK_TCOORDS_FROM_NORMALIZED_LENGTH:
            return (NormalizedLength);
        case VTK_TCOORDS_FROM_LENGTH:
            return (FromLength);
        case VTK_TCOORDS_FROM_SCALARS:
            return (FromScalars);
        default :
            return (Off);
    }
}

void CVtkSplineFilter::setGenerateTCoords(GenerateTCoords val)
{
    switch(val)
    {
        case Off:
            m_vtkSplineFilter->SetGenerateTCoords(VTK_TCOORDS_OFF);
            break;
        case NormalizedLength:
            m_vtkSplineFilter->SetGenerateTCoords(VTK_TCOORDS_FROM_NORMALIZED_LENGTH);
            break;
        case FromLength:
            m_vtkSplineFilter->SetGenerateTCoords(VTK_TCOORDS_FROM_LENGTH);
            break;
        case FromScalars:
            m_vtkSplineFilter->SetGenerateTCoords(VTK_TCOORDS_FROM_SCALARS);
            break;
        default :
            m_vtkSplineFilter->SetGenerateTCoords(VTK_TCOORDS_OFF);
            break;
    }
}


double CVtkSplineFilter::textureLength() const
{
    return m_vtkSplineFilter->GetTextureLength();
}


void CVtkSplineFilter::setTextureLength(double val)
{
    m_vtkSplineFilter->SetTextureLength(val);
}

