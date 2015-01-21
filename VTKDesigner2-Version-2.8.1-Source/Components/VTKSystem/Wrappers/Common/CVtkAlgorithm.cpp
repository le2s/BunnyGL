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

#include "CVtkAlgorithm.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include "vtkDataSet.h"
#include "vtkPointSet.h"
#include "vtkImageData.h"
#include "vtkPolyData.h"

#include "CScriptableVtkDataObject.h"
#include "CScriptableVtkDataSet.h"
#include "CScriptableVtkPointSet.h"
#include "CScriptableVtkImageData.h"
#include "CScriptableVtkPolyData.h"

DEFINE_VTK_OBJECT(CVtkAlgorithm, CVtkObject, vtkAlgorithm)
{
    pDesc->setNodeClassName("vtkAlgorithm");
    pDesc->setNodeClassCategory("Algorithm");
    pDesc->setNodeClassDescription("Superclass for all sources, filters, and sinks in VTK.");
    pDesc->setPropertyInfo("HasExecutive", "", "HasExecutive", QStringList(), QStringList(), "void", "int");
    pDesc->setPropertyInfo("AbortExecution", "SetAbortExecute", "GetAbortExecute", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ReleaseDataFlag", "SetReleaseDataFlag", "GetReleaseDataFlag", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setCreateFunction(0);

    // Statistics Output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Algorithm", IVisSystemNodeConnectionPath::OutputPath, "vtkAlgorithm", 0)
        );
}

CVtkAlgorithm::CVtkAlgorithm()
:m_vtkAlgorithm(0)
{

}

CVtkAlgorithm::~CVtkAlgorithm()
{

}

bool CVtkAlgorithm::getHasExecutive() const
{
    if(m_vtkAlgorithm)
        return m_vtkAlgorithm->HasExecutive();
    return false;
}

void CVtkAlgorithm::setAbortExecution(bool val)
{
    if(m_vtkAlgorithm)
        m_vtkAlgorithm->SetAbortExecute(val);
}

bool CVtkAlgorithm::getAbortExecution() const
{
    if(m_vtkAlgorithm)
        return m_vtkAlgorithm->GetAbortExecute();
    return false;
}

void CVtkAlgorithm::setReleaseDataFlag(bool val)
{
    if(m_vtkAlgorithm)
        m_vtkAlgorithm->SetReleaseDataFlag(val);
}

bool CVtkAlgorithm::getReleaseDataFlag() const
{
    if(m_vtkAlgorithm)
        return m_vtkAlgorithm->GetReleaseDataFlag();
    return false;
}

void CVtkAlgorithm::markModified()
{
    return m_vtkAlgorithm->Modified();
}

bool CVtkAlgorithm::isModified() const
{
    return false; // need to correct this.
}

void CVtkAlgorithm::update()
{
    command_Update();
}

void CVtkAlgorithm::forceUpdate()
{
    command_ForceUpdate();
}

int CVtkAlgorithm::getNumberOfOutputPorts() const
{
    if(m_vtkAlgorithm)
        return m_vtkAlgorithm->GetNumberOfOutputPorts();
    return 0;
}

QString CVtkAlgorithm::getOutputDataObjectType(int index) const
{
    if(!m_vtkAlgorithm)
        return QString();

    vtkDataObject* outDataObj = m_vtkAlgorithm->GetOutputDataObject(index);
    if(!outDataObj)
        return QString();

    switch(outDataObj->GetDataObjectType())
    {
    case VTK_POLY_DATA: return "PolyData";
    case VTK_STRUCTURED_POINTS: return "StructuredPoints";
    case VTK_STRUCTURED_GRID: return "StructuredGrid";
    case VTK_RECTILINEAR_GRID: return "RectilinearGrid"; 
    case VTK_UNSTRUCTURED_GRID: return "UnstructuredGrid"; 
    case VTK_PIECEWISE_FUNCTION: return "PiecewiseFunction";
    case VTK_IMAGE_DATA: return "ImageData";
    case VTK_DATA_OBJECT: return "DataObject";
    case VTK_DATA_SET: return "DataSet";
    case VTK_POINT_SET: return "PointSet";
    case VTK_UNIFORM_GRID: return "UniformGrid";
    case VTK_COMPOSITE_DATA_SET: return "CompositeDataSet";
    case VTK_HIERARCHICAL_DATA_SET: return "HierarchicalDataSet";
    case VTK_HIERARCHICAL_BOX_DATA_SET: return "HierarchicalBoxDataSet";
    case VTK_GENERIC_DATA_SET: return "GenericDataSet";
    default: break;
    }

    return "UnknownDataObject";
}

QObject* CVtkAlgorithm::getOutputDataObject(int index) const
{
    if(!m_vtkAlgorithm)
        return 0;

    if(m_outputDataObjectMap.contains(index))
        return m_outputDataObjectMap[index];

    vtkDataObject* outDataObj = m_vtkAlgorithm->GetOutputDataObject(index);
    QObject* retObj = 0;
    if(!outDataObj)
        return 0;

    CVtkAlgorithm* ncThis = const_cast<CVtkAlgorithm*>(this);

    if(vtkPolyData::SafeDownCast(outDataObj))
        retObj = new CScriptableVtkPolyData(vtkPolyData::SafeDownCast(outDataObj), ncThis);
    else if(vtkImageData::SafeDownCast(outDataObj))
        retObj = new CScriptableVtkImageData(vtkImageData::SafeDownCast(outDataObj), ncThis);
    else if(vtkPointSet::SafeDownCast(outDataObj))
        retObj = new CScriptableVtkPointSet(vtkPointSet::SafeDownCast(outDataObj), ncThis);
    else if(vtkDataSet::SafeDownCast(outDataObj))
        retObj = new CScriptableVtkDataSet(vtkDataSet::SafeDownCast(outDataObj), ncThis);
    else 
        retObj = new CScriptableVtkDataObject(outDataObj, ncThis);

    if(retObj)
    {
        m_outputDataObjectMap[index] = retObj;
        retObj->setParent( ncThis );
    }

    return retObj;
}

void CVtkAlgorithm::command_Update()
{
    m_vtkAlgorithm->Update();
}

void CVtkAlgorithm::command_ForceUpdate()
{
    m_vtkAlgorithm->Modified();
    m_vtkAlgorithm->Update();
}

bool CVtkAlgorithm::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Algorithm")
    {
        m_algorithmData.setAlgorithm(m_vtkAlgorithm);
        *outputData = &m_algorithmData;
        return true;
    }

    return CVtkObject::fetchOutput(path, outputData);
}

bool CVtkAlgorithm::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    return CVtkObject::outputDerefed(path, outputData);
}



