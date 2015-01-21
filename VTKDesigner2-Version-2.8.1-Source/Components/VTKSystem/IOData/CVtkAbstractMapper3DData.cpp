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

#include "CVtkAbstractMapper3DData.h"

CVtkAbstractMapper3DData::CVtkAbstractMapper3DData(vtkAbstractMapper3D* abstractMapper3D)
:m_vtkAbstractMapper3D(abstractMapper3D)
{

}

CVtkAbstractMapper3DData::~CVtkAbstractMapper3DData()
{

}

void CVtkAbstractMapper3DData::setAbstractMapper3D(vtkAbstractMapper3D* abstractMapper3D)
{
    m_vtkAbstractMapper3D = abstractMapper3D;
}

void* CVtkAbstractMapper3DData::dataPointer()
{
    return m_vtkAbstractMapper3D;
}

QString CVtkAbstractMapper3DData::dataTypeName()
{
    return "vtkAbstractMapper3D";
}


GCF_BEGIN_QUERY_TABLE(CVtkAbstractMapper3DData)
    GCF_IMPLEMENTS(IVtkAbstractMapper3DIOData)
    GCF_IMPLEMENTS(IVtkAbstractMapperIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkAbstractMapper3DData)

vtkAbstractMapper3D* CVtkAbstractMapper3DData::getVtkAbstractMapper3D()
{
    return m_vtkAbstractMapper3D;
}

vtkAbstractMapper* CVtkAbstractMapper3DData::getVtkAbstractMapper()
{
    return static_cast<vtkAbstractMapper*>(m_vtkAbstractMapper3D);
}

vtkObject* CVtkAbstractMapper3DData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkAbstractMapper3D );
}



