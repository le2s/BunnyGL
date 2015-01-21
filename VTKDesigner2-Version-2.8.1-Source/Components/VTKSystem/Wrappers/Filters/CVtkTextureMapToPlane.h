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
#ifndef C_VTK_TEXTURE_MAP_TO_PLANE_H
#define C_VTK_TEXTURE_MAP_TO_PLANE_H

#include "vtkTextureMapToPlane.h"
#include "CVtkDataSetFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkTextureMapToPlane : public CVtkDataSetFilter
{
    Q_OBJECT
    Q_PROPERTY(Point3D Point1 READ point1 WRITE setPoint1)
    Q_PROPERTY(Point3D Point2 READ point2 WRITE setPoint2)
    Q_PROPERTY(Point3D Origin READ origin WRITE setOrigin)
    Q_PROPERTY(Vector3D Normal READ normal WRITE setNormal)
    Q_PROPERTY(ValueRange SRange READ sRange WRITE setSRange)
    Q_PROPERTY(ValueRange TRange READ tRange WRITE setTRange)
    Q_PROPERTY(bool AutomaticPlaneGeneration READ isAutomaticPlaneGeneration WRITE setAutomaticPlaneGeneration)
    DECLARE_VTK_OBJECT

public:
    CVtkTextureMapToPlane();
    ~CVtkTextureMapToPlane();

    Point3D point1() const;
    void setPoint1(Point3D val);

    Point3D point2() const;
    void setPoint2(Point3D val);

    Point3D origin() const;
    void setOrigin(Point3D val);

    Vector3D normal() const;
    void setNormal(Vector3D val);

    ValueRange sRange() const;
    void setSRange(ValueRange val);

    ValueRange tRange() const;
    void setTRange(ValueRange val);

    bool isAutomaticPlaneGeneration() const;
    void setAutomaticPlaneGeneration(bool val);


private:
    vtkTextureMapToPlane *m_vtkTextureMapToPlane;
};
#endif//C_VTK_TEXTURE_MAP_TO_PLANE_H