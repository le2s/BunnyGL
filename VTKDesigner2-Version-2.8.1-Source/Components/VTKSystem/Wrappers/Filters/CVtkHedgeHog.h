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

#ifndef C_VTK_HEDGE_HOG_H
#define C_VTK_HEDGE_HOG_H

#include "CVtkPolyDataFilter.h"
#include "vtkHedgeHog.h"

class CVtkHedgeHog : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS (VectorMode)
    Q_PROPERTY(double ScaleFactor READ scaleFactor WRITE setScaleFactor)
    Q_PROPERTY(VectorMode VectorMode READ vectorMode WRITE setVectorMode)
    DECLARE_VTK_OBJECT

public:
    CVtkHedgeHog();
    ~CVtkHedgeHog();
    
    enum VectorMode {
        Vector = VTK_USE_VECTOR,
        Normal = VTK_USE_NORMAL
    };

    void setScaleFactor(double val);
    double scaleFactor() const;

    void setVectorMode(VectorMode val);
    VectorMode vectorMode() const;

private:
    vtkHedgeHog *m_vtkHedgeHog;
};

#endif
