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

#ifndef C_VTK_WEDGE_SOURCE_H
#define C_VTK_WEDGE_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkWedgeSource.h"
#include "CVtkPolyData.h"
#include "UserDefinedDataTypes.h"

class CVtkWedgeSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(double Width READ width WRITE setWidth)
    Q_PROPERTY(double Height READ height WRITE setHeight)
    Q_PROPERTY(double Depth READ depth WRITE setDepth)
    Q_PROPERTY(double BaseAngle READ baseAngle WRITE setBaseAngle);
    DECLARE_VTK_OBJECT

public:
    CVtkWedgeSource();
    ~CVtkWedgeSource();

    void setWidth(double w);
    double width() const;

    void setHeight(double h);
    double height() const;

    void setDepth(double d);
    double depth() const;

    void setBaseAngle(double a);
    double baseAngle() const;

signals:
    void propertyChanged(const QString& propName);

private:
    vtkWedgeSource* m_vtkWedgeSource;
};

#endif

