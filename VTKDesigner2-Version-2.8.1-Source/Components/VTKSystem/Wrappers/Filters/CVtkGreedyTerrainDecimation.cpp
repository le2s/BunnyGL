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
#include "CVtkGreedyTerrainDecimation.h"



DEFINE_VTK_OBJECT(CVtkGreedyTerrainDecimation, CVtkPolyDataFilter,vtkGreedyTerrainDecimation)
{
    pDesc->setNodeClassName("vtkGreedyTerrainDecimation");
    pDesc->setNodeClassDescription("Reduce height field (represented as image) to reduced TIN");

    pDesc->setPropertyInfo("ErrorMessage","SetErrorMessage","GetErrorMessage",QStringList() << "int",QStringList(),"void","int");
    pDesc->setPropertyInfo("NumberOfTriangles","SetNumberOfTriangles","GetNumberOfTriangles",QStringList()<<"vtkIdType",QStringList(),"void","vtkIdType");
    pDesc->setPropertyInfo("Reduction","SetReduction","GetReduction",QStringList() << "double",QStringList(),"void","double");
    pDesc->setPropertyInfo("AbsoluteError","SetAbsoluteError","GetAbsoluteError",QStringList()<<"double",QStringList(),"void","double");
    pDesc->setPropertyInfo("RelativeError","SetRelativeError","GetRelativeError",QStringList()<<"double",QStringList(),"void","double");
    pDesc->setPropertyInfo("BoundaryVertexDeletion","SetBoundaryVertexDeletion","GetBoundaryVertexDeletion",QStringList()<<"int",QStringList(),"void","int");
}

CVtkGreedyTerrainDecimation::CVtkGreedyTerrainDecimation(): m_vtkGreedyTerrainDecimation(0)
{
    CVtkGreedyTerrainDecimation::InitializeObjectDesc();
    setVtkObject(vtkGreedyTerrainDecimation::New());
}
    
CVtkGreedyTerrainDecimation::~CVtkGreedyTerrainDecimation()
{

}

CVtkGreedyTerrainDecimation::ErrorMeasure CVtkGreedyTerrainDecimation::errorMeasure() const
{
    switch(m_vtkGreedyTerrainDecimation->GetErrorMeasure())
    {
    case VTK_ERROR_NUMBER_OF_TRIANGLES: return NumberOfTriangles ; 
    case VTK_ERROR_SPECIFIED_REDUCTION: return SpecifiedReduction ;
    case VTK_ERROR_ABSOLUTE: return Absolute ;
    case VTK_ERROR_RELATIVE:return Relative;
    default:return NumberOfTriangles ; 
    }
}

void CVtkGreedyTerrainDecimation::setErrorMeasure(ErrorMeasure e)
{
    switch (e)
    {
    case NumberOfTriangles: m_vtkGreedyTerrainDecimation->SetErrorMeasure(VTK_ERROR_NUMBER_OF_TRIANGLES);break;
    case SpecifiedReduction: m_vtkGreedyTerrainDecimation->SetErrorMeasure(VTK_ERROR_SPECIFIED_REDUCTION); break;
    case Absolute : m_vtkGreedyTerrainDecimation->SetErrorMeasure(VTK_ERROR_ABSOLUTE); break;
    case Relative : m_vtkGreedyTerrainDecimation->SetErrorMeasure(VTK_ERROR_RELATIVE); break;
    default:m_vtkGreedyTerrainDecimation->SetErrorMeasure(VTK_ERROR_NUMBER_OF_TRIANGLES);break;
    }
}


int CVtkGreedyTerrainDecimation::numberOfTriangles() const
{
    return int(m_vtkGreedyTerrainDecimation->GetNumberOfTriangles());
}

void CVtkGreedyTerrainDecimation::setNumberOfTriangles(int n)
{
    m_vtkGreedyTerrainDecimation->SetNumberOfTriangles(vtkIdType(n));
}

double CVtkGreedyTerrainDecimation::reduction() const
{
    return m_vtkGreedyTerrainDecimation->GetReduction();
}

void CVtkGreedyTerrainDecimation::setReduction(double val)
{
    m_vtkGreedyTerrainDecimation->SetReduction(val);
}

double CVtkGreedyTerrainDecimation::absoluteError() const
{
    return m_vtkGreedyTerrainDecimation->GetAbsoluteError();
}

void CVtkGreedyTerrainDecimation::setAbsoluteError(double val)
{
    m_vtkGreedyTerrainDecimation->SetAbsoluteError(val);
}


double CVtkGreedyTerrainDecimation::relativeError() const
{
    return m_vtkGreedyTerrainDecimation->GetRelativeError();
}

void CVtkGreedyTerrainDecimation::setRelativeError(double val)
{
    m_vtkGreedyTerrainDecimation->SetRelativeError(val);
}


bool CVtkGreedyTerrainDecimation::isBoundaryVertexDeletion() const
{
    return m_vtkGreedyTerrainDecimation->GetBoundaryVertexDeletion();
}

void CVtkGreedyTerrainDecimation::setBoundaryVertexDeletion(bool val)
{
    m_vtkGreedyTerrainDecimation->SetBoundaryVertexDeletion(val);
}


bool CVtkGreedyTerrainDecimation::isComputeNormals() const
{
    return m_vtkGreedyTerrainDecimation->GetComputeNormals();
}

void CVtkGreedyTerrainDecimation::setComputeNormals(bool val)
{
    m_vtkGreedyTerrainDecimation->SetComputeNormals(val);
}


