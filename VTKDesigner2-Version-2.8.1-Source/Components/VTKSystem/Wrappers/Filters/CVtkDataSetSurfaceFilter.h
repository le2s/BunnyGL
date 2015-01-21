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
#ifndef C_VTK_DATA_SET_SURFACE_FILTER_H
#define C_VTK_DATA_SET_SURFACE_FILTER_H

#include "CVtkPolyDataFilter.h"
#include "vtkDataSetSurfaceFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkDataSetSurfaceFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(bool UseStrips READ isUseStrips WRITE setUseStrips);
    Q_PROPERTY(int PieceInvariant READ pieceInvariant WRITE setPieceInvariant);
    DECLARE_VTK_OBJECT

public:
    CVtkDataSetSurfaceFilter();
    ~CVtkDataSetSurfaceFilter();

    bool isUseStrips() const;
    void setUseStrips(bool val);

    int pieceInvariant() const;
    void setPieceInvariant(int pv);

private:
    vtkDataSetSurfaceFilter *m_vtkDataSetSurfaceFilter;

};

#endif 