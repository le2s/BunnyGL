#include "DICOMService.h"

#include <QDir>
#include <QFileInfo>
#include <QRegExp>
#include <QMap>

#ifdef VTK5_FOUND
#include "DICOMImageSeries.h"
#include "DICOMImageSeriesMPR.h"
#include "DICOMImageSeriesVolume.h"
#endif

#include "PatientList.h"

GCF_DEFINE_COMPONENT(DICOMService)

struct DICOMServiceData
{
#ifdef VTK5_FOUND
    QMap<QString, DICOMImageSeries*> dcmImageSeriesMap;
#endif

    QImage blankImage;

    PatientList patientList;
};

DICOMService& DICOMService::instance()
{
    static DICOMService theInstance;
    return theInstance;
}

DICOMService::DICOMService()
{
    d = new DICOMServiceData;

    d->blankImage = QImage(256, 256, QImage::Format_Mono);
    d->blankImage.fill(0);
}

DICOMService::~DICOMService()
{
    delete d;
}

bool DICOMService::loadDICOMSeries(const QString& directoryName)
{
#ifdef VTK5_FOUND
    QString dirName = directoryName;
    dirName = dirName.replace('\\', '/');

    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
    {
        DICOMImageSeries* dcmSeries = d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID];
        if(dcmSeries->directoryName() == dirName)
            return dcmSeries->isValid();

        dcmSeries->deref();
        d->dcmImageSeriesMap.remove(GCF_CURRENT_CLIENT_ID);
    }

    DICOMImageSeries* dcmSeries = DICOMImageSeries::GetImageSeries(dirName);
    if(dcmSeries)
    {
        dcmSeries->ref();
        d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID] = dcmSeries;
        return dcmSeries->isValid();
    }
#else
    Q_UNUSED(directoryName);
#endif

    return false;
}

void DICOMService::releaseDICOMSeries()
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
    {
        DICOMImageSeries* dcmSeries = d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID];
        dcmSeries->deref();
        d->dcmImageSeriesMap.remove(GCF_CURRENT_CLIENT_ID);
    }
#endif
}

bool DICOMService::isValid() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->isValid();
#endif

    return false;
}

QString DICOMService::directoryName() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->directoryName();
#endif

    return QString();
}

int DICOMService::sliceCount() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->sliceCount();
#endif

    return 0;
}

int DICOMService::sliceImageWidth() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->sliceImageWidth();
#endif

    return 0;
}

int DICOMService::sliceImageHeight() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->sliceImageHeight();
#endif

    return 0;
}

int DICOMService::currentSliceIndex() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->currentSliceIndex();
#endif

    return -1;
}

QImage DICOMService::currentSliceImage() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->currentSliceImage();
#endif

    return d->blankImage;
}

QImage DICOMService::currentSliceImageFullRes() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->currentSliceImageFullRes();
#endif

    return d->blankImage;
}

QImage DICOMService::nextSliceImage(int count) const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->nextSliceImage(count);
#else
    Q_UNUSED(count);
#endif

    return d->blankImage;
}

QImage DICOMService::previousSliceImage(int count) const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->previousSliceImage(count);
#else
    Q_UNUSED(count);
#endif

    return d->blankImage;
}

QImage DICOMService::sliceImage(int index) const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->sliceImage(index);
#else
    Q_UNUSED(index);
#endif

    return d->blankImage;
}

double DICOMService::windowLevel() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->windowLevel();
#endif

    return 0.0;
}

double DICOMService::windowWidth() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->windowWidth();
#endif

    return 0.0;
}

QImage DICOMService::setWindowLevelAndWidth(double wl, double ww)
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
    {
        d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->setWindowLevelAndWidth(wl, ww);
        return currentSliceImage();
    }
#else
    Q_UNUSED(wl);
    Q_UNUSED(ww);
#endif

    return d->blankImage;
}

QString DICOMService::patientName() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->patientName();
#endif

    return QString();
}

QString DICOMService::studyUID() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->studyUID();
#endif

    return QString();
}

QString DICOMService::studyID() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->studyID();
#endif

    return QString();
}

QString DICOMService::dimensions() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->dimensions();
#endif

    return QString();
}

QString DICOMService::pixelSpacing() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->pixelSpacing();
#endif

    return QString();
}

QImage DICOMService::currentAxialSliceImage() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->currentAxialSliceImage();
#endif

    return d->blankImage;
}

QImage DICOMService::currentSagittalSliceImage() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->currentSagittalSliceImage();
#endif

    return d->blankImage;
}

QImage DICOMService::currentCoronalSliceImage() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->currentCoronalSliceImage();
#endif

    return d->blankImage;
}

QImage DICOMService::currentAxialSliceImageFullRes() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->currentAxialSliceImageFullRes();
#endif

    return d->blankImage;
}

QImage DICOMService::currentSagittalSliceImageFullRes() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->currentSagittalSliceImageFullRes();
#endif

    return d->blankImage;
}

QImage DICOMService::currentCoronalSliceImageFullRes() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->currentCoronalSliceImageFullRes();
#endif

    return d->blankImage;
}

int DICOMService::currentAxialSliceIndex() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->currentAxialSliceIndex();
#endif

    return 0;
}

int DICOMService::currentSagittalSliceIndex() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->currentSagittalSliceIndex();
#endif

    return 0;
}

int DICOMService::currentCoronalSliceIndex() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->currentCoronalSliceIndex();
#endif

    return 0;
}

int DICOMService::axialSliceCount() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->axialSliceCount();
#endif

    return 0;
}

int DICOMService::coronalSliceCount() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->coronalSliceCount();
#endif

    return 0;
}

int DICOMService::sagittalSliceCount() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->sagittalSliceCount();
#endif

    return 0;
}

QImage DICOMService::axialSliceImage(int sliceNumber) const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->axialSliceImage(sliceNumber);
#else
    Q_UNUSED(sliceNumber);
#endif

    return d->blankImage;
}

QImage DICOMService::sagittalSliceImage(int sliceNumber) const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->sagittalSliceImage(sliceNumber);
#else
    Q_UNUSED(sliceNumber);
#endif

    return d->blankImage;
}

QImage DICOMService::coronalSliceImage(int sliceNumber) const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->coronalSliceImage(sliceNumber);
#else
    Q_UNUSED(sliceNumber);
#endif

    return d->blankImage;
}

QImage DICOMService::nextAxialSliceImage(int count) const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->nextAxialSliceImage(count);
#else
    Q_UNUSED(count);
#endif

    return d->blankImage;
}

QImage DICOMService::nextSagittalSliceImage(int count) const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->nextSagittalSliceImage(count);
#else
    Q_UNUSED(count);
#endif

    return d->blankImage;
}

QImage DICOMService::nextCoronalSliceImage(int count) const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->nextCoronalSliceImage(count);
#else
    Q_UNUSED(count);
#endif

    return d->blankImage;
}

QImage DICOMService::previousAxialSliceImage(int count) const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->previousAxialSliceImage(count);
#else
    Q_UNUSED(count);
#endif

    return d->blankImage;
}

QImage DICOMService::previousSagittalSliceImage(int count) const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->previousSagittalSliceImage(count);
#else
    Q_UNUSED(count);
#endif

    return d->blankImage;
}

QImage DICOMService::previousCoronalSliceImage(int count) const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->mpr()->previousCoronalSliceImage(count);
#else
    Q_UNUSED(count);
#endif

    return d->blankImage;
}

QImage DICOMService::prepareVolume()
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->volume()->prepareVolume();
#endif

    return d->blankImage;
}

QImage DICOMService::currentVolumeImage() const
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->volume()->currentVolumeImage();
#endif

    return d->blankImage;
}

QImage DICOMService::mousePressOnVolume(double x, double y)
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->volume()->mousePressOnVolume(x,y);
#else
    Q_UNUSED(x);
    Q_UNUSED(y);
#endif

    return d->blankImage;
}

QImage DICOMService::mouseMoveOnVolume(double x, double y)
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->volume()->mouseMoveOnVolume(x,y);
#else
    Q_UNUSED(x);
    Q_UNUSED(y);
#endif

    return d->blankImage;
}

QImage DICOMService::mouseReleaseOnVolume(double x, double y)
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->volume()->mouseReleaseOnVolume(x,y);
#else
    Q_UNUSED(x);
    Q_UNUSED(y);
#endif

    return d->blankImage;
}

QImage DICOMService::mouseWheelOnVolume(double x, double y, int delta)
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->volume()->mouseWheelOnVolume(x, y, delta);
#else
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(delta);
#endif

    return d->blankImage;
}

QImage DICOMService::resetCameraOnVolume()
{
#ifdef VTK5_FOUND
    if(d->dcmImageSeriesMap.contains(GCF_CURRENT_CLIENT_ID))
        return d->dcmImageSeriesMap[GCF_CURRENT_CLIENT_ID]->volume()->resetCameraOnVolume();
#endif

    return d->blankImage;
}

QObject* DICOMService::containerObject()
{
    return this;
}

void DICOMService::initializeComponent()
{
    this->setGuiXmlFileName(":/DICOMService/DICOMService.xml");
}

void DICOMService::finalizeComponent()
{
#ifdef VTK5_FOUND
    QList<DICOMImageSeries*> dcmSeries = d->dcmImageSeriesMap.values();
    qDeleteAll(dcmSeries);
    d->dcmImageSeriesMap.clear();
#endif
}

QObject* DICOMService::fetchObject(const QString& completeName) const
{
    if(completeName == "DICOMService.Core")
        return const_cast<DICOMService*>(this);

#ifdef VTK5_FOUND
    if(completeName == "DICOMService.PatientList")
        return &d->patientList;
#endif

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(DICOMService)
