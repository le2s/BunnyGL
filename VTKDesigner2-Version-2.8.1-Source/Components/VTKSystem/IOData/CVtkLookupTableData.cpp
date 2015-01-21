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

#include "CVtkLookupTableData.h"

CVtkLookupTableData::CVtkLookupTableData(vtkLookupTable* lookUpTable)
:m_vtkLookupTable(lookUpTable)
{

}

CVtkLookupTableData::~CVtkLookupTableData()
{

}

void CVtkLookupTableData::setLookupTable(vtkLookupTable* lookUpTable)
{
    m_vtkLookupTable = lookUpTable;
}

void* CVtkLookupTableData::dataPointer()
{
    return m_vtkLookupTable;
}

QString CVtkLookupTableData::dataTypeName()
{
    return "vtkLookupTable";
}

GCF_BEGIN_QUERY_TABLE(CVtkLookupTableData)
    GCF_IMPLEMENTS(IVtkLookupTableIOData)
    GCF_IMPLEMENTS(IVtkScalarsToColorsIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkLookupTableData)

vtkLookupTable* CVtkLookupTableData::getVtkLookupTable()
{
    return m_vtkLookupTable;
}

vtkScalarsToColors* CVtkLookupTableData::getVtkScalarsToColors()
{
    return static_cast<vtkScalarsToColors*>(m_vtkLookupTable);
}

vtkObject* CVtkLookupTableData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkLookupTable );
}


