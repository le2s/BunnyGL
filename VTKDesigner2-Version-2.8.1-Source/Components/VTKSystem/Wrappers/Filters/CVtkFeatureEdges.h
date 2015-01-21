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

#ifndef C_VTK_FEATURES_EDGES_H
#define C_VTK_FEATURES_EDGES_H

#include "CVtkPolyDataFilter.h"
#include "vtkFeatureEdges.h"

class CVtkFeatureEdges : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(bool BoundaryEdges READ isBoundaryEdges WRITE setBoundaryEdges)
    Q_PROPERTY(bool FeatureEdges READ isFeatureEdges WRITE setFeatureEdges)
    Q_PROPERTY(double FeatureAngle READ featureAngle WRITE setFeatureAngle)
    Q_PROPERTY(bool NonManifoldEdges READ isNonManifoldEdges WRITE setNonManifoldEdges)
    Q_PROPERTY(bool ManifoldEdges READ isManifoldEdges WRITE setManifoldEdges)
    Q_PROPERTY(bool Coloring READ isColoring WRITE setColoring)
    DECLARE_VTK_OBJECT

public:
    CVtkFeatureEdges();
    ~CVtkFeatureEdges();

     void     setBoundaryEdges(bool val);
     bool     isBoundaryEdges() const;

     void     setFeatureEdges(bool val);
     bool     isFeatureEdges() const;

     void     setFeatureAngle (double val);
     double featureAngle() const;

     void     setNonManifoldEdges(bool val);
     bool     isNonManifoldEdges() const;

     void     setManifoldEdges(bool val);
     bool     isManifoldEdges() const;

     void     setColoring(bool val);
     bool     isColoring() const;

private:
    vtkFeatureEdges *m_vtkFeatureEdges;
};


#endif