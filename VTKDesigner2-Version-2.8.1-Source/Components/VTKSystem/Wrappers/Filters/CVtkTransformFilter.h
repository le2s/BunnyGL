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
#ifndef C_VTK_TRANSFORM_FILTER_H
#define C_VTK_TRANSFORM_FILTER_H

#include "vtkTransformFilter.h"
#include "CVtkPointSetFilter.h"
#include "UserDefinedDataTypes.h"

class vtkTransform;
class CVtkTransformFilter : public CVtkPointSetFilter
{
    Q_OBJECT
    DECLARE_VTK_OBJECT

public:
    CVtkTransformFilter();
    ~CVtkTransformFilter();

    Q_INVOKABLE void translate(Point3D p);
    Q_INVOKABLE void translate(double x, double y, double z);
    Q_INVOKABLE void rotateX(double angle);
    Q_INVOKABLE void rotateY(double angle);
    Q_INVOKABLE void rotateZ(double angle);
    Q_INVOKABLE void scale(Point3D p);
    Q_INVOKABLE void scale(double x, double y, double z);
    Q_INVOKABLE void preMultiply();
    Q_INVOKABLE void postMultiply();
    Q_INVOKABLE void makeIdentity();
    Q_INVOKABLE void concatenate(QList<double> elements);
    Q_INVOKABLE void setMatrix(QList<double> matrix);
    Q_INVOKABLE QList<double> getMatrix();

private:
    vtkTransformFilter *m_vtkTransformFilter;
    vtkTransform* m_vtkTransform;
};

#endif//C_VTK_TRANSFORM_FILTER_H