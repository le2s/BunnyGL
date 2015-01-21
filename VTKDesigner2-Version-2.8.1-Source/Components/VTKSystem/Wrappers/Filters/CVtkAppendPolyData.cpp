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

#include "CVtkAppendPolyData.h"
#include "IVtkPolyDataIOData.h"

DEFINE_VTK_OBJECT(CVtkAppendPolyData,CVtkPolyDataFilter,vtkAppendPolyData)
{
    pDesc->setNodeClassName("vtkAppendPolyData");
    pDesc->setNodeClassDescription("Appends one or more polygonal datasets together");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("UserManagedInputs", "SetUserManagedInputs", "GetUserManagedInputs", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ParallelStreaming", "SetParallelStreaming", "GetParallelStreaming", QStringList() << "int" , QStringList(), "void", "int");

    // vtkDataObject input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("PolyData", IVisSystemNodeConnectionPath::InputPath, "vtkPolyData", 0, true)
        );
}

CVtkAppendPolyData::CVtkAppendPolyData() : m_vtkAppendPolyData(0)
{
    CVtkAppendPolyData::InitializeObjectDesc();
    setVtkObject(vtkAppendPolyData::New());
}

CVtkAppendPolyData::~CVtkAppendPolyData()
{

}

bool CVtkAppendPolyData::isUserManagedInputs() const
{
    return m_vtkAppendPolyData->GetUserManagedInputs();
}


void CVtkAppendPolyData::setUserManagedInputs(bool val)
{
    m_vtkAppendPolyData->SetUserManagedInputs(val);
}


bool CVtkAppendPolyData::isParallelStreaming() const
{
    return m_vtkAppendPolyData->GetParallelStreaming();
}


void CVtkAppendPolyData::setParallelStreaming(bool val)
{
    m_vtkAppendPolyData->SetParallelStreaming(val);
}

bool CVtkAppendPolyData::hasInput(IVisSystemNodeConnectionPath* path)
{
    if( !path )
        return false;

    if(path->pathName() == "PolyData")
    {
        return m_vtkAppendPolyData->GetInput(0);
    }

    return CVtkPolyDataFilter::hasInput(path);
}

bool CVtkAppendPolyData::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if( !path )
        return false;

    if(path->pathName() == "PolyData")
    {
        IVtkPolyDataIOData* data = 0;
        bool success = inputData->queryInterface("IVtkPolyDataIOData", (void**)&data);
        if(success && data)
        {
            vtkPolyData* pd = data->getVtkPolyData();
            m_vtkAppendPolyData->AddInput(pd);
            return true;
        }

        return false;
    }

    return CVtkPolyDataFilter::setInput(path, inputData);
}

bool CVtkAppendPolyData::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if( !path )
        return false;

    if(path->pathName() == "PolyData")
    {
        IVtkPolyDataIOData* data = 0;
        bool success = inputData->queryInterface("IVtkPolyDataIOData", (void**)&data);
        if(success && data)
        {
            vtkPolyData* pd = data->getVtkPolyData();
            m_vtkAppendPolyData->RemoveInput(pd);
            return true;
        }

        return false;
    }

    return CVtkPolyDataFilter::removeInput(path, inputData);
}


