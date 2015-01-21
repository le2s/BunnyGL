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

#ifndef C_VTK_ABSTRACT_TRANSFORM_H
#define C_VTK_ABSTRACT_TRANSFORM_H

#include "CVtkObject.h"
#include "vtkAbstractTransform.h"
#include "CVtkAbstractTransformData.h"
#include "UserDefinedDataTypes.h"

class CVtkAbstractTransform : public CVtkObject
{
    Q_OBJECT
    DECLARE_VTK_OBJECT

public:
    CVtkAbstractTransform();
    ~CVtkAbstractTransform();

    Q_INVOKABLE Point3D transformPoint(Point3D point);
    Q_INVOKABLE Vector3D transformNormalAtPoint(Point3D point, Vector3D normal);
    Q_INVOKABLE Vector3D transformVectorAtPoint(Point3D point, Vector3D vector);

protected:
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkAbstractTransform* m_vtkAbstractTransform;
    CVtkAbstractTransformData m_transformData;
};

#endif
