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

#include "CVtkMapper2D.h"

DEFINE_VTK_OBJECT(CVtkMapper2D, CVtkObject, vtkMapper2D)
{
    pDesc->setNodeClassCategory("Rendering");
    pDesc->setNodeClassName("vtkMapper2D");
    pDesc->setCreateFunction(0);

    // Mapper2D output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Mapper2D", IVisSystemNodeConnectionPath::OutputPath, "vtkMapper2D", 0)
        );
}

CVtkMapper2D::CVtkMapper2D() : m_vtkMapper2D(0)
{
    CVtkMapper2D::InitializeObjectDesc();
}

CVtkMapper2D::~CVtkMapper2D()
{

}

bool CVtkMapper2D::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if( !path || !outputData )
        return false;

    if(path->pathName() == "Mapper2D")
    {
        m_mapper2DData.setMapper2D( m_vtkMapper2D );
        *outputData = &m_mapper2DData;
        return true;
    }

    return CVtkObject::fetchOutput(path, outputData);
}

bool CVtkMapper2D::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if( !path || !outputData )
        return false;

    if(path->pathName() == "Mapper2D")
        return true;

    return CVtkObject::outputDerefed(path, outputData);
}



