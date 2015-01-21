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

#ifndef VTK_OPEN_HAPTICS_PROPERTY_H
#define VTK_OPEN_HAPTICS_PROPERTY_H

#include "vtkObject.h"

#define VTK_FRONT_FACE              1
#define VTK_BACK_FACE               2
#define VTK_FRONT_AND_BACK_FACE     3
#define VTK_TOUCH_MODEL_CONSTRAINT  4
#define VTK_TOUCH_MODEL_CONTACT     5

class vtkOpenHapticsRenderer;
class vtkOpenHapticsProperty : public vtkObject
{
public:
    static vtkOpenHapticsProperty* New();

    vtkSetClampMacro(ApplyToFace, int, VTK_FRONT_FACE, VTK_FRONT_AND_BACK_FACE);
    vtkGetMacro(ApplyToFace, int);

    vtkSetClampMacro(Stiffness, double, 0.0, 1.0);
    vtkGetMacro(Stiffness, double);

    vtkSetClampMacro(Damping, double, 0.0, 1.0);
    vtkGetMacro(Damping, double);

    vtkSetClampMacro(StaticFriction, double, 0.0, 1.0);
    vtkGetMacro(StaticFriction, double);

    vtkSetClampMacro(DynamicFriction, double, 0.0, 1.0);
    vtkGetMacro(DynamicFriction, double);

    vtkSetClampMacro(Popthrough, double, 0.0, 1.0);
    vtkGetMacro(Popthrough, double);

    vtkSetClampMacro(TouchModel, int, VTK_TOUCH_MODEL_CONSTRAINT, VTK_TOUCH_MODEL_CONTACT);
    vtkGetMacro(TouchModel, int);

    vtkSetMacro(SnapDistance, double);
    vtkGetMacro(SnapDistance, double);

    void Render(vtkOpenHapticsRenderer *ren);

protected:
    vtkOpenHapticsProperty();
    ~vtkOpenHapticsProperty();

    int ApplyToFace;
    double Stiffness;
    double Damping;
    double StaticFriction;
    double DynamicFriction;
    double Popthrough;
    int TouchModel;
    double SnapDistance;
};

#endif

