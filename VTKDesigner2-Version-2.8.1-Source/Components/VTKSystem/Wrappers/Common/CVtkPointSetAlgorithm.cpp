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

#include "CVtkPointSetAlgorithm.h"

DEFINE_VTK_OBJECT(CVtkPointSetAlgorithm, CVtkAlgorithm, vtkPointSetAlgorithm)
{
    pDesc->setNodeClassCategory("Algorithm");
    pDesc->setNodeClassName("vtkPointSetAlgorithm");
    pDesc->setNodeClassDescription("A Unstructured Grid algorithm abstraction");
    pDesc->setCreateFunction(0);

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("PointSetOutput", IVisSystemNodeConnectionPath::OutputPath,  "vtkPointSet", 0)
    );
}

CVtkPointSetAlgorithm::CVtkPointSetAlgorithm() : m_vtkPointSetAlgorithm(0)
{
    CVtkPointSetAlgorithm::InitializeObjectDesc();
}

CVtkPointSetAlgorithm::~CVtkPointSetAlgorithm()
{

}

bool CVtkPointSetAlgorithm::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path || !m_vtkPointSetAlgorithm)
        return false;

    return CVtkAlgorithm::hasInput(path);
}

bool CVtkPointSetAlgorithm::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !m_vtkPointSetAlgorithm)
        return false;

    return CVtkAlgorithm::setInput(path, inputData);
}

bool CVtkPointSetAlgorithm::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !m_vtkPointSetAlgorithm)
        return false;

    return CVtkAlgorithm::removeInput(path, inputData);
}

bool CVtkPointSetAlgorithm::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !m_vtkPointSetAlgorithm || !outputData)
        return false;

    if(path->pathName() == "PointSetOutput")
    {
        m_CVtkPointSet.setPointSet(m_vtkPointSetAlgorithm->GetOutput());
        *outputData = &m_CVtkPointSet;
        return true;
    }

    return CVtkAlgorithm::fetchOutput(path, outputData);
}

bool CVtkPointSetAlgorithm::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !m_vtkPointSetAlgorithm)
        return false;

    return CVtkAlgorithm::outputDerefed(path, outputData);
}

