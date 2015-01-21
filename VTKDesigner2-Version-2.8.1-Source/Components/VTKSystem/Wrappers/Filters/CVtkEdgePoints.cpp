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
#include "CVtkEdgePoints.h"


DEFINE_VTK_OBJECT(CVtkEdgePoints, CVtkPolyDataFilter, vtkEdgePoints)
{
    pDesc->setNodeClassName("vtkEdgePoints");
    pDesc->setNodeClassDescription("Generate points on isosurface");

    pDesc->setPropertyInfo("Value","SetValue","GetValue",QStringList() << "double", QStringList(),"void","double");    
}


CVtkEdgePoints::CVtkEdgePoints() : m_vtkEdgePoints(0)    
{
    CVtkEdgePoints::InitializeObjectDesc();
    setVtkObject(vtkEdgePoints::New());
}

CVtkEdgePoints::~CVtkEdgePoints()
{

}

void CVtkEdgePoints::setValue(double val)
{
    m_vtkEdgePoints->SetValue(val);    
}

double CVtkEdgePoints::value() const
{
    return m_vtkEdgePoints->GetValue();    
}
