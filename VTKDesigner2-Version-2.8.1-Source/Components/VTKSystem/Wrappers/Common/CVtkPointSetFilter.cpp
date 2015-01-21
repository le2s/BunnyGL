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

#include "CVtkPointSetFilter.h"
#include "IVtkDataSetIOData.h"
#include "IVtkPointSetIOData.h"

DEFINE_VTK_OBJECT(CVtkPointSetFilter, CVtkPointSetAlgorithm, vtkPointSetAlgorithm)
{
    pDesc->setNodeClassName("vtkPointSetFilter"); // Dummy class.
    pDesc->setNodeClassCategory("PointSet Filters");
    pDesc->setNodeClassDescription("Base class for all data set filters");

    pDesc->setCreateFunction(0);

    // vtkDataObject input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("DataObject", IVisSystemNodeConnectionPath::InputPath, "vtkDataObject", 0)
        );

    // vtkPointSet input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("PointSet", IVisSystemNodeConnectionPath::OutputPath,  "vtkPointSet", 0)
    );

}

CVtkPointSetFilter::CVtkPointSetFilter() : m_vtkPointSetAlgorithm(0)
{
    CVtkPointSetFilter::InitializeObjectDesc();
}

CVtkPointSetFilter::~CVtkPointSetFilter()
{

}

bool CVtkPointSetFilter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!m_vtkPointSetAlgorithm || !path)
        return false;

    if(path->pathName() == "DataObject")
    {
        return m_vtkPointSetAlgorithm->GetInput() != 0;
    }

    if(path->pathName() == "PointSet")
    {
        return m_vtkPointSetAlgorithm->GetInput() != 0;
    }

    return CVtkPointSetAlgorithm::hasInput(path);
}

bool CVtkPointSetFilter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!m_vtkPointSetAlgorithm || !path || !inputData)
        return false;

    if(path->pathName() == "DataObject")
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);
        if(success && data)
        {
            vtkDataSet* object = data->getVtkDataSet();
            if(object)
            {
                m_vtkPointSetAlgorithm->SetInput(object);
                return true;
            }
        }
        return false;
    }

    if(path->pathName() == "PointSet")
    {
        IVtkPointSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkPointSetIOData", (void**)&data);
        if(success && data)
        {
            vtkPointSet* object = data->getVtkPointSet();
            if(object)
            {
                m_vtkPointSetAlgorithm->SetInput(object);
                return true;
            }
        }
        return false;
    }

    return CVtkPointSetAlgorithm::setInput(path, inputData);
}

bool CVtkPointSetFilter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!m_vtkPointSetAlgorithm || !path || !inputData)
        return false;

    if(path->pathName() == "DataObject")
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);
        if(success && data)
        {
            vtkDataSet* object = data->getVtkDataSet();
            if(object && m_vtkPointSetAlgorithm->GetInput() == object)
            {
                m_vtkPointSetAlgorithm->SetInput((vtkDataSet*)0);
                return true;
            }
        }

        return false;
    }

    if(path->pathName() == "PointSet")
    {
        IVtkPointSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkPointSetIOData", (void**)&data);
        if(success && data)
        {
            vtkPointSet* object = data->getVtkPointSet();
            if(object && m_vtkPointSetAlgorithm->GetInput() == object)
            {
                m_vtkPointSetAlgorithm->SetInput((vtkPointSet*)0);
                return true;
            }
        }

        return false;
    }

    return CVtkPointSetAlgorithm::removeInput(path, inputData);
}


