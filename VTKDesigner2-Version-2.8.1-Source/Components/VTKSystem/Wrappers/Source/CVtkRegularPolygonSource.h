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

#ifndef C_VTK_REGULAR_POLYGON_SOURCE_H
#define C_VTK_REGULAR_POLYGON_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkRegularPolygonSource.h"
#include "UserDefinedDataTypes.h"


class CVtkRegularPolygonSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(int NumberOfSides READ numberOfSides WRITE setNumberOfSides)
    Q_PROPERTY(Point3D Center READ center WRITE setCenter)
    Q_PROPERTY(Point3D Normal READ normal WRITE setNormal)
    Q_PROPERTY(double Radius READ radius WRITE setRadius)
    Q_PROPERTY(bool GeneratePolygon READ isGeneratePolygon WRITE setGeneratePolygon)
    DECLARE_VTK_OBJECT

public:
    CVtkRegularPolygonSource();
    ~CVtkRegularPolygonSource();


    int numberOfSides() const;
    void setNumberOfSides(int n);

    Point3D center() const;
    void setCenter(Point3D c);

    Point3D normal() const;
    void setNormal(Point3D n);

    double radius() const;
    void setRadius(double r);

    bool isGeneratePolygon() const;
    void setGeneratePolygon(bool val);

private:
    vtkRegularPolygonSource *m_vtkRegularPolygonSource;

};

#endif