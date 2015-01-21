/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this flie is limited according to the terms specified by
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

#include "CVtkPointWidget.h"
#include "vtkPolyData.h"

DEFINE_VTK_OBJECT(CVtkPointWidget, CVtk3DWidget, vtkPointWidget)
{
    pDesc->setNodeClassName("vtkPointWidget");
    pDesc->setNodeClassCategory("Interactor Observers");
    pDesc->setNodeClassDescription("Provides a Point widget");

    // PolyData output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("PolyData", IVisSystemNodeConnectionPath::OutputPath, "vtkDataSet", 0));
    pDesc->setPropertyInfo("XShadows", "setXShadows", "isXShadows", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("YShadows", "setYShadows", "isYShadows", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("ZShadows", "setZShadows", "isZShadows", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("XShadows", "setXShadows", "isXShadows", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("TranslationMode", "setTranslationMode", "isTranslationMode", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("AllOn", "setAllOn", "isAllOn", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("Outline", "setOutline", "isOutline", QStringList() << "bool", QStringList(), "void", "bool");
    
};

CVtkPointWidget::CVtkPointWidget()
{
    m_vtkPointWidget = 0;
    m_pointWidgetOutput = 0;
    setVtkObject(vtkPointWidget::New());
    m_vtkPointWidget->SetEnabled(true);
    m_allOn = true;
    m_vtkPointWidget->AllOn();
}

CVtkPointWidget::~CVtkPointWidget()
{
    if(m_pointWidgetOutput)
        m_pointWidgetOutput->Delete();
}

void CVtkPointWidget::setPosition(Point3D point)
{
    m_vtkPointWidget->SetPosition(point.x, point.y, point.z);
}

Point3D CVtkPointWidget::position() const
{
    return Point3D( m_vtkPointWidget->GetPosition() );
}

void CVtkPointWidget::setOutline(bool val)
{
    m_outline = val;
    if(val)
        m_vtkPointWidget->OutlineOn();
    else
        m_vtkPointWidget->OutlineOff();
}

bool CVtkPointWidget::isOutline() const
{
    return m_outline;
}

void CVtkPointWidget::setXShadows(bool val)
{
    m_xShadows = val;

    if(val)
        m_vtkPointWidget->XShadowsOn();
    else
        m_vtkPointWidget->XShadowsOff();
}

bool CVtkPointWidget::isXShadows() const
{
    return m_xShadows;
}

void CVtkPointWidget::setYShadows(bool val)
{
    m_yShadows = val;

    if(val)
        m_vtkPointWidget->YShadowsOn();
    else
        m_vtkPointWidget->YShadowsOff();
}

bool CVtkPointWidget::isYShadows() const
{
    return m_yShadows;
}

void CVtkPointWidget::setZShadows(bool val)
{
    m_zShadows = val;

    if(val)
        m_vtkPointWidget->ZShadowsOn();
    else
        m_vtkPointWidget->ZShadowsOff();
}

bool CVtkPointWidget::isZShadows() const
{
    return m_zShadows;
}

void CVtkPointWidget::setTranslationMode(bool val)
{
    m_translationMode = val;

    if(val)
        m_vtkPointWidget->TranslationModeOn();
    else
        m_vtkPointWidget->TranslationModeOff();
}

bool CVtkPointWidget::isTranslationMode() const
{
    return m_translationMode;
}

void CVtkPointWidget::setAllOn(bool val)
{
    m_allOn = val;
    if(val)
    {
        m_vtkPointWidget->AllOn();
        setXShadows(true);
        setYShadows(true);
        setZShadows(true);
        setOutline(true);
        setTranslationMode(true);
    }
    else
    {
        m_vtkPointWidget->AllOff();
        setXShadows(false);
        setYShadows(false);
        setZShadows(false);
        setOutline(false);
        setTranslationMode(false);
    }
}

bool CVtkPointWidget::isAllOn() const
{
    return m_allOn;
}

bool CVtkPointWidget::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

  
    if(path->pathName() == "PolyData")
    {       
        if(!m_pointWidgetOutput)
            m_pointWidgetOutput = vtkPolyData::New();
        m_vtkPointWidget->GetPolyData(m_pointWidgetOutput);
        m_polyData.setPolyData(m_pointWidgetOutput);
        *outputData = &m_polyData;
        return true;
    }

    return CVtk3DWidget::fetchOutput(path, outputData);
}

bool CVtkPointWidget::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    return CVtk3DWidget::outputDerefed(path, outputData);
}
