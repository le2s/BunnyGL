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

#include "CVtkCenterlineFilter.h"

DEFINE_VTK_OBJECT(CVtkCenterlineFilter, CVtkPolyDataFilter, vtkCenterlineFilter)
{
    pDesc->setNodeClassName("vtkCenterlineFilter");
    pDesc->setNodeClassDescription("Extract a center line from a tunnel polydata");

    pDesc->setPropertyInfo("StartingPoint", "SetStartingPoint", "GetStartingPoint", QStringList()<<"double"<<"double"<<"double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("StartingDirection", "SetStartingDirection", "GetStartingDirection", QStringList()<<"double"<<"double"<<"double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("StartingViewUp", "SetStartingViewUp", "GetStartingViewUp", QStringList()<<"double"<<"double"<<"double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("StepDistance", "SetStepDistance", "GetStepDistance", QStringList()<<"double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("StartingRadius", "SetStartingRadius", "GetStartingRadius", QStringList()<<"double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("MaxIterations", "SetMaxIterations", "GetMaxIterations", QStringList() << "int", QStringList(), "void", "int");

	pDesc->setNodeIcon(qApp->windowIcon()); // vtkCustomFunction is not a standard VTK class
}

CVtkCenterlineFilter::CVtkCenterlineFilter() : m_vtkCenterlineFilter(0)
{
    CVtkCenterlineFilter::InitializeObjectDesc();
    setVtkObject(vtkCenterlineFilter::New());
}

CVtkCenterlineFilter::~CVtkCenterlineFilter()
{

}

void CVtkCenterlineFilter::setStartingPoint(Point3D point)
{
    m_vtkCenterlineFilter->SetStartingPoint(point.x, point.y, point.z);   
}

Point3D CVtkCenterlineFilter::startingPoint() const
{
    return m_vtkCenterlineFilter->GetStartingPoint();
}

void CVtkCenterlineFilter::setStartingDirection(Vector3D vec)
{
    m_vtkCenterlineFilter->SetStartingDirection(vec.x, vec.y, vec.z);
}

Vector3D CVtkCenterlineFilter::startingDirection() const
{
    return m_vtkCenterlineFilter->GetStartingDirection();
}

void CVtkCenterlineFilter::setStartingViewUp(Vector3D vec)
{
    m_vtkCenterlineFilter->SetStartingViewUp(vec.x, vec.y, vec.z);
}

Vector3D CVtkCenterlineFilter::startingViewUp() const
{
    return m_vtkCenterlineFilter->GetStartingViewUp();
}

void CVtkCenterlineFilter::setStepDistance(double val)
{
    m_vtkCenterlineFilter->SetStepDistance(val);
}

double CVtkCenterlineFilter::stepDistance() const
{
    return m_vtkCenterlineFilter->GetStepDistance();
}

void CVtkCenterlineFilter::setStartingRadius(double val)
{
    m_vtkCenterlineFilter->SetStartingRadius(val);
}

double CVtkCenterlineFilter::startingRadius() const
{
    return m_vtkCenterlineFilter->GetStartingRadius();
}

void CVtkCenterlineFilter::setMaxIterations(int iter)
{
    m_vtkCenterlineFilter->SetMaxIterations(iter);
}

int CVtkCenterlineFilter::maxIterations() const
{
    return m_vtkCenterlineFilter->GetMaxIterations();
}

