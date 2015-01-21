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
#ifndef C_VTK_GLYPH_LAYOUT_FILTER_H
#define C_VTK_GLYPH_LAYOUT_FILTER_H

#include "CVtkPolyDataFilter.h"
#include "vtkGraphLayoutFilter.h"
#include "UserDefinedDataTypes.h"


class CVtkGraphLayoutFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(bool ThreeDimensionalLayout READ isThreeDimensionalLayout WRITE setThreeDimensionalLayout)
    Q_PROPERTY(BoundingBox3D GraphBounds READ graphBounds WRITE setGraphBounds)
    Q_PROPERTY(bool AutomaticBoundsComputation READ isAutomaticBoundsComputation WRITE setAutomaticBoundsComputation)
    Q_PROPERTY(int MaxNumberOfIterations READ maxNumberOfIterations WRITE setMaxNumberOfIterations)
    Q_PROPERTY(double CoolDownRate READ coolDownRate WRITE setCoolDownRate)
    DECLARE_VTK_OBJECT

public:
    CVtkGraphLayoutFilter();
    ~CVtkGraphLayoutFilter();

    void setThreeDimensionalLayout(bool val);
    bool isThreeDimensionalLayout() const;

    BoundingBox3D graphBounds() const;
    void setGraphBounds(BoundingBox3D val);

    void setAutomaticBoundsComputation(bool val);
    bool isAutomaticBoundsComputation () const;

    void setMaxNumberOfIterations (int val);
    int maxNumberOfIterations() const;

    void setCoolDownRate(double val);
    double coolDownRate() const;

private:
    vtkGraphLayoutFilter *m_vtkGraphLayoutFilter;
};

#endif
