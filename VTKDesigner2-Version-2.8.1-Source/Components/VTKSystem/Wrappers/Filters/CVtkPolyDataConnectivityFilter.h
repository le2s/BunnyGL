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

#ifndef C_VTK_POLYDATA_CONNECTIVITY_FILTER_H
#define C_VTK_POLYDATA_CONNECTIVITY_FILTER_H

#include "vtkPolyDataConnectivityFilter.h"
#include "CVtkPolyDataFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkPolyDataConnectivityFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(ExtractionMode)
    Q_PROPERTY(bool ScalarConnectivity READ isScalarConnectivity WRITE setScalarConnectivity)
    Q_PROPERTY(ExtractionMode ExtractionMode READ extractionMode WRITE setExtractionMode)
    Q_PROPERTY(ValueRange ScalarRange READ scalarRange WRITE setScalarRange)
    Q_PROPERTY(Point3D ClosestPoint READ closestPoint WRITE setClosestPoint)
    Q_PROPERTY(bool ColorRegions READ isColorRegions WRITE setColorRegions)
    DECLARE_VTK_OBJECT

public:
    CVtkPolyDataConnectivityFilter();
    ~CVtkPolyDataConnectivityFilter();

    enum ExtractionMode
    {
        PointSeededRegions = VTK_EXTRACT_POINT_SEEDED_REGIONS,
        CellSeededRegions = VTK_EXTRACT_CELL_SEEDED_REGIONS,
        SpecifiedRegions = VTK_EXTRACT_SPECIFIED_REGIONS,
        LargestRegion =  VTK_EXTRACT_LARGEST_REGION,
        AllRegion = VTK_EXTRACT_ALL_REGIONS,
        ClosestPointRegion = VTK_EXTRACT_CLOSEST_POINT_REGION 
    };

    bool isScalarConnectivity() const;
    void setScalarConnectivity(bool val);

 

    ValueRange  scalarRange() const;
    void setScalarRange(ValueRange  val);

    ExtractionMode extractionMode() const;
    void setExtractionMode(ExtractionMode val);

    Point3D closestPoint() const;
    void setClosestPoint(Point3D p);

    bool isColorRegions() const;
    void setColorRegions(bool val);


private:
    vtkPolyDataConnectivityFilter *m_vtkPolyDataConnectivityFilter;

};


#endif