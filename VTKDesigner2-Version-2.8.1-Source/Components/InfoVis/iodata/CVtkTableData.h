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

#ifndef C_VTK_TABLE_DATA_H
#define C_VTK_TABLE_DATA_H

#include "IVtkTableIOData.h"
#include "vtkTable.h"

class CVtkTableData : virtual public IVtkTableIOData
{
public:
    CVtkTableData();
    ~CVtkTableData();

    void setTable(vtkTable* table);

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IQueryable implementation.
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IVtkTableIOData implementation
    vtkTable* getVtkTable();

private:
    vtkTable* m_table;
};


#endif
