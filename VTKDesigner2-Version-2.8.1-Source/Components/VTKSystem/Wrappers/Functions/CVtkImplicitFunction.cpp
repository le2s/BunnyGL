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
#include "CVtkImplicitFunction.h"

DEFINE_VTK_OBJECT(CVtkImplicitFunction,CVtkObject,vtkImplicitFunction)
{
    pDesc->setNodeClassName("vtkImplicitFunction");
    pDesc->setNodeClassCategory("Functions");
    pDesc->setCreateFunction(0);

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ImplicitFunction", IVisSystemNodeConnectionPath::OutputPath,  "vtkImplicitFunction", 0)
    );

}

CVtkImplicitFunction::CVtkImplicitFunction() : m_vtkImplicitFunction(0)
{
    CVtkImplicitFunction::InitializeObjectDesc();
    setVtkObject(vtkImplicitFunction::New());
}

CVtkImplicitFunction::~CVtkImplicitFunction()
{

}

double CVtkImplicitFunction::evaluateFunction(double x, double y, double z)
{
    if(m_vtkImplicitFunction)
        return m_vtkImplicitFunction->EvaluateFunction(x, y, z);
    return 0.0f;
}

bool CVtkImplicitFunction::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData || !m_vtkImplicitFunction)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        m_ImplicitFunctionData.setImplicitFunction(m_vtkImplicitFunction);
        *outputData = &m_ImplicitFunctionData;
        return true;
    }

    return CVtkObject::fetchOutput(path, outputData);
}

bool CVtkImplicitFunction::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    Q_UNUSED(path);
    Q_UNUSED(outputData);
    return CVtkObject::outputDerefed(path, outputData);
}
