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

#ifndef C_VTK_SCALARS_TO_COLORS_H
#define C_VTK_SCALARS_TO_COLORS_H

#include "CVtkObject.h"
#include "vtkScalarsToColors.h"
#include "CVtkScalarsToColorsData.h"

class CVtkScalarsToColors : public CVtkObject
{
    Q_OBJECT
    Q_ENUMS(VectorMode)
    Q_PROPERTY(double RangeMin READ rangeMin WRITE setRangeMin)
    Q_PROPERTY(double RangeMax READ rangeMax WRITE setRangeMax)
    Q_PROPERTY(double Alpha READ alpha WRITE setAlpha)
    Q_PROPERTY(VectorMode VectorMode READ vectorMode WRITE setVectorMode)
    Q_PROPERTY(int VectorComponent READ vectorComponent WRITE setVectorComponent)
    DECLARE_VTK_OBJECT

public:
    enum VectorMode
    {
        Magnitude = vtkScalarsToColors::MAGNITUDE,
        Component = vtkScalarsToColors::COMPONENT
    };

    CVtkScalarsToColors();
    ~CVtkScalarsToColors();

    Q_INVOKABLE void setRange(double min, double max);
    double rangeMin() const;
    double rangeMax() const;
    void setRangeMin(double val);
    void setRangeMax(double val);

    Q_INVOKABLE QColor color(double value) const;
    Q_INVOKABLE double opacity(double value) const;
    Q_INVOKABLE double luminance(double value) const;

    void setAlpha(double val);
    double alpha() const;

    void setVectorMode(VectorMode val);
    VectorMode vectorMode() const;

    void setVectorComponent(int comp);
    int vectorComponent() const;

protected:
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkScalarsToColors* m_vtkScalarsToColors;
    CVtkScalarsToColorsData m_scalarsToColorsData;
};

#endif
