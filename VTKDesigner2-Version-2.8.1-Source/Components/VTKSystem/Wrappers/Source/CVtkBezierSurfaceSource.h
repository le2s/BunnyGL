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

#ifndef C_VTK_BEZIER_SURFACE_SOURCE_H
#define C_VTK_BEZIER_SURFACE_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkBezierSurfaceSource.h"
#include "CVtkPolyData.h"
#include "UserDefinedDataTypes.h"

class CVtkBezierSurfaceSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(int ControlPointCountX READ controlPointCountX WRITE setControlPointCountX)
    Q_PROPERTY(int ControlPointCountY READ controlPointCountY WRITE setControlPointCountY);
    Q_PROPERTY(int GridCountX READ gridCountX WRITE setGridCountX)
    Q_PROPERTY(int GridCountY READ gridCountY WRITE setGridCountY)
    DECLARE_VTK_OBJECT

public:
    CVtkBezierSurfaceSource();
    ~CVtkBezierSurfaceSource();

    void setControlPointCountX(int m);
    int controlPointCountX() const;

    void setControlPointCountY(int n);
    int controlPointCountY() const;

    void setGridCountX(int c);
    int gridCountX() const;

    void setGridCountY(int c);
    int gridCountY() const;

    Q_INVOKABLE void setControlPoint(int m, int n, const Point3D& pt);
    Q_INVOKABLE Point3D controlPoint(int m, int n) const;
    Q_INVOKABLE void resetControlPoints();

protected slots:
    void command_ResetControlPoints() { this->resetControlPoints(); }

protected:
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkBezierSurfaceSource* m_vtkBezierSurfaceSource;
    CVtkPolyData m_controlPointOutput;
};

#endif

