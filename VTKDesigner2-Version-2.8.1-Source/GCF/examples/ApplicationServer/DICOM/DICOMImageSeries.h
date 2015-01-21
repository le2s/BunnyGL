#ifdef VTK5_FOUND

#ifndef DICOM_IMAGE_SERIES_H
#define DICOM_IMAGE_SERIES_H

#include <QObject>
#include <QImage>

class vtkImageData;
class DICOMImageSeriesMPR;
class DICOMImageSeriesVolume;

struct DICOMImageSeriesData;
class DICOMImageSeries : public QObject
{
    Q_OBJECT

public:
    // This static method helps load one DICOM series from a directory.
    static DICOMImageSeries* GetImageSeries(const QString& dirName, DICOMImageSeries* except=0);

    // This static method returns some quick meta-data about the DICOM series available in a directory
    static bool LoadInformation(const QString& dirName, QString& patientName, QImage& sampleImage, int& sliceCount);

    // This static method helps convert a vtkImageData object into QImage.
    // vtkImageData is VTK's class for 2D and 3D images.
    // Visit http://www.vtk.org for more information about VTK
    // Visit http://www.vtk.org/doc/nightly/html/classvtkImageData.html for more information about vtkImageData
    static QImage vtkImageToQImage(vtkImageData* image);
    ~DICOMImageSeries();

    // A single instance of this object can be shared among several users. We use
    // reference counting to manage the life-time of instances of DICOMImageSeries
    void ref();
    bool deref();
    bool isReffed() const;

    bool isValid() const;
    void reset();

    QString directoryName() const;

    // Finding out meta-data
    int sliceCount() const;
    int sliceImageWidth() const;
    int sliceImageHeight() const;

    // Browsing slices
    int currentSliceIndex() const;
    QImage currentSliceImage() const;
    QImage currentSliceImageFullRes() const;
    QImage nextSliceImage(int count) const;
    QImage previousSliceImage(int count) const;
    QImage sliceImage(int index) const;

    // Image processing functions
    double windowLevel() const;
    double windowWidth() const;
    void setWindowLevelAndWidth(double wl, double ww);

    // Information functions
    QString patientName() const;
    QString studyUID() const;
    QString studyID() const;
    QString dimensions() const;
    QString pixelSpacing() const;

    // MPR Support
    DICOMImageSeriesMPR* mpr() const;

    // Volume Support
    DICOMImageSeriesVolume* volume() const;

protected:
    DICOMImageSeries(const QString& dirName, QObject* parent=0);
    DICOMImageSeries(DICOMImageSeries* basedOn);

    void timerEvent(QTimerEvent* te);

private:
    DICOMImageSeriesData* d;
};

#endif

#endif // VTK5_FOUND
