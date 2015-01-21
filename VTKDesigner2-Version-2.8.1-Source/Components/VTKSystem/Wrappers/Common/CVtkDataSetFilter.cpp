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

#include "CVtkDataSetFilter.h"
#include "IVtkDataSetIOData.h"

DEFINE_VTK_OBJECT(CVtkDataSetFilter, CVtkDataSetAlgorithm, vtkDataSetAlgorithm)
{
    pDesc->setNodeClassName("vtkDataSetFilter"); // Dummy class.
    pDesc->setNodeClassCategory("DataSet Filters");
    pDesc->setNodeClassDescription("Base class for all data set filters");

    pDesc->setCreateFunction(0);

    // vtkDataObject input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("DataObject", IVisSystemNodeConnectionPath::InputPath, "vtkDataObject", 0)
        );
}

CVtkDataSetFilter::CVtkDataSetFilter() : m_vtkDataSetAlgorithm(0)
{
    CVtkDataSetFilter::InitializeObjectDesc();
}

CVtkDataSetFilter::~CVtkDataSetFilter()
{

}

bool CVtkDataSetFilter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!m_vtkDataSetAlgorithm || !path)
        return false;

    if(path->pathName() == "DataObject")
    {
        return m_vtkDataSetAlgorithm->GetInput() != 0;
    }

    return CVtkDataSetAlgorithm::hasInput(path);
}

bool CVtkDataSetFilter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!m_vtkDataSetAlgorithm || !path || !inputData)
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
                m_vtkDataSetAlgorithm->SetInput(object);
                return true;
            }
        }
        return false;
    }

    return CVtkDataSetAlgorithm::setInput(path, inputData);
}

bool CVtkDataSetFilter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!m_vtkDataSetAlgorithm || !path || !inputData)
        return false;

    if(path->pathName() == "DataObject")
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);
        if(success && data)
        {
            vtkDataSet* object = data->getVtkDataSet();
            if(object && m_vtkDataSetAlgorithm->GetInput() == object)
            {
                m_vtkDataSetAlgorithm->SetInput((vtkDataSet*)0);
                return true;
            }
        }

        return false;
    }

    return CVtkDataSetAlgorithm::removeInput(path, inputData);
}


