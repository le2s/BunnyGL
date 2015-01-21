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

#include "CVtkQtAbstractModelAdapterData.h"

CVtkQtAbstractModelAdapterData::CVtkQtAbstractModelAdapterData()
{
    m_adapter = 0;
}

CVtkQtAbstractModelAdapterData::~CVtkQtAbstractModelAdapterData()
{

}

void CVtkQtAbstractModelAdapterData::setAbstractModelAdapter(vtkQtAbstractModelAdapter* adapter)
{
    m_adapter = adapter;
}

void* CVtkQtAbstractModelAdapterData::dataPointer()
{
    return m_adapter;
}

QString CVtkQtAbstractModelAdapterData::dataTypeName()
{
    return "vtkQtAbstractModelAdapter";
}

GCF_BEGIN_QUERY_TABLE(CVtkQtAbstractModelAdapterData)
    GCF_IMPLEMENTS(IVtkQtAbstractModelAdapterIOData)
GCF_END_QUERY_TABLE(CVtkQtAbstractModelAdapterData)

vtkQtAbstractModelAdapter*
CVtkQtAbstractModelAdapterData::getVtkQtAbstractModelAdapter()
{
    return m_adapter;
}



