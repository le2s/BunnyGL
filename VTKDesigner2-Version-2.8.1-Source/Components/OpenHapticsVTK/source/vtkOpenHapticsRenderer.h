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

#ifndef VTK_OPEN_HAPTICS_RENDERER_H
#define VTK_OPEN_HAPTICS_RENDERER_H

#include "vtkOpenGLRenderer.h"
#include "vtkCommand.h"

class vtkOpenHapticsEffect;
class vtkOpenHapticsEffectCollection;

struct SProxyInfo;
class vtkOpenHapticsRenderer : public vtkOpenGLRenderer
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

    enum ShapeRenderingType
    {
        DepthBuffer,
        FeedbackBuffer
    };

    static vtkOpenHapticsRenderer* New();
    static vtkOpenHapticsRenderer* LastCreatedInstance();

    vtkGetMacro(CursorScale, double);
    vtkGetMacro(HapticsPass, int);
    vtkGetMacro(OldProxyPosition, double*);
    vtkGetMacro(ProxyPosition, double*);
    vtkGetMacro(OldProxyRotation, double*);
    vtkGetMacro(ProxyRotation, double*);
    vtkGetMacro(OldProxyTransform, double*);
    vtkGetMacro(ProxyTransform, double*);
    vtkGetMacro(DeviceForceSent, double*);
    vtkGetMacro(DeviceTorqueSent, double*);
    vtkGetMacro(DeviceForceReceived, double*);
    vtkGetMacro(DeviceTorqueReceived, double*);
    vtkGetMacro(Touched, int);
    vtkGetMacro(Button1Down, int);
    vtkGetMacro(Button2Down, int);

    vtkBooleanMacro(EnableHapticRendering, int);
    vtkSetMacro(EnableHapticRendering, int);
    vtkGetMacro(EnableHapticRendering, int);

    vtkSetMacro(ShapeRendering, ShapeRenderingType);
    vtkGetMacro(ShapeRendering, ShapeRenderingType);

    void DeviceRender();
    void Redraw();
    void CheckHapticsEvents();

    // Used for direct proxy rendering.
    vtkBooleanMacro(EnableDirectProxyRendering, int);
    vtkGetMacro(EnableDirectProxyRendering, int);
    void SetEnableDirectProxyRendering(int val);

    void SetProxyPosition(double* pos)
        { SetProxyPosition(pos[0], pos[1], pos[2]); }
    void SetProxyPosition(double x, double y, double z);

    void SetProxyRotation(double* wxyz)
        { SetProxyRotation(wxyz[0], wxyz[1], wxyz[2], wxyz[3]); }
    void SetProxyRotation(double w, double x, double y, double z);

    // Effects stuff
    void AddEffect(vtkOpenHapticsEffect* effect);
    void RemoveEffect(vtkOpenHapticsEffect* effect);
    vtkGetMacro(EffectCollection, vtkOpenHapticsEffectCollection*);

protected:
    void UpdateWorkspace(); // This should be moved into the vtkOpenHapticsRenderWindow class.

    vtkOpenHapticsRenderer();
    ~vtkOpenHapticsRenderer();

public: // Not for public use actually ;)
    void UpdatePositionRotation(SProxyInfo* info);
    void UpdateProxy();

    void HandleTouchEvent();
    void HandleUnTouchEvent();
    void HandleMotionEvent();
    void HandleButton1DownEvent();
    void HandleButton1UpEvent();
    void HandleButton2DownEvent();
    void HandleButton2UpEvent();

private:
    double CursorScale;
    int RenWinWidth;
    int RenWinHeight;
    int HapticsPass;
    ShapeRenderingType ShapeRendering;

    double OldProxyPosition[3];
    double ProxyPosition[3];
    double OldProxyRotation[4];
    double ProxyRotation[4];
    double OldProxyTransform[16];
    double ProxyTransform[16];
    double DirectProxyPosition[3];
    double DirectProxyOrientation[4];
    double DeviceForceSent[3];
    double DeviceTorqueSent[3];
    double DeviceForceReceived[3];
    double DeviceTorqueReceived[3];
    int Touched;
    int Button1Down;
    int Button2Down;
    int MoveWhenOneButtonDown;
    int EnableHapticRendering;
    int EnableDirectProxyRendering;
    int ProxyRenderingEnabled;

    vtkOpenHapticsEffectCollection* EffectCollection;
};

#endif

