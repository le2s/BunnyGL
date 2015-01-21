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

#include "CVtkTextSource.h"
#include "CVtkPolyData.h"

DEFINE_VTK_OBJECT(CVtkTextSource, CVtkPolyDataAlgorithm, vtkTextSource)
{
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassName("vtkTextSource");
    pDesc->setNodeClassDescription("Text Source Algorithm");
    pDesc->setPropertyInfo("Text", "SetText", "GetText", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("Backing", "SetBacking", "GetBacking", QStringList() << "int", QStringList(), "void", "int");
}

CVtkTextSource::CVtkTextSource() : m_vtkTextSource(0)
{
    CVtkTextSource::InitializeObjectDesc();
    setVtkObject(vtkTextSource::New());
}

CVtkTextSource::~CVtkTextSource()
{

}

void CVtkTextSource::setBacking(bool b)
{
    m_vtkTextSource->SetBacking(b);
}

bool CVtkTextSource::isBacking() const
{
    return m_vtkTextSource->GetBacking();
}

void CVtkTextSource::setText(QString str)
{
    m_vtkTextSource->SetText(qPrintable(str));
}

QString    CVtkTextSource::text() const
{
    return QString("%1").arg(m_vtkTextSource->GetText());
}

