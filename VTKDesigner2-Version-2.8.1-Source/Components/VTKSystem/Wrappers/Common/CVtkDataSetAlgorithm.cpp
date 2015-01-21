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

#include "CVtkDataSetAlgorithm.h"
#include "IVisSystemNodeControlData.h"

DEFINE_VTK_OBJECT(CVtkDataSetAlgorithm, CVtkAlgorithm, vtkDataSetAlgorithm)
{
    pDesc->setNodeClassCategory("Algorithm");
    pDesc->setNodeClassName("vtkDataSetAlgorithm");
    pDesc->setNodeClassDescription("A data set algorithm abstraction");
    pDesc->setCreateFunction(0);

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("DataSetOutput", IVisSystemNodeConnectionPath::OutputPath,  "vtkDataSet", 0)
        );
}

CVtkDataSetAlgorithm::CVtkDataSetAlgorithm() : m_vtkDataSetAlgorithm(0)
{
    CVtkDataSetAlgorithm::InitializeObjectDesc();
}

CVtkDataSetAlgorithm::~CVtkDataSetAlgorithm()
{

}

bool CVtkDataSetAlgorithm::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path || !m_vtkDataSetAlgorithm)
        return false;

    return CVtkAlgorithm::hasInput(path);
}

bool CVtkDataSetAlgorithm::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !m_vtkDataSetAlgorithm)
        return false;

    return CVtkAlgorithm::setInput(path, inputData);
}

bool CVtkDataSetAlgorithm::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !m_vtkDataSetAlgorithm)
        return false;

    return CVtkAlgorithm::removeInput(path, inputData);
}

bool CVtkDataSetAlgorithm::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !m_vtkDataSetAlgorithm || !outputData)
        return false;

    if(path->pathName() == "DataSetOutput")
    {
        m_polyData.setDataSet(m_vtkDataSetAlgorithm->GetOutput());
        *outputData = &m_polyData;
        return true;
    }

    return CVtkAlgorithm::fetchOutput(path, outputData);
}

bool CVtkDataSetAlgorithm::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !m_vtkDataSetAlgorithm)
        return false;

    return CVtkAlgorithm::outputDerefed(path, outputData);
}

