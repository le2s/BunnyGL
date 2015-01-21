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
#include "CVtkMergeFilter.h"
#include "vtkMergeFilter.h"
#include "IVtkDataSetIOData.h"
#include "vtkDataSet.h"

DEFINE_VTK_OBJECT(CVtkMergeFilter, CVtkDataSetAlgorithm, vtkMergeFilter)
{
    pDesc->setNodeClassCategory("Filters");
    pDesc->setNodeClassName("vtkMergeFilter");
    pDesc->setNodeClassDescription("extract separate components of data from different datasets");

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Dataset", IVisSystemNodeConnectionPath::InputPath, "vtkMergeFilter", 0)
        );

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Scalars", IVisSystemNodeConnectionPath::InputPath, "vtkMergeFilter", 0)
        );

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Vectors", IVisSystemNodeConnectionPath::InputPath, "vtkMergeFilter", 0)
        );

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Normals", IVisSystemNodeConnectionPath::InputPath, "vtkMergeFilter", 0)
        );

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Tcoords", IVisSystemNodeConnectionPath::InputPath, "vtkMergeFilter", 0)
        );

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Tensors", IVisSystemNodeConnectionPath::InputPath, "vtkMergeFilter", 0)
        );


}

CVtkMergeFilter::CVtkMergeFilter() : m_vtkMergeFilter(0)
{
    CVtkMergeFilter::InitializeObjectDesc();
    setVtkObject(vtkMergeFilter::New());
}

CVtkMergeFilter::~CVtkMergeFilter()
{
}

bool CVtkMergeFilter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Dataset")
        return m_vtkMergeFilter->GetGeometry() != 0 ;

    if(path->pathName() == "Scalars")
        return m_vtkMergeFilter->GetScalars() != 0 ;

    if(path->pathName() == "Vectors")
        return m_vtkMergeFilter->GetVectors() != 0 ;

    if(path->pathName() == "Normals")
        return m_vtkMergeFilter->GetNormals() != 0 ;

    if(path->pathName() == "Tcoords")
        return m_vtkMergeFilter->GetTCoords() != 0 ;

    if(path->pathName() == "Tensors")
        return m_vtkMergeFilter->GetTensors() != 0 ;

    return CVtkDataSetAlgorithm::hasInput(path);
}

bool CVtkMergeFilter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
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
            m_vtkMergeFilter->SetGeometry(ds);
            return true;
        }

        return false;
    }

    if(path->pathName() == "Scalars")
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);

        if(success)
        {
            vtkDataSet* sc = data->getVtkDataSet();
            m_vtkMergeFilter->SetScalars(sc);
            return true;
        }

        return false;
    }

    if(path->pathName() == "Vectors")
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);

        if(success)
        {
            vtkDataSet* vect = data->getVtkDataSet();
            m_vtkMergeFilter->SetVectors(vect);
            return true;
        }

        return false;
    }

    if(path->pathName() == "Normals")
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);

        if(success)
        {
            vtkDataSet* norm = data->getVtkDataSet();
            m_vtkMergeFilter->SetNormals(norm);
            return true;
        }

        return false;
    }

    if(path->pathName() == "Tcoords")
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);

        if(success)
        {
            vtkDataSet* tc = data->getVtkDataSet();
            m_vtkMergeFilter->SetTCoords(tc);
            return true;
        }

        return false;
    }

    if(path->pathName() == "Tensors")
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);

        if(success)
        {
            vtkDataSet* tensors = data->getVtkDataSet();
            m_vtkMergeFilter->SetTensors(tensors);
            return true;
        }

        return false;
    }

    return CVtkDataSetAlgorithm::setInput(path, inputData);
}

bool CVtkMergeFilter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Dataset")
    {
        m_vtkMergeFilter->SetGeometry(0);
        return true;
    }

    if(path->pathName() == "Scalars")
    {
        m_vtkMergeFilter->SetScalars(0);
        return true;
    }

    if(path->pathName() == "Vectors")
    {
        m_vtkMergeFilter->SetVectors(0);
        return true;
    }

    if(path->pathName() == "Normals")
    {
        m_vtkMergeFilter->SetNormals(0);
        return true;
    }

    if(path->pathName() == "Tcoords")
    {
        m_vtkMergeFilter->SetTCoords(0);
        return true;
    }

    if(path->pathName() == "Tensors")
    {
        m_vtkMergeFilter->SetTensors(0);
        return true;
    }

    return CVtkDataSetAlgorithm::removeInput(path, inputData);
}
