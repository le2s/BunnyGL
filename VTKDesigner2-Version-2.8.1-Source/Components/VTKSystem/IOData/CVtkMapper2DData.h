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

#ifndef C_VTK_MAPPER2D_DATA_H
#define C_VTK_MAPPER2D_DATA_H

#include "IVtkMapper2DIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkMapper2D.h"

class CVtkMapper2DData : virtual public IVtkMapper2DIOData,
                         virtual public IVtkObjectIOData
{
public:
    CVtkMapper2DData(vtkMapper2D* mapper=0);
    ~CVtkMapper2DData();

    void setMapper2D(vtkMapper2D* mapper);

    // IQueryable implementation
    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IVtkMapper2DIOData implementation
    vtkMapper2D* getVtkMapper2D();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkMapper2D* m_vtkMapper2D;
};

#endif

