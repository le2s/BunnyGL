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

#ifndef VTK_OPEN_HAPTICS_CURSOR_ACTOR_H
#define VTK_OPEN_HAPTICS_CURSOR_ACTOR_H

#include "vtkOpenGLActor.h"
#include "vtkCommand.h"

struct vtkOpenHapticsSyncHelper;
class hduMatrix;
class vtkOpenHapticsProperty;
class vtkOpenHapticsActor : public vtkOpenGLActor
{
public:
    enum HapticsEvent
    {
        TouchEvent = vtkCommand::UserEvent+1,
        UnTouchEvent,
        MotionEvent,
        Button1DownEvent,
        Button1UpEvent,
        Button2DownEvent,
        Button2UpEvent
    };

    static vtkOpenHapticsActor* New();

    void SetHapticsProperty(vtkOpenHapticsProperty* prop);
    vtkOpenHapticsProperty* GetHapticsProperty() { return this->OpenHapticsProperty; }

    vtkBooleanMacro(DynamicSurfaceChange, int);
    vtkSetMacro(DynamicSurfaceChange, int);
    vtkGetMacro(DynamicSurfaceChange, int);

    vtkGetMacro(OpenHapticsShapeID, unsigned int);
    vtkGetMacro(Touched, int);
    vtkGetMacro(Button1Down, int);
    vtkGetMacro(Button2Down, int);

    vtkBooleanMacro(EnableHapticRendering, int);
    vtkSetMacro(EnableHapticRendering, int);
    vtkGetMacro(EnableHapticRendering, int);

    vtkBooleanMacro(MoveWithStylus, int);
    vtkGetMacro(MoveWithStylus, int);
    void SetMoveWithStylus(int val);

    vtkBooleanMacro(RotateWithStylus, int);
    vtkGetMacro(RotateWithStylus, int);
    void SetRotateWithStylus(int val);

    vtkBooleanMacro(UseHapticTransform, int);
    vtkGetMacro(UseHapticTransform, int);
    void SetUseHapticTransform(int val);

    void ResetHapticTransform();
    vtkGetMacro(HapticTransform, hduMatrix*);

    void Render(vtkRenderer *ren, vtkMapper *mapper);

protected:
    vtkOpenHapticsActor();
    ~vtkOpenHapticsActor();

public: // Not for public use actually ;)
    void HandleTouchEvent();
    void HandleUnTouchEvent();
    void HandleMotionEvent();
    void HandleButton1DownEvent();
    void HandleButton1UpEvent();
    void HandleButton2DownEvent();
    void HandleButton2UpEvent();
    vtkGetMacro(SyncHelper, vtkOpenHapticsSyncHelper*);

private:
    void InitSyncHelper();
    void ReleaseSyncHelper();

    unsigned int OpenHapticsShapeID;
    vtkOpenHapticsProperty* OpenHapticsProperty;

    int DynamicSurfaceChange;

    int Touched;
    int Button1Down;
    int Button2Down;
    int EnableHapticRendering;
    int MoveWithStylus;
    int RotateWithStylus;
    int UseHapticTransform;
    hduMatrix *HapticTransform;
    vtkOpenHapticsSyncHelper* SyncHelper;
};

#endif

