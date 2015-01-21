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

#include "CVtkCleanPolyData.h"

DEFINE_VTK_OBJECT(CVtkCleanPolyData,CVtkPolyDataFilter,vtkCleanPolyData)
{
    pDesc->setNodeClassName("vtkCleanPolyData");
    pDesc->setNodeClassDescription("Merge duplicate points, and/or remove unused points and/or remove degenerate cells");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("ToleranceIsAbsolute", "SetToleranceIsAbsolute", "GetToleranceIsAbsolute", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("PieceInvariant", "SetPieceInvariant", "GetPieceInvariant", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("Tolerance", "SetTolerance", "GetTolerance", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("AbsoluteTolerance", "SetAbsoluteTolerance", "GetAbsoluteTolerance", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("ConvertLinesToPoints", "SetConvertLinesToPoints", "GetConvertLinesToPoints", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ConvertPolysToLines", "SetConvertPolysToLines", "GetConvertPolysToLines", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ConvertStripsToPolys", "SetConvertStripsToPolys", "GetConvertStripsToPolys", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("PointMerging", "SetPointMerging", "GetPointMerging", QStringList() << "int" , QStringList(), "void", "int");
}

CVtkCleanPolyData::CVtkCleanPolyData() : m_vtkCleanPolyData(0)
{
    CVtkCleanPolyData::InitializeObjectDesc();
    setVtkObject(vtkCleanPolyData::New());
}

CVtkCleanPolyData::~CVtkCleanPolyData()
{

}

bool CVtkCleanPolyData::isToleranceIsAbsolute() const
{
    return m_vtkCleanPolyData->GetToleranceIsAbsolute();
}


void CVtkCleanPolyData::setToleranceIsAbsolute(bool val)
{
    m_vtkCleanPolyData->SetToleranceIsAbsolute(val);
}


bool CVtkCleanPolyData::isPieceInvariant() const
{
    return m_vtkCleanPolyData->GetPieceInvariant();
}


void CVtkCleanPolyData::setPieceInvariant(bool val)
{
    m_vtkCleanPolyData->SetPieceInvariant(val);
}


double CVtkCleanPolyData::tolerance() const
{
    return m_vtkCleanPolyData->GetTolerance();
}


void CVtkCleanPolyData::setTolerance(double val)
{
    m_vtkCleanPolyData->SetTolerance(val);
}


double CVtkCleanPolyData::absoluteTolerance() const
{
    return m_vtkCleanPolyData->GetAbsoluteTolerance();
}


void CVtkCleanPolyData::setAbsoluteTolerance(double val)
{
    m_vtkCleanPolyData->SetAbsoluteTolerance(val);
}


bool CVtkCleanPolyData::isConvertLinesToPoints() const
{
    return m_vtkCleanPolyData->GetConvertLinesToPoints();
}


void CVtkCleanPolyData::setConvertLinesToPoints(bool val)
{
    m_vtkCleanPolyData->SetConvertLinesToPoints(val);
}


bool CVtkCleanPolyData::isConvertPolysToLines() const
{
    return m_vtkCleanPolyData->GetConvertPolysToLines();
}


void CVtkCleanPolyData::setConvertPolysToLines(bool val)
{
    m_vtkCleanPolyData->SetConvertPolysToLines(val);
}


bool CVtkCleanPolyData::isConvertStripsToPolys() const
{
    return m_vtkCleanPolyData->GetConvertStripsToPolys();
}


void CVtkCleanPolyData::setConvertStripsToPolys(bool val)
{
    m_vtkCleanPolyData->SetConvertStripsToPolys(val);
}


bool CVtkCleanPolyData::isPointMerging() const
{
    return m_vtkCleanPolyData->GetPointMerging();
}


void CVtkCleanPolyData::setPointMerging(bool val)
{
    m_vtkCleanPolyData->SetPointMerging(val);
}

