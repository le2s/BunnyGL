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
#ifndef C_VTK_PLANE_H
#define C_VTK_PLANE_H

#include "vtkPlane.h"
#include "CVtkImplicitFunction.h"
#include "UserDefinedDataTypes.h"

class CVtkPlane : public CVtkImplicitFunction
{
    Q_OBJECT
    Q_PROPERTY(Vector3D Normal READ normal WRITE setNormal)
    Q_PROPERTY(Point3D Origin READ origin WRITE setOrigin)
    DECLARE_VTK_OBJECT

public:

    CVtkPlane();
    ~CVtkPlane();

    Vector3D normal() const;
    void setNormal(Vector3D val);

    Point3D origin() const;
    void setOrigin(Point3D val);


private:
    vtkPlane *m_vtkPlane;
};
#endif//C_VTK_PLANE_H