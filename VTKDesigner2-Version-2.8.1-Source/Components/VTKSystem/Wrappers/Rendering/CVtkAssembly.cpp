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

#include "CVtkAssembly.h"
#include "vtkProp3DCollection.h"

DEFINE_VTK_OBJECT(CVtkAssembly, CVtkProp3D, vtkAssembly)
{
    pDesc->setNodeClassCategory("Rendering");
    pDesc->setNodeClassDescription("Assembly of actors");
    pDesc->setNodeClassName("vtkAssembly");

    // Transform Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Prop3D", IVisSystemNodeConnectionPath::InputPath, "vtkProp3D", 0, true)
        );
}

CVtkAssembly::CVtkAssembly() : m_vtkAssembly(0)
{
    CVtkAssembly::InitializeObjectDesc();
    setVtkObject(vtkAssembly::New());
}

CVtkAssembly::~CVtkAssembly()
{

}

bool CVtkAssembly::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!m_vtkAssembly || !path)
        return false;

    if(path->pathName() == "Prop3D")
    {
        return m_vtkAssembly->GetParts()->GetNumberOfItems() != 0;
    }

    return CVtkProp3D::hasInput(path);
}

bool CVtkAssembly::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Prop3D")
    {
        IVtkProp3DIOData* prop3DData = 0;
        bool success = inputData->queryInterface("IVtkProp3DIOData", (void**)&prop3DData);
        if(success && prop3DData)
        {
            m_vtkAssembly->AddPart(prop3DData->getVtkProp3D());
            return true;
        }

        return false;
    }

    return CVtkProp3D::setInput(path, inputData);
}

bool CVtkAssembly::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Prop3D")
    {
        IVtkProp3DIOData* prop3DData = 0;
        bool success = inputData->queryInterface("IVtkProp3DIOData", (void**)&prop3DData);
        if(success && prop3DData && m_vtkAssembly->GetParts()->IsItemPresent(prop3DData->getVtkProp3D()))
        {
            m_vtkAssembly->RemovePart(prop3DData->getVtkProp3D());
            return true;
        }

        return false;
    }

    return CVtkProp3D::removeInput(path, inputData);
}
