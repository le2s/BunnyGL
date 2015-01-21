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

#ifndef C_SCRIPTABLE_VTK_POLY_DATA_H
#define C_SCRIPTABLE_VTK_POLY_DATA_H

#include "CScriptableVtkPointSet.h"
#include "vtkPolyData.h"
#include "UserDefinedDataTypes.h"
#include <QList>

class CScriptableVtkPolyData : public CScriptableVtkPointSet
{
    Q_OBJECT

public:
    CScriptableVtkPolyData(vtkPolyData* polyData, CVtkObject* source);
    ~CScriptableVtkPolyData();

    Q_INVOKABLE void setVerts(QList<int> v);
    Q_INVOKABLE QList<int> verts() const;
    Q_INVOKABLE int vert(int index) const;
    Q_INVOKABLE void setVert(int index, int vert) const;

    Q_INVOKABLE void setLines(QList<int> lines);
    Q_INVOKABLE QList<int> lines() const;
    Q_INVOKABLE LineSegment line(int index) const;
    Q_INVOKABLE void setLine(int index, LineSegment lineSeg) const;

    Q_INVOKABLE void setStrips(QList<int> strips);
    Q_INVOKABLE QList<int> strips() const;
    Q_INVOKABLE Triangle strip(int index) const;
    Q_INVOKABLE void setStrip(int index, Triangle tgl) const;

    Q_INVOKABLE void setPolys(QList<int> polys);
    Q_INVOKABLE QList<int> polys() const;
    Q_INVOKABLE QList<int> poly(int index) const;

    Q_INVOKABLE QList<double> cellNormals() const;
    Q_INVOKABLE void setCellNormals(QList<double> normalsArray);
    Q_INVOKABLE Vector3D cellNormal(int index) const;
    Q_INVOKABLE void setCellNormal(int index, Vector3D normalVec) const;

    Q_INVOKABLE QList<double> cellScalars() const;
    Q_INVOKABLE void setCellScalars(QList<double> scalars);
    Q_INVOKABLE double cellScalar(int index) const;
    Q_INVOKABLE void setCellScalar(int index, double value) const;

    Q_INVOKABLE int numberOfVerts() const;
    Q_INVOKABLE int numberOfLines() const;
    Q_INVOKABLE int numberOfStrips() const;
    Q_INVOKABLE int numberOfPolys() const;
    Q_INVOKABLE int numberOfCells() const;

private:
    vtkPolyData* m_vtkPolyData;
};

#endif
