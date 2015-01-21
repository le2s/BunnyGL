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

#include "CVtkAbstractTransform.h"

DEFINE_VTK_OBJECT(CVtkAbstractTransform, CVtkObject, vtkAbstractTransform)
{
    pDesc->setNodeClassCategory("Transform");
    pDesc->setNodeClassName("vtkAbstractTransform");
    pDesc->setCreateFunction(0);

    // Transform Output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("TransformOutput", IVisSystemNodeConnectionPath::OutputPath, "vtkAbstractTransform", 0)
        );
}

CVtkAbstractTransform::CVtkAbstractTransform() : m_vtkAbstractTransform(0)
{
    CVtkAbstractTransform::InitializeObjectDesc();
}

CVtkAbstractTransform::~CVtkAbstractTransform()
{

}

Point3D CVtkAbstractTransform::transformPoint(Point3D point)
{
    if(m_vtkAbstractTransform)
        return m_vtkAbstractTransform->TransformDoublePoint(point.x, point.y, point.y);
    return point;
}

Vector3D CVtkAbstractTransform::transformNormalAtPoint(Point3D point, Vector3D normal)
{
    if(m_vtkAbstractTransform)
    {
        double pt[3];
        double norm[3];

        pt[0] = point.x;
        pt[1] = point.y;
        pt[2] = point.z;

        norm[0] = normal.x;
        norm[1] = normal.y;
        norm[2] = normal.z;

        return m_vtkAbstractTransform->TransformNormalAtPoint(pt, norm);
    }
    return normal;
}

Vector3D CVtkAbstractTransform::transformVectorAtPoint(Point3D point, Vector3D vector)
{
    if(m_vtkAbstractTransform)
    {
        double pt[3];
        double vec[3];

        pt[0] = point.x;
        pt[1] = point.y;
        pt[2] = point.z;

        vec[0] = vector.x;
        vec[1] = vector.y;
        vec[2] = vector.z;

        return m_vtkAbstractTransform->TransformVectorAtPoint(pt, vec);
    }
    return vector;
}

bool CVtkAbstractTransform::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "TransformOutput")
    {
        m_transformData.setAbstractTransform(m_vtkAbstractTransform);
        *outputData = &m_transformData;
        return true;
    }

    return false;
}

bool CVtkAbstractTransform::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    Q_UNUSED(path);
    Q_UNUSED(outputData);
    return true;
}


