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

#ifndef C_VTK_CYLINDER_SOURCE_H 
#define C_VTK_CYLINDER_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkCylinderSource.h"
#include "UserDefinedDataTypes.h"
#include "CVtkPolyData.h"

class CVtkCylinderSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(double Radius READ radius WRITE setRadius)
    Q_PROPERTY(double Height READ height WRITE setHeight)
    Q_PROPERTY(Point3D Center READ center WRITE setCenter)
    Q_PROPERTY(bool Capping READ isCapping WRITE setCapping)
    Q_PROPERTY(int Resolution READ resolution WRITE setResolution)
    DECLARE_VTK_OBJECT

public:
    CVtkCylinderSource();
    ~CVtkCylinderSource();

    double radius() const;
    void setRadius(double r);

    double height() const;
    void setHeight(double h);

    Point3D center() const;
    void setCenter(Point3D center);

    bool isCapping() const;
    void setCapping(bool val);

    void setResolution(int res);
    int resolution() const;

private:
    vtkCylinderSource* m_vtkCylinderSource;
};

#endif

