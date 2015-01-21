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
#include "CVtkClipDataSet.h"
#include "IVtkImplicitFunctionIOData.h"
#include "vtkImplicitFunction.h"


DEFINE_VTK_OBJECT(CVtkClipDataSet,CVtkDataSetFilter,vtkClipDataSet)
{
    pDesc->setNodeClassName("vtkClipDataSet");
    pDesc->setNodeClassDescription("Clip any dataset with user-specified implicit function or input scalar data");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("Value", "SetValue", "GetValue", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("InsideOut", "SetInsideOut", "GetInsideOut", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("GenerateClipScalars", "SetGenerateClipScalars", "GetGenerateClipScalars", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("GenerateClippedOutput", "SetGenerateClippedOutput", "GetGenerateClippedOutput", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("MergeTolerance", "SetMergeTolerance", "GetMergeTolerance", QStringList() << "double", QStringList(), "void", "double");

    // vtkImplicitFunction input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ImplicitFunction", IVisSystemNodeConnectionPath::InputPath, "vtkImplicitFunction", 0)
        );

    // Clipped vtkPolyData output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ClippedUnstructuredGrid", IVisSystemNodeConnectionPath::OutputPath, "vtkUnstructuredGrid", 0)
        );
}

CVtkClipDataSet::CVtkClipDataSet() : m_vtkClipDataSet(0)
{
    CVtkClipDataSet::InitializeObjectDesc();
    setVtkObject(vtkClipDataSet::New());
}

CVtkClipDataSet::~CVtkClipDataSet()
{

}

double CVtkClipDataSet::value() const
{
    return m_vtkClipDataSet->GetValue();
}


void CVtkClipDataSet::setValue(double val)
{
    m_vtkClipDataSet->SetValue(val);
}


bool CVtkClipDataSet::isInsideOut() const
{
    return m_vtkClipDataSet->GetInsideOut();
}


void CVtkClipDataSet::setInsideOut(bool val)
{
    m_vtkClipDataSet->SetInsideOut(val);
}


bool CVtkClipDataSet::isGenerateClipScalars() const
{
    return m_vtkClipDataSet->GetGenerateClipScalars();
}


void CVtkClipDataSet::setGenerateClipScalars(bool val)
{
    m_vtkClipDataSet->SetGenerateClipScalars(val);
}


bool CVtkClipDataSet::generateClippedOutput() const
{
    return m_vtkClipDataSet->GetGenerateClippedOutput();
}


void CVtkClipDataSet::setGenerateClippedOutput(bool val)
{
    m_vtkClipDataSet->SetGenerateClippedOutput(val);
}


double CVtkClipDataSet::mergeTolerance() const
{
    return m_vtkClipDataSet->GetMergeTolerance();
}


void CVtkClipDataSet::setMergeTolerance(double val)
{
    m_vtkClipDataSet->SetMergeTolerance(val);
}

bool CVtkClipDataSet::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        return m_vtkClipDataSet->GetClipFunction() != 0;
    }

    return CVtkDataSetFilter::hasInput(path);
}

bool CVtkClipDataSet::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        IVtkImplicitFunctionIOData* data = 0;
        bool success = inputData->queryInterface("IVtkImplicitFunctionIOData", (void**)&data);
        if(data && success)
        {
            m_vtkClipDataSet->SetClipFunction( data->getVtkImplicitFunction() );
            return true;
        }

        return false;
    }

    return CVtkDataSetFilter::setInput(path, inputData);
}

bool CVtkClipDataSet::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        IVtkImplicitFunctionIOData* data = 0;
        bool success = inputData->queryInterface("IVtkImplicitFunctionIOData", (void**)&data);
        if(data && success && m_vtkClipDataSet->GetClipFunction() == data->getVtkImplicitFunction())
        {
            m_vtkClipDataSet->SetClipFunction(0);
            return true;
        }

        return false;
    }

    return CVtkDataSetFilter::removeInput(path, inputData);
}

bool CVtkClipDataSet::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ClippedUnstructuredGrid")
    {
        m_clippedOutput.setUnstructuredGrid( m_vtkClipDataSet->GetClippedOutput() );
        *outputData = &m_clippedOutput;
        return true;
    }

    return CVtkDataSetFilter::fetchOutput(path, outputData);
}

bool CVtkClipDataSet::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ClippedUnstructuredGrid")
        return true;

    return CVtkDataSetFilter::outputDerefed(path, outputData);
}

