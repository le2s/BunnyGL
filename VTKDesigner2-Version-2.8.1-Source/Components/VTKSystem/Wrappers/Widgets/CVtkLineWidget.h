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

#ifndef C_VTK_LINE_WIDGET_H
#define C_VTK_LINE_WIDGET_H

#include "CVtk3DWidget.h"
#include "vtkLineWidget.h"
#include "UserDefinedDataTypes.h"

class CVtkLineWidget : public CVtk3DWidget
{
    Q_OBJECT
    Q_ENUMS(Alignment)
    Q_PROPERTY(int Resolution READ resolution WRITE setResolution)
    Q_PROPERTY(Point3D Point1 READ point1 WRITE setPoint1)
    Q_PROPERTY(Point3D Point2 READ point2 WRITE setPoint2)
    Q_PROPERTY(Alignment Alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(bool ClampToBounds READ isClampToBounds WRITE setClampToBounds)
    DECLARE_VTK_OBJECT

public:
    enum Alignment
    {
        NoAlignment  = 3, // vtkLineWidget::None,
        AlignToXAxis = 0, // vtkLineWidget::XAxis,
        AlignToYAxis = 1, // vtkLineWidget::YAxis,
        AlignToZAxis = 2 // vtkLineWidget::ZAxis
    };

    CVtkLineWidget();
    ~CVtkLineWidget();

    void setResolution(int val);
    int resolution() const;

    void setPoint1(Point3D point);
    Point3D point1() const;

    void setPoint2(Point3D point);
    Point3D point2() const;

    void setAlignment(Alignment val);
    Alignment alignment() const;

    void setClampToBounds(bool val);
    bool isClampToBounds() const;

private:
    vtkLineWidget* m_vtkLineWidget;

};

#endif
