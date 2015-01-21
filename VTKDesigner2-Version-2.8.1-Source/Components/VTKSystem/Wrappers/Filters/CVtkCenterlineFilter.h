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

#ifndef C_VTK_CENTERLINE_FILTER_H
#define C_VTK_CENTERLINE_FILTER_H

#include "CVtkPolyDataFilter.h"
#include "vtkCenterlineFilter.h"
#include "UserDefinedDataTypes.h"


class CVtkCenterlineFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(Point3D StartingPoint READ startingPoint WRITE setStartingPoint)
    Q_PROPERTY(Vector3D StartingDirection READ startingDirection WRITE setStartingDirection)
    Q_PROPERTY(Vector3D StartingViewUp READ startingViewUp WRITE setStartingViewUp)
    Q_PROPERTY(double StepDistance READ stepDistance WRITE setStepDistance)
    Q_PROPERTY(double StartingRadius READ startingRadius WRITE setStartingRadius)
    Q_PROPERTY(int MaxIterations READ maxIterations WRITE setMaxIterations)
    DECLARE_VTK_OBJECT

public:
    CVtkCenterlineFilter();
    ~CVtkCenterlineFilter();

    void setStartingPoint(Point3D point);
    Point3D startingPoint() const;

    void setStartingDirection(Vector3D vec);
    Vector3D startingDirection() const;

    void setStartingViewUp(Vector3D vec);
    Vector3D startingViewUp() const;

    void setStepDistance(double val);
    double stepDistance() const;

    void setStartingRadius(double val);
    double startingRadius() const;

    void setMaxIterations(int iter);
    int maxIterations() const;

private:
    vtkCenterlineFilter* m_vtkCenterlineFilter;
};

#endif


