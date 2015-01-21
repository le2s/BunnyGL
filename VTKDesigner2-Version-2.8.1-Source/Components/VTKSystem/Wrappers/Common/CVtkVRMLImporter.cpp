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

#include "CVtkVRMLImporter.h"
#include "CVtkVisSystemComponent.h"
#include <QFile>

DEFINE_VTK_OBJECT(CVtkVRMLImporter, CVtkImporter, vtkVRMLImporter)
{
    pDesc->setNodeClassName("vtkVRMLImporter");
    pDesc->setNodeClassCategory("Importers");
    pDesc->setNodeClassDescription("Imports a scene from a VRML file");
}

CVtkVRMLImporter::CVtkVRMLImporter() : m_vtkVRMLImporter(0)
{
    setVtkObject( vtkVRMLImporter::New() );
}

CVtkVRMLImporter::~CVtkVRMLImporter()
{

}

void CVtkVRMLImporter::setFileName(QString fileName)
{
    m_fileName = fileName;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_fileName);
    m_vtkVRMLImporter->SetFileName( qPrintable(fn) );

    if(!QFile::exists(fn))
        return;

    if(m_vtkVRMLImporter->GetRenderer())
        m_vtkVRMLImporter->Update();
}

QString CVtkVRMLImporter::fileName() const
{
    return m_fileName;
}

