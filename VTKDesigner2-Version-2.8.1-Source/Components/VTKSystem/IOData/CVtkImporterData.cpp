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

#include "CVtkImporterData.h"

CVtkImporterData::CVtkImporterData(vtkImporter* widget)
:m_vtkImporter(widget)
{

}

CVtkImporterData::~CVtkImporterData()
{

}

void CVtkImporterData::setImporter(vtkImporter* widget)
{
    m_vtkImporter = widget;
}


void* CVtkImporterData::dataPointer()
{
    return m_vtkImporter;
}

QString CVtkImporterData::dataTypeName()
{
    return "vtkImporter";
}

GCF_BEGIN_QUERY_TABLE(CVtkImporterData)
    GCF_IMPLEMENTS(IVtkImporterIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkImporterData)

vtkImporter* CVtkImporterData::getVtkImporter()
{
    return m_vtkImporter;
}

vtkObject* CVtkImporterData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkImporter );
}




