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

#ifndef C_VTK_B_Y_U_READER_H
#define C_VTK_B_Y_U_READER_H

#include "vtkBYUReader.h"
#include "CVtkPolyDataAlgorithm.h"

class CVtkBYUReader : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(QString GeometryFileName READ geometryFileName WRITE setGeometryFileName)
    Q_PROPERTY(QString FileName READ fileName WRITE setFileName)
    Q_PROPERTY(QString DisplacementFileName READ displacementFileName WRITE setDisplacementFileName)
    Q_PROPERTY(QString ScalarFileName READ scalarFileName WRITE setScalarFileName)
    Q_PROPERTY(QString TextureFileName READ textureFileName WRITE setTextureFileName)
    Q_PROPERTY(bool ReadDisplacement READ isReadDisplacement WRITE setReadDisplacement)
    Q_PROPERTY(bool ReadScalar READ isReadScalar WRITE setReadScalar)
    Q_PROPERTY(bool ReadTexture READ isReadTexture WRITE setReadTexture)
    Q_PROPERTY(int PartNumber READ partNumber WRITE setPartNumber)
    DECLARE_VTK_OBJECT

public:

    CVtkBYUReader();
    ~CVtkBYUReader();

    QString geometryFileName() const;
    void setGeometryFileName(QString val);

    QString fileName() const;
    void setFileName(QString val);

    QString displacementFileName() const;
    void setDisplacementFileName(QString val);

    QString scalarFileName() const;
    void setScalarFileName(QString val);

    QString textureFileName() const;
    void setTextureFileName(QString val);

    bool isReadDisplacement() const;
    void setReadDisplacement(bool val);

    bool isReadScalar() const;
    void setReadScalar(bool val);

    bool isReadTexture() const;
    void setReadTexture(bool val);

    int partNumber() const;
    void setPartNumber(int val);


private:
    vtkBYUReader *m_vtkBYUReader;
    QString m_geometryFileName;
    QString m_fileName;
    QString m_displacementFileName;
    QString m_scalarFileName;
    QString m_textureFileName;
};
#endif//C_VTK_B_Y_U_READER_H