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

#ifndef C_VTK_SPLINE_FILTER_H
#define C_VTK_SPLINE_FILTER_H

#include "vtkSplineFilter.h"
#include "CVtkPolyDataFilter.h"

class CVtkSplineFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(Subdivide)
    Q_ENUMS(GenerateTCoords)
    Q_PROPERTY(int MaximumNumberOfSubdivisions READ maximumNumberOfSubdivisions WRITE setMaximumNumberOfSubdivisions)
    Q_PROPERTY(Subdivide Subdivide READ subdivide WRITE setSubdivide)
    Q_PROPERTY(int NumberOfSubdivisions READ numberOfSubdivisions WRITE setNumberOfSubdivisions)
    Q_PROPERTY(double Length READ length WRITE setLength)
    Q_PROPERTY(GenerateTCoords GenerateTCoords READ generateTCoords WRITE setGenerateTCoords)
    Q_PROPERTY(double TextureLength READ textureLength WRITE setTextureLength)
    DECLARE_VTK_OBJECT

public:
    enum Subdivide
    {
        Specified=VTK_SUBDIVIDE_SPECIFIED,
        Length=VTK_SUBDIVIDE_LENGTH
    };
    enum GenerateTCoords
    {
        Off=VTK_TCOORDS_OFF,
        NormalizedLength=VTK_TCOORDS_FROM_NORMALIZED_LENGTH,
        FromLength=VTK_TCOORDS_FROM_LENGTH,
        FromScalars=VTK_TCOORDS_FROM_SCALARS
    };

    CVtkSplineFilter();
    ~CVtkSplineFilter();

    int maximumNumberOfSubdivisions() const;
    void setMaximumNumberOfSubdivisions(int val);

    Subdivide subdivide() const;
    void setSubdivide(Subdivide val);

    int numberOfSubdivisions() const;
    void setNumberOfSubdivisions(int val);

    double length() const;
    void setLength(double val);

    GenerateTCoords generateTCoords() const;
    void setGenerateTCoords(GenerateTCoords val);

    double textureLength() const;
    void setTextureLength(double val);


private:
    vtkSplineFilter *m_vtkSplineFilter;
};
#endif//C_VTK_SPLINE_FILTER_H