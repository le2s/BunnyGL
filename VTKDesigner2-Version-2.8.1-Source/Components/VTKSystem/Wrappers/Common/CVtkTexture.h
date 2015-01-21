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

#ifndef C_VTK_TEXTURE_H
#define C_VTK_TEXTURE_H

#include "CVtkImageAlgorithm.h"
#include "vtkTexture.h"
#include "CVtkTextureData.h"

class CVtkTexture : public CVtkImageAlgorithm
{
    Q_OBJECT
    Q_ENUMS(TextureQuality)
    Q_PROPERTY(bool Repeat READ isRepeat WRITE setRepeat)
    Q_PROPERTY(bool Interpolate READ isInterpolate WRITE setInterpolate)
    Q_PROPERTY(TextureQuality TextureQuality READ textureQuality WRITE setTextureQuality)
    Q_PROPERTY(bool MapScalarsThroughLookupTable READ isMapScalarsThroughLookupTable WRITE setMapScalarsThroughLookupTable)
    DECLARE_VTK_OBJECT

public:
    enum TextureQuality
    {
        Default = VTK_TEXTURE_QUALITY_DEFAULT,
        SixteenBit = VTK_TEXTURE_QUALITY_16BIT,
        ThirtyTwoBit = VTK_TEXTURE_QUALITY_32BIT
    };

    CVtkTexture();
    ~CVtkTexture();

    void setRepeat(bool repeat);
    bool isRepeat() const;

    void setInterpolate(bool val);
    bool isInterpolate() const;

    void setTextureQuality(TextureQuality val);
    TextureQuality textureQuality() const;

    void setMapScalarsThroughLookupTable(bool val);
    bool isMapScalarsThroughLookupTable() const;

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkTexture* m_vtkTexture;
    CVtkTextureData m_textureData;
};

#endif
