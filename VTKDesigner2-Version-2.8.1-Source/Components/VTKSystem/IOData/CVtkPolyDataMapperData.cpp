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

#include "CVtkPolyDataMapperData.h"

CVtkPolyDataMapperData::CVtkPolyDataMapperData(vtkPolyDataMapper* polyDataMapper)
:m_vtkPolyDataMapper(polyDataMapper)
{

}

CVtkPolyDataMapperData::~CVtkPolyDataMapperData()
{

}

void CVtkPolyDataMapperData::setPolyDataMapper(vtkPolyDataMapper* polyDataMapper)
{
    m_vtkPolyDataMapper = polyDataMapper;
}

void* CVtkPolyDataMapperData::dataPointer()
{
    return m_vtkPolyDataMapper;
}

QString CVtkPolyDataMapperData::dataTypeName()
{
    return "vtkPolyDataMapper";
}

GCF_BEGIN_QUERY_TABLE(CVtkPolyDataMapperData)
    GCF_IMPLEMENTS(IVtkPolyDataMapperIOData)
    GCF_IMPLEMENTS(IVtkMapperIOData)
    GCF_IMPLEMENTS(IVtkAbstractMapper3DIOData)
    GCF_IMPLEMENTS(IVtkAbstractMapperIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkPolyDataMapperData)

vtkPolyDataMapper* CVtkPolyDataMapperData::getVtkPolyDataMapper()
{
    return m_vtkPolyDataMapper;
}

vtkMapper* CVtkPolyDataMapperData::getVtkMapper()
{
    return static_cast<vtkMapper*>(m_vtkPolyDataMapper);
}

vtkAbstractMapper3D* CVtkPolyDataMapperData::getVtkAbstractMapper3D()
{
    return static_cast<vtkAbstractMapper3D*>(m_vtkPolyDataMapper);
}

vtkAbstractMapper* CVtkPolyDataMapperData::getVtkAbstractMapper()
{
    return static_cast<vtkAbstractMapper*>(m_vtkPolyDataMapper);
}

vtkObject* CVtkPolyDataMapperData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkPolyDataMapper );
}




