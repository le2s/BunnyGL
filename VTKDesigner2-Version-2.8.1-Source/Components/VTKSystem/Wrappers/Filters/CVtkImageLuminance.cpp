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

#include "CVtkImageLuminance.h"
#include "CVtkThreadedImageAlgorithm.h"
#include "IVtkImageDataIOData.h"

DEFINE_VTK_OBJECT(CVtkImageLuminance, CVtkThreadedImageAlgorithm, vtkImageLuminance)
{
    pDesc->setNodeClassName("vtkImageLuminance");
    pDesc->setNodeClassDescription("calculates luminance from an rgb input");
    pDesc->setNodeClassCategory("Image Filters");

    // ImageData input path
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("RGBInput", IVisSystemNodeConnectionPath::InputPath, "vtkImageData", 0));
}

CVtkImageLuminance::CVtkImageLuminance() : m_vtkImageLuminance(0)
{
    CVtkImageLuminance::InitializeObjectDesc();
    setVtkObject(vtkImageLuminance::New());
}

CVtkImageLuminance::~CVtkImageLuminance()
{
}

bool CVtkImageLuminance::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "RGBInput")
        return m_vtkImageLuminance->GetInput() != 0;

    return CVtkThreadedImageAlgorithm::hasInput(path);
}

bool CVtkImageLuminance::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "RGBInput")
    {
        IVtkImageDataIOData* data = 0;
        bool success = inputData->queryInterface("IVtkImageDataIOData", (void**)&data);
        if(success)
        {
            vtkImageData* imData = data->getVtkImageData();
            m_vtkImageLuminance->SetInput(imData);
            return true;
        }

        return false;
    }

    return CVtkThreadedImageAlgorithm::setInput(path, inputData);
}

bool CVtkImageLuminance::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "RGBInput")
    {
        m_vtkImageLuminance->SetInput(0);
        return true;
    }

    return CVtkThreadedImageAlgorithm::removeInput(path, inputData);
}