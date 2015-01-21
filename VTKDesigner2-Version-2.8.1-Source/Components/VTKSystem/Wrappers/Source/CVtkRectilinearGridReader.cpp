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

#include "CVtkRectilinearGridReader.h"

DEFINE_VTK_OBJECT(CVtkRectilinearGridReader,CVtkDataReader,vtkRectilinearGridReader)
{
    pDesc->setNodeClassName("vtkRectilinearGridReader");
    pDesc->setNodeClassDescription("Read vtk rectilinear grid data file");

}

CVtkRectilinearGridReader::CVtkRectilinearGridReader() : m_vtkRectilinearGridReader(0)
{
    CVtkRectilinearGridReader::InitializeObjectDesc();
    setVtkObject(vtkRectilinearGridReader::New());
}

CVtkRectilinearGridReader::~CVtkRectilinearGridReader()
{

}

IVisSystemNodeIOData* CVtkRectilinearGridReader::readerOutput()
{
    outputData.setRectilinearGrid(m_vtkRectilinearGridReader->GetOutput());
    return &outputData;
}
