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
#ifndef C_VTK_VOLUME16_READER_H
#define C_VTK_VOLUME16_READER_H

#include "vtkVolume16Reader.h"
#include "CVtkVolumeReader.h"
#include "UserDefinedDataTypes.h"

class CVtkVolume16Reader : public CVtkVolumeReader
{
    Q_OBJECT
    Q_ENUMS(DataByteOrder)
    Q_PROPERTY(int DataMask READ dataMask WRITE setDataMask)
    Q_PROPERTY(DataByteOrder DataByteOrder READ dataByteOrder WRITE setDataByteOrder)
    Q_PROPERTY(bool SwapBytes READ isSwapBytes WRITE setSwapBytes)
    Q_PROPERTY(QList<int> DataDimensions READ dataDimensions WRITE setDataDimensions)
    DECLARE_VTK_OBJECT

public:
    enum DataByteOrder
    {
        BigEndian=VTK_FILE_BYTE_ORDER_BIG_ENDIAN,
        LittleEndian=VTK_FILE_BYTE_ORDER_LITTLE_ENDIAN
    };

    CVtkVolume16Reader();
    ~CVtkVolume16Reader();

    int dataMask() const;
    void setDataMask(int val);

    DataByteOrder dataByteOrder() const;
    void setDataByteOrder(DataByteOrder val);

    bool isSwapBytes() const;
    void setSwapBytes(bool val);

    QList<int> dataDimensions() const;
    void setDataDimensions(const QList<int>& dim);

private:
    vtkVolume16Reader *m_vtkVolume16Reader;
};
#endif//C_VTK_VOLUME16_READER_H