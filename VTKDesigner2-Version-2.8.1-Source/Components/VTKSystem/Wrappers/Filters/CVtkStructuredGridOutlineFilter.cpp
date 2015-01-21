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

#include "CVtkStructuredGridOutlineFilter.h"
#include "vtkStructuredGrid.h"
#include "IVtkStructuredGridIOData.h"

DEFINE_VTK_OBJECT(CVtkStructuredGridOutlineFilter, CVtkPolyDataAlgorithm, vtkStructuredGridOutlineFilter)
{
    pDesc->setNodeClassCategory("Filters");
    pDesc->setNodeClassName("vtkStructuredGridOutlineFilter");
    pDesc->setNodeClassDescription("create wireframe outline for structured grid");

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("StructuredGridInput", IVisSystemNodeConnectionPath::InputPath,  "vtkStructuredGrid", 0)
    );
}

CVtkStructuredGridOutlineFilter::CVtkStructuredGridOutlineFilter()
{
    CVtkStructuredGridOutlineFilter::InitializeObjectDesc();
    setVtkObject(vtkStructuredGridOutlineFilter::New());
}

CVtkStructuredGridOutlineFilter::~CVtkStructuredGridOutlineFilter()
{

}

bool CVtkStructuredGridOutlineFilter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "StructuredGridInput")
    {
        return m_vtkStructuredGridOutlineFilter->GetInput() != 0;
    }

    return CVtkPolyDataAlgorithm::hasInput(path);
}

bool CVtkStructuredGridOutlineFilter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "StructuredGridInput")
    {
        IVtkStructuredGridIOData* data = 0;
        bool success = inputData->queryInterface("IVtkStructuredGridIOData", (void**)&data);
        if(success)
        {
            vtkStructuredGrid* sGrid = data->getVtkStructuredGrid();
            m_vtkStructuredGridOutlineFilter->SetInput(sGrid);
            return true;
        }

        return false;
    }

    return CVtkPolyDataAlgorithm::setInput(path, inputData);
}

bool CVtkStructuredGridOutlineFilter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "StructuredGridInput")
    {
        m_vtkStructuredGridOutlineFilter->SetInput(0);
        return true;
    }

    return CVtkPolyDataAlgorithm::removeInput(path, inputData);
}

