#ifdef VTK5_FOUND

#ifndef DICOM_IMAGE_SERIES_MPR_H
#define DICOM_IMAGE_SERIES_MPR_H

#include <QObject>
#include <QImage>
#include <QPair>
#include "vtkDICOMImageReader.h"

class DICOMImageSeries;
class vtkImageData;

struct DICOMImageSeriesMPRData;
class DICOMImageSeriesMPR : public QObject
{
    Q_OBJECT

public:
    ~DICOMImageSeriesMPR();

    void reset();

    QImage currentAxialSliceImage() const;
    QImage currentSagittalSliceImage() const;
    QImage currentCoronalSliceImage() const;

    QImage currentAxialSliceImageFullRes() const;
    QImage currentSagittalSliceImageFullRes() const;
    QImage currentCoronalSliceImageFullRes() const;

    int currentAxialSliceIndex() const;
    int currentSagittalSliceIndex() const;
    int currentCoronalSliceIndex() const;

    int axialSliceCount() const;
    int coronalSliceCount() const;
    int sagittalSliceCount() const;

    QImage axialSliceImage(int sliceNumber) const;
    QImage sagittalSliceImage(int sliceNumber) const;
    QImage coronalSliceImage(int sliceNumber) const;

    QImage nextAxialSliceImage(int count) const;
    QImage nextSagittalSliceImage(int count) const;
    QImage nextCoronalSliceImage(int count) const;

    QImage previousAxialSliceImage(int count) const;
    QImage previousSagittalSliceImage(int count) const;
    QImage previousCoronalSliceImage(int count) const;

    double windowLevel() const;
    double windowWidth() const;
    void setWindowLevelAndWidth(double wl, double ww);

protected:
    DICOMImageSeriesMPR(DICOMImageSeries* parent);

private:
    friend class DICOMImageSeries;
    DICOMImageSeriesMPRData* d;
};

struct DICOMImageSlicerData;
class DICOMImageSlicer
{
public:
    enum Axis
    {
        AxialAxis,
        CoronalAxis,
        SagittalAxis
    };

    ~DICOMImageSlicer();

    void reset();

    int sliceCount() const;

    int currentSliceIndex() const;
    void setCurrentSliceIndex(int index);
    void next(int count);
    void previous(int count);
    void recomputeCurrentSlice();

    QImage currentSliceImage() const;
    QImage currentSliceImageFullRes() const;

    double windowLevel() const;
    double windowWidth() const;
    void setWindowLevelAndWidth(double wl, double ww);

protected:
    DICOMImageSlicer(vtkImageData* dcmImage, Axis axis=AxialAxis);

private:
    friend class DICOMImageSeriesMPR;
    DICOMImageSlicerData* d;
};

class vtkDICOMImageReader2 : public vtkDICOMImageReader
{
public:
    static vtkDICOMImageReader2* New(const QString& dirName);
    QString DirectoryName;
    vtkDICOMImageReader2();
    ~vtkDICOMImageReader2();
};

#endif

#endif


