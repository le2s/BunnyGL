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

#ifndef C_VTK_EXTRACT_POLYDATA_GEOMETRY_H
#define C_VTK_EXTRACT_POLYDATA_GEOMETRY_H


#include "vtkExtractPolyDataGeometry.h"
#include "CVtkPolyDataFilter.h"

class CVtkExtractPolyDataGeometry : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(bool ExtractInside READ isExtractInside WRITE setExtractInside);
    Q_PROPERTY(bool ExtractBoundaryCells READ isExtractBoundaryCells WRITE setExtractBoundaryCells);
    DECLARE_VTK_OBJECT

public:
    CVtkExtractPolyDataGeometry();
    ~CVtkExtractPolyDataGeometry();

    bool isExtractInside() const;
    void setExtractInside(bool val);

    bool isExtractBoundaryCells() const;
    void setExtractBoundaryCells(bool val);

private:
    vtkExtractPolyDataGeometry *m_vtkExtractPolyDataGeometry;
};

#endif 