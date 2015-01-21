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
#ifndef C_VTK_POINT_LOCATOR_H
#define C_VTK_POINT_LOCATOR_H

#include "vtkPointLocator.h"
#include "CVtkLocator.h"
#include "UserDefinedDataTypes.h"

class CVtkPointLocator : public CVtkLocator
{
    Q_OBJECT
    Q_PROPERTY(int NumberOfPointsPerBucket READ numberOfPointsPerBucket WRITE setNumberOfPointsPerBucket)
    Q_PROPERTY(QList<QVariant> Divisions READ divisions WRITE setDivisions)
    DECLARE_VTK_OBJECT

public:
    CVtkPointLocator();
    ~CVtkPointLocator();

    int numberOfPointsPerBucket() const;
    void setNumberOfPointsPerBucket(int val);

    void setDivisions(QList<QVariant> list);
    QList<QVariant> divisions() const;

    Q_INVOKABLE int findClosestPointIndex(Point3D point);
    Q_INVOKABLE int findClosestPointIndexWithinRadius(Point3D point, double radius);
    Q_INVOKABLE QList<int> findClosestNPointIndicies(Point3D point, int n);
    Q_INVOKABLE QList<int> findPointIndiciesWithinRadius(Point3D point, double radius);

    Q_INVOKABLE Point3D findClosestPoint(Point3D point);
    Q_INVOKABLE Point3D findClosestPointWithinRadius(Point3D point, double radius);
    Q_INVOKABLE QList<double> findClosestNPoints(Point3D point, int n);
    Q_INVOKABLE QList<double> findPointsWithinRadius(Point3D point, double radius);

private:
    vtkPointLocator *m_vtkPointLocator;
};
#endif//C_VTK_POINT_LOCATOR_H