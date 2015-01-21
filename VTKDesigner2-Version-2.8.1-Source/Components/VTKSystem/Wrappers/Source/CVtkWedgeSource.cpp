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

#include "CVtkWedgeSource.h"

DEFINE_VTK_OBJECT(CVtkWedgeSource, CVtkPolyDataAlgorithm, vtkWedgeSource)
{
    pDesc->setNodeClassName("vtkWedgeSource");
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassDescription("Generates a solid wedge model");
    pDesc->setPropertyInfo("Height", "SetHeight", "height", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("BaseAngle", "SetBaseAngle", "baseAngle", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Width", "SetWidth", "GetWidth", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Depth", "SetDepth", "GetDepth", QStringList() << "double", QStringList(), "void", "double");
    
    pDesc->setNodeIcon(qApp->windowIcon()); // vtkCustomFunction is not a standard VTK class
}

CVtkWedgeSource::CVtkWedgeSource()
{
    CVtkWedgeSource::InitializeObjectDesc();
    setVtkObject(vtkWedgeSource::New());
}

CVtkWedgeSource::~CVtkWedgeSource()
{

}

void CVtkWedgeSource::setWidth(double w)
{
    m_vtkWedgeSource->SetWidth(w);
    emit propertyChanged("BaseAngle");
}

double CVtkWedgeSource::width() const
{
    return m_vtkWedgeSource->GetWidth();
}

void CVtkWedgeSource::setHeight(double h)
{
    m_vtkWedgeSource->SetHeight(h);
    emit propertyChanged("BaseAngle");
}

double CVtkWedgeSource::height() const
{
    return m_vtkWedgeSource->GetHeight();
}

void CVtkWedgeSource::setDepth(double d)
{
    m_vtkWedgeSource->SetDepth(d);
}
    
double CVtkWedgeSource::depth() const
{
    return m_vtkWedgeSource->GetDepth();
}

void CVtkWedgeSource::setBaseAngle(double a)
{
    m_vtkWedgeSource->SetHeightFromBaseAngle(a);
    emit propertyChanged("Height");
}

double CVtkWedgeSource::baseAngle() const
{
    return m_vtkWedgeSource->GetBaseAngle();
}

