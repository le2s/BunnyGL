/****************************************************************************
**
** Copyright (C)    VCreate Logic Private Limited, Bangalore
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

#ifndef C_VTK_BOX_WIDGET_H
#define C_VTK_BOX_WIDGET_H

#include "CVtk3DWidget.h"
#include "vtkBoxWidget.h"
#include "CVtkPolyData.h"
#include "CVtkTransform.h"

class CVtkBoxWidget : public CVtk3DWidget
{
    Q_OBJECT
    Q_PROPERTY(bool InsideOut READ isInsideOut WRITE setInsideOut)
    Q_PROPERTY(bool HandlesVisible READ isHandlesVisible WRITE setHandlesVisible)
    Q_PROPERTY(bool OutlineFaceWires READ isOutlineFaceWires WRITE setOutlineFaceWires)
    Q_PROPERTY(bool OutlineCursorWires READ isOutlineCursorWires WRITE setOutlineCursorWires)
    Q_PROPERTY(bool TranslationEnabled READ isTranslationEnabled WRITE setTranslationEnabled)
    Q_PROPERTY(bool ScalingEnabled READ isScalingEnabled WRITE setScalingEnabled)
    Q_PROPERTY(bool RotationEnabled READ isRotationEnabled WRITE setRotationEnabled)
    DECLARE_VTK_OBJECT

public:
    CVtkBoxWidget();
    ~CVtkBoxWidget();

    void setInsideOut(bool val);
    bool isInsideOut() const;

    void setHandlesVisible(bool val);
    bool isHandlesVisible() const;

    void setOutlineFaceWires(bool val);
    bool isOutlineFaceWires() const;

    void setOutlineCursorWires(bool val);
    bool isOutlineCursorWires() const;

    void setTranslationEnabled(bool val);
    bool isTranslationEnabled() const;

    void setScalingEnabled(bool val);
    bool isScalingEnabled() const;

    void setRotationEnabled(bool val);
    bool isRotationEnabled() const;

protected:
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkBoxWidget* m_vtkBoxWidget;
    vtkPolyData* m_boxWidgetOutput;
    CVtkPolyData m_polyData;
    CVtkTransform m_transform;
    bool m_handlesVisible;
};

#endif
