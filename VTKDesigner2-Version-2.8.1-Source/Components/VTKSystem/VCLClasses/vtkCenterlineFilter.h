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

#ifndef VTK_CENTERLINE_FILTER_H
#define VTK_CENTERLINE_FILTER_H

#include "vtkPolyDataAlgorithm.h"

class vtkCenterlineFilter : public vtkPolyDataAlgorithm
{
public:
    static vtkCenterlineFilter* New();

    vtkSetVector3Macro(StartingPoint,double);
    vtkGetVectorMacro(StartingPoint,double,3);

    vtkSetVector3Macro(StartingDirection,double);
    vtkGetVectorMacro(StartingDirection,double,3);

    vtkSetVector3Macro(StartingViewUp,double);
    vtkGetVectorMacro(StartingViewUp,double,3);

    vtkSetClampMacro(StepDistance,double,0.0,VTK_DOUBLE_MAX)
    vtkGetMacro(StepDistance,double);

    vtkSetClampMacro(StartingRadius,double,0.0,VTK_DOUBLE_MAX)
    vtkGetMacro(StartingRadius,double);

    vtkSetClampMacro(MaxIterations,int,0,VTK_INT_MAX);
    vtkGetMacro(MaxIterations,int);

protected:
    vtkCenterlineFilter();
    ~vtkCenterlineFilter();

    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
    double StartingPoint[3];
    double StartingDirection[3];
    double StartingViewUp[3];
    double StepDistance;
    double StartingRadius;
    int MaxIterations;
};

#endif

