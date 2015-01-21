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
#include "CVtkGraphLayoutFilter.h"

DEFINE_VTK_OBJECT(CVtkGraphLayoutFilter, CVtkPolyDataFilter, vtkGraphLayoutFilter)
{
    pDesc->setNodeClassDescription("Graph layout filter");
    pDesc->setNodeClassName("vtkGraphLayoutFilter");

    pDesc->setPropertyInfo("ThreeDimensionalLayout", "SetThreeDimensionalLayout","GetThreeDimensionalLayout",QStringList() << "int", QStringList(), "void","int");
    pDesc->setPropertyInfo("GraphBounds", "SetGraphBounds","GetGraphBounds",QStringList() << "double"<< "double"<< "double"<< "double"<< "double"<< "double", QStringList(), "void","double*");
    pDesc->setPropertyInfo("AutomaticBoundsComputation","AutomaticBoundsComputation","AutomaticBoundsComputation",QStringList() << "int", QStringList(), "void","int");
    pDesc->setPropertyInfo("MaxNumberOfIterations","SetMaxNumberOfIterations","GetMaxNumberOfIterations",QStringList() << "int", QStringList(), "void","int");
    pDesc->setPropertyInfo("CoolDownRate","SetCoolDownRate","GetCoolDownRate",QStringList() << "double", QStringList(), "void","double");
}


CVtkGraphLayoutFilter::CVtkGraphLayoutFilter() : m_vtkGraphLayoutFilter(0)
{
    CVtkGraphLayoutFilter::InitializeObjectDesc();
    setVtkObject(vtkGraphLayoutFilter::New());
}

CVtkGraphLayoutFilter::~CVtkGraphLayoutFilter()
{

}
void CVtkGraphLayoutFilter::setThreeDimensionalLayout(bool val)
{
    m_vtkGraphLayoutFilter->SetThreeDimensionalLayout(val);
}

bool CVtkGraphLayoutFilter::isThreeDimensionalLayout() const
{
    return m_vtkGraphLayoutFilter->GetThreeDimensionalLayout();
}

BoundingBox3D CVtkGraphLayoutFilter::graphBounds() const
{
    return BoundingBox3D(m_vtkGraphLayoutFilter->GetGraphBounds());
}

void CVtkGraphLayoutFilter::setGraphBounds(BoundingBox3D val)
{
    m_vtkGraphLayoutFilter->SetGraphBounds(val.xMin,val.xMax,val.yMin,val.yMax, val.zMin,val.zMax);
}

void CVtkGraphLayoutFilter::setAutomaticBoundsComputation(bool val)
{
    m_vtkGraphLayoutFilter->SetAutomaticBoundsComputation(val);
}

bool CVtkGraphLayoutFilter::isAutomaticBoundsComputation() const
{
    return m_vtkGraphLayoutFilter->GetAutomaticBoundsComputation();
}

void CVtkGraphLayoutFilter::setMaxNumberOfIterations(int val)
{
    m_vtkGraphLayoutFilter->SetMaxNumberOfIterations(val);
}

int CVtkGraphLayoutFilter::maxNumberOfIterations() const
{
    return m_vtkGraphLayoutFilter->GetMaxNumberOfIterations();
}

void CVtkGraphLayoutFilter::setCoolDownRate(double val)
{
    m_vtkGraphLayoutFilter->SetCoolDownRate(val);
}

double CVtkGraphLayoutFilter::coolDownRate() const
{
    return m_vtkGraphLayoutFilter->GetCoolDownRate();
}


