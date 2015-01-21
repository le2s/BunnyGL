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

#include "CVtkStructuredGridReader.h"

DEFINE_VTK_OBJECT(CVtkStructuredGridReader,CVtkDataReader,vtkStructuredGridReader)
{
    pDesc->setNodeClassName("vtkStructuredGridReader");
    pDesc->setNodeClassDescription("Read vtk structured grid data file");
}

CVtkStructuredGridReader::CVtkStructuredGridReader() : m_vtkStructuredGridReader(0)
{
    CVtkStructuredGridReader::InitializeObjectDesc();
    setVtkObject(vtkStructuredGridReader::New());
}

CVtkStructuredGridReader::~CVtkStructuredGridReader()
{

}

IVisSystemNodeIOData* CVtkStructuredGridReader::readerOutput()
{
    outputData.setStructuredGrid(m_vtkStructuredGridReader->GetOutput());
    return &outputData;
}
