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

#include "CVtkGenericGlyph3DFilter.h"

DEFINE_VTK_OBJECT(CVtkGenericGlyph3DFilter,CVtkPolyDataFilter,vtkGenericGlyph3DFilter)
{
    pDesc->setNodeClassName("vtkGenericGlyph3DFilter");
    pDesc->setNodeClassDescription("Copy oriented and scaled glyph geometry to every input point");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("Scaling", "SetScaling", "GetScaling", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ScaleMode", "SetScaleMode", "GetScaleMode", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ColorMode", "SetColorMode", "GetColorMode", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ScaleFactor", "SetScaleFactor", "GetScaleFactor", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Range", "SetRange", "GetRange", QStringList() << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Orient", "SetOrient", "GetOrient", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("Clamping", "SetClamping", "GetClamping", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("VectorMode", "SetVectorMode", "GetVectorMode", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("IndexMode", "SetIndexMode", "GetIndexMode", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("GeneratePointIds", "SetGeneratePointIds", "GetGeneratePointIds", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("PointIdsName", "SetPointIdsName", "GetPointIdsName", QStringList() << "QString", QStringList(), "void", "QString");
}

CVtkGenericGlyph3DFilter::CVtkGenericGlyph3DFilter() : m_vtkGenericGlyph3DFilter(0)
{
    CVtkGenericGlyph3DFilter::InitializeObjectDesc();
    setVtkObject(vtkGenericGlyph3DFilter::New());
}

CVtkGenericGlyph3DFilter::~CVtkGenericGlyph3DFilter()
{

}

bool CVtkGenericGlyph3DFilter::isScaling() const
{
    return m_vtkGenericGlyph3DFilter->GetScaling();
}


void CVtkGenericGlyph3DFilter::setScaling(bool val)
{
    m_vtkGenericGlyph3DFilter->SetScaling(val);
}


CVtkGenericGlyph3DFilter::ScaleMode CVtkGenericGlyph3DFilter::scaleMode() const
{
    switch(m_vtkGenericGlyph3DFilter->GetScaleMode())
    {
        case VTK_SCALE_BY_SCALAR:return (SScalar);
        case VTK_SCALE_BY_VECTOR:return (SVector);
        case VTK_SCALE_BY_VECTORCOMPONENTS:return (SVectorComponents);
        case VTK_DATA_SCALING_OFF:return (SOff);
        default :return (SScalar);
    }
}

void CVtkGenericGlyph3DFilter::setScaleMode(ScaleMode val)
{
    switch(val)
    {
        case SScalar:
            m_vtkGenericGlyph3DFilter->SetScaleMode(VTK_SCALE_BY_SCALAR);
            break;
        case SVector:
            m_vtkGenericGlyph3DFilter->SetScaleMode(VTK_SCALE_BY_VECTOR);
            break;
        case SVectorComponents:
            m_vtkGenericGlyph3DFilter->SetScaleMode(VTK_SCALE_BY_VECTORCOMPONENTS);
            break;
        case SOff:
            m_vtkGenericGlyph3DFilter->SetScaleMode(VTK_DATA_SCALING_OFF);
            break;
        default :
            m_vtkGenericGlyph3DFilter->SetScaleMode(VTK_SCALE_BY_SCALAR);
            break;
    }
}


CVtkGenericGlyph3DFilter::ColorMode CVtkGenericGlyph3DFilter::colorMode() const
{
    switch(m_vtkGenericGlyph3DFilter->GetColorMode())
    {
        case VTK_COLOR_BY_SCALE:return (CScale);
        case VTK_COLOR_BY_SCALAR:return (CScalar);
        case VTK_COLOR_BY_VECTOR:return (Vector);
        default :return (CScale);
    }
}

void CVtkGenericGlyph3DFilter::setColorMode(ColorMode val)
{
    switch(val)
    {
        case CScale:
            m_vtkGenericGlyph3DFilter->SetColorMode(VTK_COLOR_BY_SCALE);
            break;
        case CScalar:
            m_vtkGenericGlyph3DFilter->SetColorMode(VTK_COLOR_BY_SCALAR);
            break;
        case Vector:
            m_vtkGenericGlyph3DFilter->SetColorMode(VTK_COLOR_BY_VECTOR);
            break;
        default :
            m_vtkGenericGlyph3DFilter->SetColorMode(VTK_COLOR_BY_SCALE);
            break;
    }
}


double CVtkGenericGlyph3DFilter::scaleFactor() const
{
    return m_vtkGenericGlyph3DFilter->GetScaleFactor();
}


void CVtkGenericGlyph3DFilter::setScaleFactor(double val)
{
    m_vtkGenericGlyph3DFilter->SetScaleFactor(val);
}


ValueRange CVtkGenericGlyph3DFilter::range() const
{
    return ValueRange(m_vtkGenericGlyph3DFilter->GetRange());
}


void CVtkGenericGlyph3DFilter::setRange(ValueRange val)
{
    m_vtkGenericGlyph3DFilter->SetRange(val.min, val.max);
}


bool CVtkGenericGlyph3DFilter::isOrient() const
{
    return m_vtkGenericGlyph3DFilter->GetOrient();
}


void CVtkGenericGlyph3DFilter::setOrient(bool val)
{
    m_vtkGenericGlyph3DFilter->SetOrient(val);
}


bool CVtkGenericGlyph3DFilter::isClamping() const
{
    return m_vtkGenericGlyph3DFilter->GetClamping();
}


void CVtkGenericGlyph3DFilter::setClamping(bool val)
{
    m_vtkGenericGlyph3DFilter->SetClamping(val);
}


CVtkGenericGlyph3DFilter::VectorMode CVtkGenericGlyph3DFilter::vectorMode() const
{
    switch(m_vtkGenericGlyph3DFilter->GetVectorMode())
    {
        case VTK_USE_VECTOR:return (VVector);
        case VTK_USE_NORMAL:return (VNormal);
        case VTK_VECTOR_ROTATION_OFF:return (VRotationOff);
        default :return (VVector);
    }
}

void CVtkGenericGlyph3DFilter::setVectorMode(VectorMode val)
{
    switch(val)
    {
        case VVector:
            m_vtkGenericGlyph3DFilter->SetVectorMode(VTK_USE_VECTOR);
            break;
        case VNormal:
            m_vtkGenericGlyph3DFilter->SetVectorMode(VTK_USE_NORMAL);
            break;
        case VRotationOff:
            m_vtkGenericGlyph3DFilter->SetVectorMode(VTK_VECTOR_ROTATION_OFF);
            break;
        default :
            m_vtkGenericGlyph3DFilter->SetVectorMode(VTK_USE_VECTOR);
            break;
    }
}


CVtkGenericGlyph3DFilter::IndexMode CVtkGenericGlyph3DFilter::indexMode() const
{
    switch(m_vtkGenericGlyph3DFilter->GetIndexMode())
    {
        case VTK_INDEXING_OFF:return (IOff);
        case VTK_INDEXING_BY_SCALAR:return (IScalar);
        case VTK_INDEXING_BY_VECTOR:return (IVector);
        default :return (IOff);
    }
}

void CVtkGenericGlyph3DFilter::setIndexMode(IndexMode val)
{
    switch(val)
    {
        case IOff:
            m_vtkGenericGlyph3DFilter->SetIndexMode(VTK_INDEXING_OFF);
            break;
        case IScalar:
            m_vtkGenericGlyph3DFilter->SetIndexMode(VTK_INDEXING_BY_SCALAR);
            break;
        case IVector:
            m_vtkGenericGlyph3DFilter->SetIndexMode(VTK_INDEXING_BY_VECTOR);
            break;
        default :
            m_vtkGenericGlyph3DFilter->SetIndexMode(VTK_INDEXING_OFF);
            break;
    }
}


bool CVtkGenericGlyph3DFilter::isGeneratePointIds() const
{
    return m_vtkGenericGlyph3DFilter->GetGeneratePointIds();
}


void CVtkGenericGlyph3DFilter::setGeneratePointIds(bool val)
{
    m_vtkGenericGlyph3DFilter->SetGeneratePointIds(val);
}


QString CVtkGenericGlyph3DFilter::pointIdsName() const
{
    return m_vtkGenericGlyph3DFilter->GetPointIdsName();
}


void CVtkGenericGlyph3DFilter::setPointIdsName(QString val)
{
    m_vtkGenericGlyph3DFilter->SetPointIdsName(qPrintable(val));
}

