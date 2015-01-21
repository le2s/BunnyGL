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

#ifndef C_VTK_FLYTHROUGH_CAMERA_H
#define C_VTK_FLYTHROUGH_CAMERA_H

#include "CVtkCamera.h"
#include "vtkFlythroughCamera.h"

class vtkFlythroughCamera;
class CVtkFlythroughCamera : public CVtkCamera
{
    Q_OBJECT
    DECLARE_VTK_OBJECT
    Q_PROPERTY(Point3D InitialPosition READ initialPosition WRITE setInitialPosition)
    Q_PROPERTY(Point3D InitialFocalPoint READ initialFocalPoint WRITE setInitialFocalPoint)
    Q_PROPERTY(Vector3D InitialViewUp READ initialViewUp WRITE setInitialViewUp)

public:
    CVtkFlythroughCamera();
    ~CVtkFlythroughCamera();

    void setInitialPosition(Point3D point);
    Point3D initialPosition() const;

    void setInitialFocalPoint(Point3D point);
    Point3D initialFocalPoint() const;

    void setInitialViewUp(Vector3D point);
    Vector3D initialViewUp() const;

    Q_INVOKABLE void moveStart();
    Q_INVOKABLE void moveForward();
    Q_INVOKABLE void moveBackward();
    Q_INVOKABLE void moveForwardSwapAround();

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkFlythroughCamera* m_vtkFlythroughCamera;
};

#endif


