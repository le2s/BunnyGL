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

#include "CVtkAxes.h"
#include "CVtkPolyData.h"

DEFINE_VTK_OBJECT(CVtkAxes, CVtkPolyDataAlgorithm, vtkAxes)
{
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassName("vtkAxes");
    pDesc->setNodeClassDescription("Create an x-y-z axes");
    pDesc->setPropertyInfo("Origin", "SetOrigin", "GetOrigin", QStringList() << "double" << "double" << "double",
                           QStringList(), "void", "double*");
    pDesc->setPropertyInfo("ScaleFactor", "SetScaleFactor", "GetScaleFactor", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Symmetric", "SetSymmetric", "GetSymmetric", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ComputeNormals", "SetComputeNormals", "GetComputeNormals", QStringList() << "int", QStringList(), "void", "int");
}

CVtkAxes::CVtkAxes() : m_vtkAxes(0)
{
    CVtkAxes::InitializeObjectDesc();
    setVtkObject(vtkAxes::New());
}

CVtkAxes::~CVtkAxes()
{

}

Point3D CVtkAxes::origin() const
{
    return Point3D(m_vtkAxes->GetOrigin());
}

void CVtkAxes::setOrigin(Point3D origin)
{
    m_vtkAxes->SetOrigin(origin.x, origin.y, origin.z);
}

double CVtkAxes::scaleFactor() const
{
    return m_vtkAxes->GetScaleFactor();
}

void CVtkAxes::setScaleFactor(double scale)
{
    m_vtkAxes->SetScaleFactor(scale);
}
bool CVtkAxes::isSymmetric() const
{
    return m_vtkAxes->GetSymmetric();
}

void CVtkAxes::setSymmetric(bool s)
{
    m_vtkAxes->SetSymmetric(s);
}
bool CVtkAxes::isComputeNormals() const
{
    return m_vtkAxes->GetComputeNormals();
}

void CVtkAxes::setComputeNormals(bool c)
{
    m_vtkAxes->SetComputeNormals(c);
}


