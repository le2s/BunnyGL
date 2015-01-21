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

#ifndef C_VTK_CAMERA_H
#define C_VTK_CAMERA_H

#include "CVtkObject.h"
#include "vtkCamera.h"
#include "CVtkCameraData.h"
#include "UserDefinedDataTypes.h"

class CVtkCamera : public CVtkObject
{
    Q_OBJECT
    Q_PROPERTY(Point3D Position READ position WRITE setPosition)
    Q_PROPERTY(Point3D FocalPoint READ focalPoint WRITE setFocalPoint)
    Q_PROPERTY(Vector3D ViewUp READ viewUp WRITE setViewUp)
    Q_PROPERTY(double Distance READ distance WRITE setDistance)
    Q_PROPERTY(double Roll READ roll WRITE setRoll)
    Q_PROPERTY(bool ParallelProjection READ isParallelProjection WRITE setParallelProjection)
    Q_PROPERTY(bool UseHorizontalViewAngle READ isUseHorizontalViewAngle WRITE setUseHorizontalViewAngle)
    Q_PROPERTY(double ViewAngle READ viewAngle WRITE setViewAngle)
    Q_PROPERTY(double ParallelScale READ parallelScale WRITE setParallelScale)
    Q_PROPERTY(double Thickness READ thickness WRITE setThickness)
    Q_PROPERTY(double EyeAngle READ eyeAngle WRITE setEyeAngle)
    Q_PROPERTY(double FocalDisk READ focalDisk WRITE setFocalDisk)
    Q_PROPERTY(Point3D ViewPlaneNormal READ viewPlaneNormal WRITE setViewPlaneNormal)
    DECLARE_VTK_OBJECT

public:
    CVtkCamera(bool createVtkCamera=true);
    ~CVtkCamera();

    Point3D position() const;
    void setPosition(Point3D p);

    Point3D focalPoint() const; 
    void setFocalPoint(Point3D p);

    Vector3D viewUp() const;
    void setViewUp(Vector3D v);

    double distance() const; 
    void setDistance(double d);

    double roll() const; 
    void setRoll(double r);
    
    bool isParallelProjection() const; 
    void setParallelProjection(bool val);
    
    bool isUseHorizontalViewAngle() const;
    void setUseHorizontalViewAngle(bool val);

    double viewAngle() const;
    void setViewAngle(double angle);
    
    double parallelScale() const;
    void setParallelScale(double scale);

    double thickness() const;
    void setThickness(double t);

    double eyeAngle() const; 
    void setEyeAngle(double angle);
    
    double focalDisk() const;
    void setFocalDisk(double f);

    Point3D viewPlaneNormal() const;
    void setViewPlaneNormal(Point3D p);

    Q_INVOKABLE void computeViewUp();
    Q_INVOKABLE Vector3D direction();

protected:
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkCamera* m_vtkCamera;
    CVtkCameraData m_cameraData;
};

#endif



