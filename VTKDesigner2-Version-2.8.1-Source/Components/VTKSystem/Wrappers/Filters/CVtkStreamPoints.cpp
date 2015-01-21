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

#include "CVtkStreamPoints.h"

DEFINE_VTK_OBJECT(CVtkStreamPoints,CVtkStreamer,vtkStreamPoints)
{
    pDesc->setNodeClassName("vtkStreamPoints");
    pDesc->setNodeClassDescription("Generate points along streamer separated by constant time increment");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("TimeIncrement", "SetTimeIncrement", "GetTimeIncrement", QStringList() << "double", QStringList(), "void", "double");
}

CVtkStreamPoints::CVtkStreamPoints() : m_vtkStreamPoints(0)
{
    CVtkStreamPoints::InitializeObjectDesc();
    setVtkObject(vtkStreamPoints::New());
}

CVtkStreamPoints::~CVtkStreamPoints()
{

}

double CVtkStreamPoints::timeIncrement() const
{
    return m_vtkStreamPoints->GetTimeIncrement();
}


void CVtkStreamPoints::setTimeIncrement(double val)
{
    m_vtkStreamPoints->SetTimeIncrement(val);
}

