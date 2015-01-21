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
#ifndef C_VTK_WARP_TO_H
#define C_VTK_WARP_TO_H

#include "vtkWarpTo.h"
#include "CVtkPointSetFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkWarpTo : public CVtkPointSetFilter
{
    Q_OBJECT
    Q_PROPERTY(double ScaleFactor READ scaleFactor WRITE setScaleFactor)
    Q_PROPERTY(Point3D Position READ position WRITE setPosition)
    Q_PROPERTY(bool Absolute READ isAbsolute WRITE setAbsolute)
    DECLARE_VTK_OBJECT

public:
    CVtkWarpTo();
    ~CVtkWarpTo();

    double scaleFactor() const;
    void setScaleFactor(double val);

    Point3D position() const;
    void setPosition(Point3D val);

    bool isAbsolute() const;
    void setAbsolute(bool val);


private:
    vtkWarpTo *m_vtkWarpTo;
};
#endif//C_VTK_WARP_TO_H