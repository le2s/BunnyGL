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

#include "CVtkTransformPolyDataFilter.h"
#include "vtkTransform.h"
#include "IVtkAbstractTransformIOData.h"

DEFINE_VTK_OBJECT(CVtkTransformPolyDataFilter, CVtkPolyDataFilter, vtkTransformPolyDataFilter)
{
    pDesc->setNodeClassName("vtkTransformPolyDataFilter");
    pDesc->setNodeClassDescription("Transform PolyData filter");

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath(
                "Transform",                                // Name of the path
                IVisSystemNodeConnectionPath::InputPath,    // Path type can be OutputPath or InputPath
                "vtkTransform",                             // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

CVtkTransformPolyDataFilter::CVtkTransformPolyDataFilter() : m_vtkTransformPolyDataFilter(0)
{
    CVtkTransformPolyDataFilter::InitializeObjectDesc();
    setVtkObject(vtkTransformPolyDataFilter::New());

    m_vtkTransform = vtkTransform::New();
    m_vtkTransformPolyDataFilter->SetTransform(m_vtkTransform);
    m_vtkTransform->Delete();
    m_transformIsSet = false;
}

CVtkTransformPolyDataFilter::~CVtkTransformPolyDataFilter()
{

}

void CVtkTransformPolyDataFilter::translate(Point3D p)
{
    vtkTransform* tx = vtkTransform::SafeDownCast(m_vtkTransform);
    if(tx)
        tx->Translate(p.x, p.y, p.z);
}

void CVtkTransformPolyDataFilter::translate(double x, double y, double z)
{
    vtkTransform* tx = vtkTransform::SafeDownCast(m_vtkTransform);
    if(tx)
        tx->Translate(x, y, z);
}

void CVtkTransformPolyDataFilter::rotateX(double angle)
{
    vtkTransform* tx = vtkTransform::SafeDownCast(m_vtkTransform);
    if(tx)
        tx->RotateX(angle);
}

void CVtkTransformPolyDataFilter::rotateY(double angle)
{
    vtkTransform* tx = vtkTransform::SafeDownCast(m_vtkTransform);
    if(tx)
        tx->RotateY(angle);
}

void CVtkTransformPolyDataFilter::rotateZ(double angle)
{
    vtkTransform* tx = vtkTransform::SafeDownCast(m_vtkTransform);
    if(tx)
        tx->RotateZ(angle);
}

void CVtkTransformPolyDataFilter::scale(Point3D p)
{
    vtkTransform* tx = vtkTransform::SafeDownCast(m_vtkTransform);
    if(tx)
        tx->Scale(p.x, p.y, p.z);
}

void CVtkTransformPolyDataFilter::scale(double x, double y, double z)
{
    vtkTransform* tx = vtkTransform::SafeDownCast(m_vtkTransform);
    if(tx)
        tx->Scale(x, y, z);
}

void CVtkTransformPolyDataFilter::preMultiply()
{
    vtkTransform* tx = vtkTransform::SafeDownCast(m_vtkTransform);
    if(tx)
        tx->PreMultiply();
}

void CVtkTransformPolyDataFilter::postMultiply()
{
    vtkTransform* tx = vtkTransform::SafeDownCast(m_vtkTransform);
    if(tx)
        tx->PostMultiply();
}

void CVtkTransformPolyDataFilter::makeIdentity()
{
    vtkTransform* tx = vtkTransform::SafeDownCast(m_vtkTransform);
    if(tx)
        tx->Identity();
}

void CVtkTransformPolyDataFilter::concatenate(QList<double> elements)
{
    QVector<double> eVec = elements.toVector();
    vtkTransform* tx = vtkTransform::SafeDownCast(m_vtkTransform);
    if(tx)
        tx->Concatenate(eVec.data());
}

void CVtkTransformPolyDataFilter::setMatrix(QList<double> matrix)
{
    QVector<double> eVec = matrix.toVector();
    vtkTransform* tx = vtkTransform::SafeDownCast(m_vtkTransform);
    if(tx)
        tx->SetMatrix( eVec.data() );
}

QList<double> CVtkTransformPolyDataFilter::getMatrix()
{
    QList<double> ret;
    vtkTransform* tx = vtkTransform::SafeDownCast(m_vtkTransform);
    if(!tx)
        return ret;

    vtkMatrix4x4* matrix = tx->GetMatrix();
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            ret << matrix->Element[i][j];
    return ret;
}

bool CVtkTransformPolyDataFilter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Transform")
    {
        if(m_transformIsSet)
            return true;

        return false;
    }

    return CVtkPolyDataFilter::hasInput(path);
}

bool CVtkTransformPolyDataFilter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Transform")
    {
        IVtkAbstractTransformIOData* data = 0;
        bool success = inputData->queryInterface("IVtkAbstractTransformIOData", (void**)&data);
        if(success && data)
        {
            m_vtkTransform = data->getVtkAbstractTransform();
            m_vtkTransformPolyDataFilter->SetTransform(m_vtkTransform);
            m_transformIsSet = true;

            return true;
        }

        return false;
    }

    return CVtkPolyDataFilter::setInput(path, inputData);
}

bool CVtkTransformPolyDataFilter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Transform")
    {
        if(!m_transformIsSet)
            return false;

        m_vtkTransform = vtkTransform::New();
        m_vtkTransformPolyDataFilter->SetTransform(m_vtkTransform);
        m_vtkTransform->Delete();
        m_transformIsSet = false;

        return true;
    }

    return CVtkPolyDataFilter::removeInput(path, inputData);
}

bool CVtkTransformPolyDataFilter::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkPolyDataFilter::fetchOutput(path, outputData);
}

bool CVtkTransformPolyDataFilter::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkPolyDataFilter::outputDerefed(path, outputData);
}


