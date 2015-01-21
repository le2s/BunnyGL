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

#ifndef C_VTK_OBJECT_IO_DATA_H
#define C_VTK_OBJECT_IO_DATA_H

#include "IVtkObjectIOData.h"

class CVtkObjectIOData : virtual public IVtkObjectIOData
{
public:
    CVtkObjectIOData(vtkObject* object=0);
    ~CVtkObjectIOData();

    void setObject(vtkObject* object);

    // IQueryable implementation
    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString    dataTypeName();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkObject* m_vtkObject;
};

#endif

