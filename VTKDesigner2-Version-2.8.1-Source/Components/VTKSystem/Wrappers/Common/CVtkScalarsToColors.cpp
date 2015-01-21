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

#include "CVtkScalarsToColors.h"

DEFINE_VTK_OBJECT(CVtkScalarsToColors, CVtkObject, vtkScalarsToColors)
{
    pDesc->setNodeClassName("vtkScalarsToColors");
    pDesc->setNodeClassCategory("Algorithms");

    pDesc->setPropertyInfo("Alpha", "SetAlpha", "GetAlpha", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("VectorMode", "SetVectorMode", "GetVectorMode", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("VectorComponent", "SetVectorComponent", "GetVectorComponent", QStringList() << "int", QStringList(), "void", "int");
    
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ScalarsToColors", IVisSystemNodeConnectionPath::OutputPath,  "vtkScalarsToColors", 0)
    );

    pDesc->setCreateFunction(0);
}

CVtkScalarsToColors::CVtkScalarsToColors() : m_vtkScalarsToColors(0)
{
    CVtkScalarsToColors::InitializeObjectDesc();
    setVtkObject(vtkScalarsToColors::New());
}

CVtkScalarsToColors::~CVtkScalarsToColors()
{

}

void CVtkScalarsToColors::setRange(double min, double max)
{
    m_vtkScalarsToColors->SetRange(min, max);
}

double CVtkScalarsToColors::rangeMin() const
{
    return m_vtkScalarsToColors->GetRange()[0];
}

double CVtkScalarsToColors::rangeMax() const
{
    return m_vtkScalarsToColors->GetRange()[1];
}

void CVtkScalarsToColors::setRangeMin(double val)
{
    m_vtkScalarsToColors->SetRange(val, rangeMax());
}

void CVtkScalarsToColors::setRangeMax(double val)
{
    m_vtkScalarsToColors->SetRange(rangeMin(), val);
}

QColor CVtkScalarsToColors::color(double value) const
{
    QColor ret;
    double* color = m_vtkScalarsToColors->GetColor(value);
    ret.setRgbF(color[0], color[1], color[2]);
    return ret;
}

double CVtkScalarsToColors::opacity(double value) const
{
    return m_vtkScalarsToColors->GetOpacity(value);
}

double CVtkScalarsToColors::luminance(double value) const
{
    return m_vtkScalarsToColors->GetLuminance(value);
}


void CVtkScalarsToColors::setAlpha(double val)
{
    m_vtkScalarsToColors->SetAlpha(val);
}

double CVtkScalarsToColors::alpha() const
{
    return m_vtkScalarsToColors->GetAlpha();
}

void CVtkScalarsToColors::setVectorMode(CVtkScalarsToColors::VectorMode val)
{
    switch(val)
    {
    case Magnitude:
        m_vtkScalarsToColors->SetVectorMode(vtkScalarsToColors::MAGNITUDE);
        break;
    case Component:
        m_vtkScalarsToColors->SetVectorMode(vtkScalarsToColors::COMPONENT);
        break;
    }
}

CVtkScalarsToColors::VectorMode CVtkScalarsToColors::vectorMode() const
{
    if(m_vtkScalarsToColors->GetVectorMode() == vtkScalarsToColors::MAGNITUDE)
        return Magnitude;

    if(m_vtkScalarsToColors->GetVectorMode() == vtkScalarsToColors::MAGNITUDE)
        return Component;

    return Magnitude;
}


void CVtkScalarsToColors::setVectorComponent(int comp)
{
    m_vtkScalarsToColors->SetVectorComponent(comp);
}

int CVtkScalarsToColors::vectorComponent() const
{
    return m_vtkScalarsToColors->GetVectorComponent();
}

bool CVtkScalarsToColors::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ScalarsToColors")
    {
        m_scalarsToColorsData.setScalarsToColors(m_vtkScalarsToColors);
        *outputData = &m_scalarsToColorsData;
        return true;
    }

    return false;
}

bool CVtkScalarsToColors::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ScalarsToColors")
        return true;

    return false;
}

