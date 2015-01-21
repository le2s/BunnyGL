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

#ifndef C_VTK_RIBBON_FILTER_H
#define C_VTK_RIBBON_FILTER_H

#include "CVtkPolyDataFilter.h"
#include "vtkRibbonFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkRibbonFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(TCoords)

    Q_PROPERTY(double Width READ width WRITE setWidth)
    Q_PROPERTY(double Angle READ angle WRITE setAngle)
    Q_PROPERTY(bool VaryWidth READ isVaryWidth WRITE setVaryWidth)
    Q_PROPERTY(double WidthFactor READ widthFactor WRITE setWidthFactor)
    Q_PROPERTY(Point3D DefaultNormal READ defaultNormal WRITE setDefaultNormal)
    Q_PROPERTY(bool UseDefaultNormal READ isUseDefaultNormal WRITE setUseDefaultNormal)
    Q_PROPERTY(TCoords GenerateTCoords READ generateTCoords WRITE setGenerateTCoords)
    Q_PROPERTY(double TextureLength READ textureLength WRITE setTextureLength)
    DECLARE_VTK_OBJECT

public:
    CVtkRibbonFilter();
    ~CVtkRibbonFilter();

    enum TCoords
    {
        Off2 = VTK_TCOORDS_OFF,
        NormalizedLength = VTK_TCOORDS_FROM_NORMALIZED_LENGTH,
        Length = VTK_TCOORDS_FROM_LENGTH,
        Scalars = VTK_TCOORDS_FROM_SCALARS
    };


    double width() const;
    void setWidth(double val);

    double angle() const;
    void setAngle(double ang);

    bool isVaryWidth() const;
    void setVaryWidth(bool val);

    double widthFactor() const;
    void setWidthFactor(double wdtfct);

    Point3D defaultNormal() const;
    void setDefaultNormal(Point3D df);

    bool isUseDefaultNormal() const;
    void setUseDefaultNormal(bool udn);

    TCoords generateTCoords() const;
    void setGenerateTCoords(TCoords val);

    double textureLength() const;
    void setTextureLength(double val);

private:
    vtkRibbonFilter *m_vtkRibbonFilter;
};

#endif