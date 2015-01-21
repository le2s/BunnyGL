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
#ifndef C_VTK_CURVATURES_H
#define C_VTK_CURVATURES_H

#include "vtkCurvatures.h"
#include "CVtkPolyDataFilter.h"

class CVtkCurvatures : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(CurvatureType)
    Q_PROPERTY(CurvatureType CurvatureType READ curvatureType WRITE setCurvatureType)
    Q_PROPERTY(bool InvertMeanCurvature READ isInvertMeanCurvature WRITE setInvertMeanCurvature)
    DECLARE_VTK_OBJECT

public:
    CVtkCurvatures();
    ~CVtkCurvatures();

    enum CurvatureType
    {
        Gauss = VTK_CURVATURE_GAUSS,
        Mean = VTK_CURVATURE_MEAN,
        Maximum = VTK_CURVATURE_MAXIMUM,
        Minimum = VTK_CURVATURE_MINIMUM
    };

    CurvatureType curvatureType() const;
    void setCurvatureType(CurvatureType val);

    bool isInvertMeanCurvature() const;
    void setInvertMeanCurvature(bool val);


private:
    vtkCurvatures *m_vtkCurvatures;
};
#endif//C_VTK_CURVATURES_H