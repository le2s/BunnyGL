#ifdef VTK5_FOUND

#include "DICOMImageSeriesMPR.h"
#include "DICOMImageSeries.h"
#include "vtkDICOMImageReader.h"
#include <QMap>

///////////////////////////////////////////////////////////////////////////////

class vtkDICOMImageReader2;
class DCMReaderManagerClass
{
public:
    QMap<QString, vtkDICOMImageReader2*> DCMImageReaderMap;
    DCMReaderManagerClass();
    ~DCMReaderManagerClass();
};
static DCMReaderManagerClass DCMReaderManager;
static bool DCMReaderManagerDestruction = false;

vtkDICOMImageReader2::vtkDICOMImageReader2() { }

vtkDICOMImageReader2::~vtkDICOMImageReader2()
{
    if(!DCMReaderManagerDestruction && !DirectoryName.isEmpty())
        DCMReaderManager.DCMImageReaderMap.remove(DirectoryName);
}

vtkDICOMImageReader2* vtkDICOMImageReader2::New(const QString& dirName)
{
    if(DCMReaderManager.DCMImageReaderMap.contains(dirName))
       return DCMReaderManager.DCMImageReaderMap[dirName];

    vtkDICOMImageReader2* reader = new vtkDICOMImageReader2;
    DCMReaderManager.DCMImageReaderMap[dirName] = reader;

    QByteArray dirNameBA = dirName.toLatin1();
    reader->SetDirectoryName(dirNameBA.constData());
    reader->DirectoryName = dirName;
    reader->Update();

    return reader;
}

DCMReaderManagerClass::DCMReaderManagerClass()
{

}

DCMReaderManagerClass::~DCMReaderManagerClass()
{
    DCMReaderManagerDestruction = true;
    QList<vtkDICOMImageReader2*> dcmReaders = DCMImageReaderMap.values();
    DCMImageReaderMap.clear();

    Q_FOREACH(vtkDICOMImageReader2* dcmReader, dcmReaders)
        dcmReader->Delete();
}

///////////////////////////////////////////////////////////////////////////////

struct DICOMImageSeriesMPRData
{
    DICOMImageSeries* parent;

    vtkDICOMImageReader* dcmReader;

    DICOMImageSlicer* axialSlicer;
    DICOMImageSlicer* sagittalSlicer;
    DICOMImageSlicer* coronalSlicer;
};

DICOMImageSeriesMPR::DICOMImageSeriesMPR(DICOMImageSeries* parent)
: QObject(parent)
{
    d = new DICOMImageSeriesMPRData;
    d->parent = parent;
    d->dcmReader = vtkDICOMImageReader2::New(d->parent->directoryName());
    d->dcmReader->Register(NULL);

    d->axialSlicer = new DICOMImageSlicer(d->dcmReader->GetOutput(), DICOMImageSlicer::AxialAxis);
    d->sagittalSlicer = new DICOMImageSlicer(d->dcmReader->GetOutput(), DICOMImageSlicer::SagittalAxis);
    d->coronalSlicer = new DICOMImageSlicer(d->dcmReader->GetOutput(), DICOMImageSlicer::CoronalAxis);
}

DICOMImageSeriesMPR::~DICOMImageSeriesMPR()
{
    delete d->axialSlicer;
    delete d->sagittalSlicer;
    delete d->coronalSlicer;
    d->dcmReader->Delete();

    delete d;
}

void DICOMImageSeriesMPR::reset()
{
    d->axialSlicer->reset();
    d->sagittalSlicer->reset();
    d->coronalSlicer->reset();
}

QImage DICOMImageSeriesMPR::currentAxialSliceImage() const
{
    return d->axialSlicer->currentSliceImage();
}

QImage DICOMImageSeriesMPR::currentSagittalSliceImage() const
{
    return d->sagittalSlicer->currentSliceImage();
}

QImage DICOMImageSeriesMPR::currentCoronalSliceImage() const
{
    return d->coronalSlicer->currentSliceImage();
}

QImage DICOMImageSeriesMPR::currentAxialSliceImageFullRes() const
{
    return d->axialSlicer->currentSliceImageFullRes();
}

QImage DICOMImageSeriesMPR::currentSagittalSliceImageFullRes() const
{
    return d->sagittalSlicer->currentSliceImageFullRes();
}

QImage DICOMImageSeriesMPR::currentCoronalSliceImageFullRes() const
{
    return d->coronalSlicer->currentSliceImageFullRes();
}

int DICOMImageSeriesMPR::currentAxialSliceIndex() const
{
    return d->axialSlicer->currentSliceIndex();
}

int DICOMImageSeriesMPR::currentSagittalSliceIndex() const
{
    return d->sagittalSlicer->currentSliceIndex();
}

int DICOMImageSeriesMPR::currentCoronalSliceIndex() const
{
    return d->coronalSlicer->currentSliceIndex();
}

int DICOMImageSeriesMPR::axialSliceCount() const
{
    return d->axialSlicer->sliceCount();
}

int DICOMImageSeriesMPR::coronalSliceCount() const
{
    return d->coronalSlicer->sliceCount();
}

int DICOMImageSeriesMPR::sagittalSliceCount() const
{
    return d->sagittalSlicer->sliceCount();
}

QImage DICOMImageSeriesMPR::axialSliceImage(int sliceNumber) const
{
    d->axialSlicer->setCurrentSliceIndex(sliceNumber);
    return d->axialSlicer->currentSliceImage();
}

QImage DICOMImageSeriesMPR::sagittalSliceImage(int sliceNumber) const
{
    d->sagittalSlicer->setCurrentSliceIndex(sliceNumber);
    return d->sagittalSlicer->currentSliceImage();
}

QImage DICOMImageSeriesMPR::coronalSliceImage(int sliceNumber) const
{
    d->coronalSlicer->setCurrentSliceIndex(sliceNumber);
    return d->coronalSlicer->currentSliceImage();
}

QImage DICOMImageSeriesMPR::nextAxialSliceImage(int count) const
{
    d->axialSlicer->next(count);
    return d->axialSlicer->currentSliceImage();
}

QImage DICOMImageSeriesMPR::nextSagittalSliceImage(int count) const
{
    d->sagittalSlicer->next(count);
    return d->sagittalSlicer->currentSliceImage();
}

QImage DICOMImageSeriesMPR::nextCoronalSliceImage(int count) const
{
    d->coronalSlicer->next(count);
    return d->coronalSlicer->currentSliceImage();
}

QImage DICOMImageSeriesMPR::previousAxialSliceImage(int count) const
{
    d->axialSlicer->previous(count);
    return d->axialSlicer->currentSliceImage();
}

QImage DICOMImageSeriesMPR::previousSagittalSliceImage(int count) const
{
    d->sagittalSlicer->previous(count);
    return d->sagittalSlicer->currentSliceImage();
}

QImage DICOMImageSeriesMPR::previousCoronalSliceImage(int count) const
{
    d->coronalSlicer->previous(count);
    return d->coronalSlicer->currentSliceImage();
}

double DICOMImageSeriesMPR::windowLevel() const
{
    return d->axialSlicer->windowLevel();
}

double DICOMImageSeriesMPR::windowWidth() const
{
    return d->axialSlicer->windowWidth();
}

void DICOMImageSeriesMPR::setWindowLevelAndWidth(double wl, double ww)
{
    d->axialSlicer->setWindowLevelAndWidth(wl, ww);
    d->sagittalSlicer->setWindowLevelAndWidth(wl, ww);
    d->coronalSlicer->setWindowLevelAndWidth(wl, ww);
}

///////////////////////////////////////////////////////////////////////////////
// DICOMImageSlicer
///////////////////////////////////////////////////////////////////////////////

#include "vtkImageReslice.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkTransform.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkUnsignedShortArray.h"
#include "vtkUnsignedIntArray.h"
#include "vtkUnsignedCharArray.h"
#include "vtkShortArray.h"

struct DICOMImageSlicerData
{
    DICOMImageSlicer::Axis axis;

    vtkImageData* dcmImage;
    vtkImageData* dcmSlice;
    vtkImageMapToWindowLevelColors* colorMap;

    int sliceCount;
    int sliceIndex;
    QImage sliceImage;
    QImage sliceImageThumb;
    bool recomputeFlag;
};

DICOMImageSlicer::DICOMImageSlicer(vtkImageData* dcmImage, DICOMImageSlicer::Axis axis)
{
    d = new DICOMImageSlicerData;

    // Initialize the default values
    d->axis = axis;
    d->dcmImage = dcmImage;
    d->dcmSlice = vtkImageData::New();

    d->colorMap = vtkImageMapToWindowLevelColors::New();
    d->colorMap->SetOutputFormatToLuminance();
    d->colorMap->SetWindow(400);
    d->colorMap->SetLevel(60);

    d->sliceIndex = -1;
    d->sliceCount = 0;
    d->recomputeFlag = false;

    d->dcmSlice->SetScalarType( d->dcmImage->GetScalarType() );
    d->dcmSlice->SetNumberOfScalarComponents( d->dcmImage->GetNumberOfScalarComponents() );

    int dim[3];
    double spacing[3];
    d->dcmImage->GetDimensions(dim);
    d->dcmImage->GetSpacing(spacing);
    switch(axis)
    {
    case AxialAxis:
        d->sliceCount = dim[2];
        d->dcmSlice->SetDimensions(dim[0], dim[1], 1);
        d->dcmSlice->SetSpacing(spacing[0], spacing[1], spacing[2]);
        break;
    case SagittalAxis:
        d->sliceCount = dim[0];
        d->dcmSlice->SetDimensions(dim[2], dim[1], 1);
        d->dcmSlice->SetSpacing(spacing[2], spacing[1], spacing[0]);
        break;
    case CoronalAxis:
        d->sliceCount = dim[1];
        d->dcmSlice->SetDimensions(dim[0], dim[2], 1);
        d->dcmSlice->SetSpacing(spacing[0], spacing[2], spacing[1]);
        break;
    }

    d->dcmSlice->AllocateScalars();

    // Fetch the middle slice in the axis
    d->sliceIndex = -1;
    this->setCurrentSliceIndex( d->sliceCount/2 );
}

DICOMImageSlicer::~DICOMImageSlicer()
{
    d->colorMap->SetInput(0);
    d->colorMap->Delete();

    d->dcmSlice->Delete();

    delete d;
}

void DICOMImageSlicer::reset()
{
    this->setCurrentSliceIndex( d->sliceCount/2 );
}

int DICOMImageSlicer::sliceCount() const
{
    return d->sliceCount;
}

int DICOMImageSlicer::currentSliceIndex() const
{
    return d->sliceIndex;
}

void DICOMImageSlicer::setCurrentSliceIndex(int index)
{
    if(index < 0)
        index = 0;
    else if(index >= d->sliceCount)
        index = d->sliceCount-1;

    if(index == d->sliceIndex)
        return;

    d->sliceIndex = index;
    this->recomputeCurrentSlice();
}

void DICOMImageSlicer::next(int count)
{
    this->setCurrentSliceIndex(d->sliceIndex+count);
}

void DICOMImageSlicer::previous(int count)
{
    this->setCurrentSliceIndex(d->sliceIndex-count);
}

void DICOMImageSlicer::recomputeCurrentSlice()
{
    // Catch hold of the input and output scalars
    vtkDataArray* inScalars = d->dcmImage->GetPointData()->GetScalars();
    vtkDataArray* outScalars = d->dcmSlice->GetPointData()->GetScalars();

    // Initialize output
    int *outDim = d->dcmImage->GetDimensions();
    outScalars->SetNumberOfComponents( d->dcmImage->GetNumberOfScalarComponents() );
    outScalars->SetNumberOfTuples(outDim[0]*outDim[1]*outDim[2]);

    // Now begin transferring data
    int nrComps = outScalars->GetNumberOfComponents();
    int dataTypeSize = outScalars->GetElementComponentSize();
    int inExtent[6], scalarIncrements[3], pt[3] = {0, 0, 0};
    vtkIdType outScalarIndex = 0;

    d->dcmImage->GetExtent(inExtent);
    d->dcmImage->GetArrayIncrements(inScalars, scalarIncrements);
    d->dcmSlice->GetDimensions(outDim);

    for(int j=0; j<outDim[1]; j++)
    {
        for(int i=0; i<outDim[0]; i++)
        {
            switch(d->axis)
            {
            case AxialAxis: // X-Y
                pt[0] = inExtent[0]+i;
                pt[1] = inExtent[2]+j;
                pt[2] = d->sliceIndex;
                break;
            case SagittalAxis: // Z-Y
                pt[2] = inExtent[4]+i;
                pt[1] = inExtent[2]+j;
                pt[0] = d->sliceIndex;
                break;
            case CoronalAxis: // X-Z
                pt[0] = inExtent[0]+i;
                pt[2] = inExtent[2]+j;
                pt[1] = d->sliceIndex;
                break;
            }

            // We are not using vtkImageData::GetScalarPointer()
            // because the function performs a lot of checks and hence
            // slows down this algorithm.
            int idx = ((pt[0] - inExtent[0]) * scalarIncrements[0]
                       + (pt[1] - inExtent[2]) * scalarIncrements[1]
                       + (pt[2] - inExtent[4]) * scalarIncrements[2]);

            const void* tuple = inScalars->GetVoidPointer(idx);
            int base=(outScalarIndex++)*nrComps;
            void* outScalarsArray = outScalars->GetVoidPointer(base);
            memcpy(outScalarsArray, tuple, dataTypeSize*nrComps);
        }
    }

    d->dcmSlice->Modified();

    // Now pass the image through color map
    d->colorMap->SetInput(d->dcmSlice);
    d->colorMap->Update();

    // Convert the color-mapped output into QImage.
    QImage qImage = DICOMImageSeries::vtkImageToQImage(d->colorMap->GetOutput());
    d->sliceImage = qImage;
    d->sliceImageThumb = qImage.scaled(64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    d->recomputeFlag = false;
}

QImage DICOMImageSlicer::currentSliceImage() const
{
    if(d->recomputeFlag)
        const_cast<DICOMImageSlicer*>(this)->recomputeCurrentSlice();

    return d->sliceImageThumb;
}

QImage DICOMImageSlicer::currentSliceImageFullRes() const
{
    if(d->recomputeFlag)
        const_cast<DICOMImageSlicer*>(this)->recomputeCurrentSlice();

    return d->sliceImage;
}

double DICOMImageSlicer::windowLevel() const
{
    return d->colorMap->GetLevel();
}

double DICOMImageSlicer::windowWidth() const
{
    return d->colorMap->GetWindow();
}

void DICOMImageSlicer::setWindowLevelAndWidth(double wl, double ww)
{
    d->colorMap->SetWindow(ww);
    d->colorMap->SetLevel(wl);
    d->recomputeFlag = true;
}

#endif

