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

#include "CVtkThreadedImageAlgorithm.h"
#include "IVtkImageDataIOData.h"

DEFINE_VTK_OBJECT(CVtkThreadedImageAlgorithm, CVtkImageAlgorithm, vtkThreadedImageAlgorithm)
{
    pDesc->setNodeClassName("vtkThreadedImageAlgorithm");
    pDesc->setNodeClassCategory("ImageAlgorithm");
    pDesc->setNodeClassDescription("Class for multiple image threads");
    pDesc->setCreateFunction(0);

    pDesc->setPropertyInfo("NumberOfThreads", "SetNumberOfThreads", "GetNumberOfThreads", QStringList()<<"int",QStringList(),"void","int");
    
    // Image Data input path.
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ImageDataInput", IVisSystemNodeConnectionPath::InputPath, "vtkImageData", 0)
        );
}


CVtkThreadedImageAlgorithm::CVtkThreadedImageAlgorithm() : m_vtkThreadedImageAlgorithm(0)
{
    CVtkThreadedImageAlgorithm::InitializeObjectDesc();
}

CVtkThreadedImageAlgorithm::~CVtkThreadedImageAlgorithm()
{
}

void CVtkThreadedImageAlgorithm::setNumberOfThreads(int num)
{
    m_vtkThreadedImageAlgorithm->SetNumberOfThreads(num);
}

int CVtkThreadedImageAlgorithm::numberOfThreads() const
{
    return m_vtkThreadedImageAlgorithm->GetNumberOfThreads();
}

bool CVtkThreadedImageAlgorithm::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImageDataInput")
    {
        IVtkImageDataIOData* imageData = 0;
        bool success = inputData->queryInterface("IVtkImageDataIOData", (void**)&imageData);
        if(success && imageData)
        {
            m_vtkThreadedImageAlgorithm->SetInput(imageData->getVtkImageData());
            return true;
        }

        return false;
    }

    return CVtkImageAlgorithm::setInput(path, inputData);
}

bool CVtkThreadedImageAlgorithm::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImageDataInput")
    {
        IVtkImageDataIOData* imageData = 0;
        bool success = inputData->queryInterface("IVtkImageDataIOData", (void**)&imageData);
        if(success && imageData && m_vtkThreadedImageAlgorithm->GetInput() == imageData->getVtkImageData())
        {
            m_vtkThreadedImageAlgorithm->SetInput(0);
            return true;
        }

        return false;
    }

    return CVtkImageAlgorithm::removeInput(path, inputData);
}



