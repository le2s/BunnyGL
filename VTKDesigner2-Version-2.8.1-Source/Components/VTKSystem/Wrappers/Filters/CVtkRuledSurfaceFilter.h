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

#ifndef C_VTK_RULED_SURFACE_FILTER_H
#define C_VTK_RULED_SURFACE_FILTER_H

#include "CVtkPolyDataFilter.h"
#include "vtkRuledSurfaceFilter.h"

class CVtkRuledSurfaceFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(RuledMode)
    Q_PROPERTY(double DistanceFactor READ distanceFactor WRITE setDistanceFactor)
    Q_PROPERTY(int OnRatio READ onRatio WRITE setOnRatio)
    Q_PROPERTY(int Offset READ offset WRITE setOffset)
    Q_PROPERTY(bool CloseSurface READ isCloseSurface WRITE setCloseSurface)
    Q_PROPERTY(RuledMode RuledMode READ ruledMode WRITE setRuledMode)
    Q_PROPERTY(bool PassLines READ isPassLines WRITE setPassLines)
    DECLARE_VTK_OBJECT

public:
    CVtkRuledSurfaceFilter();
    ~CVtkRuledSurfaceFilter();

    enum RuledMode
    {
        ReSample = VTK_RULED_MODE_RESAMPLE,
        PointWalk = VTK_RULED_MODE_POINT_WALK
    };

    double distanceFactor() const;
    void setDistanceFactor(double val);

    int onRatio() const;
    void setOnRatio(int val);

    int offset() const;
    void setOffset(int val);

    bool isCloseSurface() const;
    void setCloseSurface(bool val);

    RuledMode ruledMode() const;
    void setRuledMode(RuledMode val);

    bool isPassLines() const;
    void setPassLines(bool val);

private:
    vtkRuledSurfaceFilter *m_vtkRuledSurfaceFilter;
};

#endif