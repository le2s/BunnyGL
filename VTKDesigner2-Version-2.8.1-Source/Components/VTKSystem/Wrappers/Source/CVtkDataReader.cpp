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

#include "CVtkDataReader.h"
#include "CVtkVisSystemComponent.h"

DEFINE_VTK_OBJECT(CVtkDataReader,CVtkAlgorithm,vtkDataReader)
{
    pDesc->setNodeClassName("vtkDataReader");
    pDesc->setNodeClassDescription("Helper superclass for objects that read vtk data files");
    pDesc->setNodeClassCategory("Readers");

    pDesc->setPropertyInfo("FileName", "SetFileName", "GetFileName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("InputString", "SetInputString", "GetInputString", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("ReadFromInputString", "SetReadFromInputString", "GetReadFromInputString", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ScalarsName", "SetScalarsName", "GetScalarsName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("VectorsName", "SetVectorsName", "GetVectorsName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("TensorsName", "SetTensorsName", "GetTensorsName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("NormalsName", "SetNormalsName", "GetNormalsName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("TCoordsName", "SetTCoordsName", "GetTCoordsName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("LookupTableName", "SetLookupTableName", "GetLookupTableName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("FieldDataName", "SetFieldDataName", "GetFieldDataName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("ReadAllScalars", "SetReadAllScalars", "GetReadAllScalars", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ReadAllVectors", "SetReadAllVectors", "GetReadAllVectors", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ReadAllNormals", "SetReadAllNormals", "GetReadAllNormals", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ReadAllTensors", "SetReadAllTensors", "GetReadAllTensors", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ReadAllColorScalars", "SetReadAllColorScalars", "GetReadAllColorScalars", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ReadAllTCoords", "SetReadAllTCoords", "GetReadAllTCoords", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ReadAllFields", "SetReadAllFields", "GetReadAllFields", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setCreateFunction(0);

    // data object 
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ReaderOutput", IVisSystemNodeConnectionPath::OutputPath, "vtkDataObject", 0)
        );
}

CVtkDataReader::CVtkDataReader() : m_vtkDataReader(0)
{
    CVtkDataReader::InitializeObjectDesc();
}

CVtkDataReader::~CVtkDataReader()
{

}

bool CVtkDataReader::isFileStructuredPoints() const
{
    return m_vtkDataReader->IsFileStructuredPoints();
}

bool CVtkDataReader::isFilePolyData() const
{
    return m_vtkDataReader->IsFilePolyData();
}

bool CVtkDataReader::isFileStructuredGrid() const
{
    return m_vtkDataReader->IsFileStructuredGrid();
}

bool CVtkDataReader::isFileUnstructuredGrid() const
{
    return m_vtkDataReader->IsFileUnstructuredGrid();
}

bool CVtkDataReader::isFileRectilinearGrid() const
{
    return m_vtkDataReader->IsFileRectilinearGrid();
}

QString CVtkDataReader::fileName() const
{
    return m_fileName;
}


void CVtkDataReader::setFileName(QString val)
{
    m_fileName = val;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_fileName);
    m_vtkDataReader->SetFileName(qPrintable(fn));

    qDebug("%s: %s", Q_FUNC_INFO, qPrintable(fn));
}

QString CVtkDataReader::scalarsName() const
{
    return m_vtkDataReader->GetScalarsName();
}


void CVtkDataReader::setScalarsName(QString val)
{
    m_vtkDataReader->SetScalarsName(qPrintable(val));
}


QString CVtkDataReader::vectorsName() const
{
    return m_vtkDataReader->GetVectorsName();
}


void CVtkDataReader::setVectorsName(QString val)
{
    m_vtkDataReader->SetVectorsName(qPrintable(val));
}


QString CVtkDataReader::tensorsName() const
{
    return m_vtkDataReader->GetTensorsName();
}


void CVtkDataReader::setTensorsName(QString val)
{
    m_vtkDataReader->SetTensorsName(qPrintable(val));
}


QString CVtkDataReader::normalsName() const
{
    return m_vtkDataReader->GetNormalsName();
}


void CVtkDataReader::setNormalsName(QString val)
{
    m_vtkDataReader->SetNormalsName(qPrintable(val));
}


QString CVtkDataReader::tCoordsName() const
{
    return m_vtkDataReader->GetTCoordsName();
}


void CVtkDataReader::setTCoordsName(QString val)
{
    m_vtkDataReader->SetTCoordsName(qPrintable(val));
}


QString CVtkDataReader::lookupTableName() const
{
    return m_vtkDataReader->GetLookupTableName();
}


void CVtkDataReader::setLookupTableName(QString val)
{
    m_vtkDataReader->SetLookupTableName(qPrintable(val));
}


QString CVtkDataReader::fieldDataName() const
{
    return m_vtkDataReader->GetFieldDataName();
}


void CVtkDataReader::setFieldDataName(QString val)
{
    m_vtkDataReader->SetFieldDataName(qPrintable(val));
}


bool CVtkDataReader::isReadAllScalars() const
{
    return m_vtkDataReader->GetReadAllScalars();
}


void CVtkDataReader::setReadAllScalars(bool val)
{
    m_vtkDataReader->SetReadAllScalars(val);
}


bool CVtkDataReader::isReadAllVectors() const
{
    return m_vtkDataReader->GetReadAllVectors();
}


void CVtkDataReader::setReadAllVectors(bool val)
{
    m_vtkDataReader->SetReadAllVectors(val);
}


bool CVtkDataReader::isReadAllNormals() const
{
    return m_vtkDataReader->GetReadAllNormals();
}


void CVtkDataReader::setReadAllNormals(bool val)
{
    m_vtkDataReader->SetReadAllNormals(val);
}


bool CVtkDataReader::isReadAllTensors() const
{
    return m_vtkDataReader->GetReadAllTensors();
}


void CVtkDataReader::setReadAllTensors(bool val)
{
    m_vtkDataReader->SetReadAllTensors(val);
}


bool CVtkDataReader::isReadAllColorScalars() const
{
    return m_vtkDataReader->GetReadAllColorScalars();
}


void CVtkDataReader::setReadAllColorScalars(bool val)
{
    m_vtkDataReader->SetReadAllColorScalars(val);
}


bool CVtkDataReader::isReadAllTCoords() const
{
    return m_vtkDataReader->GetReadAllTCoords();
}


void CVtkDataReader::setReadAllTCoords(bool val)
{
    m_vtkDataReader->SetReadAllTCoords(val);
}


bool CVtkDataReader::isReadAllFields() const
{
    return m_vtkDataReader->GetReadAllFields();
}


void CVtkDataReader::setReadAllFields(bool val)
{
    m_vtkDataReader->SetReadAllFields(val);
}


bool CVtkDataReader::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ReaderOutput")
    {
        *outputData = this->readerOutput();
        return true;
    }

    return CVtkAlgorithm::fetchOutput(path, outputData);
}

