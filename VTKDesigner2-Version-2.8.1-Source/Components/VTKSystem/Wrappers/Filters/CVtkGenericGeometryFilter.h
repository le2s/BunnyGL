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

#ifndef C_VTK_GENERIC_GEOMETRY_FILTER_H
#define C_VTK_GENERIC_GEOMETRY_FILTER_H

#include "vtkGenericGeometryFilter.h"
#include "CVtkPolyDataFilter.h"

class CVtkGenericGeometryFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(bool PointClipping READ isPointClipping WRITE setPointClipping)
    Q_PROPERTY(bool CellClipping READ isCellClipping WRITE setCellClipping)
    Q_PROPERTY(bool ExtentClipping READ isExtentClipping WRITE setExtentClipping)
    Q_PROPERTY(int PointMinimum READ pointMinimum WRITE setPointMinimum)
    Q_PROPERTY(int PointMaximum READ pointMaximum WRITE setPointMaximum)
    Q_PROPERTY(int CellMaximum READ cellMaximum WRITE setCellMaximum)
    Q_PROPERTY(bool Merging READ isMerging WRITE setMerging)
    DECLARE_VTK_OBJECT

public:

    CVtkGenericGeometryFilter();
    ~CVtkGenericGeometryFilter();

    bool isPointClipping() const;
    void setPointClipping(bool val);

    bool isCellClipping() const;
    void setCellClipping(bool val);

    bool isExtentClipping() const;
    void setExtentClipping(bool val);

    int pointMinimum() const;
    void setPointMinimum(int val);

    int pointMaximum() const;
    void setPointMaximum(int val);

    int cellMaximum() const;
    void setCellMaximum(int val);

    bool isMerging() const;
    void setMerging(bool val);


private:
    vtkGenericGeometryFilter *m_vtkGenericGeometryFilter;
};
#endif//C_VTK_GENERIC_GEOMETRY_FILTER_H