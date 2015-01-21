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
#include "CVtkWarpVector.h"

DEFINE_VTK_OBJECT(CVtkWarpVector,CVtkPointSetFilter,vtkWarpVector)
{
    pDesc->setNodeClassName("vtkWarpVector");
    pDesc->setNodeClassDescription("Deform geometry with vector data");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("ScaleFactor", "SetScaleFactor", "GetScaleFactor", QStringList() << "double", QStringList(), "void", "double");
}

CVtkWarpVector::CVtkWarpVector() : m_vtkWarpVector(0)
{
    CVtkWarpVector::InitializeObjectDesc();
    setVtkObject(vtkWarpVector::New());
}

CVtkWarpVector::~CVtkWarpVector()
{

}

double CVtkWarpVector::scaleFactor() const
{
    return m_vtkWarpVector->GetScaleFactor();
}


void CVtkWarpVector::setScaleFactor(double val)
{
    m_vtkWarpVector->SetScaleFactor(val);
}

