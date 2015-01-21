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

#ifndef C_VTK_ABSTRACT_MAPPER_DATA_H
#define C_VTK_ABSTRACT_MAPPER_DATA_H

#include "IVtkAbstractMapperIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkAbstractMapper.h"

class CVtkAbstractMapperData : virtual public IVtkAbstractMapperIOData,
                               virtual public IVtkObjectIOData
{
public:
    CVtkAbstractMapperData(vtkAbstractMapper* abstractMapper=0);
    ~CVtkAbstractMapperData();

    void setAbstractMapper(vtkAbstractMapper* abstractMapper);

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IQueryable implementation.
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IVtkAbstractMapper implementation
    vtkAbstractMapper* getVtkAbstractMapper();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkAbstractMapper* m_vtkAbstractMapper;
};

#endif

