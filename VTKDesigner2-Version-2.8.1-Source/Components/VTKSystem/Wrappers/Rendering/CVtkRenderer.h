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

#ifndef C_VTK_RENDERER_H
#define C_VTK_RENDERER_H

#include "CVtkObject.h"
#include "vtkRenderer.h"
#include "CVtkRendererData.h"
#include "UserDefinedDataTypes.h"

class CVtkRendererGridScene;
class CVtkRenderer : public CVtkObject
{
    Q_OBJECT
    Q_PROPERTY(QColor BackgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(bool ShowGrid READ isShowGrid WRITE setShowGrid)
    Q_PROPERTY(bool LightsFollowCamera READ isLightsFollowCamera WRITE setLightsFollowCamera)
    Q_PROPERTY(bool TwoSidedLighting READ isTwoSidedLighting WRITE setTwoSidedLighting)
    Q_PROPERTY(QRectF Viewport READ viewport WRITE setViewport)
    DECLARE_VTK_OBJECT

public:
    CVtkRenderer();
    ~CVtkRenderer();

    void setBackgroundColor(QColor color);
    QColor backgroundColor() const;

    void setShowGrid(bool val);
    bool isShowGrid() const;

    void setLightsFollowCamera(bool val);
    bool isLightsFollowCamera() const;

    void setTwoSidedLighting(bool val);
    bool isTwoSidedLighting() const;

    void setViewport(QRectF viewport);
    QRectF viewport() const;

    Q_INVOKABLE void resetCamera();
    Q_INVOKABLE void render();
    Q_INVOKABLE void createAutomaticLights();
    Q_INVOKABLE Point3D toScreenCoordinates(Point3D p);
    Q_INVOKABLE Point3D toSceneCoordinates(Point3D p);
    Q_INVOKABLE void flyTo(Point3D point, int nrFrames=24);

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
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);
    void finalize();
    void handleVtkEvent(unsigned long event, void* callData);

private:
    vtkRenderer* m_vtkRenderer;
    CVtkRendererData m_vtkRendererOutput;
    bool m_hasUserCamera;
    CVtkRendererGridScene* m_RendererSceneGrid;
};

#endif
