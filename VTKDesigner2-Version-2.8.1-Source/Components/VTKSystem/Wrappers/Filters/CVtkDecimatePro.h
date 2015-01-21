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


#ifndef C_VTK_DECIMATE_PRO_H
#define C_VTK_DECIMATE_PRO_H

#include "vtkDecimatePro.h"
#include "CVtkPolyDataFilter.h"

class CVtkDecimatePro : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(double TargetReduction READ targetReduction WRITE setTargetReduction)
    Q_PROPERTY(bool PreserveTopology READ isPreserveTopology WRITE setPreserveTopology)
    Q_PROPERTY(double FeatureAngle READ featureAngle WRITE setFeatureAngle)
    Q_PROPERTY(bool Splitting READ isSplitting WRITE setSplitting) 
    Q_PROPERTY(double SplitAngle READ splitAngle WRITE setSplitAngle)
    Q_PROPERTY(bool PreSplitMesh READ isPreSplitMesh WRITE setPreSplitMesh) 
    Q_PROPERTY(double MaximumError READ maximumError WRITE setMaximumError)
    Q_PROPERTY(bool AccumulateError  READ isAccumulateError WRITE setAccumulateError)
    Q_PROPERTY(bool ErrorIsAbsolute READ isErrorIsAbsolute WRITE setErrorIsAbsolute) 
    Q_PROPERTY(double AbsoluteError READ absoluteError WRITE setAbsoluteError)
    Q_PROPERTY(bool BoundaryVertexDeletion READ isBoundaryVertexDeletion WRITE setBoundaryVertexDeletion)
    Q_PROPERTY(int Degree READ degree WRITE setDegree)
    Q_PROPERTY(double InflectionPointRatio READ inflectionPointRatio WRITE setInflectionPointRatio)
    DECLARE_VTK_OBJECT

public:
    CVtkDecimatePro();
    ~CVtkDecimatePro();

    double targetReduction() const;
    void setTargetReduction(double val);

    bool isPreserveTopology() const;
    void setPreserveTopology(bool val);

    double featureAngle() const;
    void setFeatureAngle(double val);

    bool isSplitting() const;
    void setSplitting(bool val);

    double splitAngle() const;
    void setSplitAngle(double val);

    bool isPreSplitMesh() const;
    void setPreSplitMesh(bool val);

    double maximumError() const;
    void setMaximumError(double val);

    bool isAccumulateError() const;
    void setAccumulateError(bool val);

    bool isErrorIsAbsolute() const;
    void setErrorIsAbsolute(bool val);

    double absoluteError() const;
    void setAbsoluteError(double val);

    bool isBoundaryVertexDeletion() const;
    void setBoundaryVertexDeletion(bool val);

    int degree() const;
    void setDegree(int val);

    double inflectionPointRatio() const;
    void setInflectionPointRatio(double val);

private:
    vtkDecimatePro *m_vtkDecimatePro;

};

#endif
