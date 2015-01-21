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
#include "CVtkHedgeHog.h"

DEFINE_VTK_OBJECT(CVtkHedgeHog, CVtkPolyDataFilter, vtkHedgeHog)
{
    pDesc->setNodeClassName("vtkHedgeHog");
    pDesc->setNodeClassDescription("Create oriented lines from vector data");

    pDesc->setPropertyInfo("ScaleFactor","SetScaleFactor","GetScaleFactor",QStringList()<<"double",QStringList(),"void","double");
    pDesc->setPropertyInfo("VectorMode","SetVectorMode","GetVectorMode",QStringList()<<"int",QStringList(),"void","int");
}


CVtkHedgeHog::CVtkHedgeHog() : m_vtkHedgeHog(0)
{
    CVtkHedgeHog::InitializeObjectDesc();
    setVtkObject(vtkHedgeHog::New());
}


CVtkHedgeHog::~CVtkHedgeHog()
{

}
    
void CVtkHedgeHog::setScaleFactor(double val)
{
    m_vtkHedgeHog->SetScaleFactor(val);
}

double CVtkHedgeHog::scaleFactor() const
{
    return m_vtkHedgeHog->GetScaleFactor();
}

void CVtkHedgeHog::setVectorMode(VectorMode val)
{
    switch(val)
    {
        case Vector:m_vtkHedgeHog->SetVectorMode(VTK_USE_VECTOR);
            break;
        case Normal:m_vtkHedgeHog->SetVectorMode(VTK_USE_NORMAL);
            break;
        default:m_vtkHedgeHog->SetVectorMode(VTK_USE_VECTOR);
            break;
    }
}

CVtkHedgeHog::VectorMode CVtkHedgeHog::vectorMode() const
{
    switch(m_vtkHedgeHog->GetVectorMode())
    {
        case VTK_USE_VECTOR: return Vector;
        case VTK_USE_NORMAL:return Normal;
        default: return Vector;            
    }
}

