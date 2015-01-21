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

#include "CVtkStructuredPointsReader.h"

DEFINE_VTK_OBJECT(CVtkStructuredPointsReader,CVtkDataReader,vtkStructuredPointsReader)
{
    pDesc->setNodeClassName("vtkStructuredPointsReader");
    pDesc->setNodeClassDescription("Read vtk structured points data file");

}

CVtkStructuredPointsReader::CVtkStructuredPointsReader() : m_vtkStructuredPointsReader(0)
{
    CVtkStructuredPointsReader::InitializeObjectDesc();
    setVtkObject(vtkStructuredPointsReader::New());
}

CVtkStructuredPointsReader::~CVtkStructuredPointsReader()
{

}

IVisSystemNodeIOData* CVtkStructuredPointsReader::readerOutput()
{
    outputData.setStructuredPoints(m_vtkStructuredPointsReader->GetOutput());
    return &outputData;
}


