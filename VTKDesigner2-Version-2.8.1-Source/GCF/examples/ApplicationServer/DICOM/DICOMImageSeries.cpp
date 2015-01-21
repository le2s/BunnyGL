#ifdef VTK5_FOUND

#include "DICOMImageSeries.h"
#include "DICOMImageSeriesMPR.h"
#include "DICOMImageSeriesVolume.h"

#include <QObjectCleanupHandler>
#include <QDir>
#include <QFileInfo>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QPair>
#include <QHash>

#include <GCF/Profiler>

#include "vtkDICOMImageReader.h"
#include "vtkImageReslice.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkImageData.h"
#include "vtkUnsignedCharArray.h"
#include "vtkPointData.h"

static QObjectCleanupHandler DICOMImageSeriesCleanupHandler;
DICOMImageSeries* DICOMImageSeries::GetImageSeries(const QString& dirName, DICOMImageSeries* except)
{
    QObjectList dcmImageSeriesList = DICOMImageSeriesCleanupHandler.children();
    Q_FOREACH(QObject* dcmImageSeriesObj, dcmImageSeriesList)
    {
        DICOMImageSeries* dcmImageSeries = qobject_cast<DICOMImageSeries*>(dcmImageSeriesObj);
        if(!dcmImageSeries || (except && dcmImageSeries == except))
            continue;

        if(dcmImageSeries->directoryName() == dirName)
        {
            if(dcmImageSeries->isReffed())
            {
                DICOMImageSeries* dcmImageSeries2 = new DICOMImageSeries(dcmImageSeries);
                DICOMImageSeriesCleanupHandler.add(dcmImageSeries2);
                return dcmImageSeries2;
            }

            dcmImageSeries->reset();
            return dcmImageSeries;
        }
    }

    DICOMImageSeries* dcmImageSeries = new DICOMImageSeries(dirName, &DICOMImageSeriesCleanupHandler);
    if(!dcmImageSeries->isValid())
    {
        delete dcmImageSeries;
        dcmImageSeries = 0;
    }
    else
        DICOMImageSeriesCleanupHandler.add(dcmImageSeries);

    return dcmImageSeries;
}

struct DICOMImageSeriesData
{
    DICOMImageSeriesData() : valid(false), refCount(0),
        currentSliceIndex(-1) { }

    QString directoryName;
    QStringList dcmFileNames;
    QAtomicInt refCount;
    bool valid;
    DICOMImageSeries* dcmImageSeries;
    DICOMImageSeriesMPR* dcmImageSeriesMPR;
    DICOMImageSeriesVolume* dcmImageSeriesVolume;
    QBasicTimer deleteTimer;

    // DICOM Image / VTK stuff..
    vtkDICOMImageReader* dcmReader;
    vtkImageReslice* imageSlicer;
    vtkImageMapToWindowLevelColors* wlwwFilter;
    int currentSliceIndex;
    QString patientName;
    QString studyUID;
    QString studyID;
    QString dimensions;
    QString pixelSpacing;

    QImage currentSliceImage;
    QImage currentSliceThumbImage;

    static QImage toQImage(int width, int height, vtkUnsignedCharArray* scalars) ;
};

DICOMImageSeries::DICOMImageSeries(const QString& dirName, QObject* parent)
: QObject(parent)
{
    d = new DICOMImageSeriesData;
    d->dcmImageSeries = this;
    d->dcmImageSeriesMPR = 0;
    d->dcmImageSeriesVolume = 0;

    // Create the VTK pipeline
    d->dcmReader = vtkDICOMImageReader::New();
    d->imageSlicer = vtkImageReslice::New();
    d->wlwwFilter = vtkImageMapToWindowLevelColors::New();

    d->imageSlicer->SetOutputDimensionality(2);
    d->imageSlicer->SetInput(d->dcmReader->GetOutput());

    d->wlwwFilter->SetOutputFormatToLuminance();
    d->wlwwFilter->SetInput(d->imageSlicer->GetOutput());
    d->wlwwFilter->SetWindow(400);
    d->wlwwFilter->SetLevel(60);

    // Load all the files in the given directory
    d->directoryName = dirName;
    QDir dir(dirName);
    QFileInfoList fiList = dir.entryInfoList(QStringList("*.dcm"), QDir::Files, QDir::Name);
    for(int i=0; i<fiList.count(); i++)
        d->dcmFileNames << fiList.at(i).fileName();

    // Load the first patient
    d->valid = d->dcmFileNames.count();
    this->sliceImage( d->dcmFileNames.count()/2 );
    d->patientName = QString::fromAscii(d->dcmReader->GetPatientName());
    d->studyUID = QString::fromAscii(d->dcmReader->GetStudyUID());
    d->studyID = QString::fromAscii(d->dcmReader->GetStudyID());

    int dim[3] = {0, 0, 0};
    double spacing[3] = {0, 0, 0};
    d->dcmReader->GetOutput()->GetDimensions(dim);
    d->dcmReader->GetOutput()->GetSpacing(spacing);

    d->dimensions = QString("%1 x %2 x %3").arg(dim[0]).arg(dim[1]).arg(d->dcmFileNames.count());
    d->pixelSpacing = QString("%1 x %2 x %3").arg(spacing[0], 0, 'f', 2)
                            .arg(spacing[1], 0, 'f', 2).arg(spacing[2], 0, 'f', 2);
}

DICOMImageSeries::DICOMImageSeries(DICOMImageSeries* basedOn)
: QObject(basedOn->parent())
{
    d = new DICOMImageSeriesData;

    d->dcmImageSeries = this;
    d->dcmImageSeriesMPR = 0;

    // Create the VTK pipeline
    d->dcmReader = vtkDICOMImageReader::New();
    d->imageSlicer = vtkImageReslice::New();
    d->wlwwFilter = vtkImageMapToWindowLevelColors::New();

    d->imageSlicer->SetOutputDimensionality(2);
    d->imageSlicer->SetInput(d->dcmReader->GetOutput());

    d->wlwwFilter->SetOutputFormatToLuminance();
    d->wlwwFilter->SetInput(d->imageSlicer->GetOutput());
    d->wlwwFilter->SetWindow(400);
    d->wlwwFilter->SetLevel(60);

    d->directoryName = basedOn->d->directoryName;
    d->dcmFileNames = basedOn->d->dcmFileNames;
    d->valid = basedOn->d->valid;
    d->patientName = basedOn->d->patientName;
    d->studyUID = basedOn->d->studyUID;
    d->studyID = basedOn->d->studyID;
    d->dimensions = basedOn->d->dimensions;
    d->pixelSpacing = basedOn->d->pixelSpacing;

    d->currentSliceIndex = d->dcmFileNames.count()+1;
    this->sliceImage( d->dcmFileNames.count()/2 );
}

DICOMImageSeries::~DICOMImageSeries()
{
    d->imageSlicer->SetInput(0);
    d->wlwwFilter->SetInput(0);

    d->dcmReader->Delete();
    d->imageSlicer->Delete();
    d->wlwwFilter->Delete();

    delete d;
}

void DICOMImageSeries::ref()
{
    d->refCount.ref();
    d->deleteTimer.stop();
}

bool DICOMImageSeries::deref()
{
    if( int(d->refCount) <= 0 )
        return false;

    bool success = d->refCount.deref();
    if(!success)
    {
        // Lets see if we have any other DICOMImageSeries
        DICOMImageSeries* dcmSeries = DICOMImageSeries::GetImageSeries(d->directoryName, this);
        if(dcmSeries)
        {
            this->setParent(0);
            this->deleteLater();
            DICOMImageSeriesCleanupHandler.remove(this);
        }
        else
            d->deleteTimer.start(30*60000, this); // 30 minutes..
    }

    return success;
}

bool DICOMImageSeries::isReffed() const
{
    return int(d->refCount) > 0;
}

bool DICOMImageSeries::isValid() const
{
    return d->valid;
}

void DICOMImageSeries::reset()
{
    this->sliceImage( d->dcmFileNames.count()/2 );
    if(d->dcmImageSeriesMPR)
        d->dcmImageSeriesMPR->reset();
}

QString DICOMImageSeries::directoryName() const
{
    return d->directoryName;
}

int DICOMImageSeries::sliceCount() const
{
    return d->dcmFileNames.count();
}

int DICOMImageSeries::sliceImageWidth() const
{
    return d->currentSliceImage.width();
}

int DICOMImageSeries::sliceImageHeight() const
{
    return d->currentSliceImage.height();
}

int DICOMImageSeries::currentSliceIndex() const
{
    if(!d->valid)
        return -1;

    return d->currentSliceIndex;
}

QImage DICOMImageSeries::currentSliceImage() const
{
    if(!d->valid)
        return QImage();

    return d->currentSliceThumbImage;
}

QImage DICOMImageSeries::currentSliceImageFullRes() const
{
    if(!d->valid)
        return QImage();

    return d->currentSliceImage;
}

QImage DICOMImageSeries::nextSliceImage(int count) const
{
    if(!d->valid || count < 0 || count >= d->dcmFileNames.count())
        return QImage();

    if(d->currentSliceIndex+count < d->dcmFileNames.count())
        return this->sliceImage(d->currentSliceIndex+count);

    return this->sliceImage(d->dcmFileNames.count()-1);
}

QImage DICOMImageSeries::previousSliceImage(int count) const
{
    if(!d->valid || count < 0 || count >= d->dcmFileNames.count())
        return QImage();

    if(d->currentSliceIndex-count >= 0)
        return this->sliceImage(d->currentSliceIndex-count);

    return this->sliceImage(0);
}

QImage DICOMImageSeries::sliceImage(int index) const
{
    // GCF_PROFILE_THIS_FUNCTION;

    if(!d->valid)
    {
        d->currentSliceImage = QImage();
        d->currentSliceThumbImage = QImage();
        return d->currentSliceThumbImage;
    }

    if(index < 0)
        index = 0;
    else if(index >= d->dcmFileNames.count())
        index = d->dcmFileNames.count()-1;

    if(d->currentSliceIndex == index)
        return d->currentSliceThumbImage;

    QString fileName = QString("%1/%2").arg(d->directoryName).arg(d->dcmFileNames.at(index));
    QByteArray fileNameBA = fileName.toLatin1();

    d->currentSliceIndex = index;

    d->dcmReader->SetFileName(fileNameBA.constData());
    d->dcmReader->Update();
    d->imageSlicer->Update();
    d->wlwwFilter->Update();

    vtkImageData* image = d->wlwwFilter->GetOutput();
    QImage qImage = DICOMImageSeries::vtkImageToQImage(image);
    if(qImage.isNull())
    {
        d->currentSliceImage = QImage();
        d->currentSliceThumbImage = QImage();
        return d->currentSliceThumbImage;
    }

    d->currentSliceImage = qImage;
    d->currentSliceThumbImage = d->currentSliceImage.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    return d->currentSliceThumbImage;
}

double DICOMImageSeries::windowLevel() const
{
    return d->wlwwFilter->GetLevel();
}

double DICOMImageSeries::windowWidth() const
{
    return d->wlwwFilter->GetWindow();
}

void DICOMImageSeries::setWindowLevelAndWidth(double wl, double ww)
{
    d->wlwwFilter->SetLevel(wl);
    d->wlwwFilter->SetWindow(ww);

    int index = d->currentSliceIndex;
    d->currentSliceIndex = -1;
    this->sliceImage(index);

    if(d->dcmImageSeriesMPR)
        d->dcmImageSeriesMPR->setWindowLevelAndWidth(wl, ww);
}

QString DICOMImageSeries::patientName() const
{
    return d->patientName;
}

QString DICOMImageSeries::studyUID() const
{
    return d->studyUID;
}

QString DICOMImageSeries::studyID() const
{
    return d->studyID;
}

QString DICOMImageSeries::dimensions() const
{
    return d->dimensions;
}

QString DICOMImageSeries::pixelSpacing() const
{
    return d->pixelSpacing;
}

DICOMImageSeriesMPR* DICOMImageSeries::mpr() const
{
    if(!d->dcmImageSeriesMPR)
    {
        d->dcmImageSeriesMPR = new DICOMImageSeriesMPR(const_cast<DICOMImageSeries*>(this));
        d->dcmImageSeries->setWindowLevelAndWidth( this->windowLevel(), this->windowWidth() );
    }

    return d->dcmImageSeriesMPR;
}

DICOMImageSeriesVolume* DICOMImageSeries::volume() const
{
    if(!d->dcmImageSeriesVolume)
        d->dcmImageSeriesVolume = new DICOMImageSeriesVolume(const_cast<DICOMImageSeries*>(this));

    return d->dcmImageSeriesVolume;
}

QImage DICOMImageSeriesData::toQImage(int width, int height, vtkUnsignedCharArray* scalars)
{
    vtkIdType tupleIndex=0;
    int qImageBitIndex=0;

    /*
    Creating QImage objects of a given width and height can take time.
    To speed up, we create image of a given size only once and reuse
    the same image object again and again.
    */
    static QHash< QPair<int,int>, QImage> imageCache;
    QImage qImage;
    QPair<int,int> imageSize = qMakePair<int,int>(width, height);

    if( imageCache.contains(imageSize) )
        qImage = imageCache[imageSize];
    else
    {
        qImage = QImage(width, height, QImage::Format_ARGB32);
        imageCache[imageSize] = qImage;
    }

    QRgb* qImageBits = (QRgb*)qImage.bits();
    unsigned char* scalarTuples = scalars->GetPointer(0);

    for(int j=0; j<height; j++)
    {
        for(int i=0; i<width; i++)
        {
            unsigned char* tuple = scalarTuples+(tupleIndex++);

            QRgb color = qRgba(tuple[0], tuple[0], tuple[0], 255);
            *(qImageBits+(qImageBitIndex++))=color;
        }
    }

    qImage = qImage.mirrored(false, true);

    return qImage;
}

bool DICOMImageSeries::LoadInformation(const QString& dirName, QString& patientName, QImage& sampleImage, int& sliceCount)
{
    DICOMImageSeries* imageSeries = DICOMImageSeries::GetImageSeries(dirName);
    if(!imageSeries)
        return false;

    imageSeries->ref();

    patientName = imageSeries->patientName();
    sliceCount = imageSeries->sliceCount();
    sampleImage = imageSeries->currentSliceImageFullRes().scaled(100, 100, Qt::KeepAspectRatio);

    imageSeries->deref();

    return true;
}

QImage DICOMImageSeries::vtkImageToQImage(vtkImageData* image)
{
    if(!image)
        return QImage();

    int dim[3];

    image->GetDimensions(dim);
    if(dim[0] == 0 || dim[1] == 0 || dim[2] == 0)
        return QImage();

    vtkUnsignedCharArray* scalars
        = vtkUnsignedCharArray::SafeDownCast(image->GetPointData()->GetScalars());
    if(!scalars)
        return QImage();

    QImage qImage = DICOMImageSeriesData::toQImage(dim[0], dim[1], scalars);

    double spacing[3];
    image->GetSpacing(spacing);

    QSizeF imageSize = QSizeF(dim[0]*spacing[0], dim[1]*spacing[1]);
    imageSize.scale( QSizeF(qImage.size()), Qt::KeepAspectRatio );

    qImage = qImage.scaled(imageSize.toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    return qImage;
}

void DICOMImageSeries::timerEvent(QTimerEvent* te)
{
    if(te->timerId() == d->deleteTimer.timerId())
        this->deleteLater();
}

#endif

