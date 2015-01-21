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
#ifndef C_VTK_PROJECTED_TEXTURE_H
#define C_VTK_PROJECTED_TEXTURE_H

#include "vtkProjectedTexture.h"
#include "CVtkDataSetFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkProjectedTexture : public CVtkDataSetFilter
{
    Q_OBJECT
    Q_ENUMS(CameraMode)
    Q_PROPERTY(Vector3D Up READ up WRITE setUp)
    Q_PROPERTY(Point3D Position READ position WRITE setPosition)
    Q_PROPERTY(Point3D FocalPoint READ focalPoint WRITE setFocalPoint)
    Q_PROPERTY(double MirrorSeparation READ mirrorSeparation WRITE setMirrorSeparation)
    Q_PROPERTY(ValueRange SRange READ sRange WRITE setSRange)
    Q_PROPERTY(ValueRange TRange READ tRange WRITE setTRange)
    Q_PROPERTY(Vector3D AspectRatio READ aspectRatio WRITE setAspectRatio)
    Q_PROPERTY(CameraMode CameraMode READ cameraMode WRITE setCameraMode)
    DECLARE_VTK_OBJECT

public:
    enum CameraMode
    {
        PinHole=VTK_PROJECTED_TEXTURE_USE_PINHOLE,
        TwoMirrors=VTK_PROJECTED_TEXTURE_USE_TWO_MIRRORS
    };

    CVtkProjectedTexture();
    ~CVtkProjectedTexture();

    Vector3D up() const;
    void setUp(Vector3D val);

    Point3D position() const;
    void setPosition(Point3D val);

    Point3D focalPoint() const;
    void setFocalPoint(Point3D val);

    double mirrorSeparation() const;
    void setMirrorSeparation(double val);

    ValueRange sRange() const;
    void setSRange(ValueRange val);

    ValueRange tRange() const;
    void setTRange(ValueRange val);

    Vector3D aspectRatio() const;
    void setAspectRatio(Vector3D val);

    CameraMode cameraMode() const;
    void setCameraMode(CameraMode val);


private:
    vtkProjectedTexture *m_vtkProjectedTexture;
};
#endif//C_VTK_PROJECTED_TEXTURE_H