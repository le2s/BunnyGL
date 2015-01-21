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

#ifndef C_VTK_POINT_WIDGET_H
#define C_VTK_POINT_WIDGET_H

#include "CVtk3DWidget.h"
#include "vtkPointWidget.h"
#include "CVtkPolyData.h"
#include "UserDefinedDataTypes.h"

class CVtkPointWidget : public CVtk3DWidget
{
    Q_OBJECT

    Q_PROPERTY(bool Outline READ isOutline WRITE setOutline)
    Q_PROPERTY(bool XShadows READ isXShadows WRITE setXShadows)
    Q_PROPERTY(bool YShadows READ isYShadows WRITE setYShadows)
    Q_PROPERTY(bool ZShadows READ isZShadows WRITE setZShadows)
    Q_PROPERTY(bool TranslationMode READ isTranslationMode WRITE setTranslationMode)
    Q_PROPERTY(bool AllOn READ isAllOn WRITE setAllOn)

    DECLARE_VTK_OBJECT

public:

    CVtkPointWidget();
    ~CVtkPointWidget();

    Q_INVOKABLE void setPosition(Point3D point);
    Q_INVOKABLE Point3D position() const;

    void setOutline(bool val);
    bool isOutline() const;

    void setXShadows(bool val);
    bool isXShadows() const;

    void setYShadows(bool val);
    bool isYShadows() const;

    void setZShadows(bool val);
    bool isZShadows() const;

    void setTranslationMode(bool val);
    bool isTranslationMode() const;

    void setAllOn(bool val);
    bool isAllOn() const;

protected:
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkPointWidget* m_vtkPointWidget;
    vtkPolyData* m_pointWidgetOutput;
    CVtkPolyData m_polyData;
    bool m_outline;
    bool m_xShadows;
    bool m_yShadows;
    bool m_zShadows;
    bool m_translationMode;
    bool m_allOn;

};

#endif
