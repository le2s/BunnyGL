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


#ifndef C_VTK_DISK_SOURCE_H
#define C_VTK_DISK_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkDiskSource.h"

class CVtkDiskSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(double InnerRadius READ innerRadius WRITE setInnerRadius)
    Q_PROPERTY(double OuterRadius READ outerRadius WRITE setOuterRadius)
    Q_PROPERTY(int RadialResolution READ radialResolution WRITE setRadialResolution)
    Q_PROPERTY(int CircumferentialResolution READ circumferentialResolution WRITE setCircumferentialResolution)
    DECLARE_VTK_OBJECT

public:
    CVtkDiskSource();
    ~CVtkDiskSource();

    double innerRadius() const;
    void setInnerRadius(double ir);

    double outerRadius() const;
    void setOuterRadius(double rad);

    int radialResolution() const;
    void setRadialResolution(int rr);

    int circumferentialResolution() const;
    void setCircumferentialResolution(int cr);

private:
    vtkDiskSource* m_vtkDiskSource;
};

#endif

