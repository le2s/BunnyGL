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

#include "CVtkDataSetReader.h"

DEFINE_VTK_OBJECT(CVtkDataSetReader,CVtkDataReader,vtkDataSetReader)
{
    pDesc->setNodeClassName("vtkDataSetReader");
    pDesc->setNodeClassDescription("Class to read any type of vtk dataset");
}

CVtkDataSetReader::CVtkDataSetReader() : m_vtkDataSetReader(0)
{
    CVtkDataSetReader::InitializeObjectDesc();
    setVtkObject(vtkDataSetReader::New());
}

CVtkDataSetReader::~CVtkDataSetReader()
{

}

IVisSystemNodeIOData* CVtkDataSetReader::readerOutput()
{
    outputData.setDataSet(m_vtkDataSetReader->GetOutput());
    return &outputData;
}
