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
#ifndef C_VTK_CONE_H
#define C_VTK_CONE_H

#include "vtkCone.h"
#include "CVtkImplicitFunction.h"

class CVtkCone : public CVtkImplicitFunction
{
    Q_OBJECT
    Q_PROPERTY(double Angle READ angle WRITE setAngle)
    DECLARE_VTK_OBJECT

public:

    CVtkCone();
    ~CVtkCone();

    double angle() const;
    void setAngle(double val);


private:
    vtkCone *m_vtkCone;
};
#endif//C_VTK_CONE_H