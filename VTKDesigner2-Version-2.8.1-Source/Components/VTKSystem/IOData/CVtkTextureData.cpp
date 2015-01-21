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

#include "CVtkTextureData.h"

CVtkTextureData::CVtkTextureData(vtkTexture* texture)
:m_vtkTexture(texture)
{

}

CVtkTextureData::~CVtkTextureData()
{

}

void CVtkTextureData::setTexture(vtkTexture* texture)
{
    m_vtkTexture = texture;
}

void* CVtkTextureData::dataPointer()
{
    return m_vtkTexture;
}

QString CVtkTextureData::dataTypeName()
{
    return "vtkTexture";
}

GCF_BEGIN_QUERY_TABLE(CVtkTextureData)
    GCF_IMPLEMENTS(IVtkTextureIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkTextureData)

vtkTexture* CVtkTextureData::getVtkTexture()
{
    return m_vtkTexture;
}

vtkObject* CVtkTextureData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkTexture );
}


