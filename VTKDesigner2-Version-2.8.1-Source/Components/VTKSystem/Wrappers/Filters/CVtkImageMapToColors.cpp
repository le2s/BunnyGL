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

#include "CVtkImageMapToColors.h"
#include "IVtkScalarsToColorsIOData.h"

DEFINE_VTK_OBJECT(CVtkImageMapToColors, CVtkThreadedImageAlgorithm, vtkImageMapToColors)
{
    pDesc->setNodeClassName("vtkImageMapToColors");
    pDesc->setNodeClassCategory("Image Filters");
    pDesc->setNodeClassDescription("map the input image through a lookup table");

    pDesc->setPropertyInfo("ActiveComponent", "SetActiveComponent", "GetActiveComponent", QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("PassAlphaToOutput", "SetPassAlphaToOutput", "GetPassAlphaToOutput", QStringList()<<"bool",QStringList(),"void","bool");
    
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("LookupTable", IVisSystemNodeConnectionPath::InputPath, "vtkScalarsToColors", 0)
        );
}

CVtkImageMapToColors::CVtkImageMapToColors()
{
    CVtkImageMapToColors::InitializeObjectDesc();
}

CVtkImageMapToColors::~CVtkImageMapToColors()
{

}

void CVtkImageMapToColors::setOutputFormat(CVtkImageMapToColors::OutputFormat fmt)
{
    m_vtkImageMapToColors->SetOutputFormat(fmt);
}

CVtkImageMapToColors::OutputFormat CVtkImageMapToColors::outputFormat() const
{
    return CVtkImageMapToColors::OutputFormat(m_vtkImageMapToColors->GetOutputFormat());
}

void CVtkImageMapToColors::setActiveComponent(int val)
{
    m_vtkImageMapToColors->SetActiveComponent(val);
}

int CVtkImageMapToColors::activeComponent() const
{
    return m_vtkImageMapToColors->GetActiveComponent();
}

void CVtkImageMapToColors::setPassAlphaToOutput(bool val)
{
    m_vtkImageMapToColors->SetPassAlphaToOutput(val);
}

bool CVtkImageMapToColors::isPassAlphaToOutput() const
{
    return m_vtkImageMapToColors->GetPassAlphaToOutput();
}

void CVtkImageMapToColors::initializeNode()
{
    setVtkObject(vtkImageMapToColors::New());
}

bool CVtkImageMapToColors::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "LookupTable")
    {
        return m_vtkImageMapToColors->GetLookupTable() != 0;
    }

    return CVtkThreadedImageAlgorithm::hasInput(path);
}

bool CVtkImageMapToColors::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "LookupTable")
    {
        IVtkScalarsToColorsIOData* data = 0;
        bool success = inputData->queryInterface("IVtkScalarsToColorsIOData", (void**)&data);
        if(success && data && data->getVtkScalarsToColors())
        {
            m_vtkImageMapToColors->SetLookupTable(data->getVtkScalarsToColors());
            return true;
        }

        return false;
    }

    return CVtkThreadedImageAlgorithm::setInput(path, inputData);
}

bool CVtkImageMapToColors::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "LookupTable")
    {
        IVtkScalarsToColorsIOData* data = 0;
        bool success = inputData->queryInterface("IVtkScalarsToColorsIOData", (void**)&data);
        if(success && data && data->getVtkScalarsToColors() &&
           m_vtkImageMapToColors->GetLookupTable() == data->getVtkScalarsToColors())
        {
            m_vtkImageMapToColors->SetLookupTable(0);
            return true;
        }

        return false;
    }

    return CVtkThreadedImageAlgorithm::removeInput(path, inputData);
}


