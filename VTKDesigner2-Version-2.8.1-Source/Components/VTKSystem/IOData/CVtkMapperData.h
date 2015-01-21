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

#ifndef C_VTK_MAPPER_DATA_H
#define C_VTK_MAPPER_DATA_H

#include "IVtkMapperIOData.h"
#include "IVtkAbstractMapper3DIOData.h"
#include "IVtkAbstractMapperIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkMapper.h"

class CVtkMapperData : virtual public IVtkMapperIOData,
                   virtual public IVtkAbstractMapper3DIOData,
                   virtual public IVtkAbstractMapperIOData,
                   virtual public IVtkObjectIOData
{
public:
    CVtkMapperData(vtkMapper* mapper=0);
    ~CVtkMapperData();

    void setMapper(vtkMapper* mapper);

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IQueryable implementation.
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IVtkMapper implementation
    vtkMapper* getVtkMapper();

    // IVtkAbstractMapper3D implementaion
    vtkAbstractMapper3D* getVtkAbstractMapper3D();

    // IVtkAbstractMapper implementaion
    vtkAbstractMapper* getVtkAbstractMapper();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkMapper* m_vtkMapper;
};

#endif