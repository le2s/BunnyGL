#ifdef VTK5_FOUND

#include "DICOMImageSeriesVolume.h"
#include "DICOMImageSeries.h"
#include "DICOMImageSeriesMPR.h" // for vtkDICOMImageReader2

#include "vtkFixedPointVolumeRayCastMapper.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolume.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkDICOMImageReader.h"
#include "vtkImageData.h"

#include "vtkSmartPointer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCommand.h"
#include "vtkInteractorStyleTrackballCamera.h"

struct DICOMImageSeriesVolumeData
{
    DICOMImageSeriesVolumeData() : prepareDone(false), imageData(0) { }

    DICOMImageSeries* series;

    bool prepareDone;
    vtkSmartPointer<vtkDICOMImageReader> dicomReader;
    vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> mapper;
    vtkSmartPointer<vtkColorTransferFunction> colorFunc;
    vtkSmartPointer<vtkPiecewiseFunction> opacityFunc;
    vtkSmartPointer<vtkVolumeProperty> volumeProperty;
    vtkSmartPointer<vtkVolume> volume;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderWindow> renderWindow;

    unsigned char* imageData;
    QImage volumeImageHighRes;
    QImage volumeImageLowRes;
};

DICOMImageSeriesVolume::DICOMImageSeriesVolume(DICOMImageSeries *parent)
    : QObject(parent)
{
    d = new DICOMImageSeriesVolumeData;
    d->series = parent;
}

DICOMImageSeriesVolume::~DICOMImageSeriesVolume()
{
    if(d->imageData)
        delete d->imageData;
    delete d;
}

QImage DICOMImageSeriesVolume::prepareVolume()
{
    if(d->prepareDone)
        return d->volumeImageHighRes;

    d->dicomReader = vtkDICOMImageReader2::New(d->series->directoryName());

    d->mapper = vtkFixedPointVolumeRayCastMapper::New();
    d->mapper->SetInput( d->dicomReader->GetOutput() );

    const double samplingValue = 0.5; // MainWindow::instance()->getSamplingValue();
    d->mapper->SetSampleDistance(samplingValue);

    d->colorFunc = vtkColorTransferFunction::New();
    d->opacityFunc = vtkPiecewiseFunction::New();

    d->opacityFunc->AddSegment(450,0.0,500,1.0);
    d->opacityFunc->AddSegment(-450,0.0,-400,0.1);

    d->colorFunc->AddRGBSegment(500,1.0,.95,.80,450,1.0,1.0,1.0);
    d->colorFunc->AddRGBSegment(-425,0.0,0.0,0.0,-400,1.0,.45,.32);

    d->volumeProperty = vtkVolumeProperty::New();
    d->volumeProperty->SetIndependentComponents(true);
    d->volumeProperty->SetScalarOpacity(d->opacityFunc);
    d->volumeProperty->SetColor(d->colorFunc);
    d->volumeProperty->SetInterpolationTypeToLinear();
    d->volumeProperty->ShadeOn();
    d->volumeProperty->SetDiffuse(.9);
    d->volumeProperty->SetScalarOpacityUnitDistance(.5);

    d->volume = vtkVolume::New();
    d->volume->SetMapper(d->mapper);
    d->volume->SetProperty(d->volumeProperty);

    d->renderer = vtkRenderer::New();
    d->renderer->AddVolume(d->volume);
    d->renderer->SetBackground(0,0,0);

    d->renderWindow = vtkRenderWindow::New();
    d->renderWindow->SetOffScreenRendering(1);
    d->renderWindow->SetSize(512, 512);
    d->renderWindow->AddRenderer(d->renderer);
    d->renderWindow->Render();

    vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
    vtkInteractorStyle* s = vtkInteractorStyleTrackballCamera::New();
    iren->SetInteractorStyle(s);
    d->renderWindow->SetInteractor(iren);
    iren->Delete();

    d->prepareDone = true;

    return d->volumeImageHighRes;
}

QImage DICOMImageSeriesVolume::currentVolumeImage() const
{
    return d->volumeImageHighRes;
}

QImage DICOMImageSeriesVolume::mousePressOnVolume(double x, double y)
{
    x *= d->renderWindow->GetSize()[0];
    y *= d->renderWindow->GetSize()[1];
    vtkRenderWindowInteractor* iren = d->renderWindow->GetInteractor();
    iren->SetEventInformation(x, y);
    iren->InvokeEvent(vtkCommand::LeftButtonPressEvent);
    return this->renderVolumeImage();
}

QImage DICOMImageSeriesVolume::mouseMoveOnVolume(double x, double y)
{
    x *= d->renderWindow->GetSize()[0];
    y *= d->renderWindow->GetSize()[1];
    vtkRenderWindowInteractor* iren = d->renderWindow->GetInteractor();
    iren->SetEventInformation(x, y);
    iren->InvokeEvent(vtkCommand::MouseMoveEvent);
    return this->renderVolumeImage();
}

QImage DICOMImageSeriesVolume::mouseReleaseOnVolume(double x, double y)
{
    x *= d->renderWindow->GetSize()[0];
    y *= d->renderWindow->GetSize()[1];
    vtkRenderWindowInteractor* iren = d->renderWindow->GetInteractor();
    iren->SetEventInformation(x, y);
    iren->InvokeEvent(vtkCommand::LeftButtonReleaseEvent);
    return this->renderVolumeImage();
}

QImage DICOMImageSeriesVolume::mouseWheelOnVolume(double x, double y, int delta)
{
    x *= d->renderWindow->GetSize()[0];
    y *= d->renderWindow->GetSize()[1];
    vtkRenderWindowInteractor* iren = d->renderWindow->GetInteractor();
    iren->SetEventInformation(x, y);

    bool wheelUp = delta > 0;
    delta = qAbs(delta);
    for(int i=0; i<delta; i++)
    {
        if(wheelUp)
            iren->InvokeEvent(vtkCommand::MouseWheelForwardEvent);
        else
            iren->InvokeEvent(vtkCommand::MouseWheelBackwardEvent);
    }

    return this->renderVolumeImage();
}

QImage DICOMImageSeriesVolume::resetCameraOnVolume()
{
    d->renderer->ResetCamera();
    return this->renderVolumeImage();
}

QImage DICOMImageSeriesVolume::renderVolumeImage()
{
    // GCF_PROFILE_THIS_FUNCTION;
    d->renderWindow->Render();

    int w = d->renderWindow->GetSize()[0];
    int h = d->renderWindow->GetSize()[1];
    if(d->imageData)
        delete d->imageData;
    d->imageData = d->renderWindow->GetPixelData(0, 0, w-1, h-1, 1);
    if(d->imageData)
    {
        QImage image(d->imageData, w, h, QImage::Format_RGB888);
        d->volumeImageHighRes = image;
        d->volumeImageLowRes = image.scaled(64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    else
    {
        // We should never ever land here..
        QImage nullImage(d->renderWindow->GetSize()[0],
                         d->renderWindow->GetSize()[1],
                         QImage::Format_RGB888);
        nullImage.fill(Qt::color0);

        d->volumeImageHighRes = nullImage;
        d->volumeImageLowRes = nullImage.scaled(64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

    return d->volumeImageLowRes;
}

#endif
