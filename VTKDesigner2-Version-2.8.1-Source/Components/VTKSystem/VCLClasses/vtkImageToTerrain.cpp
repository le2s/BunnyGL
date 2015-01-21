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

#include "vtkImageToTerrain.h"
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkImageData.h"
#include <math.h>

vtkCxxRevisionMacro(vtkImageToTerrain, "$Revision: 1.53 $");
vtkStandardNewMacro(vtkImageToTerrain);

vtkImageToTerrain::vtkImageToTerrain()
{
    this->SetNumberOfInputPorts(0);
    this->Dimensions[0] = this->Dimensions[1] = 10;
    this->Distance[0] = this->Distance[1] = 0.1;
    this->HeightRange[0] = 0;
    this->HeightRange[1] = 0.1415;
    this->ImageData = 0;
    this->HeightScaleFactor = 0.1;
}

vtkImageToTerrain::~vtkImageToTerrain()
{
    if( this->ImageData )
        this->ImageData->UnRegister(this);
}

void vtkImageToTerrain::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkPolyDataAlgorithm::PrintSelf(os, indent);

    os << "Dimensions: " << this->Dimensions[0] << ", " << this->Dimensions[1] << "\n";
    os << "Distance  : " << this->Distance[0] << ", " << this->Distance[1] << "\n";
}

void vtkImageToTerrain::SetDimensions(int x, int y)
{
    if(x >= 2)
        Dimensions[0] = x;
    else
        Dimensions[0] = 2;
    if(y >= 2)
        Dimensions[1] = y;
    else
        Dimensions[1] = 2;
    this->Modified();
}

void vtkImageToTerrain::GetDimensions(int& x, int& y)
{
    x = Dimensions[0];
    y = Dimensions[1];
}

void vtkImageToTerrain::SetDistance(double x, double y)
{
    Distance[0] = x;
    Distance[1] = y;
    this->Modified();
}

void vtkImageToTerrain::GetDistance(double &x, double &y)
{
    x = Distance[0];
    y = Distance[1];
}

void vtkImageToTerrain::SetHeightScaleFactor(double val)
{
    this->HeightScaleFactor = val;
    this->Modified();
}

double vtkImageToTerrain::GetHeightScaleFactor()
{
    return this->HeightScaleFactor;
}

void vtkImageToTerrain::SetImage(vtkImageData* imageData)
{
    if( this->ImageData == imageData )
        return;

    if( this->ImageData )
        this->ImageData->UnRegister(this);

    this->ImageData = imageData;

    if( this->ImageData )
        this->ImageData->Register(this);

    this->Modified();
}

vtkImageData* vtkImageToTerrain::GetImage()
{
    return this->ImageData;
}

int vtkImageToTerrain::RequestData(
            vtkInformation *vtkNotUsed(request),
            vtkInformationVector **vtkNotUsed(inputVector),
            vtkInformationVector *outputVector
        )
{
    // get the info object
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    // get the ouptut
    vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    // Construct the basic grid
    int grid_x = Dimensions[0];
    int grid_y = Dimensions[1];
    double dx = Distance[0];
    double dy = Distance[1];
    double xMin = -(dx * double(grid_x-1)) / 2.0;
    double yMin = -(dy * double(grid_y-1)) / 2.0;
    double zVal = 0.0;

    // Adjust the height range
    this->ComputeHeightRange();

    vtkPoints* points = vtkPoints::New();
    vtkDoubleArray* tcoords = vtkDoubleArray::New();
    points->SetNumberOfPoints(grid_x*grid_y);
    tcoords->SetNumberOfComponents(2);
    tcoords->SetNumberOfTuples(grid_x*grid_y);
    for(int i=0; i<grid_x; i++)
    {
        for(int j=0; j<grid_y; j++)
        {
            double x = xMin + double(i) * dx;
            double y = yMin + double(j) * dy;
            double z = zVal;
            points->SetPoint(j*grid_x+i, x, y, z);

            double s = double(i)/double(grid_x);
            double t = double(j)/double(grid_y);
            tcoords->SetTuple2(j*grid_x+i, s, t);
        }
    }

    vtkCellArray* cells = vtkCellArray::New();
    for(int i=0; i<grid_x-1; i++)
    {
        for(int j=0; j<grid_y-1; j++)
        {
            int base = j*grid_x + i;
            int a = base;
            int b = base+1;
            int c = base+grid_x+1;
            int d = base+grid_x;

            vtkIdType tgl[3];
            tgl[2] = a; tgl[1] = b; tgl[0] = c;
            cells->InsertNextCell(3, tgl);
            tgl[2] = a; tgl[1] = c; tgl[0] = d;
            cells->InsertNextCell(3, tgl);
        }
    }

    // Now if an input image exists, then modify the grid so that
    // scalar values from the image can be taken as height values
    // of the grid.
    if( this->ImageData )
    {
        int* dim = this->ImageData->GetDimensions();
        if( (dim[0] * dim[1] * dim[2]) == 0.0 )
            goto terrain_generated;

        double w = this->Distance[0] * grid_x;
        double h = this->Distance[1] * grid_y;
        double d = sqrt( w*w + h*h );
        double maxHeight = d * this->HeightScaleFactor;
        double scalarRange[2];

        this->ImageData->GetScalarRange(scalarRange);
        double scale = maxHeight / (scalarRange[1] - scalarRange[0]);

        double gridXF = dim[0] / double(grid_x);
        double gridYF = dim[1] / double(grid_y);
        int index = 0;
        int nrPoints = int(points->GetNumberOfPoints());

        for(int i=0; i<grid_x; i++)
        {
            for(int j=0; j<grid_y; j++)
            {
                int t = int(double(i)*gridXF);
                int s = int(double(j)*gridYF);
                double height = this->GetHeightValueAt(s, t, scale);
                double* pt = points->GetPoint(index);
                pt[2] = height;
                points->SetPoint(index, pt);
                ++index;
            }
        }
    }

terrain_generated:
    output->SetPoints(points);
    points->Delete();
    output->GetPointData()->SetTCoords(tcoords);
    tcoords->Delete();
    output->SetStrips(cells);
    cells->Delete();

    return 1;
}

double vtkImageToTerrain::GetHeightValueAt(int s, int t, double scale)
{
    double ret = this->ImageData->GetScalarComponentAsDouble(s, t, 0, 0);
    ret *= scale;
    return ret;
}

void vtkImageToTerrain::ComputeHeightRange()
{
    double w = this->Dimensions[0] * this->Distance[0];
    double h = this->Dimensions[1] * this->Distance[1];
    double d = sqrt( w*w + h*h );

    this->HeightRange[0] = 0;
    this->HeightRange[1] = d * this->HeightScaleFactor;
}
