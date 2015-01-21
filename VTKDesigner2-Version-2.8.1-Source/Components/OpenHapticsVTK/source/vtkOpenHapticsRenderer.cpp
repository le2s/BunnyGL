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


#include "vtkOpenHapticsRenderer.h"
#include "vtkOpenHapticsEffect.h"
#include "vtkRenderWindow.h"
#include "vtkOutputWindow.h"
#include "vtkMath.h"
#include "vtkMatrix4x4.h"

#include "HL/hl.h"
#include "HD/hd.h"
#include "HLU/hlu.h"
#include "GL/gl.h"

struct SProxyInfo
{
    double ProxyPosition[3];
    double ProxyRotation[4];
    double ProxyTouchNormal[3];
    double ProxyTransform[16];
    double DeviceTorqueSent[3];
    double DeviceForceSent[3];
    double DeviceForceReceived[3];
    double DeviceTorqueReceived[3];
};

void HLCALLBACK vtkOpenHapticsRendererMotionCallback(HLenum, HLuint, HLenum, HLcache*, void *userdata)
{
    vtkOpenHapticsRenderer* ren = reinterpret_cast<vtkOpenHapticsRenderer*>(userdata);
    if(ren)
        ren->Redraw();
}

void HLCALLBACK vtkOpenHapticsRendererCalibrationCallback(HLenum event, HLuint, HLenum, HLcache *, void *)
{
    if (event == HL_EVENT_CALIBRATION_UPDATE)
    {
        hlUpdateCalibration();
    }
    else if (event == HL_EVENT_CALIBRATION_INPUT)
    {
        // Go hump someone ;)
    }
}

HDCallbackCode HDCALLBACK vtkOpenHapticsRendererHDCallback(void* data)
{
    SProxyInfo* info = static_cast<SProxyInfo*>(data);

    hdGetDoublev(HD_CURRENT_FORCE, info->DeviceForceReceived);
    hdGetDoublev(HD_CURRENT_TORQUE, info->DeviceTorqueReceived);

    return HD_CALLBACK_DONE;
}

void HLCALLBACK vtkOpenHapticsRendererCallback(HLenum event, HLuint, HLenum, HLcache * cache, void *userdata)
{
    vtkOpenHapticsRenderer* ren = reinterpret_cast<vtkOpenHapticsRenderer*>(userdata);
    if(!ren)
        return;

    SProxyInfo info;

    hlCacheGetDoublev(cache, HL_PROXY_POSITION, info.ProxyPosition);
    hlCacheGetDoublev(cache, HL_PROXY_ROTATION, info.ProxyRotation);
    hlCacheGetDoublev(cache, HL_PROXY_TOUCH_NORMAL, info.ProxyTouchNormal);
    hlCacheGetDoublev(cache, HL_PROXY_TRANSFORM, info.ProxyTransform);
    hlCacheGetDoublev(cache, HL_DEVICE_FORCE, info.DeviceForceSent);
    hlCacheGetDoublev(cache, HL_DEVICE_TORQUE, info.DeviceTorqueSent);
    hdScheduleSynchronous(vtkOpenHapticsRendererHDCallback, &info, HD_MAX_SCHEDULER_PRIORITY);

    ren->UpdatePositionRotation(&info);

    if(event == HL_EVENT_MOTION)
        ren->HandleMotionEvent();
    else if(event == HL_EVENT_TOUCH)
        ren->HandleTouchEvent();
    else if(event == HL_EVENT_UNTOUCH)
        ren->HandleUnTouchEvent();
    else if(event == HL_EVENT_1BUTTONDOWN)
        ren->HandleButton1DownEvent();
    else if(event == HL_EVENT_1BUTTONUP)
        ren->HandleButton1UpEvent();
    else if(event == HL_EVENT_2BUTTONDOWN)
        ren->HandleButton2DownEvent();
    else if(event == HL_EVENT_2BUTTONUP)
        ren->HandleButton2UpEvent();
}

static vtkOpenHapticsRenderer* LastCreatedInstancePtr = 0;

vtkOpenHapticsRenderer* vtkOpenHapticsRenderer::New()
{
    LastCreatedInstancePtr = new vtkOpenHapticsRenderer;
    return LastCreatedInstancePtr;
}

vtkOpenHapticsRenderer* vtkOpenHapticsRenderer::LastCreatedInstance()
{
    return LastCreatedInstancePtr;
}

void vtkOpenHapticsRenderer::DeviceRender()
{
    // Do not remove this MakeCurrent! Due to Start / End methods on
    // some objects which get executed during a pipeline update,
    // other windows might get rendered since the last time
    // a MakeCurrent was called.
    this->RenderWindow->MakeCurrent();

    // standard render method
    this->ClearLights();

    this->UpdateCamera();
    this->UpdateWorkspace();
    this->UpdateLightGeometry();
    this->UpdateLights();

    // set matrix mode for actors
    glMatrixMode(GL_MODELVIEW);

    // Pass 1: Graphics Pass.
    this->HapticsPass = 0;
    this->UpdateGeometry();

    // Pass 2: Haptics Pass.
    if(this->EnableHapticRendering)
    {
        this->HapticsPass = 1;
        hlBeginFrame();

        if(this->ShapeRendering == DepthBuffer)
            hlEnable(HL_ADAPTIVE_VIEWPORT);
        else
            hlDisable(HL_ADAPTIVE_VIEWPORT);

        this->UpdateGeometry();

        // Render effects if any
        this->EffectCollection->InitTraversal();
        while(1)
        {
            vtkOpenHapticsEffect* effect
                = this->EffectCollection->GetNextEffect();

            if(!effect)
                break;

            effect->RenderEffect();
        }

        hlEndFrame();

        // Update the proxy if needed.
        this->UpdateProxy();

        // Pass 3: Cursor pass.
        this->HapticsPass = 2;
        this->UpdateGeometry();
    }

    this->HapticsPass = 0;

    // clean up the model view matrix set up by the camera
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void vtkOpenHapticsRenderer::Redraw()
{
    HDErrorInfo errorInfo = hdGetError();
    if(HD_DEVICE_ERROR(errorInfo))
    {
        HDstring errMsg = hdGetErrorString(errorInfo.errorCode);
        char errorMessageBuffer[400];
        sprintf(errorMessageBuffer, "Could not make the haptic context current: %s\0", errMsg);
        vtkOutputWindow::GetInstance()->DisplayDebugText(errorMessageBuffer);
    }
    else
    {
        vtkRenderWindow* rWin = this->GetRenderWindow();
        if(rWin)
            rWin->Render();
    }
}

void vtkOpenHapticsRenderer::CheckHapticsEvents()
{
    hlCheckEvents();
}

void vtkOpenHapticsRenderer::SetEnableDirectProxyRendering(int val)
{
    if(this->EnableDirectProxyRendering == val)
        return;

    this->EnableDirectProxyRendering = val;
    if(this->GetRenderWindow())
        this->GetRenderWindow()->Render();
}

void vtkOpenHapticsRenderer::SetProxyPosition(double x, double y, double z)
{
    this->DirectProxyPosition[0] = x;
    this->DirectProxyPosition[1] = y;
    this->DirectProxyPosition[2] = z;

    if(this->GetRenderWindow())
        this->GetRenderWindow()->Render();
}

void vtkOpenHapticsRenderer::SetProxyRotation(double w, double x, double y, double z)
{
    this->DirectProxyOrientation[0] = w;
    this->DirectProxyOrientation[1] = x;
    this->DirectProxyOrientation[2] = y;
    this->DirectProxyOrientation[3] = z;

    if(this->GetRenderWindow())
        this->GetRenderWindow()->Render();
}

void vtkOpenHapticsRenderer::AddEffect(vtkOpenHapticsEffect* effect)
{
    this->EffectCollection->AddItem(effect);
}

void vtkOpenHapticsRenderer::RemoveEffect(vtkOpenHapticsEffect* effect)
{
    if(effect->GetStarted())
        return;

    this->EffectCollection->RemoveItem(effect);
}

void vtkOpenHapticsRenderer::UpdateWorkspace()
{
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    hlMatrixMode(HL_TOUCHWORKSPACE);
    hlLoadIdentity();
    hluFitWorkspace(projection);

    CursorScale = hluScreenToModelScale(modelview, projection, viewport);
    CursorScale *= 20; // Hardcoded.
}

vtkOpenHapticsRenderer::vtkOpenHapticsRenderer()
{
    this->ShapeRendering = FeedbackBuffer;
    this->RenWinWidth = 0;
    this->RenWinHeight = 0;
    this->ProxyPosition[0] = 0;
    this->ProxyPosition[1] = 0;
    this->ProxyPosition[2] = 0;
    this->OldProxyPosition[0] = this->ProxyPosition[0];
    this->OldProxyPosition[1] = this->ProxyPosition[1];
    this->OldProxyPosition[2] = this->ProxyPosition[2];
    this->ProxyRotation[0] = 0;
    this->ProxyRotation[1] = 0;
    this->ProxyRotation[2] = 0;
    this->ProxyRotation[3] = 0;
    this->OldProxyRotation[0] = this->ProxyRotation[0];
    this->OldProxyRotation[1] = this->ProxyRotation[1];
    this->OldProxyRotation[2] = this->ProxyRotation[2];
    this->OldProxyRotation[3] = this->ProxyRotation[3];
    this->DeviceForceSent[0] = 0;
    this->DeviceForceSent[1] = 0;
    this->DeviceForceSent[2] = 0;
    this->DeviceTorqueSent[0] = 0;
    this->DeviceTorqueSent[1] = 0;
    this->DeviceTorqueSent[2] = 0;
    this->Touched = 0;
    this->Button1Down = 0;
    this->Button2Down = 0;
    this->MoveWhenOneButtonDown = 0;
    this->EnableHapticRendering = 1;
    this->DirectProxyPosition[0] = 0;
    this->DirectProxyPosition[1] = 0;
    this->DirectProxyPosition[2] = 0;
    this->DirectProxyOrientation[0] = 0;
    this->DirectProxyOrientation[1] = 0;
    this->DirectProxyOrientation[2] = 0;
    this->DirectProxyOrientation[3] = 0;
    this->EnableDirectProxyRendering = 0;
    this->ProxyRenderingEnabled = 0;
    vtkMatrix4x4::Identity(this->ProxyTransform);
    vtkMatrix4x4::Identity(this->OldProxyTransform);

    this->EffectCollection = vtkOpenHapticsEffectCollection::New();

    // It would be difficult to support multiple renderers, if we have
    // this kind of implementation.
    hlAddEventCallback(HL_EVENT_MOTION, HL_OBJECT_ANY, HL_CLIENT_THREAD,
                       &vtkOpenHapticsRendererMotionCallback, (void*)this);
    hlAddEventCallback(HL_EVENT_CALIBRATION_UPDATE, HL_OBJECT_ANY,
                       HL_CLIENT_THREAD, &vtkOpenHapticsRendererCalibrationCallback, NULL);
    hlAddEventCallback(HL_EVENT_CALIBRATION_INPUT, HL_OBJECT_ANY,
                       HL_CLIENT_THREAD, &vtkOpenHapticsRendererCalibrationCallback, NULL);

    hlAddEventCallback(HL_EVENT_MOTION, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsRendererCallback,(void*)this);
    hlAddEventCallback(HL_EVENT_TOUCH, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsRendererCallback,(void*)this);
    hlAddEventCallback(HL_EVENT_UNTOUCH, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsRendererCallback,(void*)this);
    hlAddEventCallback(HL_EVENT_1BUTTONDOWN, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsRendererCallback,(void*)this);
    hlAddEventCallback(HL_EVENT_1BUTTONUP, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsRendererCallback,(void*)this);
    hlAddEventCallback(HL_EVENT_2BUTTONDOWN, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsRendererCallback,(void*)this);
    hlAddEventCallback(HL_EVENT_2BUTTONUP, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsRendererCallback,(void*)this);
}

vtkOpenHapticsRenderer::~vtkOpenHapticsRenderer()
{
    if(LastCreatedInstancePtr == this)
        LastCreatedInstancePtr = 0;

    if(this->EffectCollection)
        this->EffectCollection->Delete();

    hlRemoveEventCallback(HL_EVENT_MOTION, HL_OBJECT_ANY, HL_CLIENT_THREAD,
                       &vtkOpenHapticsRendererMotionCallback);
    hlRemoveEventCallback(HL_EVENT_CALIBRATION_UPDATE, HL_OBJECT_ANY,
        HL_CLIENT_THREAD, vtkOpenHapticsRendererCalibrationCallback);
    hlRemoveEventCallback(HL_EVENT_CALIBRATION_INPUT, HL_OBJECT_ANY,
        HL_CLIENT_THREAD, vtkOpenHapticsRendererCalibrationCallback);

    hlRemoveEventCallback(HL_EVENT_MOTION, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsRendererCallback);
    hlRemoveEventCallback(HL_EVENT_TOUCH, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsRendererCallback);
    hlRemoveEventCallback(HL_EVENT_UNTOUCH, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsRendererCallback);
    hlRemoveEventCallback(HL_EVENT_1BUTTONDOWN, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsRendererCallback);
    hlRemoveEventCallback(HL_EVENT_1BUTTONUP, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsRendererCallback);
    hlRemoveEventCallback(HL_EVENT_2BUTTONDOWN, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsRendererCallback);
    hlRemoveEventCallback(HL_EVENT_2BUTTONUP, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsRendererCallback);
}

void vtkOpenHapticsRenderer::UpdatePositionRotation(SProxyInfo* info)
{
    this->OldProxyPosition[0] = this->ProxyPosition[0];
    this->OldProxyPosition[1] = this->ProxyPosition[1];
    this->OldProxyPosition[2] = this->ProxyPosition[2];
    this->ProxyPosition[0] = info->ProxyPosition[0];
    this->ProxyPosition[1] = info->ProxyPosition[1];
    this->ProxyPosition[2] = info->ProxyPosition[2];

    this->OldProxyRotation[0] = this->ProxyRotation[0];
    this->OldProxyRotation[1] = this->ProxyRotation[1];
    this->OldProxyRotation[2] = this->ProxyRotation[2];
    this->OldProxyRotation[3] = this->ProxyRotation[3];
    this->ProxyRotation[0] = info->ProxyRotation[0];
    this->ProxyRotation[1] = info->ProxyRotation[1];
    this->ProxyRotation[2] = info->ProxyRotation[2];
    this->ProxyRotation[3] = info->ProxyRotation[3];

    this->DeviceForceSent[0] = info->DeviceForceSent[0];
    this->DeviceForceSent[1] = info->DeviceForceSent[1];
    this->DeviceForceSent[2] = info->DeviceForceSent[2];

    this->DeviceTorqueSent[0] = info->DeviceTorqueSent[0];
    this->DeviceTorqueSent[1] = info->DeviceTorqueSent[1];
    this->DeviceTorqueSent[2] = info->DeviceTorqueSent[2];

    this->DeviceForceReceived[0] = info->DeviceForceReceived[0];
    this->DeviceForceReceived[1] = info->DeviceForceReceived[1];
    this->DeviceForceReceived[2] = info->DeviceForceReceived[2];

    this->DeviceTorqueReceived[0] = info->DeviceTorqueReceived[0];
    this->DeviceTorqueReceived[1] = info->DeviceTorqueReceived[1];
    this->DeviceTorqueReceived[2] = info->DeviceTorqueReceived[2];

    for(int i=0; i<16; i++)
        this->OldProxyTransform[i] = this->ProxyTransform[i];

    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            this->ProxyTransform[i*4+j] = info->ProxyTransform[j*4+i];
}

void vtkOpenHapticsRenderer::UpdateProxy()
{
    // Direct proxy rendering if enabled.
    if(this->EnableDirectProxyRendering)
    {
        if(!this->ProxyRenderingEnabled)
        {
            hlDisable(HL_PROXY_RESOLUTION);
            this->ProxyRenderingEnabled = true;
        }

        HLdouble positionVector[3];
        positionVector[0] = this->DirectProxyPosition[0];
        positionVector[1] = this->DirectProxyPosition[1];
        positionVector[2] = this->DirectProxyPosition[2];
        hlProxydv(HL_PROXY_POSITION, positionVector);

        HLdouble orientationQuaternion[4];
        orientationQuaternion[0] = this->DirectProxyOrientation[0];
        orientationQuaternion[1] = this->DirectProxyOrientation[1];
        orientationQuaternion[2] = this->DirectProxyOrientation[2];
        orientationQuaternion[3] = this->DirectProxyOrientation[3];
        hlProxydv(HL_PROXY_ROTATION, orientationQuaternion);
    }
    else
    {
        if(this->ProxyRenderingEnabled)
        {
            hlEnable(HL_PROXY_RESOLUTION);
            this->ProxyRenderingEnabled = false;
        }
    }
}

void vtkOpenHapticsRenderer::HandleTouchEvent()
{
    this->Touched = 1;
    this->InvokeEvent(TouchEvent, (void*)this);
}

void vtkOpenHapticsRenderer::HandleUnTouchEvent()
{
    this->Touched = 0;
    this->InvokeEvent(UnTouchEvent, (void*)this);
}

void vtkOpenHapticsRenderer::HandleMotionEvent()
{
    this->InvokeEvent(MotionEvent, (void*)this);
}

void vtkOpenHapticsRenderer::HandleButton1DownEvent()
{
    this->Button1Down = 1;
    this->InvokeEvent(Button1DownEvent, (void*)this);
}

void vtkOpenHapticsRenderer::HandleButton1UpEvent()
{
    this->Button1Down = 0;
    this->InvokeEvent(Button1UpEvent, (void*)this);
}

void vtkOpenHapticsRenderer::HandleButton2DownEvent()
{
    this->Button2Down = 1;
    this->InvokeEvent(Button2DownEvent, (void*)this);
}

void vtkOpenHapticsRenderer::HandleButton2UpEvent()
{
    this->Button2Down = 0;
    this->InvokeEvent(Button2UpEvent, (void*)this);
}
