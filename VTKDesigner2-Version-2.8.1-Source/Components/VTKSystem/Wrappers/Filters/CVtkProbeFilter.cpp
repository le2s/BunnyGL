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

#include "CVtkProbeFilter.h"
#include "CVtkDataSetAlgorithm.h"
#include "vtkProbeFilter.h"
#include "vtkDataSet.h"
#include "IVtkDataSetIOData.h"

DEFINE_VTK_OBJECT(CVtkProbeFilter, CVtkDataSetAlgorithm, vtkProbeFilter)
{
    pDesc->setNodeClassCategory("Filters");
    pDesc->setNodeClassName("vtkProbeFilter");
    pDesc->setNodeClassDescription("sample data values at specified point locations");

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Dataset", IVisSystemNodeConnectionPath::InputPath,  "vtkDataSet", 0)
    );

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Source", IVisSystemNodeConnectionPath::InputPath,  "vtkDataSet", 0)
    );
}

CVtkProbeFilter::CVtkProbeFilter() : m_vtkProbeFilter(0)
{
    CVtkProbeFilter::InitializeObjectDesc();
    setVtkObject(vtkProbeFilter::New());
}

CVtkProbeFilter::~CVtkProbeFilter()
{
}

void CVtkProbeFilter::setSpatialMatch(int val)
{
    m_vtkProbeFilter->SetSpatialMatch(val);
}

bool CVtkProbeFilter::spatialMatch() const
{
    return m_vtkProbeFilter->GetSpatialMatch();
}

bool CVtkProbeFilter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Dataset")
        return m_vtkProbeFilter->GetInput() != 0;

    if(path->pathName() == "Source")
        return m_vtkProbeFilter->GetSource() != 0;

    return CVtkDataSetAlgorithm::hasInput(path);
}

bool CVtkProbeFilter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Dataset")
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);

        if(success)
        {
            vtkDataSet* ds = data->getVtkDataSet();
            m_vtkProbeFilter->SetInput(ds);
            return true;
        }

        return false;
    }

    if(path->pathName() == "Source")
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);

        if(success)
        {
            vtkDataSet* ds  = data->getVtkDataSet();
            m_vtkProbeFilter->SetSource(ds);
            return true;
        }

        return false;
    }

    return CVtkDataSetAlgorithm::setInput(path, inputData);
}

bool CVtkProbeFilter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Dataset")
    {
        m_vtkProbeFilter->SetInput((vtkDataSet*)0);
        return true;
    }

    if(path->pathName() == "Source")
    {
        m_vtkProbeFilter->SetSource(0);
        return true;
    }

    return CVtkDataSetAlgorithm::removeInput(path, inputData);
}

