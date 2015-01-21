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

#ifndef C_VTK_GENERIC_GLYPH3_D_FILTER_H
#define C_VTK_GENERIC_GLYPH3_D_FILTER_H

#include "vtkGenericGlyph3DFilter.h"
#include "CVtkPolyDataFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkGenericGlyph3DFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(ScaleMode)
    Q_ENUMS(ColorMode)
    Q_ENUMS(VectorMode)
    Q_ENUMS(IndexMode)
    Q_PROPERTY(bool Scaling READ isScaling WRITE setScaling)
    Q_PROPERTY(ScaleMode ScaleMode READ scaleMode WRITE setScaleMode)
    Q_PROPERTY(ColorMode ColorMode READ colorMode WRITE setColorMode)
    Q_PROPERTY(double ScaleFactor READ scaleFactor WRITE setScaleFactor)
    Q_PROPERTY(ValueRange Range READ range WRITE setRange)
    Q_PROPERTY(bool Orient READ isOrient WRITE setOrient)
    Q_PROPERTY(bool Clamping READ isClamping WRITE setClamping)
    Q_PROPERTY(VectorMode VectorMode READ vectorMode WRITE setVectorMode)
    Q_PROPERTY(IndexMode IndexMode READ indexMode WRITE setIndexMode)
    Q_PROPERTY(bool GeneratePointIds READ isGeneratePointIds WRITE setGeneratePointIds)
    Q_PROPERTY(QString PointIdsName READ pointIdsName WRITE setPointIdsName)
    DECLARE_VTK_OBJECT

public:
    enum ScaleMode
    {
        SScalar=VTK_SCALE_BY_SCALAR,
        SVector=VTK_SCALE_BY_VECTOR,
        SVectorComponents=VTK_SCALE_BY_VECTORCOMPONENTS,
        SOff=VTK_DATA_SCALING_OFF
    };
    enum ColorMode
    {
        CScale=VTK_COLOR_BY_SCALE,
        CScalar=VTK_COLOR_BY_SCALAR,
        Vector=VTK_COLOR_BY_VECTOR
    };
    enum VectorMode
    {
        VVector=VTK_USE_VECTOR,
        VNormal=VTK_USE_NORMAL,
        VRotationOff=VTK_VECTOR_ROTATION_OFF
    };
    enum IndexMode
    {
        IOff=VTK_INDEXING_OFF,
        IScalar=VTK_INDEXING_BY_SCALAR,
        IVector=VTK_INDEXING_BY_VECTOR
    };

    CVtkGenericGlyph3DFilter();
    ~CVtkGenericGlyph3DFilter();

    bool isScaling() const;
    void setScaling(bool val);

    ScaleMode scaleMode() const;
    void setScaleMode(ScaleMode val);

    ColorMode colorMode() const;
    void setColorMode(ColorMode val);

    double scaleFactor() const;
    void setScaleFactor(double val);

    ValueRange range() const;
    void setRange(ValueRange val);

    bool isOrient() const;
    void setOrient(bool val);

    bool isClamping() const;
    void setClamping(bool val);

    VectorMode vectorMode() const;
    void setVectorMode(VectorMode val);

    IndexMode indexMode() const;
    void setIndexMode(IndexMode val);

    bool isGeneratePointIds() const;
    void setGeneratePointIds(bool val);

    QString pointIdsName() const;
    void setPointIdsName(QString val);


private:
    vtkGenericGlyph3DFilter *m_vtkGenericGlyph3DFilter;
};
#endif//C_VTK_GENERIC_GLYPH3_D_FILTER_H