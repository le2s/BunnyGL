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

#include "CVtkAlgorithmData.h"
#include "vtkAlgorithm.h"

CVtkAlgorithmData::CVtkAlgorithmData(vtkAlgorithm* algorithm)
:m_vtkAlgorithm(algorithm)
{

}

CVtkAlgorithmData::~CVtkAlgorithmData()
{

}

void CVtkAlgorithmData::setAlgorithm(vtkAlgorithm* algorithm)
{
    m_vtkAlgorithm = algorithm;
}

GCF_DEFINE_QUERY_TABLE(CVtkAlgorithmData)
    GCF_IMPLEMENTS(IVtkAlgorithmIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkAlgorithmData)

void* CVtkAlgorithmData::dataPointer()
{
    return (void*)(m_vtkAlgorithm);
}

QString CVtkAlgorithmData::dataTypeName()
{
    return "vtkAlgorithm";
}

vtkAlgorithm* CVtkAlgorithmData::getVtkAlgorithm()
{
    return m_vtkAlgorithm;
}

vtkObject* CVtkAlgorithmData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkAlgorithm );
}

