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

#include "CVtkColorTransferFunction.h"

DEFINE_VTK_OBJECT(CVtkColorTransferFunction, CVtkScalarsToColors, vtkColorTransferFunction)
{
    pDesc->setNodeClassName("vtkColorTransferFunction");
    pDesc->setNodeClassCategory("Algorithm");
    
    pDesc->setPropertyInfo("Clamping", "SetClamping", "GetClamping", QStringList() << "bool", QStringList(), "void", "bool");
}

CVtkColorTransferFunction::CVtkColorTransferFunction() : m_vtkColorTransferFunction(0)
{
    CVtkColorTransferFunction::InitializeObjectDesc();
    setVtkObject(vtkColorTransferFunction::New());
    m_vtkColorTransferFunction->SetColorSpaceToRGB();
}

CVtkColorTransferFunction::~CVtkColorTransferFunction()
{

}

int CVtkColorTransferFunction::numberOfPoints()
{
    return m_vtkColorTransferFunction->GetSize();
}

void CVtkColorTransferFunction::addPoint(double value, double r, double g, double b)
{
    m_vtkColorTransferFunction->AddRGBPoint(value, r, g, b);
}

void CVtkColorTransferFunction::addPoint(double value, QColor color)
{
    m_vtkColorTransferFunction->AddRGBPoint(value, color.redF(), color.greenF(), color.blueF());
}

void CVtkColorTransferFunction::removePoint(double value)
{
    m_vtkColorTransferFunction->RemovePoint(value);
}

void CVtkColorTransferFunction::removeAllPoints()
{
    m_vtkColorTransferFunction->RemoveAllPoints();
}

void CVtkColorTransferFunction::setClamping(bool val)
{
    m_vtkColorTransferFunction->SetClamping(val);
}

bool CVtkColorTransferFunction::isClamping() const
{
    return m_vtkColorTransferFunction->GetClamping();
}


