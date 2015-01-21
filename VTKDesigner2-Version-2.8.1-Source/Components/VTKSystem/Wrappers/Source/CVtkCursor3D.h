
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

#ifndef C_VTK_CURSOR3D_H
#define C_VTK_CURSOR3D_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkCursor3D.h"
#include "UserDefinedDataTypes.h"

class CVtkCursor3D : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(Point3D FocalPoint READ focalPoint WRITE setFocalPoint)
    Q_PROPERTY(bool Outline READ isOutline WRITE setOutline)
    Q_PROPERTY(bool Axes READ isAxes WRITE setAxes)
    Q_PROPERTY(bool XShadows READ isXShadows WRITE setXShadows)
    Q_PROPERTY(bool YShadows READ isYShadows WRITE setYShadows)
    Q_PROPERTY(bool ZShadows READ isZShadows WRITE setZShadows)
    Q_PROPERTY(bool TranslationMode READ isTranslationMode WRITE setTranslationMode)
    Q_PROPERTY(bool Wrap READ isWrap WRITE setWrap)
    DECLARE_VTK_OBJECT

public:
    CVtkCursor3D();
    ~CVtkCursor3D();

    // Property methods
    Point3D focalPoint() const;
    void setFocalPoint(Point3D fp);

    bool isOutline() const;
    void setOutline(bool val);

    bool isAxes() const;
    void setAxes(bool val);

    bool isXShadows() const;
    void setXShadows(bool val);

    bool isYShadows() const;
    void setYShadows(bool val);

    bool isZShadows() const;
    void setZShadows(bool val);

    bool isTranslationMode() const;
    void setTranslationMode(bool val);

    bool isWrap() const;
    void setWrap(bool val);

private:
    vtkCursor3D* m_vtkCursor3D;
};

#endif

