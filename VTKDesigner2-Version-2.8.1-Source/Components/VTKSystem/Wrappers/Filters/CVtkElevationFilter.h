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

#ifndef C_VTK_ELEVATION_FILTER_H
#define C_VTK_ELEVATION_FILTER_H

#include "vtkElevationFilter.h"
#include "CVtkDataSetFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkElevationFilter : public CVtkDataSetFilter
{
    Q_OBJECT
    Q_PROPERTY(Point3D LowPoint READ lowPoint WRITE setLowPoint)
    Q_PROPERTY(Point3D HighPoint READ highPoint WRITE setHighPoint)
    Q_PROPERTY(ValueRange ScalarRange READ scalarRange WRITE setScalarRange)
    DECLARE_VTK_OBJECT

public:
    CVtkElevationFilter();
    ~CVtkElevationFilter();

    Point3D lowPoint() const;
    void setLowPoint(Point3D val);

    Point3D highPoint() const;
    void setHighPoint(Point3D val);

    ValueRange scalarRange() const;
    void setScalarRange(ValueRange val);

private:
    vtkElevationFilter *m_vtkElevationFilter;
};

#endif//C_VTK_ELEVATIONFILTER_H