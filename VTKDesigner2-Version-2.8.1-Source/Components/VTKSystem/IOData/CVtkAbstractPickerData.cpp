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

#include "CVtkAbstractPickerData.h"
#include "vtkAbstractPicker.h"

CVtkAbstractPickerData::CVtkAbstractPickerData(vtkAbstractPicker* actor)
:m_vtkAbstractPicker(actor)
{

}

CVtkAbstractPickerData::~CVtkAbstractPickerData()
{

}

void CVtkAbstractPickerData::setAbstractPicker(vtkAbstractPicker* actor)
{
    m_vtkAbstractPicker = actor;
}

GCF_DEFINE_QUERY_TABLE(CVtkAbstractPickerData)
    GCF_IMPLEMENTS(IVtkAbstractPickerIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkAbstractPickerData)

void* CVtkAbstractPickerData::dataPointer()
{
    return (void*)(m_vtkAbstractPicker);
}

QString CVtkAbstractPickerData::dataTypeName()
{
    return "vtkAbstractPicker";
}

vtkAbstractPicker* CVtkAbstractPickerData::getVtkAbstractPicker()
{
    return m_vtkAbstractPicker;
}

vtkObject* CVtkAbstractPickerData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkAbstractPicker );
}

