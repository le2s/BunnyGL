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

#ifndef C_VTK_DELAUNAY_2D_H
#define C_VTK_DELAUNAY_2D_H

#include "vtkDelaunay2D.h"
#include "CVtkPolyDataFilter.h"

class CVtkDelaunay2D : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(double Alpha READ alpha WRITE setAlpha)
    Q_PROPERTY(double Tolerance READ tolerance WRITE setTolerance)
    Q_PROPERTY(double Offset READ offset WRITE setOffset)
    Q_PROPERTY(bool BoundingTriangulation READ isBoundingTriangulation WRITE setBoundingTriangulation) 
    Q_PROPERTY(bool ProjectionPlaneMode READ isProjectionPlaneMode WRITE setProjectionPlaneMode)
    DECLARE_VTK_OBJECT

public:
    CVtkDelaunay2D();
    ~CVtkDelaunay2D();

    void setAlpha(double val);
    double alpha() const;

    void setTolerance(double val);
    double tolerance() const;

    void setOffset(double val);
    double offset() const;

    void setBoundingTriangulation(bool val);
    bool isBoundingTriangulation() const;

    void setProjectionPlaneMode(bool val);
    bool isProjectionPlaneMode()const;


protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkDelaunay2D *m_vtkDelaunay2D;
};


#endif