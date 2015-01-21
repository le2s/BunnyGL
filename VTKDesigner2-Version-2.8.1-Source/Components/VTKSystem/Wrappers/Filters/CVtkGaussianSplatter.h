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

#ifndef C_VTK_GAUSSIAN_SPLATTER_H
#define C_VTK_GAUSSIAN_SPLATTER_H

#include "CVtkImageAlgorithm.h"
#include "vtkGaussianSplatter.h"

class CVtkGaussianSplatter : public CVtkImageAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(QList<double> ModelBounds READ modelBounds WRITE setModelBounds)
    Q_PROPERTY(double Radius READ radius WRITE setRadius)
    Q_PROPERTY(double ScaleFactor READ scaleFactor WRITE setScaleFactor)
    Q_PROPERTY(double ExponentFactor READ exponentFactor WRITE setExponentFactor)
    Q_PROPERTY(bool NormalWarping READ isNormalWarping WRITE setNormalWarping)
    Q_PROPERTY(double Eccentricity READ eccentricity WRITE setEccentricity)
    Q_PROPERTY(bool ScalarWarping READ isScalarWarping WRITE setScalarWarping)
    Q_PROPERTY(int Capping READ isCapping WRITE setCapping)
    Q_PROPERTY(QList<int> SampleDimensions READ sampleDimensions WRITE setSampleDimensions)

    DECLARE_VTK_OBJECT

public:
    CVtkGaussianSplatter();
    ~CVtkGaussianSplatter();

    QList<double> modelBounds() const;
    void setModelBounds(const QList<double>& val);

    double radius() const;
    void setRadius(double rad);

    double scaleFactor() const;
    void setScaleFactor(double fact);

    double exponentFactor() const;
    void setExponentFactor(double fact);

    bool isNormalWarping();
    void setNormalWarping(bool val);

    double eccentricity() const;
    void setEccentricity(double ecc);

    bool isScalarWarping();
    void setScalarWarping(bool val);

    bool isCapping();
    void setCapping(bool val);

    QList<int> sampleDimensions() const;
    void setSampleDimensions(const QList<int>& dim);

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkGaussianSplatter* m_vtkGaussianSplatter;

};

#endif