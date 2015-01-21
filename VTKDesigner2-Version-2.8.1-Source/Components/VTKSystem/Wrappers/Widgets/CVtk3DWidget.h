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

#ifndef C_VTK_3D_WIDGET_H
#define C_VTK_3D_WIDGET_H

#include "CVtkInteractorObserver.h"
#include "vtk3DWidget.h"

class CVtk3DWidget : public CVtkInteractorObserver
{
    Q_OBJECT
    Q_PROPERTY(double PlaceFactor READ placeFactor WRITE setPlaceFactor)
    Q_PROPERTY(double HandleSize READ handleSize WRITE setHandleSize)
    DECLARE_VTK_OBJECT

public:
    CVtk3DWidget();
    ~CVtk3DWidget();

    Q_INVOKABLE void placeWidget(double xmin, double xmax,
                                 double ymin, double ymax,
                                 double zmin, double zmax);

    void setPlaceFactor(double val);
    double placeFactor() const;

    void setHandleSize(double val);
    double handleSize() const;

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtk3DWidget* m_vtk3DWidget;
};

#endif
