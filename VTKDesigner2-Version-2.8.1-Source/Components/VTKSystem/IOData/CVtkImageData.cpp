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

#include "CVtkImageData.h"

CVtkImageData::CVtkImageData(vtkImageData* imageData)
:m_vtkImageData(imageData)
{

}

CVtkImageData::~CVtkImageData()
{

}

void CVtkImageData::setImageData(vtkImageData* imageData)
{
    m_vtkImageData = imageData;
}

GCF_DEFINE_QUERY_TABLE(CVtkImageData)
    GCF_IMPLEMENTS(IVtkDataObjectIOData)
    GCF_IMPLEMENTS(IVtkDataSetIOData)
    GCF_IMPLEMENTS(IVtkImageDataIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkImageData)

void* CVtkImageData::dataPointer()
{
    return (void*)m_vtkImageData;
}

QString CVtkImageData::dataTypeName()
{
    return "vtkImageData";
}

vtkDataObject* CVtkImageData::getVtkDataObject()
{
    return static_cast<vtkDataObject*>(m_vtkImageData);
}

vtkDataSet* CVtkImageData::getVtkDataSet()
{
    return static_cast<vtkDataSet*>(m_vtkImageData);
}

vtkImageData* CVtkImageData::getVtkImageData()
{
    return m_vtkImageData;
}

vtkObject* CVtkImageData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkImageData );
}




