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
#include "CVtkTextureMapToCylinder.h"

DEFINE_VTK_OBJECT(CVtkTextureMapToCylinder,CVtkDataSetFilter,vtkTextureMapToCylinder)
{
    pDesc->setNodeClassName("vtkTextureMapToCylinder");
    pDesc->setNodeClassDescription("Generate texture coordinates by mapping points to cylinder");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("Point1", "SetPoint1", "GetPoint1", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Point2", "SetPoint2", "GetPoint2", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("PreventSeam", "SetPreventSeam", "GetPreventSeam", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("AutomaticCylinderGeneration", "SetAutomaticCylinderGeneration", "GetAutomaticCylinderGeneration", QStringList() << "int" , QStringList(), "void", "int");
}

CVtkTextureMapToCylinder::CVtkTextureMapToCylinder() : m_vtkTextureMapToCylinder(0)
{
    CVtkTextureMapToCylinder::InitializeObjectDesc();
    setVtkObject(vtkTextureMapToCylinder::New());
}

CVtkTextureMapToCylinder::~CVtkTextureMapToCylinder()
{

}

Point3D CVtkTextureMapToCylinder::point1() const
{
    return Point3D(m_vtkTextureMapToCylinder->GetPoint1());
}


void CVtkTextureMapToCylinder::setPoint1(Point3D val)
{
    m_vtkTextureMapToCylinder->SetPoint1(val.x, val.y, val.z);
}


Point3D CVtkTextureMapToCylinder::point2() const
{
    return Point3D(m_vtkTextureMapToCylinder->GetPoint2());
}


void CVtkTextureMapToCylinder::setPoint2(Point3D val)
{
    m_vtkTextureMapToCylinder->SetPoint2(val.x, val.y, val.z);
}


bool CVtkTextureMapToCylinder::isPreventSeam() const
{
    return m_vtkTextureMapToCylinder->GetPreventSeam();
}


void CVtkTextureMapToCylinder::setPreventSeam(bool val)
{
    m_vtkTextureMapToCylinder->SetPreventSeam(val);
}


bool CVtkTextureMapToCylinder::isAutomaticCylinderGeneration() const
{
    return m_vtkTextureMapToCylinder->GetAutomaticCylinderGeneration();
}


void CVtkTextureMapToCylinder::setAutomaticCylinderGeneration(bool val)
{
    m_vtkTextureMapToCylinder->SetAutomaticCylinderGeneration(val);
}

