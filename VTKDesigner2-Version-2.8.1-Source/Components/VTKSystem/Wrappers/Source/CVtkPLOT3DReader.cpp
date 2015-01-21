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

#include "CVtkPLOT3DReader.h"
#include "CVtkVisSystemComponent.h"

DEFINE_VTK_OBJECT(CVtkPLOT3DReader,CVtkStructuredGridSource ,vtkPLOT3DReader)
{
    pDesc->setNodeClassName("vtkPLOT3DReader");
    pDesc->setNodeClassDescription("Read PLOT3D data files");
    pDesc->setNodeClassCategory("Structured Grid Source");

    pDesc->setPropertyInfo("FileName", "SetFileName", "GetFileName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("XYZFileName", "SetXYZFileName", "GetXYZFileName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("QFileName", "SetQFileName", "GetQFileName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("BinaryFile", "SetBinaryFile", "GetBinaryFile", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("MultiGrid", "SetMultiGrid", "GetMultiGrid", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("HasByteCount", "SetHasByteCount", "GetHasByteCount", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("IBlanking", "SetIBlanking", "GetIBlanking", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("TwoDimensionalGeometry", "SetTwoDimensionalGeometry", "GetTwoDimensionalGeometry", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ForceRead", "SetForceRead", "GetForceRead", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("DoNotReduceNumberOfOutputs", "SetDoNotReduceNumberOfOutputs", "GetDoNotReduceNumberOfOutputs", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ByteOrder", "SetByteOrder", "GetByteOrder", QStringList() << "ByteOrder", QStringList(), "void", "ByteOrder");
    pDesc->setPropertyInfo("R", "SetR", "GetR", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Gamma", "SetGamma", "GetGamma", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Uvinf", "SetUvinf", "GetUvinf", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Vvinf", "SetVvinf", "GetVvinf", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Wvinf", "SetWvinf", "GetWvinf", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("ScalarFunctionNumber", "SetScalarFunctionNumber", "GetScalarFunctionNumber", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("VectorFunctionNumber", "SetVectorFunctionNumber", "GetVectorFunctionNumber", QStringList() << "int", QStringList(), "void", "int");
}

CVtkPLOT3DReader::CVtkPLOT3DReader() : m_vtkPLOT3DReader(0)
{
    CVtkPLOT3DReader::InitializeObjectDesc();
    setVtkObject(vtkPLOT3DReader::New());
}

CVtkPLOT3DReader::~CVtkPLOT3DReader()
{

}

QString CVtkPLOT3DReader::fileName() const
{
    return m_fileName;
}


void CVtkPLOT3DReader::setFileName(QString val)
{
    m_fileName = val;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_fileName);
    m_vtkPLOT3DReader->SetFileName(qPrintable(fn));
}


QString CVtkPLOT3DReader::xYZFileName() const
{
    return m_xyzFileName;
}


void CVtkPLOT3DReader::setXYZFileName(QString val)
{
    m_xyzFileName = val;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_xyzFileName);
    m_vtkPLOT3DReader->SetXYZFileName(qPrintable(fn));
}


QString CVtkPLOT3DReader::qFileName() const
{
    return m_qFileName;
}


void CVtkPLOT3DReader::setQFileName(QString val)
{
    m_qFileName = val;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_qFileName);
    m_vtkPLOT3DReader->SetQFileName(qPrintable(fn));
}


bool CVtkPLOT3DReader::isBinaryFile() const
{
    return m_vtkPLOT3DReader->GetBinaryFile();
}


void CVtkPLOT3DReader::setBinaryFile(bool val)
{
    m_vtkPLOT3DReader->SetBinaryFile(val);
}


bool CVtkPLOT3DReader::isMultiGrid() const
{
    return m_vtkPLOT3DReader->GetMultiGrid();
}


void CVtkPLOT3DReader::setMultiGrid(bool val)
{
    m_vtkPLOT3DReader->SetMultiGrid(val);
}


bool CVtkPLOT3DReader::isHasByteCount() const
{
    return m_vtkPLOT3DReader->GetHasByteCount();
}


void CVtkPLOT3DReader::setHasByteCount(bool val)
{
    m_vtkPLOT3DReader->SetHasByteCount(val);
}


bool CVtkPLOT3DReader::isIBlanking() const
{
    return m_vtkPLOT3DReader->GetIBlanking();
}


void CVtkPLOT3DReader::setIBlanking(bool val)
{
    m_vtkPLOT3DReader->SetIBlanking(val);
}


bool CVtkPLOT3DReader::isTwoDimensionalGeometry() const
{
    return m_vtkPLOT3DReader->GetTwoDimensionalGeometry();
}


void CVtkPLOT3DReader::setTwoDimensionalGeometry(bool val)
{
    m_vtkPLOT3DReader->SetTwoDimensionalGeometry(val);
}


bool CVtkPLOT3DReader::isForceRead() const
{
    return m_vtkPLOT3DReader->GetForceRead();
}


void CVtkPLOT3DReader::setForceRead(bool val)
{
    m_vtkPLOT3DReader->SetForceRead(val);
}


bool CVtkPLOT3DReader::isDoNotReduceNumberOfOutputs() const
{
    return m_vtkPLOT3DReader->GetDoNotReduceNumberOfOutputs();
}


void CVtkPLOT3DReader::setDoNotReduceNumberOfOutputs(bool val)
{
    m_vtkPLOT3DReader->SetDoNotReduceNumberOfOutputs(val);
}


CVtkPLOT3DReader::ByteOrder CVtkPLOT3DReader::byteOrder() const
{
    switch(m_vtkPLOT3DReader->GetByteOrder())
    {
        case FILE_BIG_ENDIAN:
            return BigEndian;
        
        case FILE_LITTLE_ENDIAN:
            return LittleEndian;
       
        default:
            return BigEndian;
    }
}


void CVtkPLOT3DReader::setByteOrder(CVtkPLOT3DReader::ByteOrder val)
{
    switch(val)
    {
        case BigEndian:
            m_vtkPLOT3DReader->SetByteOrder(FILE_BIG_ENDIAN);
            break;
        
        case LittleEndian:
            m_vtkPLOT3DReader->SetByteOrder(FILE_LITTLE_ENDIAN);
            break;
       
        default:
            m_vtkPLOT3DReader->SetByteOrder(FILE_BIG_ENDIAN);
            break;
    }
}


double CVtkPLOT3DReader::r() const
{
    return m_vtkPLOT3DReader->GetR();
}


void CVtkPLOT3DReader::setR(double val)
{
    m_vtkPLOT3DReader->SetR(val);
}


double CVtkPLOT3DReader::gamma() const
{
    return m_vtkPLOT3DReader->GetGamma();
}


void CVtkPLOT3DReader::setGamma(double val)
{
    m_vtkPLOT3DReader->SetGamma(val);
}


double CVtkPLOT3DReader::uvinf() const
{
    return m_vtkPLOT3DReader->GetUvinf();
}


void CVtkPLOT3DReader::setUvinf(double val)
{
    m_vtkPLOT3DReader->SetUvinf(val);
}


double CVtkPLOT3DReader::vvinf() const
{
    return m_vtkPLOT3DReader->GetVvinf();
}


void CVtkPLOT3DReader::setVvinf(double val)
{
    m_vtkPLOT3DReader->SetVvinf(val);
}


double CVtkPLOT3DReader::wvinf() const
{
    return m_vtkPLOT3DReader->GetWvinf();
}


void CVtkPLOT3DReader::setWvinf(double val)
{
    m_vtkPLOT3DReader->SetWvinf(val);
}


int CVtkPLOT3DReader::scalarFunctionNumber() const
{
    return m_vtkPLOT3DReader->GetScalarFunctionNumber();
}


void CVtkPLOT3DReader::setScalarFunctionNumber(int val)
{
    m_vtkPLOT3DReader->SetScalarFunctionNumber(val);
}


int CVtkPLOT3DReader::vectorFunctionNumber() const
{
    return m_vtkPLOT3DReader->GetVectorFunctionNumber();
}


void CVtkPLOT3DReader::setVectorFunctionNumber(int val)
{
    m_vtkPLOT3DReader->SetVectorFunctionNumber(val);
}

