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

#ifndef C_VTK_TUBE_FILTER_H
#define C_VTK_TUBE_FILTER_H

#include "CVtkPolyDataFilter.h"
#include "vtkTubeFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkTubeFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(VaryRadius)
    Q_ENUMS(TCoords)
    Q_PROPERTY(double Radius READ radius WRITE setRadius)
    Q_PROPERTY(VaryRadius VaryRadius READ varyRadius WRITE setVaryRadius)
    Q_PROPERTY(int NumberOfSides READ numberOfSides WRITE setNumberOfSides)
    Q_PROPERTY(double RadiusFactor READ radiusFactor WRITE setRadiusFactor)
    Q_PROPERTY(Point3D DefaultNormal READ defaultNormal WRITE setDefaultNormal)
    Q_PROPERTY(bool UseDefaultNormal READ isUseDefaultNormal WRITE setUseDefaultNormal)
    Q_PROPERTY(bool SidesShareVertices READ isSidesShareVertices WRITE setSidesShareVertices)
    Q_PROPERTY(bool Capping READ isCapping WRITE setCapping)
    Q_PROPERTY(int OnRatio READ onRatio WRITE setOnRatio)
    Q_PROPERTY(int Offset READ offset WRITE setOffset)
    Q_PROPERTY(TCoords GenerateTCoords READ generateTCoords WRITE setGenerateTCoords)
    DECLARE_VTK_OBJECT

public:
    CVtkTubeFilter();
    ~CVtkTubeFilter();

    enum VaryRadius
    {
        Off = VTK_VARY_RADIUS_OFF,
        Scalar = VTK_VARY_RADIUS_BY_SCALAR,
        Vector = VTK_VARY_RADIUS_BY_VECTOR,
        AbsoluteScalar = VTK_VARY_RADIUS_BY_ABSOLUTE_SCALAR
    };

    enum TCoords
    {
        Off2 = VTK_TCOORDS_OFF,
        NormalizedLength = VTK_TCOORDS_FROM_NORMALIZED_LENGTH,
        Length = VTK_TCOORDS_FROM_LENGTH,
        Scalars = VTK_TCOORDS_FROM_SCALARS
    };

    double radius() const;
    void setRadius(double r);

    VaryRadius varyRadius() const;
    void setVaryRadius(VaryRadius vr);

    int numberOfSides() const;
    void setNumberOfSides(int nos);

    double radiusFactor() const;
    void setRadiusFactor(double rf);

    Point3D defaultNormal() const;
    void setDefaultNormal(Point3D df);

    bool isUseDefaultNormal() const;
    void setUseDefaultNormal(bool udn);

    bool isSidesShareVertices() const;
    void setSidesShareVertices(bool ssv);

    bool isCapping() const;
    void setCapping(bool val);

    int onRatio() const;
    void setOnRatio(int val);

    int offset() const;
    void setOffset(int val);

    TCoords generateTCoords() const;
    void setGenerateTCoords(TCoords val);

private:
    vtkTubeFilter *m_vtkTubeFilter;
};

#endif