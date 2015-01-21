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

#include "CVtkInteractorObserver.h"
#include "IVtkRenderWindowInteractorIOData.h"

DEFINE_VTK_OBJECT(CVtkInteractorObserver, CVtkObject, vtkInteractorObserver)
{
    pDesc->setNodeClassName("vtkInteractorObserver");
    pDesc->setNodeClassCategory("Interactor Observers");
    pDesc->setNodeClassDescription("Base class for all interactor observers");

    pDesc->setCreateFunction(0);
    
    pDesc->setPropertyInfo("Priority", "SetPriority", "GetPriority", QStringList() << "float", QStringList(), "void", "float");
    pDesc->setPropertyInfo("KeyPressActivation", "SetKeyPressActivation", "GetKeyPressActivation", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("KeyPressActivationValue", "SetKeyPressActivationValue", "GetKeyPressActivationValue", QStringList() << "char", QStringList(), "void", "char");

    // RenderWindowInteractor input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("RenderWindowInteractor", IVisSystemNodeConnectionPath::InputPath, "vtkRenderWindowInteractor", 0)
        );

    // InteractorObserver output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("InteractorObserver", IVisSystemNodeConnectionPath::OutputPath, "vtkInteractorObserver", 0)
        );

}

CVtkInteractorObserver::CVtkInteractorObserver()
{
    m_vtkInteractorObserver = 0;
    m_enabled = true;
}

CVtkInteractorObserver::~CVtkInteractorObserver()
{

}

void CVtkInteractorObserver::setEnabled(bool val)
{
    if( m_vtkInteractorObserver->GetInteractor() )
        m_vtkInteractorObserver->SetEnabled(val);

    m_enabled = val;
}

bool CVtkInteractorObserver::isEnabled() const
{
    return m_enabled;
}

void CVtkInteractorObserver::setPriority(double val)
{
    m_vtkInteractorObserver->SetPriority( float(val) );
}

double CVtkInteractorObserver::priority() const
{
    return double( m_vtkInteractorObserver->GetPriority() );
}

void CVtkInteractorObserver::setKeyPressActivation(bool val)
{
    m_vtkInteractorObserver->SetKeyPressActivation(val);
}

bool CVtkInteractorObserver::isKeyPressActivation() const
{
    return m_vtkInteractorObserver->GetKeyPressActivation();
}

void CVtkInteractorObserver::setKeyPressActivationValue(QChar ch)
{
    m_vtkInteractorObserver->SetKeyPressActivationValue(ch.toAscii());
}

QChar CVtkInteractorObserver::keyPressActivationValue() const
{
    return QChar(m_vtkInteractorObserver->GetKeyPressActivationValue());
}

bool CVtkInteractorObserver::hasInput(IVisSystemNodeConnectionPath* path)
{
    if( !path )
        return false;

    if( path->pathName() == "RenderWindowInteractor" )
        return m_vtkInteractorObserver->GetInteractor() != 0;

    return CVtkObject::hasInput(path);
}

bool CVtkInteractorObserver::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if( !path || !inputData )
        return false;

    if( path->pathName() == "RenderWindowInteractor" )
    {
        IVtkRenderWindowInteractorIOData* data = 0;
        bool success = inputData->queryInterface("IVtkRenderWindowInteractorIOData", (void**)&data);
        if( success && data )
        {
            vtkRenderWindowInteractor* rWinInt = data->getVtkRenderWindowInteractor();
            m_vtkInteractorObserver->SetInteractor(rWinInt);
            if(m_enabled)
                m_vtkInteractorObserver->SetEnabled(true);
            return true;
        }

        return false;
    }

    return CVtkObject::setInput(path, inputData);
}

bool CVtkInteractorObserver::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if( !path || !inputData )
        return false;

    if( path->pathName() == "RenderWindowInteractor" )
    {
        IVtkRenderWindowInteractorIOData* data = 0;
        bool success = inputData->queryInterface("IVtkRenderWindowInteractorIOData", (void**)&data);
        if( success && data )
        {
            m_vtkInteractorObserver->SetEnabled(false);
            m_vtkInteractorObserver->SetInteractor(0);
            return true;
        }

        return false;
    }

    return CVtkObject::removeInput(path, inputData);
}

bool CVtkInteractorObserver::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if( !path || !outputData )
        return false;

    if( path->pathName() == "InteractorObserver" )
    {
        m_outputData.setInteractorObserver( m_vtkInteractorObserver );
        *outputData = &m_outputData;
        return true;
    }

    return CVtkObject::fetchOutput(path, outputData);
}

bool CVtkInteractorObserver::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if( !path || !outputData )
        return false;

    if( path->pathName() == "InteractorObserver" )
        return true;

    return CVtkObject::outputDerefed(path, outputData);
}


