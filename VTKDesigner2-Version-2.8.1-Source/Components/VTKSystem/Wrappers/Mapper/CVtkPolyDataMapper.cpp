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

#include "CVtkPolyDataMapper.h"
#include "IVtkPolyDataIOData.h"
#include "IVtkScalarsToColorsIOData.h"

DEFINE_VTK_OBJECT(CVtkPolyDataMapper, CVtkAlgorithm, vtkPolyDataMapper)
{
    pDesc->setNodeClassName("vtkPolyDataMapper");
    pDesc->setNodeClassCategory("Rendering");
    pDesc->setNodeClassDescription("Maps any vtkPolyData");
    pDesc->setPropertyInfo("ShowScalars", "setShowScalars", "isShowScalars", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("PolyData", IVisSystemNodeConnectionPath::InputPath, "vtkPolyData", 0)
        );
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("PolyDataMapper", IVisSystemNodeConnectionPath::OutputPath, "vtkPolyDataMapper", 0)
        );
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ColorTable", IVisSystemNodeConnectionPath::InputPath, "vtkScalarsToColors", 0)
        );
}

CVtkPolyDataMapper::CVtkPolyDataMapper() : m_vtkPolyDataMapper(0)
{
    CVtkPolyDataMapper::InitializeObjectDesc();
    setVtkObject(vtkPolyDataMapper::New());
}

CVtkPolyDataMapper::~CVtkPolyDataMapper()
{

}

void CVtkPolyDataMapper::setShowScalars(bool val)
{
    if(val == isShowScalars())
        return;

    m_vtkPolyDataMapper->SetScalarVisibility(val);
    m_vtkPolyDataMapper->SetColorModeToMapScalars();
}

bool CVtkPolyDataMapper::isShowScalars() const
{
    return m_vtkPolyDataMapper->GetScalarVisibility();
}

bool CVtkPolyDataMapper::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "PolyData")
        return m_vtkPolyDataMapper->GetInput() != 0;

    if(path->pathName() == "ColorTable")
        return m_vtkPolyDataMapper->GetLookupTable() != 0;

    return false;
}

bool CVtkPolyDataMapper::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "PolyData")
    {
        IVtkPolyDataIOData* polyData = 0;
        bool success = inputData->queryInterface("IVtkPolyDataIOData", (void**)&polyData);
        if(success && polyData)
        {
            m_vtkPolyDataMapper->SetInput(polyData->getVtkPolyData());
            return true;
        }
    }

    if(path->pathName() == "ColorTable")
    {
        IVtkScalarsToColorsIOData* s2cData = 0;
        bool success = inputData->queryInterface("IVtkScalarsToColorsIOData", (void**)&s2cData);
        if(success && s2cData)
        {
            m_vtkPolyDataMapper->SetLookupTable(s2cData->getVtkScalarsToColors());
            return true;
        }
    }

    return false;
}

bool CVtkPolyDataMapper::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "PolyData")
    {
        IVtkPolyDataIOData* polyData = 0;
        bool success = inputData->queryInterface("IVtkPolyDataIOData", (void**)&polyData);
        if(success && polyData && m_vtkPolyDataMapper->GetInput() == polyData->getVtkPolyData())
        {
            m_vtkPolyDataMapper->SetInput(0);
            return true;
        }
    }

    if(path->pathName() == "ColorTable")
    {
        IVtkScalarsToColorsIOData* s2cData = 0;
        bool success = inputData->queryInterface("IVtkScalarsToColorsIOData", (void**)&s2cData);
        if(success && s2cData && m_vtkPolyDataMapper->GetLookupTable() == s2cData->getVtkScalarsToColors())
        {
            m_vtkPolyDataMapper->SetLookupTable(0);
            return true;
        }
    }

    return false;
}

bool CVtkPolyDataMapper::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "PolyDataMapper")
    {
        m_vtkPolyDataMapperOutput.setPolyDataMapper(m_vtkPolyDataMapper);
        *outputData = &m_vtkPolyDataMapperOutput;
        return true;
    }

    return false;
}

bool CVtkPolyDataMapper::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    Q_UNUSED(path);
    Q_UNUSED(outputData);
    return true;
}


