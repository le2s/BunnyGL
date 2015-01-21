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

#include "CVtkImageReader.h"
#include "CScriptableVtkImageData.h"

DEFINE_VTK_OBJECT(CVtkImageReader, CVtkImageReader2, vtkImageReader)
{
    pDesc->setNodeClassName("vtkImageReader");
    pDesc->setNodeClassDescription("Reads a Image");
    pDesc->setCreateFunction(0);
}

CVtkImageReader::CVtkImageReader() : m_vtkImageReader(0), m_imageDataObject(0)
{
    CVtkImageReader::InitializeObjectDesc();
}

CVtkImageReader::~CVtkImageReader()
{
    if(m_imageDataObject)
        delete m_imageDataObject;
}

QObject* CVtkImageReader::imageDataObject()
{
    if(!m_imageDataObject)
        m_imageDataObject = new CScriptableVtkImageData(m_vtkImageReader->GetOutput(), this);
    return m_imageDataObject;
}
