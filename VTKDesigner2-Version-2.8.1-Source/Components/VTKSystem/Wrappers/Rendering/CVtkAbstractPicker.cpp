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

#include "CVtkAbstractPicker.h"
#include "vtkRenderer.h"
#include "CVtkRenderer.h"
#include "IVtkProp3DIOData.h"
#include "IVtkRendererIOData.h"
#include "vtkPropCollection.h"

DEFINE_VTK_OBJECT(CVtkAbstractPicker, CVtkObject, vtkAbstractPicker)
{
    pDesc->setNodeClassCategory("Locator");
    pDesc->setNodeClassName("vtkAbstractPicker");
    pDesc->setCreateFunction(0);

    // Prop Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Prop3D", IVisSystemNodeConnectionPath::InputPath, "vtkProp3D", 0, true)
        );

    // Renderer Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Renderer", IVisSystemNodeConnectionPath::InputPath, "vtkRenderer", 0)
        );

    // Abstract Picker Output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("AbstractPicker", IVisSystemNodeConnectionPath::InputPath, "vtkAbstractPicker", 0)
        );
}

CVtkAbstractPicker::CVtkAbstractPicker() : m_vtkAbstractPicker(0), m_vtkRenderer(0)
{
    CVtkAbstractPicker::InitializeObjectDesc();
    setVtkObject(vtkAbstractPicker::New());
}

CVtkAbstractPicker::~CVtkAbstractPicker()
{

}

Point3D CVtkAbstractPicker::getSelectionPoint()
{
    return m_vtkAbstractPicker->GetSelectionPoint();
}

Point3D CVtkAbstractPicker::getPickPosition()
{
    return m_vtkAbstractPicker->GetPickPosition();
}

int CVtkAbstractPicker::pick(Point3D point)
{
    if(m_vtkRenderer)
        return m_vtkAbstractPicker->Pick(point.x, point.y, point.z, m_vtkRenderer);
    return 0;
}

int CVtkAbstractPicker::pick(Point3D point, QObject* rendererWrapper)
{
    CVtkRenderer* ren = qobject_cast<CVtkRenderer*>(rendererWrapper);
    if(!ren)
        return 0;

    vtkRenderer* renderer = vtkRenderer::SafeDownCast(ren->getVtkObject());
    if(!renderer)
        return 0;

    return m_vtkAbstractPicker->Pick(point.x, point.y, point.z, renderer);
}

bool CVtkAbstractPicker::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Prop3D")
    {
        return m_vtkAbstractPicker->GetPickList()->GetNumberOfItems() != 0;
    }

    if(path->pathName() == "Renderer")
    {
        return m_vtkRenderer != 0;
    }

    return false;
}

bool CVtkAbstractPicker::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Prop3D")
    {
        IVtkProp3DIOData* propData = 0;
        bool success = inputData->queryInterface("IVtkProp3DIOData", (void**)&propData);
        if(success && propData)
        {
            m_vtkAbstractPicker->AddPickList(propData->getVtkProp3D());
            return true;
        }

        return false;
    }

    if(path->pathName() == "Renderer")
    {
        IVtkRendererIOData* renData = 0;
        bool success = inputData->queryInterface("IVtkRendererIOData", (void**)&renData);
        if(success && renData)
        {
            m_vtkRenderer = renData->getVtkRenderer();
            return true;
        }

        return false;
    }

    return false;
}

bool CVtkAbstractPicker::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Prop3D")
    {
        IVtkProp3DIOData* propData = 0;
        bool success = inputData->queryInterface("IVtkProp3DIOData", (void**)&propData);
        if(success && propData)
        {
            m_vtkAbstractPicker->DeletePickList(propData->getVtkProp3D());
            return true;
        }

        return false;
    }

    if(path->pathName() == "Renderer")
    {
        IVtkRendererIOData* renData = 0;
        bool success = inputData->queryInterface("IVtkRendererIOData", (void**)&renData);
        if(success && renData)
        {
            m_vtkRenderer = 0;
            return true;
        }

        return false;
    }

    return false;
}

bool CVtkAbstractPicker::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "AbstractPicker")
    {
        m_abstractPickerData.setAbstractPicker(m_vtkAbstractPicker);
        *outputData = &m_abstractPickerData;
        return true;
    }

    return false;
}

bool CVtkAbstractPicker::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    return true;
}


