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

#include "CVtkCustomFunction.h"

DEFINE_VTK_OBJECT(CVtkCustomFunction, CVtkImplicitFunction, vtkCustomFunction)
{
    pDesc->setNodeClassName("vtkCustomFunction");
    pDesc->setNodeClassCategory("Scriptable Nodes");
    pDesc->setNodeClassDescription("Scriptable implicit function");
	pDesc->setNodeIcon(qApp->windowIcon()); // vtkCustomFunction is not a standard VTK class
}

CVtkCustomFunction::CVtkCustomFunction() : m_vtkCustomFunction(0)
{
    CVtkCustomFunction::InitializeObjectDesc();
    setVtkObject(vtkCustomFunction::New());
    m_vtkCustomFunction->SetCustomFunctionWrapper(this);
}

CVtkCustomFunction::~CVtkCustomFunction()
{

}

Point3D CVtkCustomFunction::functionPoint() const
{
    return m_functionPoint;
}

void CVtkCustomFunction::setFunctionValue(double val)
{
    m_functionValue = val;
}

void CVtkCustomFunction::setFunctionGradient(Vector3D val)
{
    m_functionGradient = val;
}

double CVtkCustomFunction::functionValue() const
{
    return m_functionValue;
}

Vector3D CVtkCustomFunction::functionGradient() const
{
    return m_functionGradient;
}

void CVtkCustomFunction::setFunctionPoint(Point3D val)
{
    m_functionPoint = val;
}

void CVtkCustomFunction::markModified()
{
    m_vtkCustomFunction->Modified();
}

void CVtkCustomFunction::command_Modify()
{
    m_vtkCustomFunction->Modified();
}

//////////////////////////////////////////////////////////////////////////////////////////
/// vtkCustomFunction implementation
//////////////////////////////////////////////////////////////////////////////////////////

vtkCustomFunction* vtkCustomFunction::New()
{
    return new vtkCustomFunction;
}

double vtkCustomFunction::EvaluateFunction(double val[3])
{
    if(this->InEvaluateFunction)
        return 0;

    if(this->CustomFunctionWrapper)
    {
        Point3D point(val);
        this->CustomFunctionWrapper->setFunctionPoint(point);
        this->InEvaluateFunction = true;
        emit this->CustomFunctionWrapper->OnFunctionValueRequest(point);
        this->InEvaluateFunction = false;
        return this->CustomFunctionWrapper->functionValue();
    }

    return 1;
}

void vtkCustomFunction::EvaluateGradient(double val[3], double grad[3])
{
    if(this->InEvaluateGradient)
        return;

    if(this->CustomFunctionWrapper)
    {
        Point3D point(val);
        this->CustomFunctionWrapper->setFunctionPoint(point);
        this->InEvaluateGradient = true;
        emit this->CustomFunctionWrapper->OnFunctionGradientRequest(point);
        this->InEvaluateGradient = false;

        Vector3D gradVec = this->CustomFunctionWrapper->functionGradient();
        grad[0] = gradVec.x;
        grad[1] = gradVec.y;
        grad[2] = gradVec.z;
    }
    else
    {
        grad[0] = 0;
        grad[1] = 0;
        grad[2] = 0;
    }
}

vtkCustomFunction::vtkCustomFunction()
{
    this->CustomFunctionWrapper = 0;
    this->InEvaluateGradient = 0;
    this->InEvaluateFunction = 0;
}

vtkCustomFunction::~vtkCustomFunction()
{

}

