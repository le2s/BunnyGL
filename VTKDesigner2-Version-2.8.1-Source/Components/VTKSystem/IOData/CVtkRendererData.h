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

#ifndef C_VTK_RENDERER_DATA_H
#define C_VTK_RENDERER_DATA_H

#include "IVtkRendererIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkRenderer.h"

class CVtkRendererData : virtual public IVtkRendererIOData,
                         virtual public IVtkObjectIOData
{
public:
    CVtkRendererData(vtkRenderer* renderer=0);
    ~CVtkRendererData();

    void setRenderer(vtkRenderer* renderer);

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IQueryable implementation.
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IVtkRendererIOData implementation
    vtkRenderer* getVtkRenderer();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkRenderer* m_vtkRenderer;
};

#endif