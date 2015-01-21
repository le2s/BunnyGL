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

#include "CVtkMapperData.h"

CVtkMapperData::CVtkMapperData(vtkMapper* mapper)
:m_vtkMapper(mapper)
{

}

CVtkMapperData::~CVtkMapperData()
{

}

void CVtkMapperData::setMapper(vtkMapper* mapper)
{
    m_vtkMapper = mapper;
}

void* CVtkMapperData::dataPointer()
{
    return m_vtkMapper;
}

QString CVtkMapperData::dataTypeName()
{
    return "vtkMapper";
}

GCF_BEGIN_QUERY_TABLE(CVtkMapperData)
    GCF_IMPLEMENTS(IVtkMapperIOData)
    GCF_IMPLEMENTS(IVtkAbstractMapperIOData)
    GCF_IMPLEMENTS(IVtkAbstractMapper3DIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkMapperData)

vtkMapper* CVtkMapperData::getVtkMapper()
{
    return m_vtkMapper;
}

vtkAbstractMapper3D* CVtkMapperData::getVtkAbstractMapper3D()
{
    return static_cast<vtkAbstractMapper3D*>(m_vtkMapper);
}

vtkAbstractMapper* CVtkMapperData::getVtkAbstractMapper()
{
    return static_cast<vtkAbstractMapper*>(m_vtkMapper);
}

vtkObject* CVtkMapperData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkMapper );
}





