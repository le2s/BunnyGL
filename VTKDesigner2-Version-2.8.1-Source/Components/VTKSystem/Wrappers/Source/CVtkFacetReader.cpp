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
#include "CVtkFacetReader.h"
#include "CVtkVisSystemComponent.h"

DEFINE_VTK_OBJECT(CVtkFacetReader,CVtkPolyDataAlgorithm,vtkFacetReader)
{
    pDesc->setNodeClassName("vtkFacetReader");
    pDesc->setNodeClassDescription("Reads a dataset in Facet format");
    pDesc->setNodeClassCategory("PolyDataSource");

    pDesc->setPropertyInfo("FileName", "SetFileName", "GetFileName", QStringList() << "QString", QStringList(), "void", "QString");
}

CVtkFacetReader::CVtkFacetReader() : m_vtkFacetReader(0)
{
    CVtkFacetReader::InitializeObjectDesc();
    setVtkObject(vtkFacetReader::New());
}

CVtkFacetReader::~CVtkFacetReader()
{

}

QString CVtkFacetReader::fileName() const
{
    return m_fileName;
}


void CVtkFacetReader::setFileName(QString val)
{
    m_fileName = val;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_fileName);
    m_vtkFacetReader->SetFileName(qPrintable(fn));
}

