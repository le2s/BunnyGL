#ifdef VTK5_FOUND

#ifndef DICOMIMAGESERIESVOLUME_H
#define DICOMIMAGESERIESVOLUME_H

#include <QObject>
#include <QImage>

class DICOMImageSeries;

struct DICOMImageSeriesVolumeData;
class DICOMImageSeriesVolume : public QObject
{
    Q_OBJECT

public:
    DICOMImageSeriesVolume(DICOMImageSeries *parent = 0);
    ~DICOMImageSeriesVolume();

    QImage prepareVolume();
    QImage currentVolumeImage() const;
    QImage mousePressOnVolume(double x, double y);
    QImage mouseMoveOnVolume(double x, double y);
    QImage mouseReleaseOnVolume(double x, double y);
    QImage mouseWheelOnVolume(double x, double y, int delta);
    QImage resetCameraOnVolume();

protected:
    QImage renderVolumeImage();
    
private:
    DICOMImageSeriesVolumeData* d;
};

#endif // DICOMIMAGESERIESVOLUME_H

#endif
