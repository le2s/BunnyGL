/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore. Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CVtkOpenHapticsRenderer.h"
#include "COpenHapticsVTKComponent.h"
#include "CVtkOpenHapticsActor.h"
#include "IVtkProp3DIOData.h"
#include "IVtkCameraIOData.h"
#include "IVtkOpenHapticsEffectIOData.h"
#include "CVtkRendererData.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkOpenHapticsRenderer.h"
#include "vtkLightCollection.h"
#include "vtkLightKit.h"
#include "vtkLight.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include <QMessageBox>

struct CVtkOpenHapticsRendererData : public vtkCommand
{
    CVtkOpenHapticsRenderer* hapticsRendererWrapper;
	vtkOpenHapticsRenderer* hapticsRenderer;
	CVtkRendererData rendererData;
	bool hasUserCamera;
    int pollInterval;
    int pollTimerID;

    void Execute(vtkObject *caller, unsigned long eventId, void *)
    {
        if(caller != hapticsRenderer)
            return;

        switch(eventId)
        {
        case vtkOpenHapticsRenderer::TouchEvent:
            emit hapticsRendererWrapper->OnHapticTouch();
            break;
        case vtkOpenHapticsRenderer::UnTouchEvent:
            emit hapticsRendererWrapper->OnHapticUnTouch();
            break;
        case vtkOpenHapticsRenderer::MotionEvent:
            if(!hapticsRenderer->GetTouched())
                CVtkOpenHapticsActor::GetCurrentHapticsActor() = 0;
            emit hapticsRendererWrapper->OnHapticMotion();
            break;
        case vtkOpenHapticsRenderer::Button1DownEvent:
            emit hapticsRendererWrapper->OnHapticButton1Down();
            break;
        case vtkOpenHapticsRenderer::Button1UpEvent:
            emit hapticsRendererWrapper->OnHapticButton1Up();
            break;
        case vtkOpenHapticsRenderer::Button2DownEvent:
            emit hapticsRendererWrapper->OnHapticButton2Down();
            break;
        case vtkOpenHapticsRenderer::Button2UpEvent:
            emit hapticsRendererWrapper->OnHapticButton2Up();
            break;
        }
    }
};

DEFINE_VIS_NODE(CVtkOpenHapticsRenderer, CGenericVisNodeBase)
{
	pDesc->setNodeClassCategory("Open Haptics + VTK");
	pDesc->setNodeClassName("vtkOpenHapticsRenderer");
	pDesc->setNodeClassDescription("Open Haptics Cursor Renderer");
    pDesc->setNodeIcon( COpenHapticsVTKComponent::instance().nodeIcon() );

    // Actor Input
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath("Prop3D", IVisSystemNodeConnectionPath::InputPath, "vtkProp3D", 0, true)
        );

    // Camera Input
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath("Camera", IVisSystemNodeConnectionPath::InputPath, "vtkCamera", 0)
        );

    // Effect Input
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath("Effect", IVisSystemNodeConnectionPath::InputPath, "vtkOpenHapticsEffect", 0)
        );

    // Renderer Output
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath("OpenHapticsRenderer", IVisSystemNodeConnectionPath::OutputPath, "vtkOpenHapticsRenderer", 0)
        );

    // Renderer Output
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath("Renderer", IVisSystemNodeConnectionPath::OutputPath, "vtkOpenHapticsActor", 0)
        );
}

static CVtkOpenHapticsRenderer* CVtkOpenHapticsRendererInstancePointer = 0;
CVtkOpenHapticsRenderer* CVtkOpenHapticsRenderer::GetInstancePointer()
{
    return CVtkOpenHapticsRendererInstancePointer;
}

CVtkOpenHapticsRenderer::CVtkOpenHapticsRenderer()
{
    CVtkOpenHapticsRenderer::InitializeNodeDesc();
	d = new CVtkOpenHapticsRendererData;
	d->hapticsRenderer = vtkOpenHapticsRenderer::New();
	d->hasUserCamera = false;
    d->pollInterval = 100;
    d->pollTimerID = -1;
    d->hapticsRendererWrapper = this;
    d->hapticsRenderer->AddObserver(vtkCommand::AnyEvent, d);
    setBackgroundColor(QColor(0, 85, 127));

    CVtkOpenHapticsRendererInstancePointer = this;
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->hapticsRenderer);
}

CVtkOpenHapticsRenderer::~CVtkOpenHapticsRenderer()
{
	if(d->hapticsRenderer)
		d->hapticsRenderer->Delete();
	delete d;

    CVtkOpenHapticsRendererInstancePointer = 0;
}

void CVtkOpenHapticsRenderer::setHapticPollInterval(int mSec)
{
    if(d->pollInterval == 0)
        return;

    d->pollInterval = mSec;   
    if(d->pollTimerID >= 0)
        killTimer(d->pollTimerID);
    d->pollTimerID = -1;
    if(!d->pollInterval)
        return;
    d->pollTimerID = startTimer(d->pollInterval);
}

int CVtkOpenHapticsRenderer::hapticPollInterval() const
{
    return d->pollInterval;
}

void CVtkOpenHapticsRenderer::setBackgroundColor(QColor color)
{
    d->hapticsRenderer->SetBackground(color.redF(), color.greenF(), color.blueF());
}

QColor CVtkOpenHapticsRenderer::backgroundColor() const
{
    QColor color;
    double* bgColor = d->hapticsRenderer->GetBackground();
    color.setRgbF(bgColor[0], bgColor[1], bgColor[2]);
    return color;
}

void CVtkOpenHapticsRenderer::setLightsFollowCamera(bool val)
{
    d->hapticsRenderer->SetLightFollowCamera(val);
}

bool CVtkOpenHapticsRenderer::isLightsFollowCamera() const
{
    return d->hapticsRenderer->GetLightFollowCamera();
}

void CVtkOpenHapticsRenderer::setTwoSidedLighting(bool val)
{
    d->hapticsRenderer->SetTwoSidedLighting(val);
}

bool CVtkOpenHapticsRenderer::isTwoSidedLighting() const
{
    return d->hapticsRenderer->GetTwoSidedLighting();
}

void CVtkOpenHapticsRenderer::setViewport(QRectF vRect)
{
    d->hapticsRenderer->SetViewport(vRect.left(), vRect.bottom(), vRect.right(), vRect.top());
}

QRectF CVtkOpenHapticsRenderer::viewport() const
{
    double* v = d->hapticsRenderer->GetViewport();
    return QRectF(v[0], v[1], v[2]-v[0], v[3]-v[1]);
}

void CVtkOpenHapticsRenderer::setEnableDirectProxyRendering(bool val)
{
    d->hapticsRenderer->SetEnableDirectProxyRendering(val);
}

bool CVtkOpenHapticsRenderer::isEnableDirectProxyRendering() const
{
    return d->hapticsRenderer->GetEnableDirectProxyRendering();
}

void CVtkOpenHapticsRenderer::setShapeRendering(CVtkOpenHapticsRenderer::ShapeRenderingType type)
{
    int iType = int(type);
    vtkOpenHapticsRenderer::ShapeRenderingType aType 
        = vtkOpenHapticsRenderer::ShapeRenderingType(iType);
    d->hapticsRenderer->SetShapeRendering(aType);
}

CVtkOpenHapticsRenderer::ShapeRenderingType CVtkOpenHapticsRenderer::shapeRendering() const
{
    vtkOpenHapticsRenderer::ShapeRenderingType aType 
        = d->hapticsRenderer->GetShapeRendering();
    
    CVtkOpenHapticsRenderer::ShapeRenderingType type 
        = CVtkOpenHapticsRenderer::ShapeRenderingType( int(aType) );

    return type;
}

void CVtkOpenHapticsRenderer::createAutomaticLights()
{
    vtkLightCollection* lCol = d->hapticsRenderer->GetLights();
    while(lCol->GetNumberOfItems())
    {
        vtkLight* light = dynamic_cast<vtkLight*>(lCol->GetItemAsObject(0));
        if(light)
            d->hapticsRenderer->RemoveLight(light);
    }

    vtkLightKit* lightKit = vtkLightKit::New();
    lightKit->AddLightsToRenderer(d->hapticsRenderer);
    lightKit->Delete();
}

Point3D CVtkOpenHapticsRenderer::toScreenCoordinates(Point3D p)
{
    d->hapticsRenderer->SetWorldPoint(p.x, p.y, p.y, 1);
    d->hapticsRenderer->WorldToDisplay();
    return d->hapticsRenderer->GetDisplayPoint();
}

Point3D CVtkOpenHapticsRenderer::toSceneCoordinates(Point3D p)
{
    int* size = d->hapticsRenderer->GetRenderWindow()->GetSize();
    d->hapticsRenderer->SetDisplayPoint(p.x, size[1]-p.y, 1.0);
    d->hapticsRenderer->DisplayToWorld();
    double* world = d->hapticsRenderer->GetWorldPoint();
    if(world[3] == 0.0)
        return Point3D();
    for(int i=0; i < 3; i++)
        world[i] = world[i] / world[3];
    return Point3D(world);
}

void CVtkOpenHapticsRenderer::flyTo(Point3D point, int nrFrames)
{
    vtkRenderWindow* renderWindow = d->hapticsRenderer->GetRenderWindow();
    if(!renderWindow)
        return;

    vtkRenderWindowInteractor* rWinInt = renderWindow->GetInteractor();
    if(!rWinInt)
        return;

    rWinInt->SetNumberOfFlyFrames(nrFrames);
    rWinInt->FlyTo(d->hapticsRenderer, point.x, point.y, point.z);
}

QObject* CVtkOpenHapticsRenderer::currentHapticActor()
{
    CVtkOpenHapticsActor* currentActor = CVtkOpenHapticsActor::GetCurrentHapticsActor();
    return currentActor;
}

bool CVtkOpenHapticsRenderer::hasCurrentHapticActor()
{
    return currentHapticActor() != 0;
}

Point3D CVtkOpenHapticsRenderer::oldProxyPosition()
{
    return d->hapticsRenderer->GetOldProxyPosition();
}

Point3D CVtkOpenHapticsRenderer::proxyPosition()
{
    return d->hapticsRenderer->GetProxyPosition();
}

QList<double> CVtkOpenHapticsRenderer::oldProxyRotation()
{
    QList<double> ret;
    for(int i=0; i<4; i++)
        ret << d->hapticsRenderer->GetOldProxyRotation()[i];
    return ret;
}

QList<double> CVtkOpenHapticsRenderer::proxyRotation()
{
    QList<double> ret;
    for(int i=0; i<4; i++)
        ret << d->hapticsRenderer->GetProxyRotation()[i];
    return ret;
}

QList<double> CVtkOpenHapticsRenderer::proxyTransform()
{
    QList<double> ret;
    for(int i=0; i<16; i++)
        ret << d->hapticsRenderer->GetProxyTransform()[i];
    return ret;
}

QList<double> CVtkOpenHapticsRenderer::oldProxyTransform()
{
    QList<double> ret;
    for(int i=0; i<16; i++)
        ret << d->hapticsRenderer->GetOldProxyTransform()[i];
    return ret;
}

Vector3D CVtkOpenHapticsRenderer::deviceForceSent()
{
    return d->hapticsRenderer->GetDeviceForceSent();
}

Vector3D CVtkOpenHapticsRenderer::deviceTorqueSent()
{
    return d->hapticsRenderer->GetDeviceTorqueSent();
}

Vector3D CVtkOpenHapticsRenderer::deviceForceReceived()
{
    return d->hapticsRenderer->GetDeviceForceReceived();
}

Vector3D CVtkOpenHapticsRenderer::deviceTorqueReceived()
{
    return d->hapticsRenderer->GetDeviceTorqueReceived();
}

bool CVtkOpenHapticsRenderer::isTouched()
{
    return d->hapticsRenderer->GetTouched();
}

bool CVtkOpenHapticsRenderer::isButton1Down()
{
    return d->hapticsRenderer->GetButton1Down();
}

bool CVtkOpenHapticsRenderer::isButton2Down()
{
    return d->hapticsRenderer->GetButton2Down();
}

void CVtkOpenHapticsRenderer::resetCamera()
{
    if(d->hasUserCamera)
        return;

    command_ResetCamera();
}

void CVtkOpenHapticsRenderer::render()
{
    command_Render();
}

void CVtkOpenHapticsRenderer::setProxyPosition(Point3D pos)
{
    d->hapticsRenderer->SetProxyPosition(pos.x, pos.y, pos.z);
}

void CVtkOpenHapticsRenderer::setProxyRotation(QList<double> wxyz)
{
    d->hapticsRenderer->SetProxyRotation(wxyz[0], wxyz[1], wxyz[2], wxyz[3]);
}

void CVtkOpenHapticsRenderer::command_CreateLights()
{
    this->createAutomaticLights();
}

void CVtkOpenHapticsRenderer::command_ResetCamera()
{
    if(d->hasUserCamera)
    {
        QMessageBox::information(0, "Reset Camera Denied",
                "Cannot reset the camera, because it is a user defined camera");
    }
    else
        d->hapticsRenderer->ResetCamera();
}

void CVtkOpenHapticsRenderer::command_Render()
{
    if(d->hapticsRenderer->GetRenderWindow())
        d->hapticsRenderer->Render();
}

bool CVtkOpenHapticsRenderer::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Prop3D")
    {
        return d->hapticsRenderer->GetActors()->GetNumberOfItems() != 0 ||
               d->hapticsRenderer->GetVolumes()->GetNumberOfItems() != 0;
    }

    if(path->pathName() == "Camera")
    {
        return d->hasUserCamera;
    }

    return false;
}

bool CVtkOpenHapticsRenderer::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Prop3D")
    {
        IVtkProp3DIOData* propData = 0;
        bool success = inputData->queryInterface("IVtkProp3DIOData", (void**)&propData);
        if(success && propData)
        {
            vtkProp3D* actor = propData->getVtkProp3D();
			d->hapticsRenderer->AddActor(actor);
            if(d->hapticsRenderer->GetRenderWindow())
                d->hapticsRenderer->GetRenderWindow()->Render();
            return true;
        }

        return false;
    }

    if(path->pathName() == "Camera")
    {
        IVtkCameraIOData* cameraData = 0;
        bool success = inputData->queryInterface("IVtkCameraIOData", (void**)&cameraData);
        if(success && cameraData)
        {
            d->hapticsRenderer->SetActiveCamera(cameraData->getVtkCamera());
            d->hasUserCamera = true;
            if(d->hapticsRenderer->GetRenderWindow())
                d->hapticsRenderer->GetRenderWindow()->Render();
            return true;
        }

        return false;
    }

    if(path->pathName() == "Effect")
    {
        IVtkOpenHapticsEffectIOData* effectData = 0;
        bool success = inputData->queryInterface("IVtkOpenHapticsEffectIOData", (void**)&effectData);
        if(success && effectData)
        {
            d->hapticsRenderer->AddEffect(effectData->getVtkOpenHapticsEffect());
            return true;
        }

        return false;
    }

    return false;
}

bool CVtkOpenHapticsRenderer::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Prop3D")
    {
        IVtkProp3DIOData* propData = 0;
        bool success = inputData->queryInterface("IVtkProp3DIOData", (void**)&propData);
        if(success && propData)
        {
            vtkProp3D* actor = propData->getVtkProp3D();
            d->hapticsRenderer->RemoveActor(actor);
            return true;
        }

        return false;
    }

    if(path->pathName() == "Camera")
    {
        IVtkCameraIOData* cameraData = 0;
        bool success = inputData->queryInterface("IVtkCameraIOData", (void**)&cameraData);
        if(success && cameraData)
        {
            d->hapticsRenderer->SetActiveCamera(0);
            d->hasUserCamera = false;
            return true;
        }

        return false;
    }

    if(path->pathName() == "Effect")
    {
        IVtkOpenHapticsEffectIOData* effectData = 0;
        bool success = inputData->queryInterface("IVtkOpenHapticsEffectIOData", (void**)&effectData);
        if(success && effectData)
        {
            d->hapticsRenderer->RemoveEffect(effectData->getVtkOpenHapticsEffect());
            return true;
        }

        return false;
    }

    return false;
}

bool CVtkOpenHapticsRenderer::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "OpenHapticsRenderer" || path->pathName() == "Renderer")
    {
        if(d->rendererData.getVtkRenderer())
            return false;

        d->rendererData.setRenderer(d->hapticsRenderer);
        *outputData = &d->rendererData;
        return true;
    }

	return false;
}

bool CVtkOpenHapticsRenderer::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "OpenHapticsRenderer" || path->pathName() == "Renderer")
    {
        d->rendererData.setRenderer(0);
        return true;
    }

	return false;
}

void CVtkOpenHapticsRenderer::initializeNode()
{
	CGenericVisNodeBase::initializeNode();
    d->pollTimerID = startTimer(d->pollInterval);
}

void CVtkOpenHapticsRenderer::finalizeNode()
{
    setHapticPollInterval(0); // stop the timer.
	if(d->hapticsRenderer)
    {
        d->hapticsRenderer->SetRenderWindow(0);
		d->hapticsRenderer->Delete();
    }
	d->hapticsRenderer = 0;
}

// #define QtEvent(x) (QEvent::Type)(int(x) - int(vtkCommand::UserEvent))

void CVtkOpenHapticsRenderer::handleVtkEvent(unsigned long event, void*)
{
	/*
    QEvent::Type qtEvent = QtEvent(event);
    switch(qtEvent)
    {
    case QEvent::MouseButtonPress: {
        QMouseEvent* me = static_cast<QMouseEvent*>(callData);
        Point3D pt(me->x(), me->y(), 0);
        pt = toSceneCoordinates(pt);
        emit OnMousePress(pt);
        }
        break;
    case QEvent::MouseMove: {
        QMouseEvent* me = static_cast<QMouseEvent*>(callData);
        Point3D pt(me->x(), me->y(), 0);
        pt = toSceneCoordinates(pt);
        emit OnMouseMove(pt);
        }
        break;
    case QEvent::MouseButtonRelease: {
        QMouseEvent* me = static_cast<QMouseEvent*>(callData);
        Point3D pt(me->x(), me->y(), 0);
        pt = toSceneCoordinates(pt);
        emit OnMouseRelease(pt);
        }
        break;
    case QEvent::MouseButtonDblClick: {
        QMouseEvent* me = static_cast<QMouseEvent*>(callData);
        Point3D pt(me->x(), me->y(), 0);
        pt = toSceneCoordinates(pt);
        emit OnMouseDoubleClick(pt);
        }
        break;
    case QEvent::KeyPress: {
        QKeyEvent* ke = static_cast<QKeyEvent*>(callData);
        emit OnKeyPress(ke->text());
        }
        break;
    case QEvent::KeyRelease: {
        QKeyEvent* ke = static_cast<QKeyEvent*>(callData);
        emit OnKeyRelease(ke->text());
        }
        break;
    default:
        break;
    }
    */
}

void CVtkOpenHapticsRenderer::timerEvent(QTimerEvent*)
{
    if(d->hapticsRenderer)
        d->hapticsRenderer->CheckHapticsEvents();
}

