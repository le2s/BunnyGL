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
#include "CVtkCutter.h"
#include "IVtkImplicitFunctionIOData.h"

DEFINE_VTK_OBJECT(CVtkCutter,CVtkPolyDataFilter,vtkCutter)
{
    pDesc->setNodeClassName("vtkCutter");
    pDesc->setNodeClassDescription("Cut vtkDataSet with user-specified implicit function");

// ImplicitFunction Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ImplicitFunction", IVisSystemNodeConnectionPath::InputPath, "vtkImplicitFunction", 0)
        );

    pDesc->setPropertyInfo("NumberOfContours", "SetNumberOfContours", "GetNumberOfContours", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("GenerateCutScalars", "SetGenerateCutScalars", "GetGenerateCutScalars", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("SortBy", "SetSortBy", "GetSortBy", QStringList() << "int", QStringList(), "void", "int");
}

CVtkCutter::CVtkCutter() : m_vtkCutter(0)
{
    CVtkCutter::InitializeObjectDesc();
    setVtkObject(vtkCutter::New());
}

CVtkCutter::~CVtkCutter()
{

}

int CVtkCutter::numberOfContours() const
{
    return m_vtkCutter->GetNumberOfContours();
}


void CVtkCutter::setNumberOfContours(int val)
{
    m_vtkCutter->SetNumberOfContours(val);
    m_vtkCutter->GenerateValues(val, 0, 1);
}

void CVtkCutter::setValue(int i, double val)
{
    m_vtkCutter->SetValue(i, val);
}

bool CVtkCutter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        return m_vtkCutter->GetCutFunction() != 0;
    }

    return CVtkPolyDataFilter::hasInput(path);
}

bool CVtkCutter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        IVtkImplicitFunctionIOData* implicitFunctionData = 0;
        bool success = inputData->queryInterface("IVtkImplicitFunctionIOData", (void**)&implicitFunctionData);
        if(success && implicitFunctionData)
        {
            m_vtkCutter->SetCutFunction(implicitFunctionData->getVtkImplicitFunction());
            return true;
        }

        return false;
    }
    return CVtkPolyDataFilter::setInput(path, inputData);
}

bool CVtkCutter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        IVtkImplicitFunctionIOData* implicitFunctionData = 0;
        bool success = inputData->queryInterface("IVtkImplicitFunctionIOData", (void**)&implicitFunctionData);
        if(success && implicitFunctionData && implicitFunctionData->getVtkImplicitFunction() == m_vtkCutter->GetCutFunction())
        {
            m_vtkCutter->SetCutFunction(0);
            return true;
        }

        return false;
    }
    return CVtkPolyDataFilter::removeInput(path, inputData);
}



bool CVtkCutter::isGenerateCutScalars() const
{
    return m_vtkCutter->GetGenerateCutScalars();
}


void CVtkCutter::setGenerateCutScalars(bool val)
{
    m_vtkCutter->SetGenerateCutScalars(val);
}


CVtkCutter::SortBy CVtkCutter::sortBy() const
{
    switch(m_vtkCutter->GetSortBy())
    {
        case VTK_SORT_BY_VALUE:return (Value);
        case VTK_SORT_BY_CELL:return (Cell);
        default:return(Value);
    }
}

void CVtkCutter::setSortBy(SortBy val)
{
    switch(val)
    {
        case Value:
            m_vtkCutter->SetSortBy(VTK_SORT_BY_VALUE);
            break;
        case Cell:
            m_vtkCutter->SetSortBy(VTK_SORT_BY_CELL);
            break;
        default:
            m_vtkCutter->SetSortBy(VTK_SORT_BY_VALUE);
            break;
     }
}

