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

#include "CVtkTransform.h"
#include "IVtkAbstractTransformIOData.h"
#include <QVector>

DEFINE_VTK_OBJECT(CVtkTransform, CVtkAbstractTransform, vtkTransform)
{
    pDesc->setNodeClassCategory("Transform");
    pDesc->setNodeClassName("vtkTransform");

    // Transform Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("TransformInput", IVisSystemNodeConnectionPath::InputPath, "vtkLinearTransform", 0)
        );
}

CVtkTransform::CVtkTransform() : m_vtkTransform(0)
{
    CVtkTransform::InitializeObjectDesc();
    setVtkObject(vtkTransform::New());
}

CVtkTransform::~CVtkTransform()
{

}

void CVtkTransform::translate(Point3D p)
{
    m_vtkTransform->Translate(p.x, p.y, p.z);
}

void CVtkTransform::translate(double x, double y, double z)
{
    m_vtkTransform->Translate(x, y, z);
}

void CVtkTransform::rotateX(double angle)
{
    m_vtkTransform->RotateX(angle);
}

void CVtkTransform::rotateY(double angle)
{
    m_vtkTransform->RotateY(angle);
}

void CVtkTransform::rotateZ(double angle)
{
    m_vtkTransform->RotateZ(angle);
}

void CVtkTransform::scale(Point3D p)
{
    m_vtkTransform->Scale(p.x, p.y, p.z);
}

void CVtkTransform::scale(double x, double y, double z)
{
    m_vtkTransform->Scale(x, y, z);
}

void CVtkTransform::preMultiply()
{
    m_vtkTransform->PreMultiply();
}

void CVtkTransform::postMultiply()
{
    m_vtkTransform->PostMultiply();
}

void CVtkTransform::makeIdentity()
{
    m_vtkTransform->Identity();
}

void CVtkTransform::concatenate(QList<double> elements)
{
    QVector<double> eVec = elements.toVector();
    m_vtkTransform->Concatenate(eVec.data());
}

void CVtkTransform::setMatrix(QList<double> matrix)
{
    QVector<double> eVec = matrix.toVector();
    m_vtkTransform->SetMatrix( eVec.data() );
}

QList<double> CVtkTransform::getMatrix()
{
    QList<double> ret;
    vtkMatrix4x4* matrix = m_vtkTransform->GetMatrix();
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            ret << matrix->Element[i][j];
    return ret;
}

bool CVtkTransform::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "TransformInput")
    {
        return m_vtkTransform->GetInput();
    }

    return CVtkAbstractTransform::hasInput(path);
}

bool CVtkTransform::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path)
        return false;

    if(path->pathName() == "TransformInput")
    {
        IVtkAbstractTransformIOData* data = 0;
        bool success = inputData->queryInterface("IVtkAbstractTransformIOData", (void**)&data);
        if(success && data)
        {
            vtkLinearTransform* tx = vtkLinearTransform::SafeDownCast( data->getVtkAbstractTransform() );
            if(tx)
            {
                m_vtkTransform->SetInput(tx);
                return true;
            }
        }

        return false;
    }

    return CVtkAbstractTransform::hasInput(path);
}

bool CVtkTransform::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path)
        return false;

    if(path->pathName() == "TransformInput")
    {
        IVtkAbstractTransformIOData* data = 0;
        bool success = inputData->queryInterface("IVtkAbstractTransformIOData", (void**)&data);
        if(success && data)
        {
            vtkLinearTransform* tx = vtkLinearTransform::SafeDownCast( data->getVtkAbstractTransform() );
            if(tx && m_vtkTransform->GetInput() == tx)
            {
                m_vtkTransform->SetInput(0);
                return true;
            }
        }

        return false;
    }

    return CVtkAbstractTransform::hasInput(path);
}


