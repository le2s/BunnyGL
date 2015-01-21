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
#include "CVtkVolume16Reader.h"

DEFINE_VTK_OBJECT(CVtkVolume16Reader,CVtkVolumeReader,vtkVolume16Reader)
{
    pDesc->setNodeClassName("vtkVolume16Reader");
    pDesc->setNodeClassDescription("read 16 bit image files");
    pDesc->setNodeClassCategory("PolyDataSource");

    pDesc->setPropertyInfo("DataMask", "SetDataMask", "GetDataMask", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("DataByteOrder", "SetDataByteOrder", "GetDataByteOrder", QStringList() << "DataByteOrder", QStringList(), "void", "DataByteOrder");
    pDesc->setPropertyInfo("SwapBytes", "SetSwapBytes", "GetSwapBytes", QStringList() << "int" , QStringList(), "void", "int");
}

CVtkVolume16Reader::CVtkVolume16Reader() : m_vtkVolume16Reader(0)
{
    CVtkVolume16Reader::InitializeObjectDesc();
    setVtkObject(vtkVolume16Reader::New());
}

CVtkVolume16Reader::~CVtkVolume16Reader()
{

}


int CVtkVolume16Reader::dataMask() const
{
    return m_vtkVolume16Reader->GetDataMask();
}


void CVtkVolume16Reader::setDataMask(int val)
{
    m_vtkVolume16Reader->SetDataMask(val);
}


CVtkVolume16Reader::DataByteOrder CVtkVolume16Reader::dataByteOrder() const
{
    switch(m_vtkVolume16Reader->GetDataByteOrder())
    {
        case VTK_FILE_BYTE_ORDER_BIG_ENDIAN:
            return BigEndian;

        case VTK_FILE_BYTE_ORDER_LITTLE_ENDIAN:
            return LittleEndian;

        default:
            return BigEndian;
    }
}


void CVtkVolume16Reader::setDataByteOrder(CVtkVolume16Reader::DataByteOrder val)
{
    switch(val)
    {
        case BigEndian:
            m_vtkVolume16Reader->SetDataByteOrder(VTK_FILE_BYTE_ORDER_BIG_ENDIAN);
            break;

        case LittleEndian:
            m_vtkVolume16Reader->SetDataByteOrder(VTK_FILE_BYTE_ORDER_LITTLE_ENDIAN);
            break;

        default:
            m_vtkVolume16Reader->SetDataByteOrder(VTK_FILE_BYTE_ORDER_BIG_ENDIAN);
            break;
    }
}


bool CVtkVolume16Reader::isSwapBytes() const
{
    return m_vtkVolume16Reader->GetSwapBytes();
}


void CVtkVolume16Reader::setSwapBytes(bool val)
{
    m_vtkVolume16Reader->SetSwapBytes(val);
}

QList<int> CVtkVolume16Reader::dataDimensions() const
{
    int* val = 0;
    QList<int> list;
    val = m_vtkVolume16Reader->GetDataDimensions();
    if(val)
    {
        for(int i=0; i<2; i++)
            list.append(val[i]);
    }
    return list;
}

void CVtkVolume16Reader::setDataDimensions(const QList<int>& dim)
{
    QVector<int> dimInt = dim.toVector();
    m_vtkVolume16Reader->SetDataDimensions(dimInt.data());
}

