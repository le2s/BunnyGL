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

#ifndef C_VTK_SPLINE_WIDGET_H
#define C_VTK_SPLINE_WIDGET_H

#include "CVtk3DWidget.h"
#include "vtkSplineWidget.h"
#include "UserDefinedDataTypes.h"

class CVtkSplineWidget : public CVtk3DWidget
{
    Q_OBJECT
    Q_ENUMS(ProjectionNormalAxis)
    Q_PROPERTY(ProjectionNormalAxis Axis READ projectionNormalAxis WRITE setProjectionNormalAxis)
    Q_PROPERTY(bool ProjectToPlane READ isProjectToPlane WRITE setProjectToPlane)
    Q_PROPERTY(double ProjectionPosition READ projectionPosition WRITE setProjectionPosition)
    Q_PROPERTY(int NumberOfHandles READ numberOfHandles WRITE setNumberOfHandles)
    Q_PROPERTY(int Resolution READ resolution WRITE setResolution)
    Q_PROPERTY(bool Close READ isClosed WRITE setClosed)
    DECLARE_VTK_OBJECT

public:

    enum ProjectionNormalAxis
    {
        XAxis,
        YAxis,
        ZAxis
    };

    CVtkSplineWidget();
    ~CVtkSplineWidget();

    void setProjectionNormalAxis(ProjectionNormalAxis axis);
    ProjectionNormalAxis projectionNormalAxis() const;

    void setProjectToPlane(bool val);
    bool isProjectToPlane() const;

    void setProjectionPosition(double val);
    double projectionPosition() const;

    void setNumberOfHandles(int val);
    int numberOfHandles() const;

    void setResolution(int val);
    int resolution() const;

    Q_INVOKABLE void setHandlePosition(int val, Point3D point);
    Q_INVOKABLE Point3D handlePosition(int index) const;

    void setClosed(bool val);
    bool isClosed() const;

private:
    vtkSplineWidget* m_vtkSplineWidget;
    bool m_projectToPlane;
    bool m_closed;
};

#endif
