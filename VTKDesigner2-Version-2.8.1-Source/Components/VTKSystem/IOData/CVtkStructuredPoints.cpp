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

#include "CVtkStructuredPoints.h"

CVtkStructuredPoints::CVtkStructuredPoints(vtkStructuredPoints* sPoints)
:m_vtkStructuredPoints(sPoints)
{

}

CVtkStructuredPoints::~CVtkStructuredPoints()
{

}

void CVtkStructuredPoints::setStructuredPoints(vtkStructuredPoints* sPoints)
{
    m_vtkStructuredPoints = sPoints;
}

GCF_DEFINE_QUERY_TABLE(CVtkStructuredPoints)
    GCF_IMPLEMENTS(IVtkDataObjectIOData)
    GCF_IMPLEMENTS(IVtkDataSetIOData)
    GCF_IMPLEMENTS(IVtkImageDataIOData)
    GCF_IMPLEMENTS(IVtkStructuredPointsIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkStructuredPoints)

void* CVtkStructuredPoints::dataPointer()
{
    return (void*)m_vtkStructuredPoints;
}

QString CVtkStructuredPoints::dataTypeName()
{
    return "vtkStructuredPoints";
}

vtkDataObject* CVtkStructuredPoints::getVtkDataObject()
{
    return static_cast<vtkDataObject*>(m_vtkStructuredPoints);
}

vtkDataSet* CVtkStructuredPoints::getVtkDataSet()
{
    return static_cast<vtkDataSet*>(m_vtkStructuredPoints);
}

vtkImageData* CVtkStructuredPoints::getVtkImageData()
{
    return static_cast<vtkImageData*>(m_vtkStructuredPoints);
}

vtkStructuredPoints* CVtkStructuredPoints::getVtkStructuredPoints()
{
    return m_vtkStructuredPoints;
}

vtkObject* CVtkStructuredPoints::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkStructuredPoints );
}

