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

#include "CVtkGenericStreamTracer.h"

DEFINE_VTK_OBJECT(CVtkGenericStreamTracer,CVtkPolyDataFilter,vtkGenericStreamTracer)
{
    pDesc->setNodeClassName("vtkGenericStreamTracer");
    pDesc->setNodeClassDescription("Streamline generator");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("StartPosition", "SetStartPosition", "GetStartPosition", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("IntegratorType", "SetIntegratorType", "GetIntegratorType", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("MaximumPropagation", "SetMaximumPropagationUnit", "GetMaximumPropagationUnit", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("MinimumIntegrationStep", "SetMinimumIntegrationStepUnit", "GetMinimumIntegrationStepUnit", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("MaximumIntegrationStep", "SetMaximumIntegrationStepUnit", "GetMaximumIntegrationStepUnit", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("InitialIntegrationStep", "SetInitialIntegrationStepUnit", "GetInitialIntegrationStepUnit", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("MaximumError", "SetMaximumError", "GetMaximumError", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("MaximumNumberOfSteps", "SetMaximumNumberOfSteps", "GetMaximumNumberOfSteps", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("TerminalSpeed", "SetTerminalSpeed", "GetTerminalSpeed", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("IntegrationDirection", "SetIntegrationDirection", "GetIntegrationDirection", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ComputeVorticity", "SetComputeVorticity", "GetComputeVorticity", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("RotationScale", "SetRotationScale", "GetRotationScale", QStringList() << "double", QStringList(), "void", "double");
}

CVtkGenericStreamTracer::CVtkGenericStreamTracer() : m_vtkGenericStreamTracer(0)
{
    CVtkGenericStreamTracer::InitializeObjectDesc();
    setVtkObject(vtkGenericStreamTracer::New());
}

CVtkGenericStreamTracer::~CVtkGenericStreamTracer()
{

}

Point3D CVtkGenericStreamTracer::startPosition() const
{
    return Point3D(m_vtkGenericStreamTracer->GetStartPosition());
}


void CVtkGenericStreamTracer::setStartPosition(Point3D val)
{
    m_vtkGenericStreamTracer->SetStartPosition(val.x, val.y, val.z);
}


CVtkGenericStreamTracer::Solvers CVtkGenericStreamTracer::integratorType() const
{
    switch(m_vtkGenericStreamTracer->GetIntegratorType())
    {
        case RUNGE_KUTTA2:
            return (RK2);
        case RUNGE_KUTTA4:
            return (RK4);
        case RUNGE_KUTTA45:
            return (RK5);
        default :
            return (RK2);
    }
}

void CVtkGenericStreamTracer::setIntegratorType(Solvers val)
{
    switch(val)
    {
        case RK2:
            m_vtkGenericStreamTracer->SetIntegratorType(RUNGE_KUTTA2);
            break;
        case RK4:
            m_vtkGenericStreamTracer->SetIntegratorType(RUNGE_KUTTA4);
            break;
        case RK5:
            m_vtkGenericStreamTracer->SetIntegratorType(RUNGE_KUTTA45);
            break;
        default :
            m_vtkGenericStreamTracer->SetIntegratorType(RUNGE_KUTTA2);
            break;
    }
}


CVtkGenericStreamTracer::Units CVtkGenericStreamTracer::maximumPropagationUnit() const
{
    switch(m_vtkGenericStreamTracer->GetMaximumPropagationUnit())
    {
        case TIME_UNIT:
            return (Time);
        case LENGTH_UNIT:
            return (Length);
        case CELL_LENGTH_UNIT:
            return (CellLength);
        default :
            return (Time);
    }
}

void CVtkGenericStreamTracer::setMaximumPropagationUnit(Units val)
{
    switch(val)
    {
        case Time:
            m_vtkGenericStreamTracer->SetMaximumPropagationUnit(TIME_UNIT);
            break;
        case Length:
            m_vtkGenericStreamTracer->SetMaximumPropagationUnit(LENGTH_UNIT);
            break;
        case CellLength:
            m_vtkGenericStreamTracer->SetMaximumPropagationUnit(CELL_LENGTH_UNIT);
            break;
        default :
            m_vtkGenericStreamTracer->SetMaximumPropagationUnit(TIME_UNIT);
            break;
    }
}


CVtkGenericStreamTracer::Units CVtkGenericStreamTracer::minimumIntegrationStepUnit() const
{
    switch(m_vtkGenericStreamTracer->GetMinimumIntegrationStepUnit())
    {
        case TIME_UNIT:
            return (Time);
        case LENGTH_UNIT:
            return (Length);
        case CELL_LENGTH_UNIT:
            return (CellLength);
        default :
            return (Time);
    }
}

void CVtkGenericStreamTracer::setMinimumIntegrationStepUnit(Units val)
{
    switch(val)
    {
        case Time:
            m_vtkGenericStreamTracer->SetMinimumIntegrationStepUnit(TIME_UNIT);
            break;
        case Length:
            m_vtkGenericStreamTracer->SetMinimumIntegrationStepUnit(LENGTH_UNIT);
            break;
        case CellLength:
            m_vtkGenericStreamTracer->SetMinimumIntegrationStepUnit(CELL_LENGTH_UNIT);
            break;
        default :
            m_vtkGenericStreamTracer->SetMinimumIntegrationStepUnit(TIME_UNIT);
            break;
    }
}


CVtkGenericStreamTracer::Units CVtkGenericStreamTracer::maximumIntegrationStepUnit() const
{
    switch(m_vtkGenericStreamTracer->GetMaximumIntegrationStepUnit())
    {
        case TIME_UNIT:
            return (Time);
        case LENGTH_UNIT:
            return (Length);
        case CELL_LENGTH_UNIT:
            return (CellLength);
        default :
            return (Time);
    }
}

void CVtkGenericStreamTracer::setMaximumIntegrationStepUnit(Units val)
{
    switch(val)
    {
        case Time:
            m_vtkGenericStreamTracer->SetMaximumIntegrationStepUnit(TIME_UNIT);
            break;
        case Length:
            m_vtkGenericStreamTracer->SetMaximumIntegrationStepUnit(LENGTH_UNIT);
            break;
        case CellLength:
            m_vtkGenericStreamTracer->SetMaximumIntegrationStepUnit(CELL_LENGTH_UNIT);
            break;
        default :
            m_vtkGenericStreamTracer->SetMaximumIntegrationStepUnit(TIME_UNIT);
            break;
    }
}


CVtkGenericStreamTracer::Units CVtkGenericStreamTracer::initialIntegrationStepUnit() const
{
    switch(m_vtkGenericStreamTracer->GetInitialIntegrationStepUnit())
    {
        case TIME_UNIT:
            return (Time);
        case LENGTH_UNIT:
            return (Length);
        case CELL_LENGTH_UNIT:
            return (CellLength);
        default :
            return (Time);
    }
}

void CVtkGenericStreamTracer::setInitialIntegrationStepUnit(Units val)
{
    switch(val)
    {
        case Time:
            m_vtkGenericStreamTracer->SetInitialIntegrationStepUnit(TIME_UNIT);
            break;
        case Length:
            m_vtkGenericStreamTracer->SetInitialIntegrationStepUnit(LENGTH_UNIT);
            break;
        case CellLength:
            m_vtkGenericStreamTracer->SetInitialIntegrationStepUnit(CELL_LENGTH_UNIT);
            break;
        default :
            m_vtkGenericStreamTracer->SetInitialIntegrationStepUnit(TIME_UNIT);
            break;
    }
}


double CVtkGenericStreamTracer::maximumError() const
{
    return m_vtkGenericStreamTracer->GetMaximumError();
}


void CVtkGenericStreamTracer::setMaximumError(double val)
{
    m_vtkGenericStreamTracer->SetMaximumError(val);
}


int CVtkGenericStreamTracer::maximumNumberOfSteps() const
{
    return m_vtkGenericStreamTracer->GetMaximumNumberOfSteps();
}


void CVtkGenericStreamTracer::setMaximumNumberOfSteps(int val)
{
    m_vtkGenericStreamTracer->SetMaximumNumberOfSteps(val);
}


double CVtkGenericStreamTracer::terminalSpeed() const
{
    return m_vtkGenericStreamTracer->GetTerminalSpeed();
}


void CVtkGenericStreamTracer::setTerminalSpeed(double val)
{
    m_vtkGenericStreamTracer->SetTerminalSpeed(val);
}


CVtkGenericStreamTracer::Direction CVtkGenericStreamTracer::integrationDirection() const
{
    switch(m_vtkGenericStreamTracer->GetIntegrationDirection())
    {
        case FORWARD:
            return (Forward);
        case BACKWARD:
            return (Backward);
        case BOTH:
            return (Both);
        default :
            return (Forward);
    }
}

void CVtkGenericStreamTracer::setIntegrationDirection(Direction val)
{
    switch(val)
    {
        case Forward:
            m_vtkGenericStreamTracer->SetIntegrationDirection(FORWARD);
            break;
        case Backward:
            m_vtkGenericStreamTracer->SetIntegrationDirection(BACKWARD);
            break;
        case Both:
            m_vtkGenericStreamTracer->SetIntegrationDirection(BOTH);
            break;
        default :
            m_vtkGenericStreamTracer->SetIntegrationDirection(FORWARD);
            break;
    }
}


bool CVtkGenericStreamTracer::isComputeVorticity() const
{
    return m_vtkGenericStreamTracer->GetComputeVorticity();
}


void CVtkGenericStreamTracer::setComputeVorticity(bool val)
{
    m_vtkGenericStreamTracer->SetComputeVorticity(val);
}


double CVtkGenericStreamTracer::rotationScale() const
{
    return m_vtkGenericStreamTracer->GetRotationScale();
}


void CVtkGenericStreamTracer::setRotationScale(double val)
{
    m_vtkGenericStreamTracer->SetRotationScale(val);
}

