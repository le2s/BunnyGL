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

#include "CVtkDashedStreamLine.h"

DEFINE_VTK_OBJECT(CVtkDashedStreamLine,CVtkStreamLine,vtkDashedStreamLine)
{
    pDesc->setNodeClassName("vtkDashedStreamLine");
    pDesc->setNodeClassDescription("Generate constant-time dashed streamline in arbitrary dataset");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("DashFactor", "SetDashFactor", "GetDashFactor", QStringList() << "double", QStringList(), "void", "double");
}

CVtkDashedStreamLine::CVtkDashedStreamLine() : m_vtkDashedStreamLine(0)
{
    CVtkDashedStreamLine::InitializeObjectDesc();
    setVtkObject(vtkDashedStreamLine::New());
}

CVtkDashedStreamLine::~CVtkDashedStreamLine()
{

}

double CVtkDashedStreamLine::dashFactor() const
{
    return m_vtkDashedStreamLine->GetDashFactor();
}


void CVtkDashedStreamLine::setDashFactor(double val)
{
    m_vtkDashedStreamLine->SetDashFactor(val);
}

