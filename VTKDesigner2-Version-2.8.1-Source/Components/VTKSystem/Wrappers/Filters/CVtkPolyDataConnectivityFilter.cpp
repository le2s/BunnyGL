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
#include "CVtkPolyDataConnectivityFilter.h"


DEFINE_VTK_OBJECT(CVtkPolyDataConnectivityFilter, CVtkPolyDataFilter, vtkPolyDataConnectivityFilter)
{
    pDesc->setNodeClassName("vtkPolyDataConnectivityFilter");
    pDesc->setNodeClassDescription("Extract polygonal data based on geometric connectivity");

    pDesc->setPropertyInfo("ScalarConnectivity","SetScalarConnectivity","GetScalarConnectivity",QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("ScalarRange","SetScalarRange","GetScalarRange",QStringList()<<"double"<<"double",QStringList(),"void","double*");
    pDesc->setPropertyInfo("ExtractionMode","SetExtractionMode","GetExtractionMode",QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("ClosestPoint","SetClosestPoint","GetClosestPoint",QStringList()<<"double"<<"double"<<"double",QStringList(),"void","double*");
    pDesc->setPropertyInfo("ColorRegions","SetColorRegions","GetColorRegions",QStringList()<<"int",QStringList(),"void","int");
}


CVtkPolyDataConnectivityFilter::CVtkPolyDataConnectivityFilter() : m_vtkPolyDataConnectivityFilter(0)
{
    CVtkPolyDataConnectivityFilter::InitializeObjectDesc();
    setVtkObject(vtkPolyDataConnectivityFilter::New());
}

CVtkPolyDataConnectivityFilter::~CVtkPolyDataConnectivityFilter()
{

}

bool CVtkPolyDataConnectivityFilter::isScalarConnectivity() const
{
    return m_vtkPolyDataConnectivityFilter->GetScalarConnectivity();
}

void CVtkPolyDataConnectivityFilter::setScalarConnectivity(bool val)
{
    m_vtkPolyDataConnectivityFilter->SetScalarConnectivity(val);
}

ValueRange  CVtkPolyDataConnectivityFilter::scalarRange() const
{
    return ValueRange(m_vtkPolyDataConnectivityFilter->GetScalarRange());
}

void CVtkPolyDataConnectivityFilter::setScalarRange(ValueRange  val)
{
    m_vtkPolyDataConnectivityFilter->SetScalarRange(val.min, val.max);
}


CVtkPolyDataConnectivityFilter::ExtractionMode CVtkPolyDataConnectivityFilter::extractionMode() const
{
    switch(m_vtkPolyDataConnectivityFilter->GetExtractionMode())
    {
        case VTK_EXTRACT_POINT_SEEDED_REGIONS:return PointSeededRegions;
        case VTK_EXTRACT_CELL_SEEDED_REGIONS: return CellSeededRegions;
        case VTK_EXTRACT_SPECIFIED_REGIONS:return SpecifiedRegions;
        case VTK_EXTRACT_LARGEST_REGION: return LargestRegion;
        case VTK_EXTRACT_ALL_REGIONS:return AllRegion ;
        case VTK_EXTRACT_CLOSEST_POINT_REGION : return ClosestPointRegion; 
        default :return PointSeededRegions;
    }
}


void CVtkPolyDataConnectivityFilter::setExtractionMode(ExtractionMode val)
{
    switch(val)
    {
        case PointSeededRegions: m_vtkPolyDataConnectivityFilter->SetExtractionMode(VTK_EXTRACT_POINT_SEEDED_REGIONS);break;
        case CellSeededRegions: m_vtkPolyDataConnectivityFilter->SetExtractionMode(VTK_EXTRACT_CELL_SEEDED_REGIONS); break;
        case SpecifiedRegions: m_vtkPolyDataConnectivityFilter->SetExtractionMode(VTK_EXTRACT_SPECIFIED_REGIONS);break;
        case LargestRegion: m_vtkPolyDataConnectivityFilter->SetExtractionMode(VTK_EXTRACT_LARGEST_REGION);break;
        case AllRegion: m_vtkPolyDataConnectivityFilter->SetExtractionMode(VTK_EXTRACT_ALL_REGIONS);break;
        case ClosestPointRegion :m_vtkPolyDataConnectivityFilter->SetExtractionMode(VTK_EXTRACT_CLOSEST_POINT_REGION);break; 
        default:m_vtkPolyDataConnectivityFilter->SetExtractionMode(VTK_EXTRACT_POINT_SEEDED_REGIONS);break;
    }
}


Point3D CVtkPolyDataConnectivityFilter::closestPoint() const
{
    return Point3D(m_vtkPolyDataConnectivityFilter->GetClosestPoint());
}


void CVtkPolyDataConnectivityFilter::setClosestPoint(Point3D p)
{
    m_vtkPolyDataConnectivityFilter->SetClosestPoint(p.x,p.y,p.z);
}


bool CVtkPolyDataConnectivityFilter::isColorRegions() const
{
    return m_vtkPolyDataConnectivityFilter->GetColorRegions();
}


void CVtkPolyDataConnectivityFilter::setColorRegions(bool val)
{
    m_vtkPolyDataConnectivityFilter->SetColorRegions(val);
}
