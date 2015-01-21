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

#ifndef C_VTK_IMAGE_MAP_TO_COLORS_H
#define C_VTK_IMAGE_MAP_TO_COLORS_H

#include "CVtkThreadedImageAlgorithm.h"
#include "vtkImageMapToColors.h"

class CVtkImageMapToColors : public CVtkThreadedImageAlgorithm
{
    Q_OBJECT
    DECLARE_VTK_OBJECT

public:
    CVtkImageMapToColors();
    ~CVtkImageMapToColors();

    Q_ENUMS(OutputFormat)
    Q_PROPERTY(OutputFormat OutputFormat READ outputFormat WRITE setOutputFormat)
    enum OutputFormat
    {
        RGBA = VTK_RGBA,
        RGB = VTK_RGB,
        LuminanceAlpha = VTK_LUMINANCE_ALPHA,
        Luminance = VTK_LUMINANCE
    };
    void setOutputFormat(OutputFormat fmt);
    OutputFormat outputFormat() const;

    Q_PROPERTY(int ActiveComponent READ activeComponent WRITE setActiveComponent)
    void setActiveComponent(int val);
    int activeComponent() const;

    Q_PROPERTY(bool PassAlphaToOutput READ isPassAlphaToOutput WRITE setPassAlphaToOutput)
    void setPassAlphaToOutput(bool val);
    bool isPassAlphaToOutput() const;

protected:
    void initializeNode();

    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);


private:
    vtkImageMapToColors* m_vtkImageMapToColors;
};

#endif
