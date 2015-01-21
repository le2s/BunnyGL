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

#ifndef VTK_FLYTHROUGH_CAMERA_H
#define VTK_FLYTHROUGH_CAMERA_H

#include "vtkOpenGLCamera.h"

class vtkPolyData;
class vtkFlythroughCamera : public vtkOpenGLCamera
{
public:
    static vtkFlythroughCamera *New();

    void SetInitialPosition(double x, double y, double z);
    void SetInitialPosition(double pos[3]);
    void GetInitialPosition(double pos[3]);
    double* GetInitialPosition();

    void SetInitialFocalPoint(double x, double y, double z);
    void SetInitialFocalPoint(double pos[3]);
    void GetInitialFocalPoint(double pos[3]);
    double* GetInitialFocalPoint();
    
    void SetInitialViewUp(double x, double y, double z);
    void SetInitialViewUp(double pos[3]);
    void GetInitialViewUp(double pos[3]);
    double* GetInitialViewUp();

    void SetFlythroughPath(vtkPolyData* input);
    vtkPolyData* GetFlythroughPath();

    void MoveStart();
    void MoveForward();
    void MoveBackward();
    void MoveForwardSwapAround();

protected:
    vtkFlythroughCamera();
    ~vtkFlythroughCamera();

private:
    double InitialPosition[3];
    double InitialFocalPoint[3];
    double InitialViewUp[3];
    vtkPolyData* FlythroughPath;
    vtkIdType FlythroughLineIndex;
};

#endif // VTK_FLYTHROUGH_CAMERA_H
