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

#ifndef C_VTK_IMAGE_MAP_TO_WINDOW_LEVEL_COLORS_H
#define C_VTK_IMAGE_MAP_TO_WINDOW_LEVEL_COLORS_H

#include "CVtkImageMapToColors.h"
#include "vtkImageMapToWindowLevelColors.h"

class CVtkImageMapToWindowLevelColors : public CVtkImageMapToColors
{
    Q_OBJECT
    DECLARE_VTK_OBJECT

public:
    CVtkImageMapToWindowLevelColors();
    ~CVtkImageMapToWindowLevelColors();

    Q_PROPERTY(double Window READ window WRITE setWindow)
    void setWindow(double val);
    double window() const;

    Q_PROPERTY(double Level READ level WRITE setLevel)
    void setLevel(double val);
    double level() const;

protected:
    void initializeNode();

private:
    vtkImageMapToWindowLevelColors* m_vtkImageMapToWindowLevelColors;
};

#endif
