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
#include "CVtkPolyDataStreamer.h"

DEFINE_VTK_OBJECT(CVtkPolyDataStreamer, CVtkPolyDataFilter, vtkPolyDataStreamer)
{
    pDesc->setNodeClassDescription("Stream appends input pieces to the output");
    pDesc->setNodeClassName("vtkPolyDataStreamer");

    pDesc->setPropertyInfo("NumberOfStreamDivisions","SetNumberOfStreamDivisions","GetNumberOfStreamDivisions",QStringList() <<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("ColorByPiece","SetColorByPiece","GetColorByPiece",QStringList()<<"int",QStringList(),"void","int");
}

CVtkPolyDataStreamer::CVtkPolyDataStreamer() : m_vtkPolyDataStreamer(0)
{
    CVtkPolyDataStreamer::InitializeObjectDesc();
    setVtkObject(vtkPolyDataStreamer::New());
}

CVtkPolyDataStreamer::~CVtkPolyDataStreamer()
{

}

void CVtkPolyDataStreamer::setNumberOfStreamDivisions(int num)
{
    m_vtkPolyDataStreamer->SetNumberOfStreamDivisions(num);
}

int    CVtkPolyDataStreamer::numberOfStreamDivisions() const
{
    return m_vtkPolyDataStreamer->GetNumberOfStreamDivisions();
}

void CVtkPolyDataStreamer::setColorByPiece(bool val)
{
    m_vtkPolyDataStreamer->SetColorByPiece(val);
}

bool CVtkPolyDataStreamer::isColorByPiece() const
{
    return m_vtkPolyDataStreamer->GetColorByPiece();
}

