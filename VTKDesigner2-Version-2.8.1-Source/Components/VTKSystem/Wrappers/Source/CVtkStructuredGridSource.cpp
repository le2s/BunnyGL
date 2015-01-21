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

#include "CVtkStructuredGridSource.h"

DEFINE_VTK_OBJECT(CVtkStructuredGridSource, CVtkAlgorithm, vtkStructuredGridSource)
{
    pDesc->setNodeClassCategory("Source");
    pDesc->setNodeClassName("vtkStructuredGridSource");
    pDesc->setNodeClassDescription("A Structured Grid Source abstraction");
    pDesc->setCreateFunction(0);

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("StructuredGridOutput", IVisSystemNodeConnectionPath::OutputPath,  "vtkStructuredGrid", 0)
    );
}

CVtkStructuredGridSource::CVtkStructuredGridSource() : m_vtkStructuredGridSource(0)
{
    CVtkStructuredGridSource::InitializeObjectDesc();
}

CVtkStructuredGridSource::~CVtkStructuredGridSource()
{

}

bool CVtkStructuredGridSource::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !m_vtkStructuredGridSource || !outputData)
        return false;

    if(path->pathName() == "StructuredGridOutput")
    {
        m_CVtkStructuredGrid.setStructuredGrid(m_vtkStructuredGridSource->GetOutput());
        *outputData = &m_CVtkStructuredGrid;
        return true;
    }

    return CVtkAlgorithm::fetchOutput(path, outputData);
}

bool CVtkStructuredGridSource::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !m_vtkStructuredGridSource)
        return false;

    return CVtkAlgorithm::outputDerefed(path, outputData);
}

