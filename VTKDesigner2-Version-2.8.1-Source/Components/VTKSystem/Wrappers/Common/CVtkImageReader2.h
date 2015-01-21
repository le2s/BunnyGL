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

#ifndef C_VTK_IMAGE_READER2_H
#define C_VTK_IMAGE_READER2_H

#include "CVtkImageAlgorithm.h"
#include "vtkImageReader2.h"

class CScriptableVtkImageData;
class CVtkImageReader2 : public CVtkImageAlgorithm
{
    Q_OBJECT
    Q_ENUMS(DataScalarType)
    Q_PROPERTY(QString FileName READ fileName WRITE setFileName)
    Q_PROPERTY(DataScalarType DataScalarType READ dataScalarType WRITE setDataScalarType)
    Q_PROPERTY(int FileDimensionality READ fileDimensionality WRITE setFileDimensionality)
    DECLARE_VTK_OBJECT

public:
    enum DataScalarType
    {
        Float = VTK_FLOAT,
        Double = VTK_DOUBLE,
        Int = VTK_INT,
        Short = VTK_SHORT,
        UnsignedShort = VTK_UNSIGNED_SHORT,
        UnsignedChar = VTK_UNSIGNED_CHAR
    };

    CVtkImageReader2();
    ~CVtkImageReader2();

    void setFileName(QString fileName);
    QString fileName() const;

    void setDataScalarType(DataScalarType val);
    DataScalarType dataScalarType() const;

    void setFileDimensionality(int dim);
    int fileDimensionality() const;

    // Q_INVOKABLE CScriptableVtkImageData* imageDataObject();
    Q_INVOKABLE QObject* imageDataObject();

private:
    vtkImageReader2* m_vtkImageReader2;
    CScriptableVtkImageData* m_imageDataObject;
    QString m_fileName;
};

#endif

