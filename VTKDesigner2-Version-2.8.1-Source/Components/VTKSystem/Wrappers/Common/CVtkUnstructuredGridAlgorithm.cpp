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

#include "CVtkUnstructuredGridAlgorithm.h"

DEFINE_VTK_OBJECT(CVtkUnstructuredGridAlgorithm, CVtkAlgorithm, vtkUnstructuredGridAlgorithm)
{
    pDesc->setNodeClassCategory("Algorithm");
    pDesc->setNodeClassName("vtkUnstructuredGridAlgorithm");
    pDesc->setNodeClassDescription("A Unstructured Grid algorithm abstraction");
    pDesc->setCreateFunction(0);

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("UnstructuredGridOutput", IVisSystemNodeConnectionPath::OutputPath,  "vtkUnstructuredGrid", 0)
    );
}

CVtkUnstructuredGridAlgorithm::CVtkUnstructuredGridAlgorithm() : m_vtkUnstructuredGridAlgorithm(0)
{
    CVtkUnstructuredGridAlgorithm::InitializeObjectDesc();
}

CVtkUnstructuredGridAlgorithm::~CVtkUnstructuredGridAlgorithm()
{

}

bool CVtkUnstructuredGridAlgorithm::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path || !m_vtkUnstructuredGridAlgorithm)
        return false;

    return CVtkAlgorithm::hasInput(path);
}

bool CVtkUnstructuredGridAlgorithm::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !m_vtkUnstructuredGridAlgorithm)
        return false;

    return CVtkAlgorithm::setInput(path, inputData);
}

bool CVtkUnstructuredGridAlgorithm::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !m_vtkUnstructuredGridAlgorithm)
        return false;

    return CVtkAlgorithm::removeInput(path, inputData);
}

bool CVtkUnstructuredGridAlgorithm::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !m_vtkUnstructuredGridAlgorithm || !outputData)
        return false;

    if(path->pathName() == "UnstructuredGridOutput")
    {
        m_CVtkUnstructuredGrid.setUnstructuredGrid(m_vtkUnstructuredGridAlgorithm->GetOutput());
        *outputData = &m_CVtkUnstructuredGrid;
        return true;
    }

    return CVtkAlgorithm::fetchOutput(path, outputData);
}

bool CVtkUnstructuredGridAlgorithm::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !m_vtkUnstructuredGridAlgorithm)
        return false;

    return CVtkAlgorithm::outputDerefed(path, outputData);
}

