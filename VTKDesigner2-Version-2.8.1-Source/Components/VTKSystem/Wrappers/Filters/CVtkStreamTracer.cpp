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

#include "CVtkStreamTracer.h"

DEFINE_VTK_OBJECT(CVtkStreamTracer,CVtkPolyDataFilter,vtkStreamTracer)
{
    pDesc->setNodeClassName("vtkStreamTracer");
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

CVtkStreamTracer::CVtkStreamTracer() : m_vtkStreamTracer(0)
{
    CVtkStreamTracer::InitializeObjectDesc();
    setVtkObject(vtkStreamTracer::New());
}

CVtkStreamTracer::~CVtkStreamTracer()
{

}

Point3D CVtkStreamTracer::startPosition() const
{
    return Point3D(m_vtkStreamTracer->GetStartPosition());
}


void CVtkStreamTracer::setStartPosition(Point3D val)
{
    m_vtkStreamTracer->SetStartPosition(val.x, val.y, val.z);
}


CVtkStreamTracer::Solvers CVtkStreamTracer::integratorType() const
{
    switch(m_vtkStreamTracer->GetIntegratorType())
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

void CVtkStreamTracer::setIntegratorType(Solvers val)
{
    switch(val)
    {
        case RK2:
            m_vtkStreamTracer->SetIntegratorType(RUNGE_KUTTA2);
            break;
        case RK4:
            m_vtkStreamTracer->SetIntegratorType(RUNGE_KUTTA4);
            break;
        case RK5:
            m_vtkStreamTracer->SetIntegratorType(RUNGE_KUTTA45);
            break;
        default :
            m_vtkStreamTracer->SetIntegratorType(RUNGE_KUTTA2);
            break;
    }
}


CVtkStreamTracer::Units CVtkStreamTracer::maximumPropagationUnit() const
{
    switch(m_vtkStreamTracer->GetMaximumPropagationUnit())
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

void CVtkStreamTracer::setMaximumPropagationUnit(Units val)
{
    switch(val)
    {
        case Time:
            m_vtkStreamTracer->SetMaximumPropagationUnit(TIME_UNIT);
            break;
        case Length:
            m_vtkStreamTracer->SetMaximumPropagationUnit(LENGTH_UNIT);
            break;
        case CellLength:
            m_vtkStreamTracer->SetMaximumPropagationUnit(CELL_LENGTH_UNIT);
            break;
        default :
            m_vtkStreamTracer->SetMaximumPropagationUnit(TIME_UNIT);
            break;
    }
}


CVtkStreamTracer::Units CVtkStreamTracer::minimumIntegrationStepUnit() const
{
    switch(m_vtkStreamTracer->GetMinimumIntegrationStepUnit())
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

void CVtkStreamTracer::setMinimumIntegrationStepUnit(Units val)
{
    switch(val)
    {
        case Time:
            m_vtkStreamTracer->SetMinimumIntegrationStepUnit(TIME_UNIT);
            break;
        case Length:
            m_vtkStreamTracer->SetMinimumIntegrationStepUnit(LENGTH_UNIT);
            break;
        case CellLength:
            m_vtkStreamTracer->SetMinimumIntegrationStepUnit(CELL_LENGTH_UNIT);
            break;
        default :
            m_vtkStreamTracer->SetMinimumIntegrationStepUnit(TIME_UNIT);
            break;
    }
}


CVtkStreamTracer::Units CVtkStreamTracer::maximumIntegrationStepUnit() const
{
    switch(m_vtkStreamTracer->GetMaximumIntegrationStepUnit())
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

void CVtkStreamTracer::setMaximumIntegrationStepUnit(Units val)
{
    switch(val)
    {
        case Time:
            m_vtkStreamTracer->SetMaximumIntegrationStepUnit(TIME_UNIT);
            break;
        case Length:
            m_vtkStreamTracer->SetMaximumIntegrationStepUnit(LENGTH_UNIT);
            break;
        case CellLength:
            m_vtkStreamTracer->SetMaximumIntegrationStepUnit(CELL_LENGTH_UNIT);
            break;
        default :
            m_vtkStreamTracer->SetMaximumIntegrationStepUnit(TIME_UNIT);
            break;
    }
}


CVtkStreamTracer::Units CVtkStreamTracer::initialIntegrationStepUnit() const
{
    switch(m_vtkStreamTracer->GetInitialIntegrationStepUnit())
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

void CVtkStreamTracer::setInitialIntegrationStepUnit(Units val)
{
    switch(val)
    {
        case Time:
            m_vtkStreamTracer->SetInitialIntegrationStepUnit(TIME_UNIT);
            break;
        case Length:
            m_vtkStreamTracer->SetInitialIntegrationStepUnit(LENGTH_UNIT);
            break;
        case CellLength:
            m_vtkStreamTracer->SetInitialIntegrationStepUnit(CELL_LENGTH_UNIT);
            break;
        default :
            m_vtkStreamTracer->SetInitialIntegrationStepUnit(TIME_UNIT);
            break;
    }
}


double CVtkStreamTracer::maximumError() const
{
    return m_vtkStreamTracer->GetMaximumError();
}


void CVtkStreamTracer::setMaximumError(double val)
{
    m_vtkStreamTracer->SetMaximumError(val);
}


int CVtkStreamTracer::maximumNumberOfSteps() const
{
    return m_vtkStreamTracer->GetMaximumNumberOfSteps();
}


void CVtkStreamTracer::setMaximumNumberOfSteps(int val)
{
    m_vtkStreamTracer->SetMaximumNumberOfSteps(val);
}


double CVtkStreamTracer::terminalSpeed() const
{
    return m_vtkStreamTracer->GetTerminalSpeed();
}


void CVtkStreamTracer::setTerminalSpeed(double val)
{
    m_vtkStreamTracer->SetTerminalSpeed(val);
}


CVtkStreamTracer::Direction CVtkStreamTracer::integrationDirection() const
{
    switch(m_vtkStreamTracer->GetIntegrationDirection())
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

void CVtkStreamTracer::setIntegrationDirection(Direction val)
{
    switch(val)
    {
        case Forward:
            m_vtkStreamTracer->SetIntegrationDirection(FORWARD);
            break;
        case Backward:
            m_vtkStreamTracer->SetIntegrationDirection(BACKWARD);
            break;
        case Both:
            m_vtkStreamTracer->SetIntegrationDirection(BOTH);
            break;
        default :
            m_vtkStreamTracer->SetIntegrationDirection(FORWARD);
            break;
    }
}


bool CVtkStreamTracer::isComputeVorticity() const
{
    return m_vtkStreamTracer->GetComputeVorticity();
}


void CVtkStreamTracer::setComputeVorticity(bool val)
{
    m_vtkStreamTracer->SetComputeVorticity(val);
}


double CVtkStreamTracer::rotationScale() const
{
    return m_vtkStreamTracer->GetRotationScale();
}


void CVtkStreamTracer::setRotationScale(double val)
{
    m_vtkStreamTracer->SetRotationScale(val);
}

