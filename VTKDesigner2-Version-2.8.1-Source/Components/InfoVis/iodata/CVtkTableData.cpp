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

#include "CVtkTableData.h"

CVtkTableData::CVtkTableData()
{
    m_table = 0;
}

CVtkTableData::~CVtkTableData()
{

}

void CVtkTableData::setTable(vtkTable* table)
{
    m_table = table;
}

void* CVtkTableData::dataPointer()
{
    return m_table;
}

QString CVtkTableData::dataTypeName()
{
    return "vtkTable";
}

GCF_BEGIN_QUERY_TABLE(CVtkTableData)
    GCF_IMPLEMENTS(IVtkTableIOData)
GCF_END_QUERY_TABLE(CVtkTableData)

vtkTable* CVtkTableData::getVtkTable()
{
    return m_table;
}



