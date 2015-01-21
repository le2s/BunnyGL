#include "vtkCircleSource.h"
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkImageData.h"
#include "vtkExecutive.h"
#include "vtkOutputWindow.h"
#include <math.h>

vtkCxxRevisionMacro(vtkCircleSource, "$Revision: 1.53 $");
vtkStandardNewMacro(vtkCircleSource);

vtkCircleSource::vtkCircleSource()
{
    this->Resolution = 100;
    this->Radius = 1;

    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

vtkCircleSource::~vtkCircleSource()
{

}

void vtkCircleSource::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkPolyDataAlgorithm::PrintSelf(os, indent);

    os << "Resolution : " << this->Resolution << endl;
    os << "Radius : " << this->Radius << endl;
}

int vtkCircleSource::RequestData(
            vtkInformation *vtkNotUsed(request),
            vtkInformationVector **vtkNotUsed(inputVector),
            vtkInformationVector *outputVector
        )
{
    vtkInformation *outputInfo = outputVector->GetInformationObject(0);
    if(!outputInfo)
        return 1;

    vtkPolyData *output = vtkPolyData::SafeDownCast(outputInfo->Get(vtkDataObject::DATA_OBJECT()));

    // Create Geometry
    vtkPoints* points = vtkPoints::New();
    points->SetNumberOfPoints(Resolution);

    double angleStep = 3.1415*2.0/double(Resolution);
    double angle = 0;

    for(int i=0; i<this->Resolution; i++)
    {
        points->SetPoint( i, this->Radius * cos(angle), this->Radius * sin(angle),  0.0);
        angle = angle + angleStep;
    }

    // Associate geometry with the output
    output->SetPoints( points );
    points->Delete();

    // Create topology
    vtkCellArray* cells = vtkCellArray::New();
    vtkIdType* line = new vtkIdType[this->Resolution];

    for(int i=0; i<this->Resolution; i++)
        line[i] =  i;

    cells->InsertNextCell( this->Resolution, line );
    delete [] line;

    // Associate topology with output
    output->SetPolys(cells);
    cells->Delete();

    // Return that the algorithm was successful
    return 1;
}

