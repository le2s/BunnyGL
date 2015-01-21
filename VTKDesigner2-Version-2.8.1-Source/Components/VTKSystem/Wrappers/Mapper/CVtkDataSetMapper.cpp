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

#include "CVtkDataSetMapper.h"
#include "IVtkDataSetIOData.h"
#include "vtkDataSetWriter.h"
#include "vtkDataSet.h"
#include "IVtkScalarsToColorsIOData.h"

DEFINE_VTK_OBJECT(CVtkDataSetMapper, CVtkAlgorithm, vtkDataSetMapper)
{
    pDesc->setNodeClassName("vtkDataSetMapper");
    pDesc->setNodeClassCategory("Rendering");
    pDesc->setNodeClassDescription("Maps any vtkDataSet");
    pDesc->setPropertyInfo("ShowScalars", "setShowScalars", "isShowScalars", QStringList() << "bool", QStringList(), "void", "bool");

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("DataSet", IVisSystemNodeConnectionPath::InputPath, "vtkDataSet", 0)
        );

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("DataSetMapper", IVisSystemNodeConnectionPath::OutputPath, "vtkDataSetMapper", 0)
        );

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ColorTable", IVisSystemNodeConnectionPath::InputPath, "vtkScalarsToColors", 0)
        );
}

CVtkDataSetMapper::CVtkDataSetMapper() : m_vtkDataSetMapper(0)
{
    CVtkDataSetMapper::InitializeObjectDesc();
    setVtkObject(vtkDataSetMapper::New());

    m_vtkDataSetMapper->SetScalarMaterialModeToAmbientAndDiffuse();
    m_hasLookupTable = 0;
}

CVtkDataSetMapper::~CVtkDataSetMapper()
{

}

void CVtkDataSetMapper::setShowScalars(bool val)
{
    if(val == isShowScalars())
        return;

    m_vtkDataSetMapper->SetScalarVisibility(val);
    // m_vtkDataSetMapper->SetColorModeToMapScalars();
}

bool CVtkDataSetMapper::isShowScalars() const
{
    return m_vtkDataSetMapper->GetScalarVisibility();
}

void CVtkDataSetMapper::command_ExportData()
{
    if(!m_vtkDataSetMapper->GetInput())
    {
        qDebug("Cannot export data because there is no input");
        return;
    }

    vtkDataSetWriter* dsWriter = vtkDataSetWriter::New();
    dsWriter->SetFileName("E:\\dataset.vtk");
    dsWriter->SetInput(m_vtkDataSetMapper->GetInput());
    dsWriter->Write();
    dsWriter->Delete();
    qDebug("Exported data into E:\\dataset.vtk");
}

bool CVtkDataSetMapper::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "DataSet")
        return m_vtkDataSetMapper->GetInput() != 0;

    if(path->pathName() == "ColorTable")
        return m_hasLookupTable;

    return CVtkAlgorithm::hasInput(path);
}

bool CVtkDataSetMapper::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "DataSet")
    {
        IVtkDataSetIOData* polyData = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&polyData);
        if(success && polyData)
        {
            m_vtkDataSetMapper->SetInput(polyData->getVtkDataSet());
            return true;
        }

        return false;
    }

    if(path->pathName() == "ColorTable")
    {
        IVtkScalarsToColorsIOData* s2cData = 0;
        bool success = inputData->queryInterface("IVtkScalarsToColorsIOData", (void**)&s2cData);
        if(success && s2cData)
        {
            m_vtkDataSetMapper->SetLookupTable(s2cData->getVtkScalarsToColors());
            m_hasLookupTable = true;
            return true;
        }

        m_hasLookupTable = false;
        return false;
    }

    return CVtkAlgorithm::setInput(path, inputData);
}

bool CVtkDataSetMapper::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "DataSet")
    {
        IVtkDataSetIOData* polyData = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&polyData);
        if(success && polyData && m_vtkDataSetMapper->GetInput() == polyData->getVtkDataSet())
        {
            m_vtkDataSetMapper->SetInput(0);
            return true;
        }

        return false;
    }

    if(path->pathName() == "ColorTable")
    {
        IVtkScalarsToColorsIOData* s2cData = 0;
        bool success = inputData->queryInterface("IVtkScalarsToColorsIOData", (void**)&s2cData);
        if(success && s2cData && m_vtkDataSetMapper->GetLookupTable() == s2cData->getVtkScalarsToColors())
        {
            m_vtkDataSetMapper->SetLookupTable(0);
            m_hasLookupTable = false;
            return true;
        }

        return false;
    }

    return CVtkAlgorithm::removeInput(path, inputData);
}

bool CVtkDataSetMapper::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "DataSetMapper")
    {
        m_vtkDataSetMapperOutput.setDataSetMapper(m_vtkDataSetMapper);
        *outputData = &m_vtkDataSetMapperOutput;
        return true;
    }

    return false;
}

bool CVtkDataSetMapper::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    Q_UNUSED(path);
    Q_UNUSED(outputData);
    return true;
}


