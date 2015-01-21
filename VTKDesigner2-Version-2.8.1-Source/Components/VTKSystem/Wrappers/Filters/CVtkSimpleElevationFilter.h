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
#ifndef C_VTK_SIMPLE_ELEVATION_FILTER_H
#define C_VTK_SIMPLE_ELEVATION_FILTER_H

#include "vtkSimpleElevationFilter.h"
#include "CVtkDataSetFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkSimpleElevationFilter : public CVtkDataSetFilter
{
    Q_OBJECT
    Q_PROPERTY(Vector3D Vector READ vector WRITE setVector)
    DECLARE_VTK_OBJECT

public:

    CVtkSimpleElevationFilter();
    ~CVtkSimpleElevationFilter();

    Vector3D vector() const;
    void setVector(Vector3D val);


private:
    vtkSimpleElevationFilter *m_vtkSimpleElevationFilter;
};
#endif//C_VTK_SIMPLE_ELEVATION_FILTER_H