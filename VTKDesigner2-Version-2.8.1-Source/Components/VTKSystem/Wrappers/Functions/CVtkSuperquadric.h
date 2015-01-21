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
#ifndef C_VTK_SUPERQUADRIC_H
#define C_VTK_SUPERQUADRIC_H

#include "vtkSuperquadric.h"
#include "CVtkImplicitFunction.h"
#include "UserDefinedDataTypes.h"

class CVtkSuperquadric : public CVtkImplicitFunction
{
    Q_OBJECT
    Q_PROPERTY(Point3D Center READ center WRITE setCenter)
    Q_PROPERTY(Point3D Scale READ scale WRITE setScale)
    Q_PROPERTY(double Thickness READ thickness WRITE setThickness)
    Q_PROPERTY(double PhiRoundness  READ phiRoundness  WRITE setPhiRoundness )
    Q_PROPERTY(double ThetaRoundness READ thetaRoundness WRITE setThetaRoundness)
    Q_PROPERTY(double Size READ size WRITE setSize)
    Q_PROPERTY(bool Toroidal READ isToroidal WRITE setToroidal)
    DECLARE_VTK_OBJECT

public:

    CVtkSuperquadric();
    ~CVtkSuperquadric();

    Point3D center() const;
    void setCenter(Point3D val);

    Point3D scale() const;
    void setScale(Point3D val);

    double thickness() const;
    void setThickness(double val);

    double phiRoundness () const;
    void setPhiRoundness (double val);

    double thetaRoundness() const;
    void setThetaRoundness(double val);

    double size() const;
    void setSize(double val);

    bool isToroidal() const;
    void setToroidal(bool val);


private:
    vtkSuperquadric *m_vtkSuperquadric;
};
#endif//C_VTK_SUPERQUADRIC_H