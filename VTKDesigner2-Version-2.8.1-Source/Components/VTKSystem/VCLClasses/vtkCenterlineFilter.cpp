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

#include "vtkCenterlineFilter.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolygon.h"
#include "vtkCutter.h"
#include "vtkPlane.h"
#include "vtkTransform.h"
#include "vtkSphere.h"
#include "vtkImplicitBoolean.h"

#include <QtGlobal>

static bool GetCenter(vtkPolyData* polyData, double center[3], double &radius);
static void RotateVector(const double vecToRotate[3], const double axis[3], const double angle, double result[3]);

vtkStandardNewMacro(vtkCenterlineFilter);

vtkCenterlineFilter::vtkCenterlineFilter()
{
    this->StartingPoint[0] = this->StartingPoint[1] = this->StartingPoint[2] = 0;
    this->StartingDirection[0] = this->StartingDirection[1] = this->StartingDirection[2] = 0;
    this->StartingViewUp[0] = this->StartingViewUp[1] = this->StartingViewUp[2] = 0;
    this->StartingDirection[2] = 1;
    this->StartingViewUp[1] = 1;
    this->StepDistance = 0.1;
    this->StartingRadius = 1.0;
    this->MaxIterations = 0;
}

vtkCenterlineFilter::~vtkCenterlineFilter()
{
    
}

int vtkCenterlineFilter::RequestData(vtkInformation *vtkNotUsed(request),
                                     vtkInformationVector **inputVector,
                                     vtkInformationVector *outputVector)

{
    // get the info objects
    vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation *outInfo = outputVector->GetInformationObject(0);
    
    // get the input and ouptut
    vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    // Safety checks
    if(!input->GetPoints() || !input->GetPoints()->GetNumberOfPoints())
        return 1;

    /*
    The whole logic behind this algorithm is very simple. But before I go about explaining this algorithm,
    let me put this BIG FAT NOTE:

        I have no intentions to argue about the good/bad of this algorithm with the other well established
        algorithms in the world. I have written this class only to serve one of my purposes, and nothing
        more. If it helps you, I am glad to know :)

    We start from a point, aptly called as the starting point. Then from this point we move a distance
    equal to step distance along the starting direction. At this point we will compute the equation of a 
    plane such that the direction is normal to the plane and the starting up vector lies on the plane.
    Then we use this plane to cut through the input polydata and evaluate the center point of that segment
    of the tube. We continue like this and adjust the direction and up vector at every stage, this forming
    the center line.
    */

    double point[3], dir[3], viewUp[3], &dist = this->StepDistance, radius = this->StartingRadius;
    vtkPoints* points = vtkPoints::New();
    vtkCellArray* lines = vtkCellArray::New();
    vtkCutter* cutter = vtkCutter::New();
    vtkPlane* planeFunc = vtkPlane::New();
    vtkSphere* sphereFunc = vtkSphere::New();
    vtkImplicitBoolean* func = vtkImplicitBoolean::New();
    int iterCount=0;

    // Initial values of algorithm variables.
    point[0] = this->StartingPoint[0];
    point[1] = this->StartingPoint[1];
    point[2] = this->StartingPoint[2];

    dir[0] = this->StartingDirection[0];    
    dir[1] = this->StartingDirection[1];
    dir[2] = this->StartingDirection[2];
    vtkMath::Normalize(dir);

    viewUp[0] = this->StartingViewUp[0];
    viewUp[1] = this->StartingViewUp[1];
    viewUp[2] = this->StartingViewUp[2];
    vtkMath::Normalize(viewUp);

    // Setup the cutter with the input function and the input polydata itself.
    func->AddFunction(planeFunc);
    func->AddFunction(sphereFunc);
    func->SetOperationTypeToIntersection();
    cutter->SetCutFunction(func);
    cutter->SetInput(input);
    cutter->SetNumberOfContours(1);
    cutter->GenerateValues(1, 0, 1);

    // Insert the first function into the output.
    points->InsertNextPoint(point);
    while(1)
    {
        double newPoint[3];
        double newDir[3];
        double newViewUp[3];
        double cp[3];
        double angle;

        // Move point by distance units along direction.
        newPoint[0] = point[0] + dir[0]*dist;
        newPoint[1] = point[1] + dir[1]*dist;
        newPoint[2] = point[2] + dir[2]*dist;

        // Construct a plane at point and setup the normals properly.
        planeFunc->SetOrigin(point);
        planeFunc->SetNormal(dir);
        sphereFunc->SetRadius(radius);
        sphereFunc->SetCenter(point);

        // Now ask the cutter to cut the input data set.
        cutter->Update();

        // Get the points on the cut plane.
        vtkPolyData* cutData = cutter->GetOutput();
        if(!GetCenter(cutData, newPoint, radius) && !this->MaxIterations)
            break; // Loop break condition.

        // Now compute the new direction vector.
        newDir[0] = newPoint[0] - point[0];
        newDir[1] = newPoint[1] - point[1];
        newDir[2] = newPoint[2] - point[2];
        vtkMath::Normalize(newDir);

        // Now compute the angle between the new direction and the old direction.
        angle = acos(vtkMath::Dot(dir, newDir));

        // Now rotate the view up vector about a vector perpendicular to view up and
        // dir, by angle. This will give us the new view up vector.
        vtkMath::Cross(dir, newDir, cp);
        RotateVector(viewUp, cp, angle, newViewUp);

        // Now lets add the new point to the points array that would go into the output
        // polygon.
        points->InsertNextPoint(newPoint);

        // Make all the new stuff as current stuff for the next iteration.
        point[0] = newPoint[0];
        point[1] = newPoint[1];
        point[2] = newPoint[2];
        dir[0] = newDir[0];
        dir[1] = newDir[1];
        dir[2] = newDir[2];
        viewUp[0] = newViewUp[0];
        viewUp[1] = newViewUp[1];
        viewUp[2] = newViewUp[2];

        // Loop away!
        iterCount++;
        if(this->MaxIterations && iterCount >= this->MaxIterations)
            break;
    }

    // Now create line segments and make a polyline.
    for(vtkIdType i=0; i<points->GetNumberOfPoints()-1; i++)
    {
        vtkIdType nPts = 2;
        vtkIdType pts[2];
        pts[0] = i;
        pts[1] = i+1;
        lines->InsertNextCell(nPts, pts);
    }

    // Create the output
    output->SetPoints(points);
    output->SetLines(lines);

    // Release resources.
    points->Delete();
    lines->Delete();
    planeFunc->Delete();
    sphereFunc->Delete();
    func->Delete();
    cutter->Delete();

    // All done, return success.
    return 1;
}

bool GetCenter(vtkPolyData* polyData, double center[3], double& radius)
{
    vtkPoints* points = polyData->GetPoints();
    if(points->GetNumberOfPoints() == 0)
        return false;

    double x=0, y=0, z=0;
    double minx, miny, minz, maxx, maxy, maxz;
    for(vtkIdType i=0; i<points->GetNumberOfPoints(); i++)
    {
        double* pt = points->GetPoint(i);
        x += pt[0];
        y += pt[1];
        z += pt[2];

        if(!i)
        {
            minx = maxx = x;
            miny = maxy = y;
            minz = maxz = z;
        }
        else
        {
            if(minx > x) minx = x;
            else if(maxx < x) maxx = x;
            if(miny > y) miny = y;
            else if(maxy < y) maxy = y;
            if(minz > z) minz = z;
            else if(maxz < z) maxz = z;
        }
    }

    center[0] = x / points->GetNumberOfPoints();
    center[1] = y / points->GetNumberOfPoints();
    center[2] = z / points->GetNumberOfPoints();

    double pt1[3], pt2[3];
    pt1[0] = x; pt1[1] = y; pt1[2] = z;
    pt2[0] = x; pt2[1] = y; pt2[2] = z;
    radius = vtkMath::Distance2BetweenPoints(pt1, pt2);

    return true;
}

void RotateVector(const double vecToRotate[3], const double axis[3], const double angle, double result[3])
{
    vtkTransform* transform = vtkTransform::New();

    transform->Identity();
    transform->RotateWXYZ(angle, axis);
    
    double in[4], out[4];
    in[0] = vecToRotate[0];
    in[1] = vecToRotate[1];
    in[2] = vecToRotate[2];
    in[3] = 1;

    transform->MultiplyPoint(in, out);

    result[0] = out[0];
    result[1] = out[1];
    result[2] = out[2];
}







