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

#ifndef C_VTK_POINT_SOURCE_H
#define C_VTK_POINT_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkPointSource.h"
#include "UserDefinedDataTypes.h"
#include "CVtkPolyData.h"

class CVtkPointSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(int PointCount READ pointCount WRITE setPointCount)
    Q_PROPERTY(double Radius READ radius WRITE setRadius)
    Q_PROPERTY(Point3D Center READ center WRITE setCenter)
    Q_PROPERTY(int Distribution READ distribution WRITE setDistribution)
    DECLARE_VTK_OBJECT

public:
    CVtkPointSource();
    ~CVtkPointSource();

    // Property methods
    int pointCount() const;
    void setPointCount(int c);

    double radius() const;
    void setRadius(double r);

    Point3D center() const;
    void setCenter(Point3D center);

    int distribution() const;
    void setDistribution(int d);

private:
    vtkPointSource* m_vtkPointSource;
};

#endif

