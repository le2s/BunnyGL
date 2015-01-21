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

#ifndef C_VTK_GREEDY_TERRAIN_DECIMATION_H
#define C_VTK_GREEDY_TERRAIN_DECIMATION_H

#include "CVtkPolyDataFilter.h"
#include "vtkGreedyTerrainDecimation.h"


class CVtkGreedyTerrainDecimation : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(ErrorMeasure)
    Q_PROPERTY(ErrorMeasure ErrorMeasure READ errorMeasure WRITE setErrorMeasure)
    Q_PROPERTY(int NumberOfTriangles READ numberOfTriangles WRITE setNumberOfTriangles)
    Q_PROPERTY(double Reduction READ reduction WRITE setReduction)
    Q_PROPERTY(double AbsoluteError READ absoluteError WRITE setAbsoluteError)
    Q_PROPERTY(double RelativeError READ relativeError WRITE setRelativeError)
    Q_PROPERTY(bool BoundaryVertexDeletion READ isBoundaryVertexDeletion  WRITE setBoundaryVertexDeletion)
    Q_PROPERTY(bool ComputeNormals READ isComputeNormals WRITE setComputeNormals)
    DECLARE_VTK_OBJECT

public:
    CVtkGreedyTerrainDecimation();
    ~CVtkGreedyTerrainDecimation();

    enum ErrorMeasure
    {
        NumberOfTriangles = VTK_ERROR_NUMBER_OF_TRIANGLES,
        SpecifiedReduction = VTK_ERROR_SPECIFIED_REDUCTION,
        Absolute = VTK_ERROR_ABSOLUTE,
        Relative = VTK_ERROR_RELATIVE        
    };

    ErrorMeasure errorMeasure() const;
    void setErrorMeasure(ErrorMeasure e);

    int numberOfTriangles() const;
    void setNumberOfTriangles(int n);

    double reduction() const;
    void setReduction(double val);

    double absoluteError() const;
    void setAbsoluteError(double val);

    double relativeError() const;
    void setRelativeError(double val);

    bool isBoundaryVertexDeletion() const;
    void setBoundaryVertexDeletion(bool val);

    bool isComputeNormals() const;
    void setComputeNormals(bool val);


private:
    vtkGreedyTerrainDecimation *m_vtkGreedyTerrainDecimation;

};

#endif