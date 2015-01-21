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

#ifndef C_VTK_AXES_SOURCE_H
#define C_VTK_AXES_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkAxes.h"
#include "UserDefinedDataTypes.h"
#include "CVtkPolyData.h"

class CVtkAxes : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    
    Q_PROPERTY(Point3D Origin READ origin WRITE setOrigin)
    Q_PROPERTY(double ScaleFactor READ scaleFactor WRITE setScaleFactor)
    Q_PROPERTY(bool Symmetric READ isSymmetric WRITE setSymmetric)
    Q_PROPERTY(bool ComputeNormals READ isComputeNormals WRITE setComputeNormals)
    DECLARE_VTK_OBJECT

public:
    CVtkAxes();
    ~CVtkAxes();

    // Property methods
    Point3D origin() const;
    void setOrigin(Point3D origin);
    
    double scaleFactor() const;
    void setScaleFactor(double scale);

    bool isSymmetric() const;
    void setSymmetric(bool s);

    bool isComputeNormals() const;
    void setComputeNormals(bool c);
    
private:
    vtkAxes* m_vtkAxes;
};

#endif

