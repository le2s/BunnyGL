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
#ifndef C_VTK_EARTH_SOURCE_H
#define C_VTK_EARTH_SOURCE_H

#include "CVtkPolyDataFilter.h"
#include "vtkEarthSource.h"

class CVtkEarthSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(double Radius READ radius WRITE setRadius)
    Q_PROPERTY(int OnRatio READ onRatio WRITE setOnRatio)
    Q_PROPERTY(bool Outline READ isOutline WRITE setOutline)
    DECLARE_VTK_OBJECT

public:
    CVtkEarthSource();
    ~CVtkEarthSource();

    double radius() const;    
    void setRadius(double r);

    int onRatio() const;
    void setOnRatio(int ratio);

    bool isOutline() const;
    void setOutline(bool ol);

private:
    vtkEarthSource *m_vtkEarthSource;
};

#endif
