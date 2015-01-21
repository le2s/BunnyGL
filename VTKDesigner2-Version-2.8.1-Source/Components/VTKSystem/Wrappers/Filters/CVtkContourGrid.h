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

#ifndef C_VTK_CONTOUR_GRID_H
#define C_VTK_CONTOUR_GRID_H

#include "vtkContourGrid.h"
#include "CVtkPolyDataFilter.h"

class CVtkContourGrid : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(int NumberOfContours READ numberOfContours WRITE setNumberOfContours)
    Q_PROPERTY(bool ComputeNormals READ isComputeNormals WRITE setComputeNormals)
    Q_PROPERTY(bool ComputeGradients READ isComputeGradients WRITE setComputeGradients)
    Q_PROPERTY(bool ComputeScalars READ isComputeScalars WRITE setComputeScalars)
    Q_PROPERTY(int UseScalarTree READ isUseScalarTree WRITE setUseScalarTree)
    DECLARE_VTK_OBJECT

public:
    CVtkContourGrid();
    ~CVtkContourGrid();

    void setNumberOfContours(int number);
    int numberOfContours ()const;

    void     setComputeNormals(bool val);
    bool     isComputeNormals() const;
    
    void     setComputeGradients (bool val);
    int     isComputeGradients() const;

    void     setComputeScalars(bool val);
    int     isComputeScalars() const;

    void     setUseScalarTree(bool val);
    int     isUseScalarTree() const;

private:
    vtkContourGrid *m_vtkContourGrid;
};

#endif 
