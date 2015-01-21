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

#ifndef C_VTK_IMAGE_RESLICE_H
#define C_VTK_IMAGE_RESLICE_H

#include "CVtkThreadedImageAlgorithm.h"
#include "vtkImageReslice.h"
#include "UserDefinedDataTypes.h"

class vtkTransform;
class CVtkImageReslice : public CVtkThreadedImageAlgorithm
{
    Q_OBJECT
    DECLARE_VTK_OBJECT

public:
    CVtkImageReslice();
    ~CVtkImageReslice();

    Q_PROPERTY(bool TransformInputSampling READ isTransformInputSampling WRITE setTransformInputSampling)
    void setTransformInputSampling(int val);
    bool isTransformInputSampling() const;

    Q_PROPERTY(bool AutoCropOutput READ isAutoCropOutput WRITE setAutoCropOutput)
    void setAutoCropOutput(bool val);
    bool isAutoCropOutput() const;

    Q_PROPERTY(bool Wrap READ isWrap WRITE setWrap)
    void setWrap(bool val);
    bool isWrap() const;

    Q_PROPERTY(bool Mirror READ isMirror WRITE setMirror)
    void setMirror(bool val);
    bool isMirror() const;

    Q_PROPERTY(bool Border READ isBorder WRITE setBorder)
    void setBorder(bool val);
    bool isBorder() const;

    enum InterpolationMode
    {
        NearestNeighbor = VTK_RESLICE_NEAREST,
        Linear = VTK_RESLICE_LINEAR,
        Cubic = VTK_RESLICE_CUBIC
    };
    Q_ENUMS(InterpolationMode)
    Q_PROPERTY(InterpolationMode InterpolationMode READ interpolationMode WRITE setInterpolationMode)
    void setInterpolationMode(InterpolationMode mode);
    InterpolationMode interpolationMode() const;

    Q_PROPERTY(bool UseOptimization READ isUseOptimazation WRITE setUseOptimization)
    void setUseOptimization(bool val);
    bool isUseOptimazation() const;

    Q_PROPERTY(QColor BackgroundColor READ backgroundColor WRITE setBackgroundColor)
    void setBackgroundColor(const QColor& color);
    QColor backgroundColor() const;

    Q_PROPERTY(double BackgroundColorAlpha READ backgroundColorAlpha WRITE setBackgroundColorAlpha)
    void setBackgroundColorAlpha(double val);
    double backgroundColorAlpha() const;

    Q_PROPERTY(QList<double> OutputSpacing READ outputSpacing WRITE setOutputSpacing)
    void setOutputSpacing(const QList<double>& val);
    QList<double> outputSpacing() const;

    Q_PROPERTY(Point3D OutputOrigin READ outputOrigin WRITE setOutputOrigin)
    void setOutputOrigin(const Point3D& val);
    Point3D outputOrigin() const;

    Q_PROPERTY(QList<double> OutputExtent READ outputExtent WRITE setOutputExtent)
    void setOutputExtent(const QList<double>& val);
    QList<double> outputExtent() const;

    Q_PROPERTY(int OutputDimensionality READ outputDimensionality WRITE setOutputDimensionality)
    void setOutputDimensionality(int val);
    int outputDimensionality() const;

    Q_PROPERTY(bool Interpolate READ isInterpolate WRITE setInterpolate)
    void setInterpolate(bool val);
    bool isInterpolate() const;

    Q_INVOKABLE void resetTransform();
    Q_INVOKABLE void translate(double x, double y, double z);
    Q_INVOKABLE void rotateX(double x);
    Q_INVOKABLE void rotateY(double y);
    Q_INVOKABLE void rotateZ(double z);
    Q_INVOKABLE void rotateWXYZ(double w, double x, double y, double z);

private:
    vtkImageReslice* m_vtkImageReslice;
    vtkTransform* m_resliceTransform;
};

#endif
