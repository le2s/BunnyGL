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

#include "CVtkDataSetMapperData.h"

CVtkDataSetMapperData::CVtkDataSetMapperData(vtkDataSetMapper* dataSetMapper)
:m_vtkDataSetMapper(dataSetMapper)
{

}

CVtkDataSetMapperData::~CVtkDataSetMapperData()
{

}

void CVtkDataSetMapperData::setDataSetMapper(vtkDataSetMapper* dataSetMapper)
{
    m_vtkDataSetMapper = dataSetMapper;
}

void* CVtkDataSetMapperData::dataPointer()
{
    return m_vtkDataSetMapper;
}

QString CVtkDataSetMapperData::dataTypeName()
{
    return "vtkDataSetMapper";
}

GCF_BEGIN_QUERY_TABLE(CVtkDataSetMapperData)
    GCF_IMPLEMENTS(IVtkDataSetMapperIOData)
    GCF_IMPLEMENTS(IVtkMapperIOData)
    GCF_IMPLEMENTS(IVtkAbstractMapper3DIOData)
    GCF_IMPLEMENTS(IVtkAbstractMapperIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkDataSetMapperData)

vtkDataSetMapper* CVtkDataSetMapperData::getVtkDataSetMapper()
{
    return m_vtkDataSetMapper;
}

vtkMapper* CVtkDataSetMapperData::getVtkMapper()
{
    return static_cast<vtkMapper*>(m_vtkDataSetMapper);
}

vtkAbstractMapper3D* CVtkDataSetMapperData::getVtkAbstractMapper3D()
{
    return static_cast<vtkAbstractMapper3D*>(m_vtkDataSetMapper);
}

vtkAbstractMapper* CVtkDataSetMapperData::getVtkAbstractMapper()
{
    return static_cast<vtkAbstractMapper*>(m_vtkDataSetMapper);
}

vtkObject* CVtkDataSetMapperData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkDataSetMapper );
}










