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

#include "CVtkTransformFilter.h"
#include "vtkTransform.h"

DEFINE_VTK_OBJECT(CVtkTransformFilter,CVtkPointSetFilter,vtkTransformFilter)
{
    pDesc->setNodeClassName("vtkTransformFilter");
    pDesc->setNodeClassDescription("Transform points and associated normals and vectors");
    pDesc->setNodeClassCategory("PolyData Filters");
}

CVtkTransformFilter::CVtkTransformFilter() : m_vtkTransformFilter(0)
{
    CVtkTransformFilter::InitializeObjectDesc();
    setVtkObject(vtkTransformFilter::New());

    m_vtkTransform = vtkTransform::New();
    m_vtkTransformFilter->SetTransform(m_vtkTransform);
    m_vtkTransform->Delete();
}

CVtkTransformFilter::~CVtkTransformFilter()
{

}

void CVtkTransformFilter::translate(Point3D p)
{
    m_vtkTransform->Translate(p.x, p.y, p.z);
}

void CVtkTransformFilter::translate(double x, double y, double z)
{
    m_vtkTransform->Translate(x, y, z);
}

void CVtkTransformFilter::rotateX(double angle)
{
    m_vtkTransform->RotateX(angle);
}

void CVtkTransformFilter::rotateY(double angle)
{
    m_vtkTransform->RotateY(angle);
}

void CVtkTransformFilter::rotateZ(double angle)
{
    m_vtkTransform->RotateZ(angle);
}

void CVtkTransformFilter::scale(Point3D p)
{
    m_vtkTransform->Scale(p.x, p.y, p.z);
}

void CVtkTransformFilter::scale(double x, double y, double z)
{
    m_vtkTransform->Scale(x, y, z);
}

void CVtkTransformFilter::preMultiply()
{
    m_vtkTransform->PreMultiply();
}

void CVtkTransformFilter::postMultiply()
{
    m_vtkTransform->PostMultiply();
}

void CVtkTransformFilter::makeIdentity()
{
    m_vtkTransform->Identity();
}


void CVtkTransformFilter::concatenate(QList<double> elements)
{
    QVector<double> eVec = elements.toVector();
    m_vtkTransform->Concatenate(eVec.data());
}

void CVtkTransformFilter::setMatrix(QList<double> matrix)
{
    QVector<double> eVec = matrix.toVector();
    m_vtkTransform->SetMatrix( eVec.data() );
}

QList<double> CVtkTransformFilter::getMatrix()
{
    QList<double> ret;
    vtkMatrix4x4* matrix = m_vtkTransform->GetMatrix();
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            ret << matrix->Element[i][j];
    return ret;
}

