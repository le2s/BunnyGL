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

#include "CVtk3DSImporter.h"
#include "CVtkVisSystemComponent.h"
#include <QFile>

DEFINE_VTK_OBJECT(CVtk3DSImporter, CVtkImporter, vtk3DSImporter)
{
    pDesc->setNodeClassName("vtk3DSImporter");
    pDesc->setNodeClassCategory("Importers");
    pDesc->setNodeClassDescription("Imports scene from a 3DStudio Max");

    pDesc->setPropertyInfo("FileName", "SetFileName", "GetFileName", QStringList() << "const char*",
                           QStringList(), "void", "const char*");
    pDesc->setPropertyInfo("ComputeNormals", "SetComputeNormals", "GetComputeNormals",
                           QStringList() << "int", QStringList(),
                           "void", "int");
}

CVtk3DSImporter::CVtk3DSImporter() : m_vtk3DSImporter(0)
{
    setVtkObject( vtk3DSImporter::New() );
}

CVtk3DSImporter::~CVtk3DSImporter()
{

}

void CVtk3DSImporter::setFileName(QString fileName)
{
    m_fileName = fileName;

    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_fileName);
    m_vtk3DSImporter->SetFileName( qPrintable(fn) );

    if(!QFile::exists(fn))
        return;

    if(m_vtk3DSImporter->GetRenderer())
        m_vtk3DSImporter->Update();
}

QString CVtk3DSImporter::fileName() const
{
    return m_fileName;
}

void CVtk3DSImporter::setComputeNormals(bool val)
{
    m_vtk3DSImporter->SetComputeNormals(val);
}

bool CVtk3DSImporter::isComputeNormals() const
{
    return m_vtk3DSImporter->GetComputeNormals();
}





