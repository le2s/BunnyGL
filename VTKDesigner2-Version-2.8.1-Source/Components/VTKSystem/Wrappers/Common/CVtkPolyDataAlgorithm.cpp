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

#include "CVtkPolyDataAlgorithm.h"
#include "IVisSystemNodeControlData.h"

DEFINE_VTK_OBJECT(CVtkPolyDataAlgorithm, CVtkAlgorithm, vtkPolyDataAlgorithm)
{
    pDesc->setNodeClassCategory("Algorithm");
    pDesc->setNodeClassName("vtkPolyDataAlgorithm");
    pDesc->setNodeClassDescription("A poly data algorithm abstraction");
    pDesc->setCreateFunction(0);

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("PolyDataOutput", IVisSystemNodeConnectionPath::OutputPath,  "vtkPolyData", 0)
        );
}

CVtkPolyDataAlgorithm::CVtkPolyDataAlgorithm() : m_vtkPolyDataAlgorithm(0)
{
    CVtkPolyDataAlgorithm::InitializeObjectDesc();
}

CVtkPolyDataAlgorithm::~CVtkPolyDataAlgorithm()
{

}

bool CVtkPolyDataAlgorithm::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path || !m_vtkPolyDataAlgorithm)
        return false;

    return CVtkAlgorithm::hasInput(path);
}

bool CVtkPolyDataAlgorithm::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !m_vtkPolyDataAlgorithm)
        return false;

    return CVtkAlgorithm::setInput(path, inputData);
}

bool CVtkPolyDataAlgorithm::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !m_vtkPolyDataAlgorithm)
        return false;

    return CVtkAlgorithm::removeInput(path, inputData);
}

bool CVtkPolyDataAlgorithm::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !m_vtkPolyDataAlgorithm || !outputData)
        return false;

    if(path->pathName() == "PolyDataOutput")
    {
        m_polyData.setPolyData(m_vtkPolyDataAlgorithm->GetOutput());
        *outputData = &m_polyData;
        return true;
    }

    return CVtkAlgorithm::fetchOutput(path, outputData);
}

bool CVtkPolyDataAlgorithm::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !m_vtkPolyDataAlgorithm)
        return false;

    return CVtkAlgorithm::outputDerefed(path, outputData);
}

