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

#ifndef C_VTK_SPHERE_SOURCE_H
#define C_VTK_SPHERE_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkSphereSource.h"
#include "UserDefinedDataTypes.h"

class CVtkSphereSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(double Radius READ radius WRITE setRadius)
    Q_PROPERTY(Point3D Center READ center WRITE setCenter)
    Q_PROPERTY(int ThetaResolution READ thetaResolution WRITE setThetaResolution)
    Q_PROPERTY(int PhiResolution READ phiResolution WRITE setPhiResolution)
    Q_PROPERTY(double StartTheta READ startTheta WRITE setStartTheta)
    Q_PROPERTY(double EndTheta READ endTheta WRITE setEndTheta)
    Q_PROPERTY(double StartPhi READ startPhi WRITE setStartPhi)
    Q_PROPERTY(double EndPhi READ endPhi WRITE setEndPhi)
    Q_PROPERTY(bool LatLongTessellation READ latLongTessellation WRITE setLatLongTessellation)
    DECLARE_VTK_OBJECT

public:
    CVtkSphereSource();
    ~CVtkSphereSource();

    void setRadius(double r);
    double radius() const;

    void setCenter(Point3D p);
    Point3D center() const;

    void setThetaResolution(int res);
    int thetaResolution() const;

    void setPhiResolution(int res);
    int phiResolution() const;

    void setStartTheta(double val);
    double startTheta() const;

    void setStartPhi(double val);
    double startPhi() const;

    void setEndTheta(double val);
    double endTheta() const;

    void setEndPhi(double val);
    double endPhi() const;

    void setLatLongTessellation(bool val);
    bool latLongTessellation() const;

private:
    vtkSphereSource* m_vtkSphereSource;
};

#endif
