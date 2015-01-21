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

#include "CVtkImplicitBoolean.h"
#include "vtkImplicitFunctionCollection.h"
#include "IVtkImplicitFunctionIOData.h"

DEFINE_VTK_OBJECT(CVtkImplicitBoolean, CVtkImplicitFunction, vtkImplicitBoolean)
{
    pDesc->setNodeClassName("vtkImplicitBoolean");
    pDesc->setNodeClassDescription("Performs a boolean operation on one or more functions");

    pDesc->setPropertyInfo(
            "Operation", "SetOperationType", "GetOperationType", QStringList()<<"int", QStringList(), "void", "int"
        );

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ImplicitFunctionInput", IVisSystemNodeConnectionPath::InputPath,  "vtkImplicitFunction", 0, true)
    );

}

CVtkImplicitBoolean::CVtkImplicitBoolean() : m_vtkImplicitBoolean(0)
{
    CVtkImplicitBoolean::InitializeObjectDesc();
    setVtkObject(vtkImplicitBoolean::New());
}

CVtkImplicitBoolean::~CVtkImplicitBoolean()
{

}

void CVtkImplicitBoolean::setOperation(CVtkImplicitBoolean::Operation oper)
{
    m_vtkImplicitBoolean->SetOperationType(int(oper));
}

CVtkImplicitBoolean::Operation CVtkImplicitBoolean::operation() const
{
    return CVtkImplicitBoolean::Operation(m_vtkImplicitBoolean->GetOperationType());
}

bool CVtkImplicitBoolean::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "ImplicitFunctionInput")
    {
        return m_vtkImplicitBoolean->GetFunction() && m_vtkImplicitBoolean->GetFunction()->GetNumberOfItems();
    }

    return CVtkImplicitFunction::hasInput(path);
}

bool CVtkImplicitBoolean::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImplicitFunctionInput")
    {
        IVtkImplicitFunctionIOData* data = 0;
        bool success = inputData->queryInterface("IVtkImplicitFunctionIOData", (void**)&data);
        if(success && data->getVtkImplicitFunction())
        {
            m_vtkImplicitBoolean->AddFunction(data->getVtkImplicitFunction());
            return true;
        }
    
        return false;
    }

    return CVtkImplicitFunction::setInput(path, inputData);
}

bool CVtkImplicitBoolean::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImplicitFunctionInput")
    {
        IVtkImplicitFunctionIOData* data = 0;
        bool success = inputData->queryInterface("IVtkImplicitFunctionIOData", (void**)&data);
        if(success && data->getVtkImplicitFunction())
        {
            m_vtkImplicitBoolean->RemoveFunction(data->getVtkImplicitFunction());
            return true;
        }
    
        return false;
    }

    return CVtkImplicitFunction::removeInput(path, inputData);
}


