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
#ifndef C_VTK_CYLINDER_H
#define C_VTK_CYLINDER_H

#include "vtkCylinder.h"
#include "CVtkImplicitFunction.h"
#include "UserDefinedDataTypes.h"

class CVtkCylinder : public CVtkImplicitFunction
{
    Q_OBJECT
    Q_PROPERTY(double Radius READ radius WRITE setRadius)
    Q_PROPERTY(Point3D Center READ center WRITE setCenter)
    DECLARE_VTK_OBJECT

public:

    CVtkCylinder();
    ~CVtkCylinder();

    double radius() const;
    void setRadius(double val);

    Point3D center() const;
    void setCenter(Point3D val);


private:
    vtkCylinder *m_vtkCylinder;
};
#endif//C_VTK_CYLINDER_H