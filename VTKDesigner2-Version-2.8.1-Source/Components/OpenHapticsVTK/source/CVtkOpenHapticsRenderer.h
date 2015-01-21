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

#ifndef C_VTK_OPEN_HAPTICS_RENDERER_H
#define C_VTK_OPEN_HAPTICS_RENDERER_H

#include "UserDefinedDataTypes.h"
#include "CGenericVisNodeBase.h"

struct CVtkOpenHapticsRendererData;
class CVtkOpenHapticsRenderer : public CGenericVisNodeBase
{
    Q_OBJECT
    Q_ENUMS(ShapeRenderingType)
    Q_PROPERTY(QColor BackgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(bool LightsFollowCamera READ isLightsFollowCamera WRITE setLightsFollowCamera)
    Q_PROPERTY(bool TwoSidedLighting READ isTwoSidedLighting WRITE setTwoSidedLighting)
    Q_PROPERTY(QRectF Viewport READ viewport WRITE setViewport)
    Q_PROPERTY(bool EnableDirectProxyRendering READ isEnableDirectProxyRendering WRITE setEnableDirectProxyRendering)
    Q_PROPERTY(ShapeRenderingType ShapeRendering READ shapeRendering WRITE setShapeRendering)
	DECLARE_VIS_NODE

public:
    // This enum is copied from vtkOpenHapticsRenderer.h
    // It should be a copy always.
    enum ShapeRenderingType
    {
        DepthBuffer,
        FeedbackBuffer
    };

    static CVtkOpenHapticsRenderer* GetInstancePointer();

    CVtkOpenHapticsRenderer();
    ~CVtkOpenHapticsRenderer();

    // This is not a Q_PROPERTY in purpose.
    void setHapticPollInterval(int mSec);
    int hapticPollInterval() const;

    void setBackgroundColor(QColor color);
    QColor backgroundColor() const;

    void setLightsFollowCamera(bool val);
    bool isLightsFollowCamera() const;

    void setTwoSidedLighting(bool val);
    bool isTwoSidedLighting() const;

    void setViewport(QRectF viewport);
    QRectF viewport() const;

    void setEnableDirectProxyRendering(bool val);
    bool isEnableDirectProxyRendering() const;

    void setShapeRendering(ShapeRenderingType type);
    ShapeRenderingType shapeRendering() const;

    Q_INVOKABLE void resetCamera();
    Q_INVOKABLE void render();
    Q_INVOKABLE void createAutomaticLights();
    Q_INVOKABLE Point3D toScreenCoordinates(Point3D p);
    Q_INVOKABLE Point3D toSceneCoordinates(Point3D p);
    Q_INVOKABLE void flyTo(Point3D point, int nrFrames=24);

    Q_INVOKABLE QObject* currentHapticActor();
    Q_INVOKABLE bool hasCurrentHapticActor();
    Q_INVOKABLE Point3D oldProxyPosition();
    Q_INVOKABLE Point3D proxyPosition();
    Q_INVOKABLE QList<double> oldProxyRotation();
    Q_INVOKABLE QList<double> proxyRotation();
    Q_INVOKABLE QList<double> proxyTransform();
    Q_INVOKABLE QList<double> oldProxyTransform();

	Q_INVOKABLE Vector3D deviceForceSent();
	Q_INVOKABLE Vector3D deviceTorqueSent();
	Q_INVOKABLE Vector3D deviceForceReceived();
	Q_INVOKABLE Vector3D deviceTorqueReceived();

    Q_INVOKABLE bool isTouched();
    Q_INVOKABLE bool isButton1Down();
    Q_INVOKABLE bool isButton2Down();
    Q_INVOKABLE void setProxyPosition(Point3D pos);
    Q_INVOKABLE void setProxyRotation(QList<double> wxyz);

signals:
    void OnMousePress(Point3D p);
    void OnMouseMove(Point3D p);
    void OnMouseRelease(Point3D p);
    void OnMouseDoubleClick(Point3D p);
    void OnKeyPress(QString text);
    void OnKeyRelease(QString text);

protected slots:
    void command_CreateLights();
    void command_ResetCamera();
    void command_Render();

protected:
    // IVisSystemNode implementation
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);
    void initializeNode();
    void finalizeNode();
    void handleVtkEvent(unsigned long event, void* callData);
    void timerEvent(QTimerEvent*);

signals: // haptic signals
    void OnHapticTouch();
    void OnHapticMotion();
    void OnHapticUnTouch();
    void OnHapticButton1Down();
    void OnHapticButton1Up();
    void OnHapticButton2Down();
    void OnHapticButton2Up();

private:
    friend struct CVtkOpenHapticsRendererData;
    CVtkOpenHapticsRendererData* d;
};

#endif

