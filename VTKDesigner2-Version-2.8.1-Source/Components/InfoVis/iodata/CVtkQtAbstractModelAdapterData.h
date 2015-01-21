/****************************************************************************
**
** Copyright (C) UNO and VCreate Logic
**
** Use of this file is limited according to the terms specified by
** UNO and VCreate Logic.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef C_VTK_QT_ABSTRACT_MODEL_ADAPTER_DATA_H
#define C_VTK_QT_ABSTRACT_MODEL_ADAPTER_DATA_H

#include "IVtkQtAbstractModelAdapterIOData.h"
#include "vtkQtAbstractModelAdapter.h"

class CVtkQtAbstractModelAdapterData : virtual public IVtkQtAbstractModelAdapterIOData
{
public:
    CVtkQtAbstractModelAdapterData();
    ~CVtkQtAbstractModelAdapterData();

    void setAbstractModelAdapter(vtkQtAbstractModelAdapter* adapter);

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IQueryable implementation.
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IVtkQtAbstractModelAdapterIOData implementation
    vtkQtAbstractModelAdapter* getVtkQtAbstractModelAdapter();

private:
    vtkQtAbstractModelAdapter* m_adapter;
};

#endif

