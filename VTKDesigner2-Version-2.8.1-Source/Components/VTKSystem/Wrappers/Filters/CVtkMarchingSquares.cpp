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

#include "CVtkMarchingSquares.h"

DEFINE_VTK_OBJECT(CVtkMarchingSquares, CVtkPolyDataFilter, vtkMarchingSquares)
{
    pDesc->setNodeClassName("vtkMarchingSquares");
    pDesc->setNodeClassDescription("Marching Contour filter");

    pDesc->setPropertyInfo("NumberOfContours","SetNumberOfContours","GetNumberOfContours",QStringList() << "int", QStringList(),"void","int");
}

CVtkMarchingSquares::CVtkMarchingSquares() : m_vtkMarchingSquares(0)
{
    CVtkMarchingSquares::InitializeObjectDesc();
    setVtkObject(vtkMarchingSquares::New());
}

CVtkMarchingSquares::~CVtkMarchingSquares()
{

}

void CVtkMarchingSquares::setNumberOfContours(int number)
{
    m_vtkMarchingSquares->SetNumberOfContours(number);
}

int CVtkMarchingSquares::numberOfContours() const
{
    return m_vtkMarchingSquares->GetNumberOfContours();
}
