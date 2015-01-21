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

#include "CVtkBandedPolyDataContourFilter.h"
                  
DEFINE_VTK_OBJECT(CVtkBandedPolyDataContourFilter, CVtkPolyDataFilter, vtkBandedPolyDataContourFilter)
{
    pDesc->setNodeClassName("vtkBandedPolyDataContourFilter");
    pDesc->setNodeClassDescription("Banded Polydata Contour Filter");
    pDesc->setPropertyInfo("Clipping", "SetClipping", "GetClipping", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ScalarMode","SetScalarMode","GetScalarMode",QStringList() << "int", QStringList(), "void","int");
    pDesc->setPropertyInfo("GenerateContourEdges", "SetGenerateContourEdges", "GetGenerateContourEdges", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("NumberOfContours","SetNumberOfContours","GetNumberOfContours",QStringList() << "int", QStringList(), "void","int");
}

CVtkBandedPolyDataContourFilter::CVtkBandedPolyDataContourFilter() : m_vtkBandedPolyDataContourFilter(0)
{
    CVtkBandedPolyDataContourFilter::InitializeObjectDesc();
    setVtkObject(vtkBandedPolyDataContourFilter::New());
}

CVtkBandedPolyDataContourFilter::~CVtkBandedPolyDataContourFilter()
{

}

int CVtkBandedPolyDataContourFilter::numberOfContours() const
{
    return m_vtkBandedPolyDataContourFilter->GetNumberOfContours();
}

void CVtkBandedPolyDataContourFilter::setNumberOfContours(int n)
{
    m_vtkBandedPolyDataContourFilter->SetNumberOfContours(n);
}


void CVtkBandedPolyDataContourFilter::setClipping(bool val)
{
    m_vtkBandedPolyDataContourFilter->SetClipping(val);    
}

bool CVtkBandedPolyDataContourFilter::isClipping() const
{
    return (m_vtkBandedPolyDataContourFilter->GetClipping());
}

CVtkBandedPolyDataContourFilter::ScalarMode CVtkBandedPolyDataContourFilter::scalarMode() const
{
    switch(m_vtkBandedPolyDataContourFilter->GetScalarMode())
    {
        case VTK_SCALAR_MODE_INDEX : return Index;    
        case VTK_SCALAR_MODE_VALUE : return Value; 
        default: return Index;
    }
}

void CVtkBandedPolyDataContourFilter::setScalarMode(CVtkBandedPolyDataContourFilter::ScalarMode s)
{
    switch(s)
    {
        case Index:    m_vtkBandedPolyDataContourFilter->SetScalarMode(VTK_SCALAR_MODE_INDEX); 
            break;
        case Value:m_vtkBandedPolyDataContourFilter->SetScalarMode(VTK_SCALAR_MODE_VALUE); 
            break;
    
        default:m_vtkBandedPolyDataContourFilter->SetScalarMode(VTK_SCALAR_MODE_INDEX);
            break;
    }
}

void CVtkBandedPolyDataContourFilter::setGenerateContourEdges(bool val)
{
    m_vtkBandedPolyDataContourFilter->SetGenerateContourEdges(val);
}

bool CVtkBandedPolyDataContourFilter::isGenerateContourEdges() const
{
    return (m_vtkBandedPolyDataContourFilter->GetGenerateContourEdges());
}
