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

#include "vtkFlythroughCamera.h"
#include "vtkSetGet.h"
#include "vtkPolyData.h"
#include "vtkCellArray.h"
#include "vtkPoints.h"
#include "vtkMath.h"

vtkFlythroughCamera* vtkFlythroughCamera::New()
{
    vtkFlythroughCamera* newCamera = new vtkFlythroughCamera;
    return newCamera;
}

vtkFlythroughCamera::vtkFlythroughCamera()
{
    this->FlythroughPath = 0;
    this->FlythroughLineIndex = -1;

    this->InitialPosition[0] = 0;
    this->InitialPosition[1] = 0;
    this->InitialPosition[2] = 0;

    this->InitialFocalPoint[0] = 0;
    this->InitialFocalPoint[1] = 0;
    this->InitialFocalPoint[2] = 1;

    this->InitialViewUp[0] = 0;
    this->InitialViewUp[1] = 1;
    this->InitialViewUp[2] = 0;
}

vtkFlythroughCamera::~vtkFlythroughCamera()
{
    if(this->FlythroughPath)
        this->FlythroughPath->UnRegister(this);
}

void vtkFlythroughCamera::SetInitialPosition(double x, double y, double z)
{
    this->InitialPosition[0] = x;
    this->InitialPosition[1] = y;
    this->InitialPosition[2] = z;
    vtkOpenGLCamera::SetPosition(x, y, z);
}

void vtkFlythroughCamera::SetInitialPosition(double pos[3])
{
    this->SetInitialPosition(pos[0], pos[1], pos[2]);
}

void vtkFlythroughCamera::GetInitialPosition(double pos[3])
{
    pos[0] = this->InitialPosition[0];
    pos[1] = this->InitialPosition[1];
    pos[2] = this->InitialPosition[2];
}

double* vtkFlythroughCamera::GetInitialPosition()
{
    return this->InitialPosition;
}

void vtkFlythroughCamera::SetInitialFocalPoint(double x, double y, double z)
{
    this->InitialFocalPoint[0] = x;
    this->InitialFocalPoint[1] = y;
    this->InitialFocalPoint[2] = z;
    vtkOpenGLCamera::SetFocalPoint(x, y, z);
}

void vtkFlythroughCamera::SetInitialFocalPoint(double pos[3])
{
    this->SetInitialFocalPoint(pos[0], pos[1], pos[2]);
}

void vtkFlythroughCamera::GetInitialFocalPoint(double pos[3])
{
    pos[0] = this->InitialFocalPoint[0];
    pos[1] = this->InitialFocalPoint[1];
    pos[2] = this->InitialFocalPoint[2];
}

double* vtkFlythroughCamera::GetInitialFocalPoint()
{
    return this->InitialFocalPoint;
}

void vtkFlythroughCamera::SetInitialViewUp(double x, double y, double z)
{
    this->InitialViewUp[0] = x;
    this->InitialViewUp[1] = y;
    this->InitialViewUp[2] = z;
    vtkOpenGLCamera::SetViewUp(x, y, z);
}

void vtkFlythroughCamera::SetInitialViewUp(double pos[3])
{
    this->SetInitialViewUp(pos[0], pos[1], pos[2]);
}

void vtkFlythroughCamera::GetInitialViewUp(double pos[3])
{
    pos[0] = this->InitialViewUp[0];
    pos[1] = this->InitialViewUp[1];
    pos[2] = this->InitialViewUp[2];
}

double* vtkFlythroughCamera::GetInitialViewUp()
{
    return this->InitialViewUp;
}

void vtkFlythroughCamera::SetFlythroughPath(vtkPolyData* input)
{
    if(this->FlythroughPath == input)
        return;

    if(this->FlythroughPath)
    {
        this->FlythroughPath->UnRegister(this);
        this->FlythroughPath = 0;
        this->FlythroughLineIndex = -1;
    }

    if(input)
    {
        input->Register(this);
        this->FlythroughLineIndex = 0;
    }

    this->FlythroughPath = input;
    this->Modified();
}

vtkPolyData* vtkFlythroughCamera::GetFlythroughPath()
{
    return this->FlythroughPath;
}

void vtkFlythroughCamera::MoveStart()
{
    vtkOpenGLCamera::SetPosition(this->InitialPosition);
    vtkOpenGLCamera::SetFocalPoint(this->InitialFocalPoint);
    vtkOpenGLCamera::SetViewUp(this->InitialViewUp);
    this->FlythroughLineIndex = 0;
}

void vtkFlythroughCamera::MoveForward()
{
    if(!this->FlythroughPath)
        return;

    // Basic checks.
    vtkCellArray* lines = this->FlythroughPath->GetLines();
    if(!lines)
        return;
    vtkIdType* lineIndexArray = lines->GetPointer();
    if(!lineIndexArray)
        return;
    vtkIdType nextIndex = this->FlythroughLineIndex+1;
    if(nextIndex >= lines->GetNumberOfCells())
        return;
    
    vtkIdType ptIdx1 = lineIndexArray[nextIndex*3+1];
    vtkIdType ptIdx2 = lineIndexArray[nextIndex*3+2];
    double currPos[3], currFp[3], nextPos[3], nextFp[3];
    vtkOpenGLCamera::GetPosition(currPos);
    vtkOpenGLCamera::GetFocalPoint(currFp);
    this->FlythroughPath->GetPoint(ptIdx1, nextPos);
    this->FlythroughPath->GetPoint(ptIdx2, nextFp);

    // Find out the angle between (currPos->currFP) and (nextPos->nextFP) vectors.
    double currVec[3], nextVec[3], dotProduct, angle;
    currVec[0] = currFp[0] - currPos[0];
    currVec[1] = currFp[1] - currPos[1];
    currVec[2] = currFp[2] - currPos[2];
    nextVec[0] = nextFp[0] - nextPos[0];
    nextVec[1] = nextFp[1] - nextPos[1];
    nextVec[2] = nextFp[2] - nextPos[2];
    vtkMath::Normalize(currVec);
    vtkMath::Normalize(nextVec);
    dotProduct = vtkMath::Dot(currVec, nextVec);
    angle = acos(dotProduct);

    /*// Find out distance between currPos and nextPos
    double dist = vtkMath::Distance2BetweenPoints(currPos, nextPos);

    // Move the focal point by that distance.
    nextFp[0] = currFp[0] + dist * nextVec[0];
    nextFp[1] = currFp[1] + dist * nextVec[1];
    nextFp[2] = currFp[2] + dist * nextVec[2];*/

    // Now turn the view up vector by that angle.
    vtkOpenGLCamera::Pitch(angle);
    vtkOpenGLCamera::SetPosition(nextPos);
    vtkOpenGLCamera::SetFocalPoint(nextFp);

    this->FlythroughLineIndex = nextIndex;
}

void vtkFlythroughCamera::MoveBackward()
{
    if(!this->FlythroughPath)
        return;

    // Basic checks.
    vtkCellArray* lines = this->FlythroughPath->GetLines();
    if(!lines)
        return;
    vtkIdType* lineIndexArray = lines->GetPointer();
    if(!lineIndexArray)
        return;
    vtkIdType nextIndex = this->FlythroughLineIndex ? this->FlythroughLineIndex-1 : 0;
    if(nextIndex == this->FlythroughLineIndex)
        return;

    vtkIdType ptIdx1 = lineIndexArray[nextIndex*3+1];
    vtkIdType ptIdx2 = lineIndexArray[nextIndex*3+2];
    double currPos[3], currFp[3], nextPos[3], nextFp[3];
    vtkOpenGLCamera::GetPosition(currPos);
    vtkOpenGLCamera::GetFocalPoint(currFp);
    this->FlythroughPath->GetPoint(ptIdx1, nextPos);
    this->FlythroughPath->GetPoint(ptIdx2, nextFp);

    // Find out the angle between (currPos->currFP) and (nextPos->nextFP) vectors.
    double currVec[3], nextVec[3], dotProduct, angle;
    currVec[0] = currFp[0] - currPos[0];
    currVec[1] = currFp[1] - currPos[1];
    currVec[2] = currFp[2] - currPos[2];
    nextVec[0] = nextFp[0] - nextPos[0];
    nextVec[1] = nextFp[1] - nextPos[1];
    nextVec[2] = nextFp[2] - nextPos[2];
    vtkMath::Normalize(currVec);
    vtkMath::Normalize(nextVec);
    dotProduct = vtkMath::Dot(currVec, nextVec);
    angle = acos(dotProduct);

    // Now turn the view up vector by that angle.
    vtkOpenGLCamera::Pitch (angle);
    vtkOpenGLCamera::SetPosition(nextPos);
    vtkOpenGLCamera::SetFocalPoint(nextFp);

    this->FlythroughLineIndex = nextIndex;
}

void vtkFlythroughCamera::MoveForwardSwapAround()
{
    if(!this->FlythroughPath)
        return;

    vtkCellArray* lines = this->FlythroughPath->GetLines();
    if(!lines)
        return;

    vtkIdType nextIndex = this->FlythroughLineIndex+1;
    if(nextIndex >= lines->GetNumberOfCells())
        this->MoveStart();
    else
        this->MoveForward();
}
