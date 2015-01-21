/****************************************************************************
**
** Copyright (C) VCreate Logic Pvt Ltd
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Pvt Ltd.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "QImageToVtkImageData.h"

#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkExecutive.h"
#include "vtkPointData.h"
#include "vtkLookupTable.h"
#include "vtkDataArray.h"
#include "vtkIntArray.h"

#include "vtkStreamingDemandDrivenPipeline.h"

vtkCxxRevisionMacro(QImageToVtkImageData, "$Revision: 1.00 $");
vtkStandardNewMacro(QImageToVtkImageData);

QImageToVtkImageData::QImageToVtkImageData()
{
    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);

    m_lookupTable = vtkLookupTable::New();
}

QImageToVtkImageData::~QImageToVtkImageData()
{
    if(m_lookupTable)
        m_lookupTable->Delete();
}

void QImageToVtkImageData::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkImageAlgorithm::PrintSelf(os, indent);
}

int QImageToVtkImageData::RequestInformation(
                        vtkInformation* vtkNotUsed(request),
                        vtkInformationVector** vtkNotUsed(inputVector),
                        vtkInformationVector* outputVector)
{
    // get the info objects
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    int wExt[6];
    wExt[0] = 0; wExt[2] = 0; wExt[4] = 0;
    wExt[1] = m_image.width()-1;
    wExt[3] = m_image.height()-1;
    wExt[5] = 0;
    outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), wExt, 6);
    outInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_EXTENT(), wExt, 6);

    double spacing[3], origin[3];
    spacing[0] = 1;
    spacing[1] = 1;
    spacing[2] = 1;

    origin[0] = 0;
    origin[1] = 0;
    origin[2] = 0;

    outInfo->Set(vtkDataObject::ORIGIN(), origin, 3);
    outInfo->Set(vtkDataObject::SPACING(), spacing, 3);

    vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_INT, -1);

    return 1;
}

void QImageToVtkImageData::ExecuteData(vtkDataObject *out)
{
    vtkImageData* imageData = vtkImageData::SafeDownCast(out);
    imageData->Initialize();

    int width = m_image.width();
    int height = m_image.height();
    if(width*height == 0)
        return;

    imageData->SetDimensions(width, height, 1);
    imageData->SetSpacing(1, 1, 1);
    imageData->SetOrigin(0, 0, 0);

    vtkIntArray* scalars = vtkIntArray::New();
    scalars->SetNumberOfComponents(1);
    scalars->SetNumberOfTuples(width*height);

    vtkIdType scalarIndex = 0;

    QList<QRgb> colorTable;

    for(int x=0; x<width; x++)
    {
        for(int y=0; y<height; y++)
        {
            QRgb pixel = m_image.pixel(x, y);
            int pixelIndex = colorTable.indexOf(pixel);
            if(pixelIndex < 0)
            {
                pixelIndex = colorTable.count();
                colorTable.append(pixel);
            }

            scalars->SetTuple1(scalarIndex++, pixelIndex);
        }
    }

    m_lookupTable->SetNumberOfTableValues(colorTable.count());
    for(int i=0; i<colorTable.count(); i++)
    {
        QRgb color = colorTable.at(i);

        m_lookupTable->SetTableValue(i, double(qRed(color))/255.0, 
                double(qGreen(color))/255.0,
                double(qBlue(color))/255.0 );
    }

    scalars->SetLookupTable(m_lookupTable);

    imageData->GetPointData()->SetScalars(scalars);
    scalars->Delete();
}

