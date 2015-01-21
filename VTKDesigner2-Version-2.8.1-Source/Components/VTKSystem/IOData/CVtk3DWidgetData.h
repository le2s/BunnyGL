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

#ifndef C_VTK_3D_WIDGET_DATA_H
#define C_VTK_3D_WIDGET_DATA_H

#include "IVtk3DWidgetIOData.h"
#include "IVtkObjectIOData.h"
#include "vtk3DWidget.h"

class CVtk3DWidgetData : virtual public IVtk3DWidgetIOData,
                         virtual public IVtkObjectIOData
{
public:
    CVtk3DWidgetData(vtk3DWidget* widget=0);
    ~CVtk3DWidgetData();

    void set3DWidget(vtk3DWidget* widget);

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IQueryable implementation.
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IVtk3DWidgetIOData implementation
    vtk3DWidget* getVtk3DWidget();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtk3DWidget* m_vtk3DWidget;
};

#endif