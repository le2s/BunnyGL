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

#ifndef C_SCRIPTABLE_VTK_POINT_SET_H
#define C_SCRIPTABLE_VTK_POINT_SET_H

#include "CScriptableVtkDataSet.h"
#include "vtkPointSet.h"
#include <QList>

class CScriptableVtkPointSet : public CScriptableVtkDataSet
{
    Q_OBJECT

public:
    CScriptableVtkPointSet(vtkPointSet* pointSet, CVtkObject* source);
    ~CScriptableVtkPointSet();

    Q_INVOKABLE QList<double> points() const;
    Q_INVOKABLE void setPoints(QList<double> pArray);
    Q_INVOKABLE Point3D point(int index) const;
    Q_INVOKABLE void setPoint(int index, Point3D point);

    Q_INVOKABLE QList<double> pointNormals() const;
    Q_INVOKABLE void setPointNormals(QList<double> normalsArray);
    Q_INVOKABLE Vector3D pointNormal(int index) const;
    Q_INVOKABLE void setPointNormal(int index, Vector3D vec);

    Q_INVOKABLE QList<double> pointScalars() const;
    Q_INVOKABLE void setPointScalars(QList<double> normalsArray);
    Q_INVOKABLE double pointScalar(int index) const;
    Q_INVOKABLE void setPointScalar(int index, double val);

    Q_INVOKABLE QList<double> pointTCoords() const;
    Q_INVOKABLE void setPointTCoords(QList<double> tcoords);
    Q_INVOKABLE QList<double> pointTCoord(int index) const;
    Q_INVOKABLE void setPointTCoord(int index, QList<double> val);

    Q_INVOKABLE int numberOfPoints() const;

private:
    vtkPointSet* m_vtkPointSet;
};

#endif

