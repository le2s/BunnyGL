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

#include "CVtkVectorText.h"
#include "CVtkPolyData.h"

DEFINE_VTK_OBJECT(CVtkVectorText, CVtkPolyDataAlgorithm, vtkVectorText)
{
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassName("vtkVectorText");
    pDesc->setNodeClassDescription("Create polygonal text");
    pDesc->setPropertyInfo("Text", "SetText", "GetText", QStringList() << "QString", QStringList(), "void", "QString");
}

CVtkVectorText::CVtkVectorText() : m_vtkVectorText(0)
{
    CVtkVectorText::InitializeObjectDesc();
    setVtkObject(vtkVectorText::New());
}

CVtkVectorText::~CVtkVectorText()
{

}

void CVtkVectorText::setText(QString str)
{
    m_vtkVectorText->SetText(qPrintable(str));
}

QString    CVtkVectorText::text() const
{
    return QString("%1").arg(m_vtkVectorText->GetText());
}

