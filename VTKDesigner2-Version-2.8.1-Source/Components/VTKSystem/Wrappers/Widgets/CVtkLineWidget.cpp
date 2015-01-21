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

#include "CVtkLineWidget.h"

DEFINE_VTK_OBJECT(CVtkLineWidget, CVtk3DWidget, vtkLineWidget)
{
    pDesc->setNodeClassName("vtkLineWidget");
    pDesc->setNodeClassCategory("Interactor Observers");
    pDesc->setNodeClassDescription("Provides a Line widget");
    
    pDesc->setPropertyInfo("Resolution", "SetResolution", "GetResolution", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Point1", "SetPoint1", "GetPoint1", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Point2", "SetPoint2", "GetPoint2", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Alignment", "SetAlign", "GetAlign", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ClampToBounds", "SetClampToBounds", "GetClampToBounds", QStringList() << "bool", QStringList(), "void", "bool");
};

CVtkLineWidget::CVtkLineWidget()
{
    m_vtkLineWidget = 0;
    setVtkObject(vtkLineWidget::New());
}

CVtkLineWidget::~CVtkLineWidget()
{

}


void CVtkLineWidget::setResolution(int val)
{
    m_vtkLineWidget->SetResolution(val);
}

int CVtkLineWidget::resolution() const
{
    return m_vtkLineWidget->GetResolution();
}


void CVtkLineWidget::setPoint1(Point3D point)
{
    m_vtkLineWidget->SetPoint1(point.x, point.y, point.z);
}

Point3D CVtkLineWidget::point1() const
{
    double* pointVal = new double(3);
    pointVal = m_vtkLineWidget->GetPoint1();
    return Point3D(pointVal[0],pointVal[1],pointVal[2]);
}


void CVtkLineWidget::setPoint2(Point3D point)
{
    m_vtkLineWidget->SetPoint2(point.x, point.y, point.z);
}

Point3D CVtkLineWidget::point2() const
{
    double* pointVal = new double(3);
    pointVal = m_vtkLineWidget->GetPoint2();
    return Point3D(pointVal[0],pointVal[1],pointVal[2]);
}


void CVtkLineWidget::setAlignment(CVtkLineWidget::Alignment val)
{
    m_vtkLineWidget->SetAlign( int(val) );
}

CVtkLineWidget::Alignment CVtkLineWidget::alignment() const
{
    return CVtkLineWidget::Alignment( m_vtkLineWidget->GetAlign() );
}

void CVtkLineWidget::setClampToBounds(bool val)
{
    m_vtkLineWidget->SetClampToBounds(val);
}

bool CVtkLineWidget::isClampToBounds() const
{
    return m_vtkLineWidget->GetClampToBounds();
}


