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

#ifndef C_VTK_POLY_DATA_MAPPER_DATA_H
#define C_VTK_POLY_DATA_MAPPER_DATA_H

#include "IVtkPolyDataMapperIOData.h"
#include "IVtkMapperIOData.h"
#include "IVtkAbstractMapper3DIOData.h"
#include "IVtkAbstractMapperIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkPolyDataMapper.h"

class CVtkPolyDataMapperData : virtual public IVtkPolyDataMapperIOData,
                               virtual public IVtkMapperIOData,
                               virtual public IVtkAbstractMapper3DIOData,
                               virtual public IVtkAbstractMapperIOData,
                               virtual public IVtkObjectIOData
{
public:
    CVtkPolyDataMapperData(vtkPolyDataMapper* polyDataMapper=0);
    ~CVtkPolyDataMapperData();

    void setPolyDataMapper(vtkPolyDataMapper* polyDataMapper);

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IQueryable implementation.
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IVtkPolyDataMapper implementation
    vtkPolyDataMapper* getVtkPolyDataMapper();

    // IVtkMapper implementation
    vtkMapper* getVtkMapper();

    // IVtkAbstractMapper3D implementation
    vtkAbstractMapper3D* getVtkAbstractMapper3D();

    // IVtkAbstractMapper implementation
    vtkAbstractMapper* getVtkAbstractMapper();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkPolyDataMapper* m_vtkPolyDataMapper;
};

#endif