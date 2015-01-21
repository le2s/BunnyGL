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

#include "CVtkImageToPolyDataFilter.h"

DEFINE_VTK_OBJECT(CVtkImageToPolyDataFilter, CVtkPolyDataFilter, vtkImageToPolyDataFilter)
{
    pDesc->setNodeClassName("vtkImageToPolyDataFilter");
    pDesc->setNodeClassDescription("Generate linear primitives (vtkPolyData) from an image");

    pDesc->setPropertyInfo("OutputStyle", "SetOutputStyle", "GetOutputStyle", QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("ColorMode","SetColorMode","GetColorMode",QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("Smoothing","SetSmoothing","GetSmoothing",QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("NumberOfSmoothingIterations","SetNumberOfSmoothingIterations","GetNumberOfSmoothingIterations",QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("Decimation","SetDecimation","GetDecimation",QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("DecimationError","SetDecimationError","GetDecimationError",QStringList()<<"double",QStringList(),"void","double");
    pDesc->setPropertyInfo("Error","SetError","GetError",QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("SubImageSize","SetSubImageSize","GetSubImageSize",QStringList()<<"int",QStringList(),"void","int");
}

CVtkImageToPolyDataFilter::CVtkImageToPolyDataFilter(): m_vtkImageToPolyDataFilter(0)
{
    CVtkImageToPolyDataFilter::InitializeObjectDesc();
    setVtkObject(vtkImageToPolyDataFilter::New());
}

CVtkImageToPolyDataFilter::~CVtkImageToPolyDataFilter()
{

}

CVtkImageToPolyDataFilter::OutputStyle CVtkImageToPolyDataFilter::outputStyle() const
{
    switch(m_vtkImageToPolyDataFilter->GetOutputStyle())
    {
        case VTK_STYLE_PIXELIZE:return(Pixelize);
        case VTK_STYLE_POLYGONALIZE:return(Polygonalize);
        case VTK_STYLE_RUN_LENGTH:return(RunLength);
        default:return(Pixelize);
    }
}

void CVtkImageToPolyDataFilter::setOutputStyle(OutputStyle o)
{
    switch(o)
    {
        case Pixelize:     m_vtkImageToPolyDataFilter->SetOutputStyle(VTK_STYLE_PIXELIZE);break;
        case Polygonalize: m_vtkImageToPolyDataFilter->SetOutputStyle(VTK_STYLE_POLYGONALIZE);break;
        case RunLength: m_vtkImageToPolyDataFilter->SetOutputStyle(VTK_STYLE_RUN_LENGTH);break;
        default : m_vtkImageToPolyDataFilter->SetOutputStyle(VTK_STYLE_PIXELIZE);break;
    }
}

CVtkImageToPolyDataFilter::ColorMode CVtkImageToPolyDataFilter::colorMode() const
{
    switch(m_vtkImageToPolyDataFilter->GetColorMode())
    {
        case VTK_COLOR_MODE_LUT: return (Lut);break;
        case VTK_COLOR_MODE_LINEAR_256: return (Linear256);break;
        default :return (Lut);break;
    }
}

void CVtkImageToPolyDataFilter::setColorMode(ColorMode c)
{
    switch(c)
    {
        case Lut:    m_vtkImageToPolyDataFilter->SetColorMode(VTK_COLOR_MODE_LUT);
        case Linear256:m_vtkImageToPolyDataFilter->SetColorMode(VTK_COLOR_MODE_LINEAR_256);
        default:    m_vtkImageToPolyDataFilter->SetColorMode(VTK_COLOR_MODE_LUT);
    }
}

bool CVtkImageToPolyDataFilter::isSmoothing() const
{
    return m_vtkImageToPolyDataFilter->GetSmoothing();
}

void CVtkImageToPolyDataFilter::setSmoothing(bool val)
{
    m_vtkImageToPolyDataFilter->SetSmoothing(val);
}


int CVtkImageToPolyDataFilter::numberOfSmoothingIterations() const
{
    return m_vtkImageToPolyDataFilter->GetNumberOfSmoothingIterations();
}

void CVtkImageToPolyDataFilter::setNumberOfSmoothingIterations(int val)
{
    m_vtkImageToPolyDataFilter->SetNumberOfSmoothingIterations(val);
}


bool CVtkImageToPolyDataFilter::isDecimation() const
{
    return m_vtkImageToPolyDataFilter->GetDecimation();
}

void CVtkImageToPolyDataFilter::setDecimation(bool val)
{
    m_vtkImageToPolyDataFilter->SetDecimation(val);
}


double CVtkImageToPolyDataFilter::decimationError() const
{
    return m_vtkImageToPolyDataFilter->GetDecimationError();
}

void CVtkImageToPolyDataFilter::setDecimationError(double val)
{
    m_vtkImageToPolyDataFilter->SetDecimationError(val);
}


int CVtkImageToPolyDataFilter::error() const
{
    return m_vtkImageToPolyDataFilter->GetError();
}

void CVtkImageToPolyDataFilter::setError(int val)
{
    m_vtkImageToPolyDataFilter->SetError(val);
}


int CVtkImageToPolyDataFilter::subImageSize() const
{
    return m_vtkImageToPolyDataFilter->GetSubImageSize();
}

void CVtkImageToPolyDataFilter::setSubImageSize(int val)
{
    m_vtkImageToPolyDataFilter->SetSubImageSize(val);
}


