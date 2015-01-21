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

#ifndef C_VTK_PLANE_SOURCE_H
#define C_VTK_PLANE_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkPlaneSource.h"
#include "UserDefinedDataTypes.h"
#include "CVtkPolyData.h"

class CVtkPlaneSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(int XResolution READ xResolution WRITE setXResolution)
    Q_PROPERTY(int YResolution READ yResolution WRITE setYResolution)
    Q_PROPERTY(Point3D Origin READ origin WRITE setOrigin)
    Q_PROPERTY(Point3D Point1 READ point1 WRITE setPoint1)
    Q_PROPERTY(Point3D Point2 READ point2 WRITE setPoint2)
    Q_PROPERTY(Point3D Center READ center WRITE setCenter)
    Q_PROPERTY(Point3D Normal READ normal WRITE setNormal)
    
    DECLARE_VTK_OBJECT

public:
    CVtkPlaneSource();
    ~CVtkPlaneSource();

    int xResolution() const;
    void setXResolution(int x);

    int yResolution() const;
    void setYResolution(int y);

    Point3D origin() const;
    void setOrigin(Point3D p);

    Point3D point1() const;
    void setPoint1(Point3D point1);

    Point3D point2() const;
    void setPoint2(Point3D point2);

    Point3D center() const;
    void setCenter(Point3D center);

    Point3D normal() const;
    void setNormal(Point3D norm);
    
private:
    vtkPlaneSource* m_vtkPlaneSource;
};

#endif

