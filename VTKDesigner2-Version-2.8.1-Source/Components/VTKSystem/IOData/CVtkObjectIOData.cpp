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

#include "CVtkObjectIOData.h"

CVtkObjectIOData::CVtkObjectIOData(vtkObject* object)
:m_vtkObject(object)
{

}

CVtkObjectIOData::~CVtkObjectIOData()
{

}

void CVtkObjectIOData::setObject(vtkObject* object)
{
    m_vtkObject = object;
}

GCF_BEGIN_QUERY_TABLE(CVtkObjectIOData)
    GCF_IMPLEMENTS(IVisSystemNodeIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkObjectIOData)

void* CVtkObjectIOData::dataPointer()
{
    return m_vtkObject;
}

QString    CVtkObjectIOData::dataTypeName()
{
    return "vtkObject";
}

vtkObject* CVtkObjectIOData::getVtkObject()
{
    return m_vtkObject;
}

