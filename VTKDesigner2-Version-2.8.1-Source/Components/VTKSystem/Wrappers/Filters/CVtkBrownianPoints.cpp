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
#include "CVtkBrownianPoints.h"

DEFINE_VTK_OBJECT(CVtkBrownianPoints,CVtkDataSetFilter,vtkBrownianPoints)
{
    pDesc->setNodeClassName("vtkBrownianPoints");
    pDesc->setNodeClassDescription("Assign random vector to points");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("MinimumSpeed", "SetMinimumSpeed", "GetMinimumSpeed", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("MaximumSpeed", "SetMaximumSpeed", "GetMaximumSpeed", QStringList() << "double", QStringList(), "void", "double");
}

CVtkBrownianPoints::CVtkBrownianPoints() : m_vtkBrownianPoints(0)
{
    CVtkBrownianPoints::InitializeObjectDesc();
    setVtkObject(vtkBrownianPoints::New());
}

CVtkBrownianPoints::~CVtkBrownianPoints()
{

}

double CVtkBrownianPoints::minimumSpeed() const
{
    return m_vtkBrownianPoints->GetMinimumSpeed();
}


void CVtkBrownianPoints::setMinimumSpeed(double val)
{
    m_vtkBrownianPoints->SetMinimumSpeed(val);
}


double CVtkBrownianPoints::maximumSpeed() const
{
    return m_vtkBrownianPoints->GetMaximumSpeed();
}


void CVtkBrownianPoints::setMaximumSpeed(double val)
{
    m_vtkBrownianPoints->SetMaximumSpeed(val);
}

