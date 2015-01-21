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
#include "CVtkGenericCutter.h"
#include "IVtkImplicitFunctionIOData.h"

DEFINE_VTK_OBJECT(CVtkGenericCutter,CVtkPolyDataFilter,vtkGenericCutter)
{
    pDesc->setNodeClassName("vtkGenericCutter");
    pDesc->setNodeClassDescription("Cut a vtkGenericDataSet with an implicit function or scalar data");

    // ImplicitFunction Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ImplicitFunction", IVisSystemNodeConnectionPath::InputPath, "vtkImplicitFunction", 0)
        );

    pDesc->setPropertyInfo("NumberOfContours", "SetNumberOfContours", "GetNumberOfContours", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("GenerateCutScalars", "SetGenerateCutScalars", "GetGenerateCutScalars", QStringList() << "int", QStringList(), "void", "int");
}

CVtkGenericCutter::CVtkGenericCutter() : m_vtkGenericCutter(0)
{
    CVtkGenericCutter::InitializeObjectDesc();
    setVtkObject(vtkGenericCutter::New());
}

CVtkGenericCutter::~CVtkGenericCutter()
{

}

int CVtkGenericCutter::numberOfContours() const
{
    return m_vtkGenericCutter->GetNumberOfContours();
}


void CVtkGenericCutter::setNumberOfContours(int val)
{
    m_vtkGenericCutter->SetNumberOfContours(val);
}


bool CVtkGenericCutter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        return m_vtkGenericCutter->GetCutFunction() != 0;
    }

    return CVtkPolyDataFilter::hasInput(path);
}

bool CVtkGenericCutter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        IVtkImplicitFunctionIOData* implicitFunctionData = 0;
        bool success = inputData->queryInterface("IVtkImplicitFunctionIOData", (void**)&implicitFunctionData);
        if(success && implicitFunctionData)
        {
            m_vtkGenericCutter->SetCutFunction(implicitFunctionData->getVtkImplicitFunction());
            return true;
        }

        return false;
    }
    return CVtkPolyDataFilter::setInput(path, inputData);
}

bool CVtkGenericCutter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        IVtkImplicitFunctionIOData* implicitFunctionData = 0;
        bool success = inputData->queryInterface("IVtkImplicitFunctionIOData", (void**)&implicitFunctionData);
        if(success && implicitFunctionData && implicitFunctionData->getVtkImplicitFunction() == m_vtkGenericCutter->GetCutFunction())
        {
            m_vtkGenericCutter->SetCutFunction(0);
            return true;
        }

        return false;
    }
    return CVtkPolyDataFilter::removeInput(path, inputData);
}


bool CVtkGenericCutter::isGenerateCutScalars() const
{
    return m_vtkGenericCutter->GetGenerateCutScalars();
}


void CVtkGenericCutter::setGenerateCutScalars(bool val)
{
    m_vtkGenericCutter->SetGenerateCutScalars(val);
}

