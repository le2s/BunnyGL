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
#ifndef C_VTK_SMOOTH_POLY_DATA_FILTER_H
#define C_VTK_SMOOTH_POLY_DATA_FILTER_H

#include "vtkSmoothPolyDataFilter.h"
#include "CVtkPolyDataFilter.h"

class CVtkSmoothPolyDataFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(double Convergence READ convergence WRITE setConvergence)
    Q_PROPERTY(int NumberOfIterations READ numberOfIterations WRITE setNumberOfIterations)
    Q_PROPERTY(double RelaxationFactor READ relaxationFactor WRITE setRelaxationFactor)
    Q_PROPERTY(bool FeatureEdgeSmoothing READ isFeatureEdgeSmoothing WRITE setFeatureEdgeSmoothing)
    Q_PROPERTY(double FeatureAngle READ featureAngle WRITE setFeatureAngle)
    Q_PROPERTY(double EdgeAngle READ edgeAngle WRITE setEdgeAngle)
    Q_PROPERTY(bool BoundarySmoothing READ isBoundarySmoothing WRITE setBoundarySmoothing)
    Q_PROPERTY(bool GenerateErrorScalars READ isGenerateErrorScalars WRITE setGenerateErrorScalars)
    Q_PROPERTY(bool GenerateErrorVectors READ isGenerateErrorVectors WRITE setGenerateErrorVectors)
    DECLARE_VTK_OBJECT

public:

    CVtkSmoothPolyDataFilter();
    ~CVtkSmoothPolyDataFilter();

    double convergence() const;
    void setConvergence(double val);

    int numberOfIterations() const;
    void setNumberOfIterations(int val);

    double relaxationFactor() const;
    void setRelaxationFactor(double val);

    bool isFeatureEdgeSmoothing() const;
    void setFeatureEdgeSmoothing(bool val);

    double featureAngle() const;
    void setFeatureAngle(double val);

    double edgeAngle() const;
    void setEdgeAngle(double val);

    bool isBoundarySmoothing() const;
    void setBoundarySmoothing(bool val);

    bool isGenerateErrorScalars() const;
    void setGenerateErrorScalars(bool val);

    bool isGenerateErrorVectors() const;
    void setGenerateErrorVectors(bool val);

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkSmoothPolyDataFilter *m_vtkSmoothPolyDataFilter;
};
#endif//C_VTK_SMOOTH_POLY_DATA_FILTER_H