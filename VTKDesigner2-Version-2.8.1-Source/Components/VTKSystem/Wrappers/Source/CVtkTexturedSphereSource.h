/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
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
#ifndef C_VTK_TEXTURED_SPHERE_SOURCE_H
#define C_VTK_TEXTURED_SPHERE_SOURCE_H

#include "vtkTexturedSphereSource.h"
#include "CVtkPolyDataAlgorithm.h"

class CVtkTexturedSphereSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(double Radius READ radius WRITE setRadius)
    Q_PROPERTY(int ThetaResolution READ thetaResolution WRITE setThetaResolution)
    Q_PROPERTY(int PhiResolution READ phiResolution WRITE setPhiResolution)
    Q_PROPERTY(double Theta READ theta WRITE setTheta)
    Q_PROPERTY(double Phi READ phi WRITE setPhi)
    DECLARE_VTK_OBJECT

public:
    CVtkTexturedSphereSource();
    ~CVtkTexturedSphereSource();

    void setRadius(double val);
    double radius() const;

    void setThetaResolution(int res);
    int thetaResolution() const;

    void setPhiResolution(int res);
    int phiResolution() const;

    void setTheta(double val);
    double theta() const;

    void setPhi(double val);
    double phi() const;

private:
    vtkTexturedSphereSource *m_vtkTexturedSphereSource;
};
#endif
