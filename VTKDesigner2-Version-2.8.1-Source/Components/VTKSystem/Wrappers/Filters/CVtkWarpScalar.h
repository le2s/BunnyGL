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
#ifndef C_VTK_WARP_SCALAR_H
#define C_VTK_WARP_SCALAR_H

#include "vtkWarpScalar.h"
#include "CVtkPointSetFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkWarpScalar : public CVtkPointSetFilter
{
    Q_OBJECT
    Q_PROPERTY(double ScaleFactor READ scaleFactor WRITE setScaleFactor)
    Q_PROPERTY(bool UseNormal READ isUseNormal WRITE setUseNormal)
    Q_PROPERTY(Vector3D Normal READ normal WRITE setNormal)
    Q_PROPERTY(bool XYPlane READ isXYPlane WRITE setXYPlane)
    DECLARE_VTK_OBJECT

public:
    CVtkWarpScalar();
    ~CVtkWarpScalar();

    double scaleFactor() const;
    void setScaleFactor(double val);

    bool isUseNormal() const;
    void setUseNormal(bool val);

    Vector3D normal() const;
    void setNormal(Vector3D val);

    bool isXYPlane() const;
    void setXYPlane(bool val);


private:
    vtkWarpScalar *m_vtkWarpScalar;
};
#endif//C_VTK_WARP_SCALAR_H