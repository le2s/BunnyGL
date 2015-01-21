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

#ifndef C_VTK_POLY_DATA_NORMALS_H
#define C_VTK_POLY_DATA_NORMALS_H

#include "CVtkPolyDataFilter.h"
#include "vtkPolyDataNormals.h"
#include "UserDefinedDataTypes.h"

class CVtkPolyDataNormals : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(double FeatureAngle READ featureAngle WRITE setFeatureAngle)
    Q_PROPERTY(bool Splitting READ isSplitting WRITE setSplitting)
    Q_PROPERTY(bool Consistency READ isConsistency WRITE setConsistency)
    Q_PROPERTY(bool AutoOrientNormals READ isAutoOrientNormals WRITE setAutoOrientNormals)
    Q_PROPERTY(bool ComputePointNormals READ isComputePointNormals WRITE setComputePointNormals)
    Q_PROPERTY(bool ComputeCellNormals READ isComputeCellNormals WRITE setComputeCellNormals)
    Q_PROPERTY(bool FlipNormals READ isFlipNormals WRITE setFlipNormals)
    Q_PROPERTY(bool NonManifoldTraversal READ isNonManifoldTraversal WRITE setNonManifoldTraversal)
    DECLARE_VTK_OBJECT

public:
    CVtkPolyDataNormals();
    ~CVtkPolyDataNormals();

    void setFeatureAngle(double val);
    double featureAngle() const;

    void setSplitting(bool val);
    bool isSplitting() const;

    void setConsistency(bool val);
    bool isConsistency() const;

    void setAutoOrientNormals(bool val);
    bool isAutoOrientNormals() const;

    void setComputePointNormals(bool val);
    bool isComputePointNormals() const;

    void setComputeCellNormals(bool val);
    bool isComputeCellNormals() const;

    void setFlipNormals(bool val);
    bool isFlipNormals() const;

    void setNonManifoldTraversal(bool val);
    bool isNonManifoldTraversal() const;

private:
    vtkPolyDataNormals* m_vtkPolyDataNormals;
};

#endif
