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

#include "CVtkProp3D.h"

#include "IVtkAbstractTransformIOData.h"
#include "vtkTransform.h"
#include "vtkMatrix4x4.h"
#include "vtkMath.h"

DEFINE_VTK_OBJECT(CVtkProp3D, CVtkObject, vtkProp3D)
{
    pDesc->setNodeClassCategory("Rendering");
    pDesc->setNodeClassDescription("Generic Actor");
    pDesc->setNodeClassName("vtkProp3D");

    // Transform Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("LinearTransform", IVisSystemNodeConnectionPath::InputPath, "vtkLinearTransform", 0)
        );

    // Actor Output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Prop3D", IVisSystemNodeConnectionPath::OutputPath, "vtkProp3D", 0)
        );

    // Property information
    pDesc->setPropertyInfo("Position", "SetPosition", "GetPosition", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Scale", "SetScale", "GetScale", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Orientation", "SetOrientation", "GetOrientation", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");

    pDesc->setCreateFunction(0);
}

CVtkProp3D::CVtkProp3D() : m_vtkProp3D(0)
{
    CVtkProp3D::InitializeObjectDesc();
}

CVtkProp3D::~CVtkProp3D()
{

}

Point3D CVtkProp3D::position() const
{
    return Point3D(m_vtkProp3D->GetPosition());
}

void CVtkProp3D::setPosition(Point3D pos)
{
    m_vtkProp3D->SetPosition(pos.x, pos.y, pos.z);
}

Point3D CVtkProp3D::scale() const
{
    return Point3D(m_vtkProp3D->GetScale());
}

void CVtkProp3D::setScale(Point3D s)
{
    m_vtkProp3D->SetScale(s.x, s.y, s.z);
}

Point3D CVtkProp3D::orientation() const
{
    return Point3D(m_vtkProp3D->GetOrientation());
}

void CVtkProp3D::setVisible(bool val)
{
    m_vtkProp3D->SetVisibility(val);
}

bool CVtkProp3D::isVisible() const
{
    return m_vtkProp3D->GetVisibility();
}

void CVtkProp3D::setPickable(bool val)
{
    m_vtkProp3D->SetPickable(val);
}

bool CVtkProp3D::isPickable() const
{
    return m_vtkProp3D->GetPickable();
}

void CVtkProp3D::setDragable(bool val)
{
    m_vtkProp3D->SetDragable(val);
}

bool CVtkProp3D::isDragable() const
{
    return m_vtkProp3D->GetDragable();
}

void CVtkProp3D::setOrientation(Point3D o)
{
    m_vtkProp3D->SetOrientation(o.x, o.y, o.z);
}

void CVtkProp3D::makeIdentity()
{
    vtkTransform* t = userTransform();
    t->Identity();
}

void CVtkProp3D::translate(const Point3D& dp)
{
    vtkTransform* t = userTransform();
    t->Translate(dp.x, dp.y, dp.z);
}

void CVtkProp3D::scale(double x, double y, double z)
{
    vtkTransform* t = userTransform();
    t->Scale(x, y, z);
}

void CVtkProp3D::rotateX(double x)
{
    vtkTransform* t = userTransform();
    t->RotateX(x);
}

void CVtkProp3D::rotateY(double y)
{
    vtkTransform* t = userTransform();
    t->RotateY(y);
}

void CVtkProp3D::rotateZ(double z)
{
    vtkTransform* t = userTransform();
    t->RotateZ(z);
}

void CVtkProp3D::rotate(QList<double> quat)
{
    if(quat.count() != 4)
        return;

    vtkTransform* t = userTransform();
    t->RotateWXYZ(quat[0], quat[1], quat[2], quat[3]);
}

void CVtkProp3D::rotate(double w, double x, double y, double z)
{
    vtkTransform* t = userTransform();
    t->RotateWXYZ(w, x, y, z);
}

QList<double> CVtkProp3D::rotation()
{
    vtkTransform* t = userTransform();
    double* orient = t->GetOrientationWXYZ();
    QList<double> ret;
    for(int i=0; i<4; i++)
        ret << orient[i];
    return ret;
}

Point3D CVtkProp3D::getCenter()
{
    return Point3D( m_vtkProp3D->GetCenter() );
}

QList<double> CVtkProp3D::getXRange()
{
    QList<double> ret;
    double* range = m_vtkProp3D->GetXRange();
    ret << range[0] << range[1];
    return ret;
}

QList<double> CVtkProp3D::getYRange()
{
    QList<double> ret;
    double* range = m_vtkProp3D->GetYRange();
    ret << range[0] << range[1];
    return ret;
}

QList<double> CVtkProp3D::getZRange()
{
    QList<double> ret;
    double* range = m_vtkProp3D->GetZRange();
    ret << range[0] << range[1];
    return ret;
}

double CVtkProp3D::getXLength()
{
    double* range = m_vtkProp3D->GetXRange();
    return qAbs(range[1]-range[0]);
}

double CVtkProp3D::getYLength()
{
    double* range = m_vtkProp3D->GetYRange();
    return qAbs(range[1]-range[0]);
}

double CVtkProp3D::getZLength()
{
    double* range = m_vtkProp3D->GetZRange();
    return qAbs(range[1]-range[0]);
}

double CVtkProp3D::getDiagonalLength()
{
    return m_vtkProp3D->GetLength();
}

QList<double> CVtkProp3D::getBounds() const
{
    QList<double> ret;
    double bounds[6];
    m_vtkProp3D->GetBounds(bounds);
    for(int i=0; i<6; i++)
        ret << bounds[i];
    return ret;
}

bool CVtkProp3D::collidesWith(QObject* anotherProp3D) const
{
    // Catch hold of the other prop3d
    IVtkObjectWrapper* wrapper = qobject_cast<IVtkObjectWrapper*>(anotherProp3D);
    if(!wrapper)
        return false;

    vtkProp3D* thisProp3D = m_vtkProp3D;
    vtkProp3D* thatProp3D = vtkProp3D::SafeDownCast( wrapper->getVtkObject() );
    if(!thisProp3D || !thatProp3D)
        return false;

    // First see if the bounding boxes collide
    double* thisBox = thisProp3D->GetBounds();
    double* thatBox = thatProp3D->GetBounds();
    double  delta[] = {0.000001,0.000001,0.000001};
    if(!thisBox || !thatBox)
        return false;

    int ret = vtkMath::BoundsIsWithinOtherBounds(thisBox, thatBox, delta);
    if(!ret)
        return false;

    // TODO: Now that the bounding box collides, lets see where the collision is
    // happening.
    
    return true;
}

bool CVtkProp3D::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!m_vtkProp3D || !path)
        return false;

    if(path->pathName() == "LinearTransform")
    {
        return m_vtkProp3D->GetUserTransform();
    }

    return CVtkObject::hasInput(path);
}

bool CVtkProp3D::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "LinearTransform")
    {
        IVtkAbstractTransformIOData* data = 0;
        bool success = inputData->queryInterface("IVtkAbstractTransformIOData", (void**)&data);
        if(success && data)
        {
            vtkLinearTransform* tx = vtkLinearTransform::SafeDownCast(data->getVtkAbstractTransform());
            if(tx)
            {
                m_vtkProp3D->SetUserTransform(tx);
                return true;
            }
        }

        return false;
    }

    return CVtkObject::setInput(path, inputData);
}

bool CVtkProp3D::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "LinearTransform")
    {
        IVtkAbstractTransformIOData* data = 0;
        bool success = inputData->queryInterface("IVtkAbstractTransformIOData", (void**)&data);
        if(success && data)
        {
            vtkLinearTransform* tx = vtkLinearTransform::SafeDownCast(data->getVtkAbstractTransform());
            if(tx && m_vtkProp3D->GetUserTransform() == tx)
            {
                m_vtkProp3D->SetUserTransform(0);
                return true;
            }
        }

        return false;
    }

    return CVtkObject::setInput(path, inputData);
}

bool CVtkProp3D::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Prop3D")
    {
        m_prop3DData.setProp3D(m_vtkProp3D);
        *outputData = &m_prop3DData;
        return true;
    }

    return CVtkObject::fetchOutput(path, outputData);
}

bool CVtkProp3D::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    Q_UNUSED(path);
    Q_UNUSED(outputData);
    return CVtkObject::outputDerefed(path, outputData);
}

vtkTransform* CVtkProp3D::userTransform()
{
    if(!m_vtkProp3D)
        return 0;

    vtkTransform* transform = vtkTransform::SafeDownCast(m_vtkProp3D->GetUserTransform());
    if(!transform)
    {
        transform = vtkTransform::New();
        m_vtkProp3D->SetUserTransform(transform);
        transform->Delete();
    }

    return transform;
}


