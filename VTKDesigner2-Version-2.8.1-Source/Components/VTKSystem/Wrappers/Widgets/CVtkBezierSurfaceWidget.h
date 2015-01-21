/****************************************************************************
**
** Copyright (C)    VCreate Logic Private Limited, Bangalore
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

#ifndef C_VTK_BEZIER_SURFACE_WIDGET_H
#define C_VTK_BEZIER_SURFACE_WIDGET_H

#include "CVtk3DWidget.h"
#include "vtkBezierSurfaceWidget.h"

class CVtkBezierSurfaceWidget : public CVtk3DWidget
{
    Q_OBJECT
    DECLARE_VTK_OBJECT

public:
    CVtkBezierSurfaceWidget();
    ~CVtkBezierSurfaceWidget();

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkBezierSurfaceWidget* m_vtkBezierSurfaceWidget;
};

#endif

