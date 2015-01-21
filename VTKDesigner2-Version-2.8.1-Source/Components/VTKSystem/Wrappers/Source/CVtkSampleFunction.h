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
#ifndef C_VTK_SAMPLE_FUNCTION_H
#define C_VTK_SAMPLE_FUNCTION_H

#include "vtkSampleFunction.h"
#include "CVtkImageAlgorithm.h"
#include "UserDefinedDataTypes.h"

class CVtkSampleFunction : public CVtkImageAlgorithm
{
    Q_OBJECT
    Q_ENUMS(OutputScalarType)
    Q_PROPERTY(OutputScalarType OutputScalarType READ outputScalarType WRITE setOutputScalarType)
    Q_PROPERTY(QList<QVariant> ModelBounds READ modelBounds WRITE setModelBounds)
    Q_PROPERTY(bool Capping READ isCapping WRITE setCapping)
    Q_PROPERTY(double CapValue READ capValue WRITE setCapValue)
    Q_PROPERTY(bool ComputeNormals READ isComputeNormals WRITE setComputeNormals)
    Q_PROPERTY(QList<QVariant> SampleDimensions READ sampleDimensions WRITE setSampleDimensions)
    DECLARE_VTK_OBJECT

public:
    enum OutputScalarType
    {
        Double=VTK_DOUBLE,
        Float=VTK_FLOAT,
        Long=VTK_LONG,
        UnsignedLong=VTK_UNSIGNED_LONG,
        Int=VTK_INT,
        UnsignedInt=VTK_UNSIGNED_INT,
        Short=VTK_SHORT,
        UnsignedShort=VTK_UNSIGNED_SHORT,
        Char=VTK_CHAR,
        UnsignedChar=VTK_UNSIGNED_CHAR
    };

    CVtkSampleFunction();
    ~CVtkSampleFunction();

    OutputScalarType outputScalarType() const;
    void setOutputScalarType(OutputScalarType val);

    QList<QVariant> modelBounds() const;
    void setModelBounds(QList<QVariant> val);

    bool isCapping() const;
    void setCapping(bool val);

    double capValue() const;
    void setCapValue(double val);

    bool isComputeNormals() const;
    void setComputeNormals(bool val);

    QList<QVariant> sampleDimensions() const;
    void setSampleDimensions(QList<QVariant> val);

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkSampleFunction *m_vtkSampleFunction;
};
#endif//C_VTK_SAMPLE_FUNCTION_H