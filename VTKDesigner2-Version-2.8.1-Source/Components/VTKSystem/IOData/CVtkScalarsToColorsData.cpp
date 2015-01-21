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

#include "CVtkScalarsToColorsData.h"

CVtkScalarsToColorsData::CVtkScalarsToColorsData(vtkScalarsToColors* scalarsToColors)
:m_vtkScalarsToColors(scalarsToColors)
{

}

CVtkScalarsToColorsData::~CVtkScalarsToColorsData()
{

}

void CVtkScalarsToColorsData::setScalarsToColors(vtkScalarsToColors* scalarsToColors)
{
    m_vtkScalarsToColors = scalarsToColors;
}

void* CVtkScalarsToColorsData::dataPointer()
{
    return m_vtkScalarsToColors;
}

QString CVtkScalarsToColorsData::dataTypeName()
{
    return "vtkScalarsToColors";
}

GCF_BEGIN_QUERY_TABLE(CVtkScalarsToColorsData)
    GCF_IMPLEMENTS(IVtkScalarsToColorsIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkScalarsToColorsData)

vtkScalarsToColors* CVtkScalarsToColorsData::getVtkScalarsToColors()
{
    return m_vtkScalarsToColors;
}

vtkObject* CVtkScalarsToColorsData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkScalarsToColors );
}





