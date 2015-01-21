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
#ifndef C_VTK_PARTICLE_READER_H
#define C_VTK_PARTICLE_READER_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkParticleReader.h"

enum FILE_TYPE { FILE_TYPE_IS_UNKNOWN = 0, 
    FILE_TYPE_IS_TEXT, FILE_TYPE_IS_BINARY };

class CVtkParticleReader : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_ENUMS(DataByteOrder)
    Q_ENUMS(FileType)
    Q_ENUMS(DataType)
    Q_PROPERTY(QString FileName READ fileName WRITE setFileName)
    Q_PROPERTY(DataByteOrder DataByteOrder READ dataByteOrder WRITE setDataByteOrder)
    Q_PROPERTY(bool SwapBytes READ isSwapBytes WRITE setSwapBytes)
    Q_PROPERTY(bool HasScalar READ isHasScalar WRITE setHasScalar)
    Q_PROPERTY(FileType FileType READ fileType WRITE setFileType)
    Q_PROPERTY(DataType DataType READ dataType WRITE setDataType)
    DECLARE_VTK_OBJECT

public:
    enum DataByteOrder
    {
        BigEndian = VTK_FILE_BYTE_ORDER_BIG_ENDIAN,
        LittleEndian = VTK_FILE_BYTE_ORDER_LITTLE_ENDIAN
    };
    
    enum FileType
    {
        Unknown = FILE_TYPE_IS_UNKNOWN,
        Text = FILE_TYPE_IS_TEXT,
        Binary = FILE_TYPE_IS_BINARY
    };

    enum DataType
    {
        Float = VTK_FLOAT,
        Double = VTK_DOUBLE
    };

    CVtkParticleReader();
    ~CVtkParticleReader();

    QString fileName() const;
    void setFileName(QString val);

    DataByteOrder dataByteOrder() const;
    void setDataByteOrder(DataByteOrder val);

    bool isSwapBytes() const;
    void setSwapBytes(bool val);

    bool isHasScalar() const;
    void setHasScalar(bool val);

    FileType fileType() const;
    void setFileType(FileType val);

    DataType dataType() const;
    void setDataType(DataType val);


private:
    vtkParticleReader *m_vtkParticleReader;
    QString m_fileName;
};
#endif//C_VTK_PARTICLE_READER_H