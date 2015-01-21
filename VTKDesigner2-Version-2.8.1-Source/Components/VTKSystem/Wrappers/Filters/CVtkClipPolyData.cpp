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

#include "CVtkClipPolyData.h"
#include "IVtkImplicitFunctionIOData.h"
#include "vtkImplicitFunction.h"

DEFINE_VTK_OBJECT(CVtkClipPolyData,CVtkPolyDataFilter,vtkClipPolyData)
{
    pDesc->setNodeClassName("vtkClipPolyData");
    pDesc->setNodeClassDescription("Clip polygonal data with user-specified implicit function or input scalar data");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("Value", "SetValue", "GetValue", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("InsideOut", "SetInsideOut", "GetInsideOut", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("GenerateClipScalars", "SetGenerateClipScalars", "GetGenerateClipScalars", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("GenerateClippedOutput", "SetGenerateClippedOutput", "GetGenerateClippedOutput", QStringList() << "bool", QStringList(), "void", "bool");

    // vtkImplicitFunction input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ImplicitFunction", IVisSystemNodeConnectionPath::InputPath, "vtkImplicitFunction", 0)
        );

    // Clipped vtkPolyData output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ClippedPolyData", IVisSystemNodeConnectionPath::OutputPath, "vtkPolyData", 0)
        );
}

CVtkClipPolyData::CVtkClipPolyData() : m_vtkClipPolyData(0)
{
    CVtkClipPolyData::InitializeObjectDesc();
    setVtkObject(vtkClipPolyData::New());
}

CVtkClipPolyData::~CVtkClipPolyData()
{

}

double CVtkClipPolyData::value() const
{
    return m_vtkClipPolyData->GetValue();
}


void CVtkClipPolyData::setValue(double val)
{
    m_vtkClipPolyData->SetValue(val);
}


bool CVtkClipPolyData::isInsideOut() const
{
    return m_vtkClipPolyData->GetInsideOut();
}


void CVtkClipPolyData::setInsideOut(bool val)
{
    m_vtkClipPolyData->SetInsideOut(val);
}


bool CVtkClipPolyData::isGenerateClipScalars() const
{
    return m_vtkClipPolyData->GetGenerateClipScalars();
}


void CVtkClipPolyData::setGenerateClipScalars(bool val)
{
    m_vtkClipPolyData->SetGenerateClipScalars(val);
}


bool CVtkClipPolyData::generateClippedOutput() const
{
    return m_vtkClipPolyData->GetGenerateClippedOutput();
}


void CVtkClipPolyData::setGenerateClippedOutput(bool val)
{
    m_vtkClipPolyData->SetGenerateClippedOutput(val);
}

bool CVtkClipPolyData::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        return m_vtkClipPolyData->GetClipFunction() != 0;
    }

    return CVtkPolyDataFilter::hasInput(path);
}

bool CVtkClipPolyData::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        IVtkImplicitFunctionIOData* data = 0;
        bool success = inputData->queryInterface("IVtkImplicitFunctionIOData", (void**)&data);
        if(data && success)
        {
            m_vtkClipPolyData->SetClipFunction( data->getVtkImplicitFunction() );
            return true;
        }

        return false;
    }

    return CVtkPolyDataFilter::setInput(path, inputData);
}

bool CVtkClipPolyData::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        IVtkImplicitFunctionIOData* data = 0;
        bool success = inputData->queryInterface("IVtkImplicitFunctionIOData", (void**)&data);
        if(data && success && m_vtkClipPolyData->GetClipFunction() == data->getVtkImplicitFunction())
        {
            m_vtkClipPolyData->SetClipFunction(0);
            return true;
        }

        return false;
    }

    return CVtkPolyDataFilter::removeInput(path, inputData);
}

bool CVtkClipPolyData::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ClippedPolyData")
    {
        m_clippedOutput.setPolyData( m_vtkClipPolyData->GetClippedOutput() );
        *outputData = &m_clippedOutput;
        return true;
    }

    return CVtkPolyDataFilter::fetchOutput(path, outputData);
}

bool CVtkClipPolyData::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ClippedPolyData")
        return true;

    return CVtkPolyDataFilter::outputDerefed(path, outputData);
}
