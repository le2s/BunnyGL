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

#include "CVtkBoxWidget.h"

DEFINE_VTK_OBJECT(CVtkBoxWidget, CVtk3DWidget, vtkBoxWidget)
{
    pDesc->setNodeClassName("vtkBoxWidget");
    pDesc->setNodeClassCategory("Interactor Observers");
    pDesc->setNodeClassDescription("Provides a box widget");

    pDesc->setPropertyInfo("InsideOut", "SetInsideOut", "GetInsideOut", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("OutlineFaceWires", "SetOutlineFaceWires", "GetOutlineFaceWires", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("OutlineCursorWires", "SetOutlineCursorWires", "GetOutlineCursorWires", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("TranslationEnabled", "SetTranslationEnabled", "GetTranslationEnabled", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("RotationEnabled", "SetRotationEnabled", "GetRotationEnabled", QStringList() << "bool", QStringList(), "void", "bool");
    
    // PolyData output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("PolyData", IVisSystemNodeConnectionPath::OutputPath, "vtkDataSet", 0)
    );
};

CVtkBoxWidget::CVtkBoxWidget()
{
    m_vtkBoxWidget = 0;
    m_boxWidgetOutput = 0;
    setVtkObject(vtkBoxWidget::New());
    m_handlesVisible = true;
    m_vtkBoxWidget->HandlesOn();
}

CVtkBoxWidget::~CVtkBoxWidget()
{
    if(m_boxWidgetOutput)
        m_boxWidgetOutput->Delete();
}

void CVtkBoxWidget::setInsideOut(bool val)
{
    m_vtkBoxWidget->SetInsideOut(val);
}

bool CVtkBoxWidget::isInsideOut() const
{
    return m_vtkBoxWidget->GetInsideOut();
}

void CVtkBoxWidget::setHandlesVisible(bool val)
{
    m_handlesVisible = val;

    if(val)
        m_vtkBoxWidget->HandlesOn();
    else
        m_vtkBoxWidget->HandlesOff();
}

bool CVtkBoxWidget::isHandlesVisible() const
{
    return m_handlesVisible;
}

void CVtkBoxWidget::setOutlineFaceWires(bool val)
{
    m_vtkBoxWidget->SetOutlineFaceWires(val);
}

bool CVtkBoxWidget::isOutlineFaceWires() const
{
    return m_vtkBoxWidget->GetOutlineFaceWires();
}

void CVtkBoxWidget::setOutlineCursorWires(bool val)
{
    m_vtkBoxWidget->SetOutlineCursorWires(val);
}

bool CVtkBoxWidget::isOutlineCursorWires() const
{
    return m_vtkBoxWidget->GetOutlineCursorWires();
}

void CVtkBoxWidget::setTranslationEnabled(bool val)
{
    m_vtkBoxWidget->SetTranslationEnabled(val);
}

bool CVtkBoxWidget::isTranslationEnabled() const
{
    return m_vtkBoxWidget->GetTranslationEnabled();
}

void CVtkBoxWidget::setScalingEnabled(bool val)
{
    m_vtkBoxWidget->SetScalingEnabled(val);
}

bool CVtkBoxWidget::isScalingEnabled() const
{
    return m_vtkBoxWidget->GetScalingEnabled();
}

void CVtkBoxWidget::setRotationEnabled(bool val)
{
    m_vtkBoxWidget->SetRotationEnabled(val);
}

bool CVtkBoxWidget::isRotationEnabled() const
{
    return m_vtkBoxWidget->GetRotationEnabled();
}

bool CVtkBoxWidget::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;


    if(path->pathName() == "PolyData")
    {
        if(!m_boxWidgetOutput)
            m_boxWidgetOutput = vtkPolyData::New();
        m_vtkBoxWidget->GetPolyData(m_boxWidgetOutput);
        m_polyData.setPolyData(m_boxWidgetOutput);
        *outputData = &m_polyData;
        return true;
    }

    return CVtk3DWidget::fetchOutput(path, outputData);
}

bool CVtkBoxWidget::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    return CVtk3DWidget::outputDerefed(path, outputData);
}



