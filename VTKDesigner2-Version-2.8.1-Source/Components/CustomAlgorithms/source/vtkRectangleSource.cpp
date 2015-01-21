#include "vtkRectangleSource.h"
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

vtkCxxRevisionMacro(vtkRectangleSource, "$Revision: 1.53 $");
vtkStandardNewMacro(vtkRectangleSource);

vtkRectangleSource::vtkRectangleSource()
{
    this->Length = 1;
    this->Breadth = 1;

    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

vtkRectangleSource::~vtkRectangleSource()
{

}

void vtkRectangleSource::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkPolyDataAlgorithm::PrintSelf(os, indent);

    os << "Length : " << this->Length << endl;
    os << "Breadth : " << this->Breadth << endl;
}

int vtkRectangleSource::RequestData(
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
    points->SetNumberOfPoints(4);
    points->SetPoint(0, this->Length/2, this->Breadth/2, 0);
    points->SetPoint(1, -this->Length/2, this->Breadth/2, 0);
    points->SetPoint(2, -this->Length/2, -this->Breadth/2, 0);
    points->SetPoint(3, this->Length/2, -this->Breadth/2, 0);

    // Associate geometry with the output
    output->SetPoints( points );
    points->Delete();

    // Create topology
    vtkCellArray* cells = vtkCellArray::New();
    vtkIdType rectangle[] = { 0, 1, 2, 3 };
    cells->InsertNextCell(4, rectangle);

    // Associate topology with output
    output->SetPolys(cells);
    cells->Delete();

    // Return that the algorithm was successful
    return 1;
}

