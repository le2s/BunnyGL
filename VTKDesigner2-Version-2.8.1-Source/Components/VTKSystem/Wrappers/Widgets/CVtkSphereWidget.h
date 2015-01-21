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

#ifndef C_VTK_SPHERE_WIDGET_H
#define C_VTK_SPHERE_WIDGET_H

#include "CVtk3DWidget.h"
#include "vtkSphereWidget.h"
#include "UserDefinedDataTypes.h"

class CVtkSphereWidget : public CVtk3DWidget
{
    Q_OBJECT
    Q_ENUMS(Representation)
    Q_PROPERTY(Representation Representation READ representation WRITE setRepresentation)
    Q_PROPERTY(int ThetaResolution READ thetaResolution WRITE setThetaResolution)
    Q_PROPERTY(int PhiResolution READ phiResolution WRITE setPhiResolution)
    Q_PROPERTY(double Radius READ radius WRITE setRadius)
    Q_PROPERTY(Point3D Center READ center WRITE setCenter)
    Q_PROPERTY(bool Translation READ isTranslation WRITE setTranslation)
    Q_PROPERTY(bool Scale READ isScale WRITE setScale)
    Q_PROPERTY(bool HandlesVisible READ isHandlesVisible WRITE setHandlesVisible)
    Q_PROPERTY(Point3D HandleDirection READ handleDirection WRITE setHandleDirection)
    Q_PROPERTY(Point3D HandlePosition READ handlePosition)
    DECLARE_VTK_OBJECT

public:
    enum Representation
    {
        Off       = 0,
        WireFrame = 1,
        Surface  = 2
    };

    CVtkSphereWidget();
    ~CVtkSphereWidget();

    void setRepresentation(Representation val);
    Representation representation() const;

    void setThetaResolution(int val);
    int  thetaResolution() const;

    void setPhiResolution(int val);
    int  phiResolution() const;

    void setRadius(double val);
    double radius() const;

    void setCenter (Point3D point);
    Point3D center() const;

    void setTranslation(bool val);
    bool isTranslation() const;

    void setScale(bool val);
    bool isScale() const;

    void setHandlesVisible(bool val);
    bool isHandlesVisible() const;

    void setHandleDirection(Point3D point);
    Point3D handleDirection() const;

    Point3D handlePosition() const;

private:
    vtkSphereWidget* m_vtkSphereWidget;
    bool m_handlesVisible;
    bool m_translation;
    bool m_scale;

};

#endif
