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

#include "CVtkAbstractMapperData.h"

CVtkAbstractMapperData::CVtkAbstractMapperData(vtkAbstractMapper* abstractMapper)
:m_vtkAbstractMapper(abstractMapper)
{

}

CVtkAbstractMapperData::~CVtkAbstractMapperData()
{

}

void CVtkAbstractMapperData::setAbstractMapper(vtkAbstractMapper* abstractMapper)
{
    m_vtkAbstractMapper = abstractMapper;
}

void* CVtkAbstractMapperData::dataPointer()
{
    return m_vtkAbstractMapper;
}

QString CVtkAbstractMapperData::dataTypeName()
{
    return "vtkAbstractMapper";
}

GCF_BEGIN_QUERY_TABLE(CVtkAbstractMapperData)
    GCF_IMPLEMENTS(IVtkAbstractMapperIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkAbstractMapperData)

vtkAbstractMapper* CVtkAbstractMapperData::getVtkAbstractMapper()
{
    return m_vtkAbstractMapper;
}

vtkObject* CVtkAbstractMapperData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkAbstractMapper );
}


