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
#ifndef C_VTK_WARP_LENS_H
#define C_VTK_WARP_LENS_H

#include "vtkWarpLens.h"
#include "CVtkPointSetFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkWarpLens : public CVtkPointSetFilter
{
    Q_OBJECT
    Q_PROPERTY(double Kappa READ kappa WRITE setKappa)
    Q_PROPERTY(Point3D Center READ center WRITE setCenter)
    Q_PROPERTY(Point3D PrincipalPoint READ principalPoint WRITE setPrincipalPoint)
    Q_PROPERTY(double K1 READ k1 WRITE setK1)
    Q_PROPERTY(double K2 READ k2 WRITE setK2)
    Q_PROPERTY(double P1 READ p1 WRITE setP1)
    Q_PROPERTY(double P2 READ p2 WRITE setP2)
    Q_PROPERTY(double FormatWidth READ formatWidth WRITE setFormatWidth)
    Q_PROPERTY(double FormatHeight READ formatHeight WRITE setFormatHeight)
    Q_PROPERTY(int ImageWidth READ imageWidth WRITE setImageWidth)
    Q_PROPERTY(int ImageHeight READ imageHeight WRITE setImageHeight)
    DECLARE_VTK_OBJECT

public:
    CVtkWarpLens();
    ~CVtkWarpLens();

    double kappa() const;
    void setKappa(double val);

    Point3D center() const;
    void setCenter(Point3D val);

    Point3D principalPoint() const;
    void setPrincipalPoint(Point3D val);

    double k1() const;
    void setK1(double val);

    double k2() const;
    void setK2(double val);

    double p1() const;
    void setP1(double val);

    double p2() const;
    void setP2(double val);

    double formatWidth() const;
    void setFormatWidth(double val);

    double formatHeight() const;
    void setFormatHeight(double val);

    int imageWidth() const;
    void setImageWidth(int val);

    int imageHeight() const;
    void setImageHeight(int val);

private:
    vtkWarpLens *m_vtkWarpLens;
};
#endif//C_VTK_WARP_LENS_H