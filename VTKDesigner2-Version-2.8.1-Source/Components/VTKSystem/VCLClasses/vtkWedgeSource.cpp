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

#include "vtkWedgeSource.h"
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkExecutive.h"
#include "vtkOutputWindow.h"
#include "vtkMath.h"
#include <math.h>

vtkCxxRevisionMacro(vtkWedgeSource, "$Revision: 1.53 $");
vtkStandardNewMacro(vtkWedgeSource);

vtkWedgeSource::vtkWedgeSource()
{
    this->Width = 1;
    this->Height = 1;
    this->Depth = 1;

    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

vtkWedgeSource::~vtkWedgeSource()
{

}

void vtkWedgeSource::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkPolyDataAlgorithm::PrintSelf(os, indent);

    os << "Width: " << this->Width << "\n";
    os << "Height: " << this->Height << "\n";
    os << "Depth: " << this->Depth << "\n";
}

void vtkWedgeSource::SetHeightFromBaseAngle(double angleDeg)
{
    if( angleDeg <= 0 || angleDeg >= 90 )
        return;

    // tan(angle) = this->Height / this->Width;
    // this->Height = this->Width * tan(angle);
    double angle = vtkMath::Pi() * angleDeg / 180.0;
    SetHeight( tan(angle)*this->Width );
}

double vtkWedgeSource::GetBaseAngle()
{
    double tanTheta = this->Height / this->Width;
    double angle = atan(tanTheta);
    return angle * 180.0 / vtkMath::Pi();
}

int vtkWedgeSource::RequestData(
            vtkInformation *vtkNotUsed(request),
            vtkInformationVector **vtkNotUsed(inputVector),
            vtkInformationVector *outputVector
        )
{
    vtkInformation *outputInfo = outputVector->GetInformationObject(0);
    if(!outputInfo)
        return 1;

    vtkPolyData *output = vtkPolyData::SafeDownCast(outputInfo->Get(vtkDataObject::DATA_OBJECT()));

    // Geometry of the wedge. Consists of 6 points.
    vtkPoints* points = vtkPoints::New();
    points->SetNumberOfPoints(6);
    points->SetPoint(0, 0,           0,             this->Depth/2);
    points->SetPoint(1, this->Width, 0,             this->Depth/2);
    points->SetPoint(2, 0,           this->Height,  this->Depth/2);
    points->SetPoint(3, 0,           0,            -this->Depth/2);
    points->SetPoint(4, this->Width, 0,            -this->Depth/2);
    points->SetPoint(5, 0,           this->Height, -this->Depth/2);
    
    // Topoloty of the wedge. There is a vtkWedge cell, but I really have
    // no idea how it is initialized. So I am going to do it the old fashioned way!
    vtkCellArray* cells = vtkCellArray::New();
    static vtkIdType ca[] = { 0, 1, 2, 
                              5, 4, 3,
                              1, 4, 5, 
                              1, 5, 2,
                              1, 0, 3, 
                              1, 3, 4,
                              0, 2, 5, 
                              0, 5, 3 };

    for(vtkIdType i=0; i<8; i++)
        cells->InsertNextCell(3, ca+i*3);

    // Store points and cells now and return
    output->SetPoints(points);
    points->Delete();

    output->SetStrips(cells);
    cells->Delete();
    
    return 1;
}

