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
#include "CVtkTextureMapToSphere.h"

DEFINE_VTK_OBJECT(CVtkTextureMapToSphere,CVtkDataSetFilter,vtkTextureMapToSphere)
{
    pDesc->setNodeClassName("vtkTextureMapToSphere");
    pDesc->setNodeClassDescription("Generate texture coordinates by mapping points to sphere");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("Center", "SetCenter", "GetCenter", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("PreventSeam", "SetPreventSeam", "GetPreventSeam", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("AutomaticSphereGeneration", "SetAutomaticSphereGeneration", "GetAutomaticSphereGeneration", QStringList() << "int" , QStringList(), "void", "int");
}

CVtkTextureMapToSphere::CVtkTextureMapToSphere() : m_vtkTextureMapToSphere(0)
{
    CVtkTextureMapToSphere::InitializeObjectDesc();
    setVtkObject(vtkTextureMapToSphere::New());
}

CVtkTextureMapToSphere::~CVtkTextureMapToSphere()
{

}

Point3D CVtkTextureMapToSphere::center() const
{
    return Point3D(m_vtkTextureMapToSphere->GetCenter());
}


void CVtkTextureMapToSphere::setCenter(Point3D val)
{
    m_vtkTextureMapToSphere->SetCenter(val.x, val.y, val.z);
}


bool CVtkTextureMapToSphere::isPreventSeam() const
{
    return m_vtkTextureMapToSphere->GetPreventSeam();
}


void CVtkTextureMapToSphere::setPreventSeam(bool val)
{
    m_vtkTextureMapToSphere->SetPreventSeam(val);
}


bool CVtkTextureMapToSphere::isAutomaticSphereGeneration() const
{
    return m_vtkTextureMapToSphere->GetAutomaticSphereGeneration();
}


void CVtkTextureMapToSphere::setAutomaticSphereGeneration(bool val)
{
    m_vtkTextureMapToSphere->SetAutomaticSphereGeneration(val);
}

