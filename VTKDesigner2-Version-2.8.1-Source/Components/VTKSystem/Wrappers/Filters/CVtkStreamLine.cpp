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

#include "CVtkStreamLine.h"

DEFINE_VTK_OBJECT(CVtkStreamLine,CVtkStreamer,vtkStreamLine)
{
    pDesc->setNodeClassName("vtkStreamLine");
    pDesc->setNodeClassDescription("Generate streamline in arbitrary dataset");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("StepLength", "SetStepLength", "GetStepLength", QStringList() << "double", QStringList(), "void", "double");
}

CVtkStreamLine::CVtkStreamLine() : m_vtkStreamLine(0)
{
    CVtkStreamLine::InitializeObjectDesc();
    setVtkObject(vtkStreamLine::New());
}

CVtkStreamLine::~CVtkStreamLine()
{

}

double CVtkStreamLine::stepLength() const
{
    return m_vtkStreamLine->GetStepLength();
}


void CVtkStreamLine::setStepLength(double val)
{
    m_vtkStreamLine->SetStepLength(val);
}

