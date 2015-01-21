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

#ifndef C_VTK_IMAGE_TO_POLYDATA_FILTER_H
#define C_VTK_IMAGE_TO_POLYDATA_FILTER_H

#include "CVtkPolyDataFilter.h"
#include "vtkImageToPolyDataFilter.h"

class CVtkImageToPolyDataFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(OutputStyle)
    Q_ENUMS(ColorMode)
    Q_PROPERTY(OutputStyle OutputStyle READ outputStyle WRITE setOutputStyle)
    Q_PROPERTY(ColorMode ColorMode READ colorMode WRITE setColorMode)
    Q_PROPERTY(bool Smoothing READ isSmoothing WRITE setSmoothing)
    Q_PROPERTY(int NumberOfSmoothingIterations READ numberOfSmoothingIterations WRITE setNumberOfSmoothingIterations)
    Q_PROPERTY(bool Decimation READ isDecimation WRITE setDecimation)
    Q_PROPERTY(double DecimationError READ decimationError WRITE setDecimationError)
    Q_PROPERTY(int Error READ error WRITE setError)
    Q_PROPERTY(int SubImageSize READ subImageSize WRITE setSubImageSize)
    DECLARE_VTK_OBJECT

public:
    CVtkImageToPolyDataFilter();
    ~CVtkImageToPolyDataFilter();

    enum OutputStyle
    {
        Pixelize = VTK_STYLE_PIXELIZE,
        Polygonalize = VTK_STYLE_POLYGONALIZE,
        RunLength = VTK_STYLE_RUN_LENGTH
    };

    enum ColorMode
    {
        Lut = VTK_COLOR_MODE_LUT,
        Linear256 = VTK_COLOR_MODE_LINEAR_256
    };

    OutputStyle outputStyle() const;
    void setOutputStyle(OutputStyle o);

    ColorMode colorMode() const;
    void setColorMode(ColorMode c);

    bool isSmoothing() const;
    void setSmoothing(bool val);

    int numberOfSmoothingIterations() const;
    void setNumberOfSmoothingIterations(int val);

    bool isDecimation() const;
    void setDecimation(bool val);

    double decimationError() const;
    void setDecimationError(double val);

    int error() const;
    void setError(int val);

    int subImageSize() const;
    void setSubImageSize(int val);

private:
    vtkImageToPolyDataFilter *m_vtkImageToPolyDataFilter;
};


#endif