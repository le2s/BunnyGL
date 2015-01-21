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

#include "CVtkOpenHapticsEffectIOData.h"
#include "vtkOpenHapticsEffect.h"

CVtkOpenHapticsEffectIOData::CVtkOpenHapticsEffectIOData(vtkOpenHapticsEffect* object)
{
    setObject(object);
}

CVtkOpenHapticsEffectIOData::~CVtkOpenHapticsEffectIOData()
{

}

void CVtkOpenHapticsEffectIOData::setObject(vtkOpenHapticsEffect* object)
{
    m_vtkOpenHapticsEffect = object;
}

GCF_BEGIN_QUERY_TABLE(CVtkOpenHapticsEffectIOData)
    GCF_IMPLEMENTS(IVtkOpenHapticsEffectIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkOpenHapticsEffectIOData)

void* CVtkOpenHapticsEffectIOData::dataPointer()
{
    return m_vtkOpenHapticsEffect;
}

QString CVtkOpenHapticsEffectIOData::dataTypeName()
{
    return "vtkOpenHapticsEffect";
}

vtkOpenHapticsEffect* CVtkOpenHapticsEffectIOData::getVtkOpenHapticsEffect()
{
    return m_vtkOpenHapticsEffect;
}

vtkObject* CVtkOpenHapticsEffectIOData::getVtkObject()
{
    return vtkObject::SafeDownCast(m_vtkOpenHapticsEffect);
}


