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

#include "CVtkParticleReader.h"
#include "CVtkVisSystemComponent.h"

DEFINE_VTK_OBJECT(CVtkParticleReader,CVtkPolyDataAlgorithm,vtkParticleReader)
{
    pDesc->setNodeClassName("vtkParticleReader");
    pDesc->setNodeClassDescription("Read ASCII or binary particle data and (optionally) one scalar value associated with each particle");
    pDesc->setNodeClassCategory("PolyDataSource");

    pDesc->setPropertyInfo("FileName", "SetFileName", "GetFileName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("DataByteOrder", "SetDataByteOrder", "GetDataByteOrder", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("SwapBytes", "SetSwapBytes", "GetSwapBytes", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("HasScalar", "SetHasScalar", "GetHasScalar", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("FileType", "SetFileType", "GetFileType", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("DataType", "SetDataType", "GetDataType", QStringList() << "int", QStringList(), "void", "int");
}

CVtkParticleReader::CVtkParticleReader() : m_vtkParticleReader(0)
{
    CVtkParticleReader::InitializeObjectDesc();
    setVtkObject(vtkParticleReader::New());
}

CVtkParticleReader::~CVtkParticleReader()
{

}

QString CVtkParticleReader::fileName() const
{
    return m_fileName;
}


void CVtkParticleReader::setFileName(QString val)
{
    m_fileName = val;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_fileName);
    m_vtkParticleReader->SetFileName(qPrintable(fn));
}


CVtkParticleReader::DataByteOrder CVtkParticleReader::dataByteOrder() const
{
    switch(m_vtkParticleReader->GetDataByteOrder())
    {
        case VTK_FILE_BYTE_ORDER_BIG_ENDIAN:return (BigEndian);
        case VTK_FILE_BYTE_ORDER_LITTLE_ENDIAN:return (LittleEndian);
        default:return (BigEndian);
    }
}

void CVtkParticleReader::setDataByteOrder(DataByteOrder val)
{
    switch(val)
    {
        case BigEndian:
            m_vtkParticleReader->SetDataByteOrder(VTK_FILE_BYTE_ORDER_BIG_ENDIAN);
            break;
        case LittleEndian:
            m_vtkParticleReader->SetDataByteOrder(VTK_FILE_BYTE_ORDER_LITTLE_ENDIAN);
            break;
        default:
            m_vtkParticleReader->SetDataByteOrder(VTK_FILE_BYTE_ORDER_BIG_ENDIAN);
            break;
     }
}


bool CVtkParticleReader::isSwapBytes() const
{
    return m_vtkParticleReader->GetSwapBytes();
}


void CVtkParticleReader::setSwapBytes(bool val)
{
    m_vtkParticleReader->SetSwapBytes(val);
}


bool CVtkParticleReader::isHasScalar() const
{
    return m_vtkParticleReader->GetHasScalar();
}


void CVtkParticleReader::setHasScalar(bool val)
{
    m_vtkParticleReader->SetHasScalar(val);
}


CVtkParticleReader::FileType CVtkParticleReader::fileType() const
{
    switch(m_vtkParticleReader->GetFileType())
    {
        case FILE_TYPE_IS_UNKNOWN:return (Unknown);
        case FILE_TYPE_IS_TEXT:return (Text);
        case FILE_TYPE_IS_BINARY:return (Binary);
        default:return (Unknown);
    }
}

void CVtkParticleReader::setFileType(FileType val)
{
    switch(val)
    {
        case Unknown:
            m_vtkParticleReader->SetFileType(FILE_TYPE_IS_UNKNOWN);
            break;
        case Text:
            m_vtkParticleReader->SetFileType(FILE_TYPE_IS_TEXT);
            break;
        case Binary:
            m_vtkParticleReader->SetFileType(FILE_TYPE_IS_BINARY);
            break;
        default:
            m_vtkParticleReader->SetFileType(FILE_TYPE_IS_UNKNOWN);
            break;
     }
}


CVtkParticleReader::DataType CVtkParticleReader::dataType() const
{
    switch(m_vtkParticleReader->GetDataType())
    {
        case VTK_FLOAT:return (Float);
        case VTK_DOUBLE:return (Double);
        default : return (Float);
    }
}

void CVtkParticleReader::setDataType(DataType val)
{
    switch(val)
    {
        case Float:
            m_vtkParticleReader->SetDataType(VTK_FLOAT);
            break;
        case Double:
            m_vtkParticleReader->SetDataType(VTK_DOUBLE);
            break;
        default:
            m_vtkParticleReader->SetDataType(VTK_FLOAT);
            break;
     }
}

