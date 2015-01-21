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

#include "vtkOpenHapticsActor.h"
#include "vtkOpenHapticsRenderer.h"
#include "vtkOutputWindow.h"
#include "vtkMath.h"
#include "vtkMapper.h"
#include "vtkDataSet.h"
#include "vtkOpenHapticsProperty.h"
#include <list>

#include "HL/hl.h"
#include "HD/hd.h"
#include "HDU/hduMath.h"
#include "HDU/hduMatrix.h"
#include "HDU/hduQuaternion.h"
#include "HDU/hduError.h"
#include "GL/gl.h"

using namespace std;

static std::list<vtkOpenHapticsSyncHelper*> SyncHelperList;
struct vtkOpenHapticsSyncHelper
{
    vtkOpenHapticsSyncHelper(vtkOpenHapticsActor* actor) : Actor(actor) {
        SyncHelperList.push_back(this);

        hlGetDoublev(HL_PROXY_POSITION, StartProxyPos);
        hlGetDoublev(HL_PROXY_ROTATION, StartProxyRot);
        StartObjTransform = *(actor->GetHapticTransform());      
    }

    ~vtkOpenHapticsSyncHelper() {
        SyncHelperList.remove(this);
    }

    hduVector3Dd StartProxyPos;
    hduQuaternion StartProxyRot;
    hduMatrix StartObjTransform;

    vtkOpenHapticsActor* GetActor() { return this->Actor; }

private:
    vtkOpenHapticsActor* Actor;
};

void HLCALLBACK vtkOpenHapticsSyncActorCallback(HLenum, HLuint, HLenum, HLcache*, void*)
{
    if(SyncHelperList.size() == 0)
        return;

    std::list<vtkOpenHapticsSyncHelper*>::iterator it = SyncHelperList.begin();
    std::list<vtkOpenHapticsSyncHelper*>::iterator end = SyncHelperList.end();
    while(it != end)
    {
        vtkOpenHapticsSyncHelper* syncHelper = *it;
        vtkOpenHapticsActor* hActor = syncHelper->GetActor();

        if( (hActor->GetMoveWithStylus() || hActor->GetRotateWithStylus()) && syncHelper )
        {
            hduVector3Dd dragDeltaTransl;
            hduMatrix deltaRotMat;

            if(hActor->GetMoveWithStylus())
            {
                hduVector3Dd proxyPos;
                hlGetDoublev(HL_PROXY_POSITION, proxyPos);
                dragDeltaTransl = proxyPos - syncHelper->StartProxyPos;
            }

            if(hActor->GetRotateWithStylus())
            {
                hduQuaternion proxyRotq;
                hlGetDoublev(HL_PROXY_ROTATION, proxyRotq);
                hduQuaternion dragDeltaRot = syncHelper->StartProxyRot.inverse() * proxyRotq;
                dragDeltaRot.normalize();
                dragDeltaRot.toRotationMatrix(deltaRotMat);

                // Want to rotate about the proxy position, not the origin,
                // so need to translate to/from proxy pos.
                hduMatrix toProxy = hduMatrix::createTranslation(-syncHelper->StartProxyPos);
                hduMatrix fromProxy = hduMatrix::createTranslation(syncHelper->StartProxyPos);
                deltaRotMat = toProxy * deltaRotMat * fromProxy;
            }

            // Compose rotation and translation deltas.
            hduMatrix deltaMat = deltaRotMat * hduMatrix::createTranslation(dragDeltaTransl);

            // Update the actor's haptic transformation matrix.
            *(hActor->GetHapticTransform()) = syncHelper->StartObjTransform * deltaMat;
        }

        ++it;
    }
}

void HLCALLBACK vtkOpenHapticsActorCallback(HLenum event, HLuint, HLenum, HLcache *, void *userdata)
{
    vtkOpenHapticsActor* hActor = reinterpret_cast<vtkOpenHapticsActor*>(userdata);
    if(!hActor)
        return;
    
    if(event == HL_EVENT_MOTION)
        hActor->HandleMotionEvent();
    else if(event == HL_EVENT_TOUCH)
        hActor->HandleTouchEvent();
    else if(event == HL_EVENT_UNTOUCH)
        hActor->HandleUnTouchEvent();
    else if(event == HL_EVENT_1BUTTONDOWN)
        hActor->HandleButton1DownEvent();
    else if(event == HL_EVENT_1BUTTONUP)
        hActor->HandleButton1UpEvent();
    else if(event == HL_EVENT_2BUTTONDOWN)
        hActor->HandleButton2DownEvent();
    else if(event == HL_EVENT_2BUTTONUP)
        hActor->HandleButton2UpEvent();
}

vtkOpenHapticsActor* vtkOpenHapticsActor::New()
{
    return new vtkOpenHapticsActor;
}

vtkOpenHapticsActor::vtkOpenHapticsActor()
{
    this->DynamicSurfaceChange = 0;
    this->OpenHapticsShapeID = hlGenShapes(1);
    this->Touched = 0;
    this->Button1Down = 0;
    this->Button2Down = 0;
    this->EnableHapticRendering = 1;
    this->HapticTransform = new hduMatrix();
    this->MoveWithStylus = 0;
    this->RotateWithStylus = 0;
    this->UseHapticTransform = 0;
    this->SyncHelper = 0;
    this->OpenHapticsProperty = vtkOpenHapticsProperty::New();

    hlAddEventCallback(HL_EVENT_MOTION, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsSyncActorCallback,(void*)this);
    hlAddEventCallback(HL_EVENT_MOTION, this->OpenHapticsShapeID, HL_CLIENT_THREAD, vtkOpenHapticsActorCallback,(void*)this);
    hlAddEventCallback(HL_EVENT_TOUCH, this->OpenHapticsShapeID, HL_CLIENT_THREAD, vtkOpenHapticsActorCallback,(void*)this);
    hlAddEventCallback(HL_EVENT_UNTOUCH, this->OpenHapticsShapeID, HL_CLIENT_THREAD, vtkOpenHapticsActorCallback,(void*)this);
    hlAddEventCallback(HL_EVENT_1BUTTONDOWN, this->OpenHapticsShapeID, HL_CLIENT_THREAD, vtkOpenHapticsActorCallback,(void*)this);
    hlAddEventCallback(HL_EVENT_1BUTTONUP, this->OpenHapticsShapeID, HL_CLIENT_THREAD, vtkOpenHapticsActorCallback,(void*)this);
    hlAddEventCallback(HL_EVENT_2BUTTONDOWN, this->OpenHapticsShapeID, HL_CLIENT_THREAD, vtkOpenHapticsActorCallback,(void*)this);
    hlAddEventCallback(HL_EVENT_2BUTTONUP, this->OpenHapticsShapeID, HL_CLIENT_THREAD, vtkOpenHapticsActorCallback,(void*)this);
}

vtkOpenHapticsActor::~vtkOpenHapticsActor()
{
    hlAddEventCallback(HL_EVENT_MOTION, HL_OBJECT_ANY, HL_CLIENT_THREAD, vtkOpenHapticsSyncActorCallback,(void*)this);
    hlRemoveEventCallback(HL_EVENT_MOTION, this->OpenHapticsShapeID, HL_CLIENT_THREAD, vtkOpenHapticsActorCallback);
    hlRemoveEventCallback(HL_EVENT_TOUCH, this->OpenHapticsShapeID, HL_CLIENT_THREAD, vtkOpenHapticsActorCallback);
    hlRemoveEventCallback(HL_EVENT_UNTOUCH, this->OpenHapticsShapeID, HL_CLIENT_THREAD, vtkOpenHapticsActorCallback);
    hlRemoveEventCallback(HL_EVENT_1BUTTONDOWN, this->OpenHapticsShapeID, HL_CLIENT_THREAD, vtkOpenHapticsActorCallback);
    hlRemoveEventCallback(HL_EVENT_1BUTTONUP, this->OpenHapticsShapeID, HL_CLIENT_THREAD, vtkOpenHapticsActorCallback);
    hlRemoveEventCallback(HL_EVENT_2BUTTONDOWN, this->OpenHapticsShapeID, HL_CLIENT_THREAD, vtkOpenHapticsActorCallback);
    hlRemoveEventCallback(HL_EVENT_2BUTTONUP, this->OpenHapticsShapeID, HL_CLIENT_THREAD, vtkOpenHapticsActorCallback);

    hlDeleteShapes(this->OpenHapticsShapeID, 1);
    if(this->OpenHapticsProperty)
        this->OpenHapticsProperty->UnRegister(this);
    if(this->HapticTransform)
        delete this->HapticTransform;
}

void vtkOpenHapticsActor::HandleTouchEvent()
{
    this->Touched = 1;
    this->InvokeEvent(TouchEvent, (void*)this);
}

void vtkOpenHapticsActor::HandleUnTouchEvent()
{
    this->Touched = 0;
    this->InvokeEvent(UnTouchEvent, (void*)this);
}

void vtkOpenHapticsActor::HandleMotionEvent()
{
    this->InvokeEvent(MotionEvent, (void*)this);
}

void vtkOpenHapticsActor::HandleButton1DownEvent()
{
    this->Button1Down = 1;
    this->InvokeEvent(Button1DownEvent, (void*)this);
}

void vtkOpenHapticsActor::HandleButton1UpEvent()
{
    this->Button1Down = 0;
    this->InvokeEvent(Button1UpEvent, (void*)this);
}

void vtkOpenHapticsActor::HandleButton2DownEvent()
{
    this->Button2Down = 1;
    this->InvokeEvent(Button2DownEvent, (void*)this);
}

void vtkOpenHapticsActor::HandleButton2UpEvent()
{
    this->Button2Down = 0;
    this->InvokeEvent(Button2UpEvent, (void*)this);
}

void vtkOpenHapticsActor::SetHapticsProperty(vtkOpenHapticsProperty* prop)
{
    if(this->OpenHapticsProperty == prop)
        return;

    if(this->OpenHapticsProperty)
        this->OpenHapticsProperty->UnRegister(this);
    this->OpenHapticsProperty = prop;
    if(this->OpenHapticsProperty)
        this->OpenHapticsProperty->Register(this);
    else
        this->OpenHapticsProperty = vtkOpenHapticsProperty::New();

    this->Modified();
}

void vtkOpenHapticsActor::SetMoveWithStylus(int val)
{
    if(!this->UseHapticTransform)
        return;

    if(this->MoveWithStylus == val)
        return;

    this->MoveWithStylus = val;

    if(!val)
        ReleaseSyncHelper();
    else
        InitSyncHelper();
}

void vtkOpenHapticsActor::SetRotateWithStylus(int val)
{
    if(!this->UseHapticTransform)
        return;

    if(this->RotateWithStylus == val)
        return;

    this->RotateWithStylus = val;

    if(!val)
        ReleaseSyncHelper();
    else
        InitSyncHelper();
}

void vtkOpenHapticsActor::SetUseHapticTransform(int val)
{
    if(this->UseHapticTransform == val)
        return;

    this->UseHapticTransform = val;

    if(!val)
    {
        this->MoveWithStylus = 0;
        this->RotateWithStylus = 0;
        ReleaseSyncHelper();
        this->HapticTransform->makeIdentity();
    }
}

void vtkOpenHapticsActor::ResetHapticTransform()
{
    this->HapticTransform->makeIdentity();
}

void vtkOpenHapticsActor::Render(vtkRenderer *ren, vtkMapper *mapper)
{
    vtkOpenHapticsRenderer* hRen = dynamic_cast<vtkOpenHapticsRenderer*>(ren);
    if(hRen && hRen->GetHapticsPass() > 1)
        return;

    bool shapeStarted = false;
    bool useDepthBuffer = false;
    
    useDepthBuffer = (hRen->GetShapeRendering()==vtkOpenHapticsRenderer::DepthBuffer);

    if(this->UseHapticTransform)
    {
        glPushMatrix();
        glMultMatrixd(*(this->HapticTransform));
    }

    if(this->EnableHapticRendering && hRen && hRen->GetHapticsPass() == 1)
    {
        vtkDataSet* dataSet = vtkDataSet::SafeDownCast(mapper->GetInput());
        int vertexCount = dataSet ? dataSet->GetNumberOfPoints() : 0;

        if(this->DynamicSurfaceChange)
            hlHintb(HL_SHAPE_DYNAMIC_SURFACE_CHANGE, HL_TRUE);

        if(this->OpenHapticsProperty)
            this->OpenHapticsProperty->Render(hRen);

        if(useDepthBuffer)
        {
            // start the haptic shape
            hlBeginShape(HL_SHAPE_DEPTH_BUFFER, this->OpenHapticsShapeID);
        }
        else
        {
            hlHinti(HL_SHAPE_FEEDBACK_BUFFER_VERTICES, vertexCount);
            hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, this->OpenHapticsShapeID);
        }
        shapeStarted = true;
    }

    vtkOpenGLActor::Render(ren, mapper);

    if(shapeStarted)
    {
		hlEndShape();

        if(this->DynamicSurfaceChange)
            hlHintb(HL_SHAPE_DYNAMIC_SURFACE_CHANGE, HL_FALSE);
    }

    if(this->UseHapticTransform)
        glPopMatrix();
}

void vtkOpenHapticsActor::InitSyncHelper()
{
    if(this->SyncHelper)
        return;

    if(this->MoveWithStylus || this->RotateWithStylus)
        this->SyncHelper = new vtkOpenHapticsSyncHelper(this);
}

void vtkOpenHapticsActor::ReleaseSyncHelper()
{
    if(!this->SyncHelper)
        return;

    if(!this->MoveWithStylus && !this->RotateWithStylus)
    {
        delete this->SyncHelper;
        this->SyncHelper = 0;
    }
}

