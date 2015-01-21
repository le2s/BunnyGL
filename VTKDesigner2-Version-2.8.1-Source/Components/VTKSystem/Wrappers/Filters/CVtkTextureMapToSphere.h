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
#ifndef C_VTK_TEXTURE_MAP_TO_SPHERE_H
#define C_VTK_TEXTURE_MAP_TO_SPHERE_H

#include "vtkTextureMapToSphere.h"
#include "CVtkDataSetFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkTextureMapToSphere : public CVtkDataSetFilter
{
    Q_OBJECT
    Q_PROPERTY(Point3D Center READ center WRITE setCenter)
    Q_PROPERTY(bool PreventSeam READ isPreventSeam WRITE setPreventSeam)
    Q_PROPERTY(bool AutomaticSphereGeneration READ isAutomaticSphereGeneration WRITE setAutomaticSphereGeneration)
    DECLARE_VTK_OBJECT

public:
    CVtkTextureMapToSphere();
    ~CVtkTextureMapToSphere();

    Point3D center() const;
    void setCenter(Point3D val);

    bool isPreventSeam() const;
    void setPreventSeam(bool val);

    bool isAutomaticSphereGeneration() const;
    void setAutomaticSphereGeneration(bool val);


private:
    vtkTextureMapToSphere *m_vtkTextureMapToSphere;
};
#endif//C_VTK_TEXTURE_MAP_TO_SPHERE_H