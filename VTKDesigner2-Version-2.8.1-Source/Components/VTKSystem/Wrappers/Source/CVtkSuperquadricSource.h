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

#ifndef C_VTK_SUPER_QUADRIC_SOURCE_H
#define C_VTK_SUPER_QUADRIC_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkSuperquadricSource.h"
#include "UserDefinedDataTypes.h"

class CVtkSuperquadricSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(Point3D Center READ center WRITE setCenter)
    Q_PROPERTY(Point3D Scale READ scale WRITE setScale)
    Q_PROPERTY(int ThetaResolution READ thetaResolution WRITE setThetaResolution)
    Q_PROPERTY(int PhiResolution READ phiResolution WRITE setPhiResolution)
    Q_PROPERTY(double Thickness READ thickness WRITE setThickness)
    Q_PROPERTY(double PhiRoundness READ phiRoundness WRITE setPhiRoundness)
    Q_PROPERTY(double Size READ size WRITE setSize)
    Q_PROPERTY(double ThetaRoundness READ thetaRoundness WRITE setThetaRoundness)
    Q_PROPERTY(bool Toroidal READ isToroidal WRITE setToroidal)
    DECLARE_VTK_OBJECT

public:
    CVtkSuperquadricSource();
    ~CVtkSuperquadricSource();

    void setCenter(Point3D c);
    Point3D center() const;

    void setScale(Point3D s);
    Point3D scale() const;

    void setThetaResolution(int tr);
    int thetaResolution() const;

    void setPhiResolution(int ps);
    int phiResolution() const;

    void setThetaRoundness(double st);
    double thetaRoundness() const;

    void setThickness(double t);
    double thickness() const;

    void setPhiRoundness(double pr);
    double phiRoundness() const;

    void setSize(double s);
    double size() const;

    void setToroidal(bool val);
    bool  isToroidal() const;

private:
    vtkSuperquadricSource *m_vtkSuperquadricSource;
};

#endif