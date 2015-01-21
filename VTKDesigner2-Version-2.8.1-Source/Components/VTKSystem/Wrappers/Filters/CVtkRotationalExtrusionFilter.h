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

#ifndef C_VTK_ROTATIONAL_EXTRUSION_FILTER_H
#define C_VTK_ROTATIONAL_EXTRUSION_FILTER_H

#include "CVtkPolyDataFilter.h"
#include "vtkRotationalExtrusionFilter.h"

class CVtkRotationalExtrusionFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(int Resolution READ resolution WRITE setResolution)
    Q_PROPERTY(bool Capping READ isCapping WRITE setCapping)
    Q_PROPERTY(double Angle READ angle WRITE setAngle)
    Q_PROPERTY(double Translation READ translation WRITE setTranslation)
    Q_PROPERTY(double DeltaRadius READ deltaRadius WRITE setDeltaRadius)
    DECLARE_VTK_OBJECT

public:
    CVtkRotationalExtrusionFilter();
    ~CVtkRotationalExtrusionFilter();

    int resolution() const;
    void setResolution(int r);

    bool isCapping() const;
    void setCapping(bool val);

    double angle() const;
    void setAngle(double val);

    double translation() const;
    void setTranslation(double val);

    double deltaRadius() const;
    void setDeltaRadius(double r);
    
private:
    vtkRotationalExtrusionFilter * m_vtkRotationalExtrusionFilter;

};

#endif
