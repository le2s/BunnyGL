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

#include "CVtkImageAlgorithm.h"

DEFINE_VTK_OBJECT(CVtkImageAlgorithm, CVtkAlgorithm, vtkImageAlgorithm)
{
    pDesc->setNodeClassName("vtkImageAlgorithm");
    pDesc->setNodeClassCategory("Algorithm");
    pDesc->setNodeClassDescription("Base class for image algorithms");
    pDesc->setCreateFunction(0);

    // Image Data output path.
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ImageData", IVisSystemNodeConnectionPath::OutputPath, "vtkImageData", 0)
        );
}

CVtkImageAlgorithm::CVtkImageAlgorithm() : m_vtkImageAlgorithm(0)
{
    CVtkImageAlgorithm::InitializeObjectDesc();
}

CVtkImageAlgorithm::~CVtkImageAlgorithm()
{

}

bool CVtkImageAlgorithm::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** ioData)
{
    if(!path || !ioData || !m_vtkImageAlgorithm)
        return false;

    if(path->pathName() == "ImageData")
    {
        m_imageData.setImageData(m_vtkImageAlgorithm->GetOutput());
        *ioData = &m_imageData;
        return true;
    }

    return CVtkAlgorithm::fetchOutput(path, ioData);
}

bool CVtkImageAlgorithm::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* ioData)
{
    return CVtkAlgorithm::outputDerefed(path, ioData);
}

