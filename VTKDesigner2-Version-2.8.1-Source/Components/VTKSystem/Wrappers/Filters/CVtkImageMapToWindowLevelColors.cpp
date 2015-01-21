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

#include "CVtkImageMapToWindowLevelColors.h"

DEFINE_VTK_OBJECT(CVtkImageMapToWindowLevelColors, CVtkImageMapToColors, vtkImageMapToWindowLevelColors)
{
    pDesc->setNodeClassName("vtkImageMapToWindowLevelColors");
    pDesc->setNodeClassCategory("Image Filters");
    pDesc->setNodeClassDescription("map the input image through a lookup table and window / level it");
    
    pDesc->setPropertyInfo("Window", "SetWindow", "GetWindow", QStringList()<<"double",QStringList(),"void","double");
    pDesc->setPropertyInfo("Level", "SetLevel", "GetLevel", QStringList()<<"double",QStringList(),"void","double");
}

CVtkImageMapToWindowLevelColors::CVtkImageMapToWindowLevelColors()
{
    CVtkImageMapToWindowLevelColors::InitializeObjectDesc();
}

CVtkImageMapToWindowLevelColors::~CVtkImageMapToWindowLevelColors()
{

}

void CVtkImageMapToWindowLevelColors::setWindow(double val)
{
    m_vtkImageMapToWindowLevelColors->SetWindow(val);
}

double CVtkImageMapToWindowLevelColors::window() const
{
    return m_vtkImageMapToWindowLevelColors->GetWindow();
}

void CVtkImageMapToWindowLevelColors::setLevel(double val)
{
    m_vtkImageMapToWindowLevelColors->SetLevel(val);
}

double CVtkImageMapToWindowLevelColors::level() const
{
    return m_vtkImageMapToWindowLevelColors->GetLevel();
}

void CVtkImageMapToWindowLevelColors::initializeNode()
{
    setVtkObject(vtkImageMapToWindowLevelColors::New());
}

