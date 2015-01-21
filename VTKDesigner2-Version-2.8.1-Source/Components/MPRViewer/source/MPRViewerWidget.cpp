/****************************************************************************
**
** Copyright (C) VCreateLogic
**
** Use of this file is limited according to the terms specified by
** VCreateLogic.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "MPRViewerWidget.h"
#include "ui_MPRViewerForm.h"

#include "vtkImageData.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkImageViewer2.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkAlgorithmOutput.h"
#include "vtkAlgorithm.h"
#include "QVTKWidget.h"

#include "vtkImageReslice.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkTexture.h"
#include "vtkPlaneSource.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkTransform.h"

#include <QDebug>
#include <QShowEvent>

#define AXIAL 0
#define SAGITTAL 1
#define CORONAL 2
#define OBLIQUE 3

class DICOMPipeline
{
public:

    enum Orientation   {
        OrientationXY,
        OrientationYZ,
        OrientationXZ,
    };

    DICOMPipeline()
    {
        this->reslicer = vtkImageReslice::New();
        this->imToWLC = vtkImageMapToWindowLevelColors::New();
        this->texture = vtkTexture::New();
        this->dataSetMapper = vtkDataSetMapper::New();
        this->actor = vtkActor::New();
        this->renderer = vtkRenderer::New();
        this->transform = vtkTransform::New();
        
        this->planeSource = 0;
        this->dicomImage = 0;

        this->reslicer->SetOutputDimensionality(2);
    }

    ~DICOMPipeline()
    {
        planeSource->Delete();
        dicomImage->Delete();
        reslicer->Delete();
        imToWLC->Delete();
        texture->Delete();
        dataSetMapper->Delete();
        actor->Delete();
        renderer->Delete();
    }

    void connectPipeline()
    {
        if(!planeSource)
            return;

        this->imToWLC->SetInput(reslicer->GetOutput());
        this->texture->SetInput(imToWLC->GetOutput());
        this->dataSetMapper->SetInput(planeSource->GetOutput());
        this->actor->SetMapper(dataSetMapper);
        this->actor->SetTexture(texture);
        this->renderer->AddViewProp(actor);
    }

    void setDicomImage(vtkImageData* image)
    {
        if(!image || (this->dicomImage == image))
            return;

        this->dicomImage = image;
        this->reslicer->SetInput(image);

        this->dicomImage->GetDimensions(dim);

        this->transform->Identity();
        this->transform->Translate(dim[0]/2, dim[1]/2, dim[2]/2);
        if(this->orientation == OrientationYZ)
            this->transform->RotateY(90);
        else if(this->orientation == OrientationXZ)
            this->transform->RotateX(90);
        this->reslicer->SetResliceAxes(this->transform->GetMatrix());
    }

    void setOrientationXY()
    {
        vtkCamera *cam = this->renderer ? this->renderer->GetActiveCamera() : NULL;

        if(!cam)
            return;

        this->orientation = OrientationXY;
        
        cam->SetFocalPoint(0,0,0);
        cam->SetPosition(0,0,1); // -1 if medical ?
        cam->SetViewUp(0,1,0);
    }

    void setOrientationYZ()
    {
        vtkCamera *cam = this->renderer ? this->renderer->GetActiveCamera() : NULL;
        
        if(!cam)
            return;
        
        this->orientation = OrientationYZ;

        /*
        cam->SetFocalPoint(0,0,0);
        cam->SetPosition(0,-1,0); // 1 if medical ?
        cam->SetViewUp(0,0,1);
        */
    }

    void setOrientationXZ()
    {
        vtkCamera *cam = this->renderer ? this->renderer->GetActiveCamera() : NULL;
        
        if(!cam)
            return;
        
        this->orientation = OrientationXZ;

        /*
        cam->SetFocalPoint(0,0,0);
        cam->SetPosition(1,0,0); // -1 if medical ?
        cam->SetViewUp(0,0,1);
        */
    }

    void setSlice(const int val)
    {
        this->transform->Identity();

        switch(this->orientation)
        {
        case OrientationXY:
            this->transform->Translate(dim[0]/2, dim[1]/2, val);
            break;
        
        case OrientationYZ:
            this->transform->Translate(val, dim[1]/2, dim[2]/2);
            this->transform->RotateY(90);
            break;
        
        case OrientationXZ:
            this->transform->Translate(dim[0]/2, val, dim[2]/2);
            this->transform->RotateX(90);
            break;
        }

        this->reslicer->SetResliceAxes(this->transform->GetMatrix());
        this->renderer->GetRenderWindow()->Render();
    }

    void setWindow(const double val)
    {
        this->imToWLC->SetWindow(val);
    }

    void setLevel(const double val)
    {
        this->imToWLC->SetLevel(val);
    }

    vtkPlaneSource* planeSource;
    vtkImageData* dicomImage;

    // Pipeline
    vtkImageReslice* reslicer;
    vtkImageMapToWindowLevelColors* imToWLC;
    vtkTexture* texture;
    vtkDataSetMapper* dataSetMapper;
    vtkActor* actor;
    vtkRenderer* renderer;
    vtkTransform* transform;

    Orientation orientation;
    int dim[3];
};

struct MPRViewerWidgetData
{
    MPRViewerWidgetData() : dicomImage(0) { }

	Ui::MPRViewerForm ui;

    vtkImageData* dicomImage;
    vtkPlaneSource* planeSource;

    QAbstractSlider* sliders[3];
    DICOMPipeline* pipelines[4];
};

MPRViewerWidget::MPRViewerWidget(QWidget* parent)
				: QWidget(parent)
{
	d = new MPRViewerWidgetData;
	d->ui.setupUi(this);

    d->sliders[AXIAL] = d->ui.axialSlider;
    d->sliders[SAGITTAL] = d->ui.sagittalSlider;
    d->sliders[CORONAL] = d->ui.coronalSlider;

    d->planeSource = vtkPlaneSource::New();
    for(int i=0; i<3; i++)
    {
        d->pipelines[i] = new DICOMPipeline;
        d->pipelines[i]->planeSource = d->planeSource;
        d->pipelines[i]->connectPipeline();
    }

    d->pipelines[AXIAL]->setOrientationXY();
    d->pipelines[SAGITTAL]->setOrientationYZ();
    d->pipelines[CORONAL]->setOrientationXZ();

    d->ui.axialView->GetRenderWindow()->AddRenderer(d->pipelines[AXIAL]->renderer);
    d->ui.sagittalView->GetRenderWindow()->AddRenderer(d->pipelines[SAGITTAL]->renderer);
    d->ui.coronalView->GetRenderWindow()->AddRenderer(d->pipelines[CORONAL]->renderer);

    connect(d->ui.axialSlider, SIGNAL(valueChanged(int)), this, SLOT(axialSliderValueChanged(int)), Qt::QueuedConnection);
    connect(d->ui.sagittalSlider, SIGNAL(valueChanged(int)), this, SLOT(sagittalSliderValueChanged(int)), Qt::QueuedConnection);
    connect(d->ui.coronalSlider, SIGNAL(valueChanged(int)), this, SLOT(coronalSliderValueChanged(int)), Qt::QueuedConnection);
}

MPRViewerWidget::~MPRViewerWidget()
{
	delete d;
}

void MPRViewerWidget::setDicomImage(vtkImageData* image)
{
    if(!image || d->dicomImage == image)
        return;

    d->dicomImage = image;

    for(int i=0; i<3; i++)
    {
        d->pipelines[i]->setDicomImage(image);
        d->pipelines[i]->setWindow(400);
        d->pipelines[i]->setLevel(60);
    }

    // Extract the middle slice
    int dim[3];
    if(!d->dicomImage)
        return;

    d->dicomImage->GetDimensions(dim);
    d->sliders[AXIAL]->setRange( 0, dim[2] - 1);
    d->sliders[AXIAL]->setValue( dim[2] / 2 );
    d->sliders[SAGITTAL]->setRange( 0, dim[0] - 1);
    d->sliders[SAGITTAL]->setValue( dim[0] / 2 );
    d->sliders[CORONAL]->setRange( 0, dim[1] - 1);
    d->sliders[CORONAL]->setValue( dim[1] / 2 );
}

vtkImageData* MPRViewerWidget::dicomImage() const
{
    return d->dicomImage;
}

void MPRViewerWidget::axialSliderValueChanged(int val)
{
    d->pipelines[AXIAL]->setSlice(val);
}

void MPRViewerWidget::sagittalSliderValueChanged(int val)
{
    d->pipelines[SAGITTAL]->setSlice(val);
}

void MPRViewerWidget::coronalSliderValueChanged(int val)
{
    d->pipelines[CORONAL]->setSlice(val);
}

void MPRViewerWidget::showEvent(QShowEvent* se)
{
    QWidget::showEvent(se);

    /*
    // Ensure that the input image is constructed
    if(!d->dicomImage)
        return;

    vtkAlgorithmOutput* producerPort = d->dicomImage->GetProducerPort();
    if(!producerPort)
        return;

    vtkAlgorithm* producer = producerPort->GetProducer();
    if(!producer)
        return;

    producer->Update();
    */
}

