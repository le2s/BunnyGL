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

#include "CVtkTexture.h"
#include "IVtkImageDataIOData.h"

DEFINE_VTK_OBJECT(CVtkTexture, CVtkImageAlgorithm, vtkTexture)
{
    pDesc->setNodeClassName("vtkTexture");

    pDesc->setPropertyInfo("Repeat", "SetRepeat", "GetRepeat", QStringList()<<"int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Interpolate", "SetInterpolate", "GetInterpolate", QStringList()<<"int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("TextureQuality", "SetTextureQuality", "GetTextureQuality", QStringList()<<"int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("MapScalarsThroughLookupTable", "SetMapColorScalarsThroughLookupTable", "GetMapColorScalarsThroughLookupTable", QStringList()<<"int", QStringList(), "void", "int");

    // Image Data input path.
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ImageData", IVisSystemNodeConnectionPath::InputPath, "vtkImageData", 0)
        );

    // Texture Output Path
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Texture", IVisSystemNodeConnectionPath::OutputPath, "vtkTexture", 0)
        );
}

CVtkTexture::CVtkTexture() : m_vtkTexture(0)
{
    CVtkTexture::InitializeObjectDesc();
    setVtkObject(vtkTexture::New());
}

CVtkTexture::~CVtkTexture()
{

}

void CVtkTexture::setRepeat(bool repeat)
{
    m_vtkTexture->SetRepeat(repeat);
}

bool CVtkTexture::isRepeat() const
{
    return m_vtkTexture->GetRepeat();
}

void CVtkTexture::setInterpolate(bool val)
{
    m_vtkTexture->SetInterpolate(val);
}

bool CVtkTexture::isInterpolate() const
{
    return m_vtkTexture->GetInterpolate();
}

void CVtkTexture::setTextureQuality(CVtkTexture::TextureQuality val)
{
    switch(val)
    {
    case Default: m_vtkTexture->SetQuality(VTK_TEXTURE_QUALITY_DEFAULT); break;
    case SixteenBit: m_vtkTexture->SetQuality(VTK_TEXTURE_QUALITY_16BIT); break;
    case ThirtyTwoBit: m_vtkTexture->SetQuality(VTK_TEXTURE_QUALITY_32BIT); break;
    default: break;
    }
}

CVtkTexture::TextureQuality CVtkTexture::textureQuality() const
{
    int tq = m_vtkTexture->GetQuality();

    switch(tq)
    {
    case VTK_TEXTURE_QUALITY_DEFAULT: return Default;
    case VTK_TEXTURE_QUALITY_16BIT: return SixteenBit;
    case VTK_TEXTURE_QUALITY_32BIT: return ThirtyTwoBit;
    }

    return Default;
}

void CVtkTexture::setMapScalarsThroughLookupTable(bool val)
{
    m_vtkTexture->SetMapColorScalarsThroughLookupTable(val);
}

bool CVtkTexture::isMapScalarsThroughLookupTable() const
{
    return m_vtkTexture->GetMapColorScalarsThroughLookupTable();
}

bool CVtkTexture::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "ImageData")
    {
        return (m_vtkTexture->GetInput() != 0);
    }

    return CVtkImageAlgorithm::hasInput(path);
}

bool CVtkTexture::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImageData")
    {
        IVtkImageDataIOData* imageData = 0;
        bool success = inputData->queryInterface("IVtkImageDataIOData", (void**)&imageData);
        if(success && imageData)
        {
            vtkImageData* data = imageData->getVtkImageData();
            m_vtkTexture->SetInput(data);
            return true;
        }
        return false;
    }

    return CVtkImageAlgorithm::setInput(path, inputData);
}

bool CVtkTexture::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImageData")
    {
        IVtkImageDataIOData* imageData = 0;
        bool success = inputData->queryInterface("IVtkImageDataIOData", (void**)&imageData);
        if(success && imageData && m_vtkTexture->GetInput() == imageData->getVtkImageData())
        {
            m_vtkTexture->SetInput(0);
            return true;
        }
        return false;
    }

    return CVtkImageAlgorithm::removeInput(path, inputData);
}

bool CVtkTexture::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path)
        return false;

    if(path->pathName() == "Texture")
    {
        m_textureData.setTexture(m_vtkTexture);
        *outputData = &m_textureData;
        return true;
    }

    return CVtkImageAlgorithm::fetchOutput(path, outputData);
}

bool CVtkTexture::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    return CVtkImageAlgorithm::outputDerefed(path, outputData);
}

