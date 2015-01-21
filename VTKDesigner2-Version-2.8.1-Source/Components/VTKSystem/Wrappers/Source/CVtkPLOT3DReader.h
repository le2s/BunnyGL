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
#ifndef C_VTK_P_L_O_T3_D_READER_H
#define C_VTK_P_L_O_T3_D_READER_H

#include "vtkPLOT3DReader.h"
#include "CVtkStructuredGridSource.h"

enum      
{ FILE_BIG_ENDIAN = 0, 
  FILE_LITTLE_ENDIAN = 1 
};

class CVtkPLOT3DReader : public CVtkStructuredGridSource
{
    Q_OBJECT
    Q_ENUMS(ByteOrder)
    Q_PROPERTY(QString FileName READ fileName WRITE setFileName)
    Q_PROPERTY(QString XYZFileName READ xYZFileName WRITE setXYZFileName)
    Q_PROPERTY(QString QFileName READ qFileName WRITE setQFileName)
    Q_PROPERTY(bool BinaryFile READ isBinaryFile WRITE setBinaryFile)
    Q_PROPERTY(bool MultiGrid READ isMultiGrid WRITE setMultiGrid)
    Q_PROPERTY(bool HasByteCount READ isHasByteCount WRITE setHasByteCount)
    Q_PROPERTY(bool IBlanking READ isIBlanking WRITE setIBlanking)
    Q_PROPERTY(bool TwoDimensionalGeometry READ isTwoDimensionalGeometry WRITE setTwoDimensionalGeometry)
    Q_PROPERTY(bool ForceRead READ isForceRead WRITE setForceRead)
    Q_PROPERTY(bool DoNotReduceNumberOfOutputs READ isDoNotReduceNumberOfOutputs WRITE setDoNotReduceNumberOfOutputs)
    Q_PROPERTY(ByteOrder ByteOrder READ byteOrder WRITE setByteOrder)
    Q_PROPERTY(double R READ r WRITE setR)
    Q_PROPERTY(double Gamma READ gamma WRITE setGamma)
    Q_PROPERTY(double Uvinf READ uvinf WRITE setUvinf)
    Q_PROPERTY(double Vvinf READ vvinf WRITE setVvinf)
    Q_PROPERTY(double Wvinf READ wvinf WRITE setWvinf)
    Q_PROPERTY(int ScalarFunctionNumber READ scalarFunctionNumber WRITE setScalarFunctionNumber)
    Q_PROPERTY(int VectorFunctionNumber READ vectorFunctionNumber WRITE setVectorFunctionNumber)
    DECLARE_VTK_OBJECT

public:
    enum ByteOrder
    {
        BigEndian =FILE_BIG_ENDIAN,
        LittleEndian =FILE_LITTLE_ENDIAN
    };

    CVtkPLOT3DReader();
    ~CVtkPLOT3DReader();

    QString fileName() const;
    void setFileName(QString val);

    QString xYZFileName() const;
    void setXYZFileName(QString val);

    QString qFileName() const;
    void setQFileName(QString val);

    bool isBinaryFile() const;
    void setBinaryFile(bool val);

    bool isMultiGrid() const;
    void setMultiGrid(bool val);

    bool isHasByteCount() const;
    void setHasByteCount(bool val);

    bool isIBlanking() const;
    void setIBlanking(bool val);

    bool isTwoDimensionalGeometry() const;
    void setTwoDimensionalGeometry(bool val);

    bool isForceRead() const;
    void setForceRead(bool val);

    bool isDoNotReduceNumberOfOutputs() const;
    void setDoNotReduceNumberOfOutputs(bool val);

    ByteOrder byteOrder() const;
    void setByteOrder(ByteOrder val);

    double r() const;
    void setR(double val);

    double gamma() const;
    void setGamma(double val);

    double uvinf() const;
    void setUvinf(double val);

    double vvinf() const;
    void setVvinf(double val);

    double wvinf() const;
    void setWvinf(double val);

    int scalarFunctionNumber() const;
    void setScalarFunctionNumber(int val);

    int vectorFunctionNumber() const;
    void setVectorFunctionNumber(int val);


private:
    vtkPLOT3DReader *m_vtkPLOT3DReader;
    QString m_fileName;
    QString m_xyzFileName;
    QString m_qFileName;
};
#endif//C_VTK_P_L_O_T3_D_READER_H