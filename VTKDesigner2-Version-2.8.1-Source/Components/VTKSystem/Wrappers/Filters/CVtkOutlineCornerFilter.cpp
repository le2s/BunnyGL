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
#include "CVtkOutlineCornerFilter.h"

DEFINE_VTK_OBJECT(CVtkOutlineCornerFilter, CVtkPolyDataFilter, vtkOutlineCornerFilter)
{
    pDesc->setNodeClassName("vtkOutlineCornerFilter");
    pDesc->setNodeClassDescription("Outline Corner Filter");

    pDesc->setPropertyInfo("CornerFactor","SetCornerFactor","GetCornerFactor",QStringList() << "double", QStringList(), "void","double");
}



CVtkOutlineCornerFilter::CVtkOutlineCornerFilter() : m_vtkOutlineCornerFilter(0)
{
    CVtkOutlineCornerFilter::InitializeObjectDesc();
    setVtkObject(vtkOutlineCornerFilter::New());
}

CVtkOutlineCornerFilter::~CVtkOutlineCornerFilter()
{

}


void CVtkOutlineCornerFilter::setCornerFactor (double val)
{
    m_vtkOutlineCornerFilter->SetCornerFactor(val);
}

double     CVtkOutlineCornerFilter::cornerFactor() const
{
    return m_vtkOutlineCornerFilter->GetCornerFactor();
}