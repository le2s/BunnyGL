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

#ifndef C_VTK_LINEAR_EXTRUSION_FILTER_H
#define C_VTK_LINEAR_EXTRUSION_FILTER_H

#include "CVtkPolyDataFilter.h"
#include "vtkLinearExtrusionFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkLinearExtrusionFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(ExtrusionType)
    Q_PROPERTY(ExtrusionType ExtrusionType READ extrusionType WRITE setExtrusionType)
    Q_PROPERTY(bool Capping READ isCapping WRITE setCapping)
    Q_PROPERTY(double ScaleFactor READ scaleFactor WRITE setScaleFactor)
    Q_PROPERTY(Point3D Vector READ vector WRITE setVector)
    Q_PROPERTY(Point3D ExtrusionPoint READ extrusionPoint WRITE setExtrusionPoint)
    DECLARE_VTK_OBJECT

public:
    enum ExtrusionType
    {
        Vector = VTK_VECTOR_EXTRUSION,
        Normal = VTK_NORMAL_EXTRUSION,
        Point = VTK_POINT_EXTRUSION
    };

    CVtkLinearExtrusionFilter();
    ~CVtkLinearExtrusionFilter();

    void setExtrusionType(ExtrusionType type);
    ExtrusionType extrusionType() const;

    void setCapping(bool val);
    bool isCapping() const;

    void setScaleFactor(double factor);
    double scaleFactor() const;

    void setVector(Point3D vec);
    Point3D vector() const;

    void setExtrusionPoint(Point3D ep);
    Point3D extrusionPoint() const;

private:
    vtkLinearExtrusionFilter* m_vtkLinearExtrusionFilter;
};

#endif
