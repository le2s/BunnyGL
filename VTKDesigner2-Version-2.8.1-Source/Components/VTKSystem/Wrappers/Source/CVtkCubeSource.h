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


#ifndef C_VTK_CUBE_SOURCE_H
#define C_VTK_CUBE_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkCubeSource.h"
#include "UserDefinedDataTypes.h"
#include "CVtkPolyData.h"

class CVtkCubeSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(double XLength READ xLength WRITE setXLength)
    Q_PROPERTY(double YLength READ yLength WRITE setYLength)
    Q_PROPERTY(double ZLength READ zLength WRITE setZLength)
    Q_PROPERTY(Point3D Center READ center WRITE setCenter)
    DECLARE_VTK_OBJECT

public:
    CVtkCubeSource();
    ~CVtkCubeSource();

    double xLength() const;
    void setXLength(double xl);

    double yLength() const;
    void setYLength(double yl);

    double zLength() const;
    void setZLength(double zl);

    Point3D center() const;
    void setCenter(Point3D p);

private:
    vtkCubeSource* m_vtkCubeSource;
};

#endif

