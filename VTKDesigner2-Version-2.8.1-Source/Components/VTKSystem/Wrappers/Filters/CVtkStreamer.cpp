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

#include "CVtkStreamer.h"
#include "IVtkDataSetIOData.h"
#include "vtkDataSet.h"
DEFINE_VTK_OBJECT(CVtkStreamer,CVtkPolyDataFilter,vtkStreamer)
{
    pDesc->setNodeClassName("vtkStreamer");
    pDesc->setNodeClassDescription("Abstract object implements integration of massless particle through vector field");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("NumberOfThreads", "SetNumberOfThreads", "GetNumberOfThreads", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("StartPosition", "SetStartPosition", "GetStartPosition", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("MaximumPropagationTime", "SetMaximumPropagationTime", "GetMaximumPropagationTime", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("IntegrationDirection", "SetIntegrationDirection", "GetIntegrationDirection", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("IntegrationStepLength", "SetIntegrationStepLength", "GetIntegrationStepLength", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("SpeedScalars", "SetSpeedScalars", "GetSpeedScalars", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("OrientationScalars", "SetOrientationScalars", "GetOrientationScalars", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("TerminalSpeed", "SetTerminalSpeed", "GetTerminalSpeed", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Vorticity", "SetVorticity", "GetVorticity", QStringList() << "int" , QStringList(), "void", "int");

    // Source input path
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Source", IVisSystemNodeConnectionPath::InputPath, "vtkDataSet", 0)
        );
}

CVtkStreamer::CVtkStreamer() : m_vtkStreamer(0)
{
    CVtkStreamer::InitializeObjectDesc();
}

CVtkStreamer::~CVtkStreamer()
{

}

int CVtkStreamer::numberOfThreads() const
{
    return m_vtkStreamer->GetNumberOfThreads();
}


void CVtkStreamer::setNumberOfThreads(int val)
{
    m_vtkStreamer->SetNumberOfThreads(val);
}


Point3D CVtkStreamer::startPosition() const
{
    return Point3D(m_vtkStreamer->GetStartPosition());
}


void CVtkStreamer::setStartPosition(Point3D val)
{
    m_vtkStreamer->SetStartPosition(val.x, val.y, val.z);
}


double CVtkStreamer::maximumPropagationTime() const
{
    return m_vtkStreamer->GetMaximumPropagationTime();
}


void CVtkStreamer::setMaximumPropagationTime(double val)
{
    m_vtkStreamer->SetMaximumPropagationTime(val);
}


CVtkStreamer::Direction CVtkStreamer::integrationDirection() const
{
    switch(m_vtkStreamer->GetIntegrationDirection())
    {
        case VTK_INTEGRATE_FORWARD:return (Forward);
        case VTK_INTEGRATE_BACKWARD:return (Backword);
        case VTK_INTEGRATE_BOTH_DIRECTIONS:return (BothDirections);
        default :return (Forward);
    }
}

void CVtkStreamer::setIntegrationDirection(Direction val)
{
    switch(val)
    {
        case Forward:
            m_vtkStreamer->SetIntegrationDirection(VTK_INTEGRATE_FORWARD);
            break;
        case Backword:
            m_vtkStreamer->SetIntegrationDirection(VTK_INTEGRATE_BACKWARD);
            break;
        case BothDirections:
            m_vtkStreamer->SetIntegrationDirection(VTK_INTEGRATE_BOTH_DIRECTIONS);
            break;
        default :
            m_vtkStreamer->SetIntegrationDirection(VTK_INTEGRATE_FORWARD);
            break;
    }
}


double CVtkStreamer::integrationStepLength() const
{
    return m_vtkStreamer->GetIntegrationStepLength();
}


void CVtkStreamer::setIntegrationStepLength(double val)
{
    m_vtkStreamer->SetIntegrationStepLength(val);
}


bool CVtkStreamer::isSpeedScalars() const
{
    return m_vtkStreamer->GetSpeedScalars();
}


void CVtkStreamer::setSpeedScalars(bool val)
{
    m_vtkStreamer->SetSpeedScalars(val);
}


bool CVtkStreamer::isOrientationScalars() const
{
    return m_vtkStreamer->GetOrientationScalars();
}


void CVtkStreamer::setOrientationScalars(bool val)
{
    m_vtkStreamer->SetOrientationScalars(val);
}


double CVtkStreamer::terminalSpeed() const
{
    return m_vtkStreamer->GetTerminalSpeed();
}


void CVtkStreamer::setTerminalSpeed(double val)
{
    m_vtkStreamer->SetTerminalSpeed(val);
}


bool CVtkStreamer::isVorticity() const
{
    return m_vtkStreamer->GetVorticity();
}


void CVtkStreamer::setVorticity(bool val)
{
    m_vtkStreamer->SetVorticity(val);
}

bool CVtkStreamer::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Source")
            return m_vtkStreamer->GetSource();
    
    return CVtkPolyDataFilter::hasInput(path);
}

bool CVtkStreamer::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Source")
    {
        IVtkDataSetIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataSetIOData", (void**)&data);
        if(success)
        {
            vtkDataSet* ds = data->getVtkDataSet();
            m_vtkStreamer->SetSource(ds);
            return true;
        }

        return false;
    }

    return CVtkPolyDataFilter::setInput(path, inputData);
}

bool CVtkStreamer::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
    return false;

    if(path->pathName() == "Source")
    {
        m_vtkStreamer->SetSource(0);
        return true;
    }

    return CVtkPolyDataFilter::removeInput(path, inputData);
}

