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

#include "CVtkPLYReader.h"
#include "CVtkVisSystemComponent.h"

DEFINE_VTK_OBJECT(CVtkPLYReader,CVtkPolyDataAlgorithm,vtkPLYReader)
{
    pDesc->setNodeClassName("vtkPLYReader");
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassDescription("Read Stanford University PLY polygonal file format");

    pDesc->setPropertyInfo("FileName", "SetFileName", "GetFileName", QStringList() << "QString", QStringList(), "void", "QString");
}

CVtkPLYReader::CVtkPLYReader() : m_vtkPLYReader(0)
{
    CVtkPLYReader::InitializeObjectDesc();
    setVtkObject(vtkPLYReader::New());
}

CVtkPLYReader::~CVtkPLYReader()
{

}

QString CVtkPLYReader::fileName() const
{
    return m_fileName;
}


void CVtkPLYReader::setFileName(QString val)
{
    m_fileName = val;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_fileName);
    m_vtkPLYReader->SetFileName(qPrintable(fn));
}

