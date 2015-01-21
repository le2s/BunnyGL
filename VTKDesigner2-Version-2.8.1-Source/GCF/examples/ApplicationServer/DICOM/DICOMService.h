#ifndef DICOM_SERVICE_H
#define DICOM_SERVICE_H

#include <GCF/AbstractComponent>
#include <GCF/WSMacros>

#include <QImage>

struct DICOMServiceData;
class DICOMService : public GCF::AbstractComponent
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(DICOMService)

    public:
        static DICOMService& instance();
    ~DICOMService();

    Q_INVOKABLE bool loadDICOMSeries(const QString& dirName);
    Q_INVOKABLE void releaseDICOMSeries();

    Q_INVOKABLE bool isValid() const;
    Q_INVOKABLE QString directoryName() const;
    Q_INVOKABLE int sliceCount() const;
    Q_INVOKABLE int sliceImageWidth() const;
    Q_INVOKABLE int sliceImageHeight() const;
    Q_INVOKABLE int currentSliceIndex() const;
    Q_INVOKABLE QImage currentSliceImage() const;
    Q_INVOKABLE QImage currentSliceImageFullRes() const;
    Q_INVOKABLE QImage nextSliceImage(int count) const;
    Q_INVOKABLE QImage previousSliceImage(int count) const;
    Q_INVOKABLE QImage sliceImage(int index) const;
    Q_INVOKABLE double windowLevel() const;
    Q_INVOKABLE double windowWidth() const;
    Q_INVOKABLE QImage setWindowLevelAndWidth(double wl, double ww);

    // Patient information
    Q_INVOKABLE QString patientName() const;
    Q_INVOKABLE QString studyUID() const;
    Q_INVOKABLE QString studyID() const;
    Q_INVOKABLE QString dimensions() const;
    Q_INVOKABLE QString pixelSpacing() const;

    // MPR functions
    Q_INVOKABLE QImage currentAxialSliceImage() const;
    Q_INVOKABLE QImage currentSagittalSliceImage() const;
    Q_INVOKABLE QImage currentCoronalSliceImage() const;
    Q_INVOKABLE QImage currentAxialSliceImageFullRes() const;
    Q_INVOKABLE QImage currentSagittalSliceImageFullRes() const;
    Q_INVOKABLE QImage currentCoronalSliceImageFullRes() const;
    Q_INVOKABLE int currentAxialSliceIndex() const;
    Q_INVOKABLE int currentSagittalSliceIndex() const;
    Q_INVOKABLE int currentCoronalSliceIndex() const;
    Q_INVOKABLE int axialSliceCount() const;
    Q_INVOKABLE int coronalSliceCount() const;
    Q_INVOKABLE int sagittalSliceCount() const;
    Q_INVOKABLE QImage axialSliceImage(int sliceNumber) const;
    Q_INVOKABLE QImage sagittalSliceImage(int sliceNumber) const;
    Q_INVOKABLE QImage coronalSliceImage(int sliceNumber) const;
    Q_INVOKABLE QImage nextAxialSliceImage(int count) const;
    Q_INVOKABLE QImage nextSagittalSliceImage(int count) const;
    Q_INVOKABLE QImage nextCoronalSliceImage(int count) const;
    Q_INVOKABLE QImage previousAxialSliceImage(int count) const;
    Q_INVOKABLE QImage previousSagittalSliceImage(int count) const;
    Q_INVOKABLE QImage previousCoronalSliceImage(int count) const;

    // Volume Rendering functions
    Q_INVOKABLE QImage prepareVolume();
    Q_INVOKABLE QImage currentVolumeImage() const;
    Q_INVOKABLE QImage mousePressOnVolume(double x, double y);
    Q_INVOKABLE QImage mouseMoveOnVolume(double x, double y);
    Q_INVOKABLE QImage mouseReleaseOnVolume(double x, double y);
    Q_INVOKABLE QImage mouseWheelOnVolume(double x, double y, int delta);
    Q_INVOKABLE QImage resetCameraOnVolume();

    // IContainer implementation
    QObject* containerObject();

protected:
    DICOMService();
    void initializeComponent();
    void finalizeComponent();
    QObject* fetchObject(const QString& completeName) const;

private:
    DICOMServiceData* d;
};

#endif

