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

#ifndef C_VTK_LINE_SOURCE_H
#define C_VTK_LINE_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkLineSource.h"
#include "UserDefinedDataTypes.h"
#include "CVtkPolyData.h"

class CVtkLineSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    
    Q_PROPERTY(Point3D Point1 READ point1 WRITE setPoint1)
    Q_PROPERTY(Point3D Point2 READ point2 WRITE setPoint2)
    Q_PROPERTY(int Resolution READ resolution WRITE setResolution)
    DECLARE_VTK_OBJECT

public:
    CVtkLineSource();
    ~CVtkLineSource();

    Point3D point1() const;
    void setPoint1(Point3D point1);

    Point3D point2() const;
    void setPoint2(Point3D point2);

    void setResolution(int res);
    int resolution() const;

private:
    vtkLineSource* m_vtkLineSource;
};

#endif

