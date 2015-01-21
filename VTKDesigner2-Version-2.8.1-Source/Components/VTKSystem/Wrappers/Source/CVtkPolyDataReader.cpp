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

#include "CVtkPolyDataReader.h"

DEFINE_VTK_OBJECT(CVtkPolyDataReader,CVtkDataReader,vtkPolyDataReader)
{
    pDesc->setNodeClassName("vtkPolyDataReader");
    pDesc->setNodeClassDescription("Read vtk polygonal data file");

}

CVtkPolyDataReader::CVtkPolyDataReader() : m_vtkPolyDataReader(0)
{
    CVtkPolyDataReader::InitializeObjectDesc();
    setVtkObject(vtkPolyDataReader::New());
}

CVtkPolyDataReader::~CVtkPolyDataReader()
{

}

IVisSystemNodeIOData* CVtkPolyDataReader::readerOutput()
{
    outputData.setPolyData(m_vtkPolyDataReader->GetOutput());
    return &outputData;
}
