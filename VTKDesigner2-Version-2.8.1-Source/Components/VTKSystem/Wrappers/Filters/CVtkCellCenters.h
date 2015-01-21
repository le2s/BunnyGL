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

#ifndef C_VTK_CELL_CENTERS_H
#define C_VTK_CELL_CENTERS_H

#include "vtkCellCenters.h"
#include "CVtkPolyDataFilter.h"

class CVtkCellCenters : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(bool VertexCells READ isVertexCells WRITE setVertexCells)
    DECLARE_VTK_OBJECT

public:

    CVtkCellCenters();
    ~CVtkCellCenters();

    bool isVertexCells() const;
    void setVertexCells(bool val);


private:
    vtkCellCenters *m_vtkCellCenters;
};
#endif//C_VTK_CELL_CENTERS_H