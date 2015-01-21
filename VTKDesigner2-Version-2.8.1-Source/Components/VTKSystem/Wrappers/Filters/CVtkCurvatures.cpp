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
#include "CVtkCurvatures.h"

DEFINE_VTK_OBJECT(CVtkCurvatures,CVtkPolyDataFilter,vtkCurvatures)
{
    pDesc->setNodeClassName("vtkCurvatures");
    pDesc->setNodeClassDescription("Compute curvatures (Gauss and mean) of a Polydata object");

    pDesc->setPropertyInfo("CurvatureType", "SetCurvatureType", "GetCurvatureType", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("InvertMeanCurvature", "SetInvertMeanCurvature", "GetInvertMeanCurvature", QStringList() << "int", QStringList(), "void", "int");
}

CVtkCurvatures::CVtkCurvatures() : m_vtkCurvatures(0)
{
    CVtkCurvatures::InitializeObjectDesc();
    setVtkObject(vtkCurvatures::New());
}

CVtkCurvatures::~CVtkCurvatures()
{

}

CVtkCurvatures::CurvatureType CVtkCurvatures::curvatureType() const
{
    switch(m_vtkCurvatures->GetCurvatureType())
    {
        case VTK_CURVATURE_GAUSS:return (Gauss);
        case VTK_CURVATURE_MEAN:return (Mean);
        case VTK_CURVATURE_MAXIMUM:return (Maximum);
        case VTK_CURVATURE_MINIMUM:return (Minimum);
    }
    return CVtkCurvatures::Gauss;
}


void CVtkCurvatures::setCurvatureType(CurvatureType val)
{
    switch(val)
    {
        case Gauss:
            m_vtkCurvatures->SetCurvatureType(VTK_CURVATURE_GAUSS);
            break;
        case Mean:
            m_vtkCurvatures->SetCurvatureType(VTK_CURVATURE_MEAN);
            break;
        case Maximum:
            m_vtkCurvatures->SetCurvatureType(VTK_CURVATURE_MAXIMUM);
            break;
        case Minimum:
            m_vtkCurvatures->SetCurvatureType(VTK_CURVATURE_MINIMUM);
            break;
     }
}

bool CVtkCurvatures::isInvertMeanCurvature() const
{
    return m_vtkCurvatures->GetInvertMeanCurvature();
}


void CVtkCurvatures::setInvertMeanCurvature(bool val)
{
    m_vtkCurvatures->SetInvertMeanCurvature(val);
}

