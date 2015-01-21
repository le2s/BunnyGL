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

#ifndef C_VTK_TRANSFORM_POLYDATA_FILTER_H
#define C_VTK_TRANSFORM_POLYDATA_FILTER_H


#include "CVtkPolyDataFilter.h"
#include "vtkTransformPolyDataFilter.h"
#include "UserDefinedDataTypes.h"

class vtkAbstractTransform;
class CVtkTransformPolyDataFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    DECLARE_VTK_OBJECT

public:
    CVtkTransformPolyDataFilter();
    ~CVtkTransformPolyDataFilter();

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

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkTransformPolyDataFilter *m_vtkTransformPolyDataFilter;
    vtkAbstractTransform* m_vtkTransform;
    bool m_transformIsSet;
};

#endif