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

#include "CVtkSphereWidget.h"

DEFINE_VTK_OBJECT(CVtkSphereWidget, CVtk3DWidget, vtkSphereWidget)
{
    pDesc->setNodeClassName("vtkSphereWidget");
    pDesc->setNodeClassCategory("Interactor Observers");
    pDesc->setNodeClassDescription("Provides a Sphere widget");
    
    pDesc->setPropertyInfo("Representation", "SetRepresentation", "GetRepresentation", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ThetaResolution", "SetThetaResolution", "GetThetaResolution", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("PhiResolution", "SetPhiResolution", "GetPhiResolution", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Radius", "SetRadius", "GetRadius", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Center", "SetCenter", "GetCenter", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("HandleDirection", "SetHandleDirection", "GetHandleDirection", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
};

CVtkSphereWidget::CVtkSphereWidget()
{
    m_vtkSphereWidget = 0;
    setVtkObject(vtkSphereWidget::New());

    m_handlesVisible = true;
    m_vtkSphereWidget->HandleVisibilityOn();

    m_translation = true;
    m_vtkSphereWidget->TranslationOn();

    m_scale = true;
    m_vtkSphereWidget->ScaleOn();
}

CVtkSphereWidget::~CVtkSphereWidget()
{

}


void CVtkSphereWidget::setRepresentation(CVtkSphereWidget::Representation val)
{
    m_vtkSphereWidget->SetRepresentation( int(val) );
}

CVtkSphereWidget::Representation CVtkSphereWidget::representation() const
{
    return CVtkSphereWidget::Representation( m_vtkSphereWidget->GetRepresentation() );
}

void CVtkSphereWidget::setThetaResolution(int val)
{
    m_vtkSphereWidget->SetThetaResolution(val);
}

int CVtkSphereWidget::thetaResolution() const
{
    return m_vtkSphereWidget->GetThetaResolution();
}


void CVtkSphereWidget::setPhiResolution(int val)
{
    m_vtkSphereWidget->SetPhiResolution(val);
}

int  CVtkSphereWidget::phiResolution() const
{
    return m_vtkSphereWidget->GetPhiResolution();
}


void CVtkSphereWidget::setRadius(double val)
{
    m_vtkSphereWidget->SetRadius(val);
}


double CVtkSphereWidget::radius() const
{
    return m_vtkSphereWidget->GetRadius();
}


void CVtkSphereWidget::setCenter (Point3D point)
{
    m_vtkSphereWidget->SetCenter(point.x,point.y,point.z);
}

Point3D CVtkSphereWidget::center() const
{
    double* val = new double(3);
    val = m_vtkSphereWidget->GetCenter();
    return Point3D(val[0],val[1],val[2]);
}


void CVtkSphereWidget::setTranslation(bool val)
{
    m_translation = val;

    if(val)
        m_vtkSphereWidget->TranslationOn();
    else
        m_vtkSphereWidget->TranslationOff();

}

bool CVtkSphereWidget::isTranslation() const
{
    return m_translation;
}


void CVtkSphereWidget::setScale(bool val)
{
    m_scale = val;

    if(val)
        m_vtkSphereWidget->ScaleOn();
    else
        m_vtkSphereWidget->ScaleOff();
}

bool CVtkSphereWidget::isScale() const
{
    return m_scale;
}


void CVtkSphereWidget::setHandlesVisible(bool val)
{
    m_handlesVisible = val;

    if(val)
        m_vtkSphereWidget->HandleVisibilityOn();
    else
        m_vtkSphereWidget->HandleVisibilityOff();

}

bool CVtkSphereWidget::isHandlesVisible() const
{
    return m_handlesVisible;
}


void CVtkSphereWidget::setHandleDirection(Point3D point)
{
    m_vtkSphereWidget->SetHandleDirection(point.x, point.y, point.z);
}

Point3D CVtkSphereWidget::handleDirection() const
{
    double* val = new double(3);
    val = m_vtkSphereWidget->GetHandleDirection();
    return Point3D(val[0], val[1], val[2]);
}


Point3D CVtkSphereWidget::handlePosition() const
{
    double* val = new double(3);
    val = m_vtkSphereWidget->GetHandlePosition();
    return Point3D(val[0], val[1], val[2]);
}


