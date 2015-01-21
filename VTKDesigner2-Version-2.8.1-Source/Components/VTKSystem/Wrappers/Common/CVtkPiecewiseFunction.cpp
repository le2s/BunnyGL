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

#include "CVtkPiecewiseFunction.h"

DEFINE_VTK_OBJECT(CVtkPiecewiseFunction, CVtkObject, vtkPiecewiseFunction)
{
    pDesc->setNodeClassName("vtkPiecewiseFunction");
    pDesc->setNodeClassCategory("Algorithm");

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("PiecewiseFunction", IVisSystemNodeConnectionPath::OutputPath,  "vtkPiecewiseFunction", 0)
    );
}

CVtkPiecewiseFunction::CVtkPiecewiseFunction() : m_vtkPiecewiseFunction(0)
{
    CVtkPiecewiseFunction::InitializeObjectDesc();
    setVtkObject(vtkPiecewiseFunction::New());
}

CVtkPiecewiseFunction::~CVtkPiecewiseFunction()
{

}

int CVtkPiecewiseFunction::size() const
{
    return m_vtkPiecewiseFunction->GetSize();
}

void CVtkPiecewiseFunction::addPoint(double x, double val)
{
    m_vtkPiecewiseFunction->AddPoint(x, val);
}

void CVtkPiecewiseFunction::removePoint(double x)
{
    m_vtkPiecewiseFunction->RemovePoint(x);
}

double CVtkPiecewiseFunction::value(double x) const
{
    return m_vtkPiecewiseFunction->GetValue(x);
}

void CVtkPiecewiseFunction::adjustRange(double min, double max)
{
    double range[2];
    range[0] = min;
    range[1] = max;
    m_vtkPiecewiseFunction->AdjustRange(range);
}

void CVtkPiecewiseFunction::removeAllPoints()
{
    m_vtkPiecewiseFunction->RemoveAllPoints();
}

void CVtkPiecewiseFunction::setClamping(bool val)
{
    m_vtkPiecewiseFunction->SetClamping(val);
}

bool CVtkPiecewiseFunction::isClamping() const
{
    return m_vtkPiecewiseFunction->GetClamping();
}

bool CVtkPiecewiseFunction::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "PiecewiseFunction")
    {
        m_piecewiseFunctionData.setPiecewiseFunction(m_vtkPiecewiseFunction);
        *outputData = &m_piecewiseFunctionData;
        return true;
    }

    return false;
}

bool CVtkPiecewiseFunction::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "PiecewiseFunction")
        return true;

    return false;
}


