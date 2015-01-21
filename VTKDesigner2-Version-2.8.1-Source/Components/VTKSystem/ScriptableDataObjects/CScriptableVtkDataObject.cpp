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

#include "CScriptableVtkDataObject.h"
#include "CVtkObject.h"

CScriptableVtkDataObject::CScriptableVtkDataObject(vtkDataObject* dataObject, CVtkObject* source)
:m_vtkDataObject(dataObject), m_CVtkObject(source)
{

}

CScriptableVtkDataObject::~CScriptableVtkDataObject()
{

}

QObject* CScriptableVtkDataObject::source() const
{
    return m_CVtkObject;
}

vtkDataObject* CScriptableVtkDataObject::dataObject() const
{
    return m_vtkDataObject;
}

int CScriptableVtkDataObject::dataObjectType() const
{
    return m_vtkDataObject->GetDataObjectType();
}

unsigned long CScriptableVtkDataObject::updateTime() const
{
    return m_vtkDataObject->GetUpdateTime();
}

unsigned long CScriptableVtkDataObject::actualMemorySize() const
{
    return m_vtkDataObject->GetActualMemorySize();
}

void CScriptableVtkDataObject::setReleaseData(bool val)
{
    m_vtkDataObject->SetReleaseDataFlag(val);
}

bool CScriptableVtkDataObject::isReleaseData() const
{
    return m_vtkDataObject->GetReleaseDataFlag();
}


