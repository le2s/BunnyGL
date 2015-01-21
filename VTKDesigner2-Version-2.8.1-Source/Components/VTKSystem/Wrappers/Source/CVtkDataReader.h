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
#ifndef C_VTK_DATA_READER_H
#define C_VTK_DATA_READER_H

#include "vtkDataReader.h"
#include "CVtkAlgorithm.h"

class CVtkDataReader : public CVtkAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(QString FileName READ fileName WRITE setFileName)
    Q_PROPERTY(QString ScalarsName READ scalarsName WRITE setScalarsName)
    Q_PROPERTY(QString VectorsName READ vectorsName WRITE setVectorsName)
    Q_PROPERTY(QString TensorsName READ tensorsName WRITE setTensorsName)
    Q_PROPERTY(QString NormalsName READ normalsName WRITE setNormalsName)
    Q_PROPERTY(QString TCoordsName READ tCoordsName WRITE setTCoordsName)
    Q_PROPERTY(QString LookupTableName READ lookupTableName WRITE setLookupTableName)
    Q_PROPERTY(QString FieldDataName READ fieldDataName WRITE setFieldDataName)
    Q_PROPERTY(bool ReadAllScalars READ isReadAllScalars WRITE setReadAllScalars)
    Q_PROPERTY(bool ReadAllVectors READ isReadAllVectors WRITE setReadAllVectors)
    Q_PROPERTY(bool ReadAllNormals READ isReadAllNormals WRITE setReadAllNormals)
    Q_PROPERTY(bool ReadAllTensors READ isReadAllTensors WRITE setReadAllTensors)
    Q_PROPERTY(bool ReadAllColorScalars READ isReadAllColorScalars WRITE setReadAllColorScalars)
    Q_PROPERTY(bool ReadAllTCoords READ isReadAllTCoords WRITE setReadAllTCoords)
    Q_PROPERTY(bool ReadAllFields READ isReadAllFields WRITE setReadAllFields)
    DECLARE_VTK_OBJECT

public:
    CVtkDataReader();
    ~CVtkDataReader();

    Q_INVOKABLE bool isFileStructuredPoints() const;
    Q_INVOKABLE bool isFilePolyData() const;
    Q_INVOKABLE bool isFileStructuredGrid() const;
    Q_INVOKABLE bool isFileUnstructuredGrid() const;
    Q_INVOKABLE bool isFileRectilinearGrid() const;

    QString fileName() const;
    void setFileName(QString val);

    QString scalarsName() const;
    void setScalarsName(QString val);

    QString vectorsName() const;
    void setVectorsName(QString val);

    QString tensorsName() const;
    void setTensorsName(QString val);

    QString normalsName() const;
    void setNormalsName(QString val);

    QString tCoordsName() const;
    void setTCoordsName(QString val);

    QString lookupTableName() const;
    void setLookupTableName(QString val);

    QString fieldDataName() const;
    void setFieldDataName(QString val);

    bool isReadAllScalars() const;
    void setReadAllScalars(bool val);

    bool isReadAllVectors() const;
    void setReadAllVectors(bool val);

    bool isReadAllNormals() const;
    void setReadAllNormals(bool val);

    bool isReadAllTensors() const;
    void setReadAllTensors(bool val);

    bool isReadAllColorScalars() const;
    void setReadAllColorScalars(bool val);

    bool isReadAllTCoords() const;
    void setReadAllTCoords(bool val);

    bool isReadAllFields() const;
    void setReadAllFields(bool val);

protected:
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    virtual IVisSystemNodeIOData* readerOutput() { return 0; }

private:
    vtkDataReader *m_vtkDataReader;
    QString m_fileName;
};
#endif//C_VTK_DATA_READER_H