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
#ifndef C_VTK_GLYPH3_D_H
#define C_VTK_GLYPH3_D_H

#include "vtkGlyph3D.h"
#include "CVtkPolyDataFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkGlyph3D : public CVtkPolyDataFilter
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
        Scalar=VTK_SCALE_BY_SCALAR,
        Vector=VTK_SCALE_BY_VECTOR,
        VectorComponents=VTK_SCALE_BY_VECTORCOMPONENTS,
        ScalingOff=VTK_DATA_SCALING_OFF
    };
    enum ColorMode
    {
        Scale=VTK_COLOR_BY_SCALE,
        CScalar=VTK_COLOR_BY_SCALAR,
        Vector3D=VTK_COLOR_BY_VECTOR
    };
    enum VectorMode
    {
        VVector=VTK_USE_VECTOR,
        Normal=VTK_USE_NORMAL,
        VectorRotationOff=VTK_VECTOR_ROTATION_OFF
    };
    enum IndexMode
    {
        Off=VTK_INDEXING_OFF,
        IScalar=VTK_INDEXING_BY_SCALAR,
        IVector=VTK_INDEXING_BY_VECTOR
    };

    CVtkGlyph3D();
    ~CVtkGlyph3D();

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

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkGlyph3D *m_vtkGlyph3D;
    vtkPolyData* m_emptyPolyData;
};
#endif//C_VTK_GLYPH3_D_H