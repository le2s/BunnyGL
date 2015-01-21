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

#ifndef C_VTK_TRIANGLE_FILTER_H
#define C_VTK_TRIANGLE_FILTER_H

#include "CVtkPolyDataFilter.h"
#include "vtkTriangleFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkTriangleFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(bool PassVerts READ isPassVerts WRITE setPassVerts)
    Q_PROPERTY(bool PassLines READ isPassLines WRITE setPassLines)
    DECLARE_VTK_OBJECT

public:
    CVtkTriangleFilter();
    ~CVtkTriangleFilter();


    void setPassVerts(bool val);
    bool isPassVerts() const;

    void setPassLines(bool val);
    bool isPassLines() const;

private:
    vtkTriangleFilter* m_vtkTriangleFilter;
};

#endif
