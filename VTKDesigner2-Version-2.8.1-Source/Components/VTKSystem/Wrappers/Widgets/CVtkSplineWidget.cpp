/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this flie is limited according to the terms specified by
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

#include "CVtkSplineWidget.h"

DEFINE_VTK_OBJECT(CVtkSplineWidget, CVtk3DWidget, vtkSplineWidget)
{
    pDesc->setNodeClassName("vtkSplineWidget");
    pDesc->setNodeClassCategory("Interactor Observers");
    pDesc->setNodeClassDescription("Provides a Spline widget");
    
    pDesc->setPropertyInfo("ProjectionPosition", "SetProjectionPosition", "GetProjectionPosition", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("NumberOfHandles", "SetNumberOfHandles", "GetNumberOfHandles", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Resolution", "SetResolution", "GetResolution", QStringList() << "int", QStringList(), "void", "int");
};

CVtkSplineWidget::CVtkSplineWidget()
{
    m_vtkSplineWidget = 0;
    setVtkObject(vtkSplineWidget::New());
    m_vtkSplineWidget->SetEnabled(true);
    m_projectToPlane = true;
    m_vtkSplineWidget->ProjectToPlaneOn();
    m_closed = false;
    m_vtkSplineWidget->ClosedOff();
}

CVtkSplineWidget::~CVtkSplineWidget()
{

}

void CVtkSplineWidget::setProjectionNormalAxis(CVtkSplineWidget::ProjectionNormalAxis axis)
{
   if(axis == 0) // XAxis
        m_vtkSplineWidget->SetProjectionNormalToXAxes();
   if(axis == 1) // YAxis
        m_vtkSplineWidget->SetProjectionNormalToYAxes();
   if(axis == 2) // ZAxis
        m_vtkSplineWidget->SetProjectionNormalToZAxes();

}

CVtkSplineWidget::ProjectionNormalAxis CVtkSplineWidget::projectionNormalAxis() const
{
    return CVtkSplineWidget::ProjectionNormalAxis(m_vtkSplineWidget->GetProjectionNormal());
}

void CVtkSplineWidget::setProjectToPlane(bool val)
{
    m_projectToPlane = val;

    if(val)
        m_vtkSplineWidget->ProjectToPlaneOn();
    else
        m_vtkSplineWidget->ProjectToPlaneOff();
}

bool CVtkSplineWidget::isProjectToPlane() const
{
    return m_projectToPlane;
}

void CVtkSplineWidget::setProjectionPosition(double val)
{
    m_vtkSplineWidget->SetProjectionPosition(val);
}

double CVtkSplineWidget::projectionPosition() const
{
    return m_vtkSplineWidget->GetProjectionPosition();
}

void CVtkSplineWidget::setNumberOfHandles(int val)
{
    m_vtkSplineWidget->SetNumberOfHandles(val);
}

int CVtkSplineWidget::numberOfHandles() const
{
    return m_vtkSplineWidget->GetNumberOfHandles();
}

void CVtkSplineWidget::setResolution(int val)
{
    m_vtkSplineWidget->SetResolution(val);
}

int CVtkSplineWidget::resolution() const
{
    return m_vtkSplineWidget->GetResolution();
}

void CVtkSplineWidget::setHandlePosition(int val, Point3D point)
{
    m_vtkSplineWidget->SetHandlePosition(val, point.x, point.y, point.z);
}

Point3D CVtkSplineWidget::handlePosition(int index) const
{
    return Point3D( m_vtkSplineWidget->GetHandlePosition(index) );
}

void CVtkSplineWidget::setClosed(bool val)
{
    m_closed = val;
    if(val)
        m_vtkSplineWidget->ClosedOn();
    else
        m_vtkSplineWidget->ClosedOff();
}

bool CVtkSplineWidget::isClosed() const
{
    return m_closed;
}
