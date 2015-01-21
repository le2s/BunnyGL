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

#include "CVtkPolyDataFilter.h"
#include "IVtkDataObjectIOData.h"

DEFINE_VTK_OBJECT(CVtkPolyDataFilter, CVtkPolyDataAlgorithm, vtkPolyDataAlgorithm)
{
    pDesc->setNodeClassName("vtkPolyDataFilter"); // Dummy class.
    pDesc->setNodeClassCategory("PolyData Filters");
    pDesc->setNodeClassDescription("Base class for all polydata filters");

    pDesc->setCreateFunction(0);

    // vtkDataObject input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("DataObject", IVisSystemNodeConnectionPath::InputPath, "vtkDataObject", 0)
        );
}

CVtkPolyDataFilter::CVtkPolyDataFilter() : m_vtkPolyDataAlgorithm(0)
{
    CVtkPolyDataFilter::InitializeObjectDesc();
}

CVtkPolyDataFilter::~CVtkPolyDataFilter()
{

}

bool CVtkPolyDataFilter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!m_vtkPolyDataAlgorithm || !path)
        return false;

    if(path->pathName() == "DataObject")
    {
        return m_vtkPolyDataAlgorithm->GetInput() != 0;
    }

    return CVtkPolyDataAlgorithm::hasInput(path);
}

bool CVtkPolyDataFilter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!m_vtkPolyDataAlgorithm || !path || !inputData)
        return false;

    if(path->pathName() == "DataObject")
    {
        IVtkDataObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataObjectIOData", (void**)&data);
        if(success && data)
        {
            vtkDataObject* object = data->getVtkDataObject();
            if(object)
            {
                m_vtkPolyDataAlgorithm->SetInput(object);
                return true;
            }
        }
        return false;
    }

    return CVtkPolyDataAlgorithm::setInput(path, inputData);
}

bool CVtkPolyDataFilter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!m_vtkPolyDataAlgorithm || !path || !inputData)
        return false;

    if(path->pathName() == "DataObject")
    {
        IVtkDataObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataObjectIOData", (void**)&data);
        if(success && data)
        {
            vtkDataObject* object = data->getVtkDataObject();
            if(object && m_vtkPolyDataAlgorithm->GetInput() == object)
            {
                m_vtkPolyDataAlgorithm->SetInput(0);
                return true;
            }
        }

        return false;
    }

    return CVtkPolyDataAlgorithm::removeInput(path, inputData);
}


