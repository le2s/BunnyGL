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

#ifndef C_VTK_CONE_SOURCE_H
#define C_VTK_CONE_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkConeSource.h"
#include "UserDefinedDataTypes.h"
#include "CVtkPolyData.h"

class CVtkConeSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(double Height READ height WRITE setHeight)
    Q_PROPERTY(double Radius READ radius WRITE setRadius)
    Q_PROPERTY(Point3D Center READ center WRITE setCenter)
    Q_PROPERTY(Vector3D Direction READ direction WRITE setDirection)
    Q_PROPERTY(double Angle READ angle WRITE setAngle)
    Q_PROPERTY(bool Capping READ isCapping WRITE setCapping)
    Q_PROPERTY(int Resolution READ resolution WRITE setResolution)
    DECLARE_VTK_OBJECT

public:
    CVtkConeSource();
    ~CVtkConeSource();

    // Property methods
    double height() const;
    void setHeight(double h);

    double radius() const;
    void setRadius(double r);

    Point3D center() const;
    void setCenter(Point3D center);

    Vector3D direction() const;
    void setDirection(Vector3D direction);

    bool isCapping() const;
    void setCapping(bool val);

    double angle() const;
    void setAngle(double angle);

    void setResolution(int res);
    int resolution() const;

private:
    vtkConeSource* m_vtkConeSource;
    CVtkPolyData m_vtkConeSourceOutput;
};

#endif

