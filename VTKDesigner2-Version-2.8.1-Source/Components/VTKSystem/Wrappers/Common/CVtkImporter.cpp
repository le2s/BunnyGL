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

#include "CVtkImporter.h"

DEFINE_VTK_OBJECT(CVtkImporter, CVtkObject, vtkImporter)
{
    pDesc->setNodeClassName("vtkImporter");
    pDesc->setNodeClassCategory("Importers");
    pDesc->setNodeClassDescription("Base class for importers");
    pDesc->setCreateFunction(0);

    // Statistics Output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Importer", IVisSystemNodeConnectionPath::OutputPath, "vtkImporter", 0)
        );

}

CVtkImporter::CVtkImporter() : m_vtkImporter(0)
{
    CVtkImporter::InitializeObjectDesc();
}

CVtkImporter::~CVtkImporter()
{

}

bool CVtkImporter::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Importer")
    {
        m_ImporterData.setImporter(m_vtkImporter);
        *outputData = &m_ImporterData;
        return true;
    }

    return CVtkObject::fetchOutput(path, outputData);
}

bool CVtkImporter::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    return CVtkObject::outputDerefed(path, outputData);
}

