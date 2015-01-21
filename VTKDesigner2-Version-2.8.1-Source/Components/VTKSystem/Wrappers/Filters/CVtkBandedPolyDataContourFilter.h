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

#ifndef C_VTK_BANDED_POLYDATA_CONTOUR_FILTER_H
#define C_VTK_BANDED_POLYDATA_CONTOUR_FILTER_H

#include "CVtkPolyDataFilter.h"
#include "vtkBandedPolyDataContourFilter.h"
#include "UserDefinedDataTypes.h"
     
class CVtkBandedPolyDataContourFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(ScalarMode)
    Q_PROPERTY(int NumberOfContours READ numberOfContours WRITE setNumberOfContours)
    Q_PROPERTY(bool Clipping READ isClipping WRITE setClipping)
    Q_PROPERTY(ScalarMode ScalarMode READ scalarMode WRITE setScalarMode)
    Q_PROPERTY(bool GenerateContourEdges READ isGenerateContourEdges WRITE setGenerateContourEdges)
    DECLARE_VTK_OBJECT

public:
    CVtkBandedPolyDataContourFilter();
    ~CVtkBandedPolyDataContourFilter();

    enum ScalarMode
    {
        Index = VTK_SCALAR_MODE_INDEX,
        Value = VTK_SCALAR_MODE_VALUE
    };

    int numberOfContours() const;
    void setNumberOfContours(int n);

    void setClipping(bool val);
    bool isClipping() const;

    ScalarMode scalarMode() const;
    void setScalarMode(ScalarMode s);

    void setGenerateContourEdges(bool val);
    bool isGenerateContourEdges() const;

private:
    vtkBandedPolyDataContourFilter* m_vtkBandedPolyDataContourFilter;
};

#endif
