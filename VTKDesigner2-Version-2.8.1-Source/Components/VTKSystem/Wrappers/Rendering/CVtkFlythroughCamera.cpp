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

#include "CVtkFlythroughCamera.h"
#include "IVtkPolyDataIOData.h"

DEFINE_VTK_OBJECT(CVtkFlythroughCamera, CVtkCamera, vtkFlythroughCamera)
{
    pDesc->setNodeClassName("vtkFlythroughCamera");
    pDesc->setNodeClassDescription("A flythrough camera");

    // Flythrough input path
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("FlythroughPath", IVisSystemNodeConnectionPath::InputPath, "vtkFlythroughCamera", 0)
        );

    // Properties
    pDesc->setPropertyInfo("InitialPosition","SetInitialPosition","GetInitialPosition", QStringList()<<"double"<<"double"<<"double",
                        QStringList(), "void","double*");
    pDesc->setPropertyInfo("InitialFocalPoint","SetInitialFocalPoint","GetInitialFocalPoint", QStringList()<<"double"<<"double"<<"double",
                        QStringList(), "void","double*");
    pDesc->setPropertyInfo("InitialViewUp","SetInitialViewUp","GetInitialViewUp", QStringList()<<"double"<<"double"<<"double",
                        QStringList(), "void","double*");

	pDesc->setNodeIcon(qApp->windowIcon()); // vtkCustomFunction is not a standard VTK class
};

CVtkFlythroughCamera::CVtkFlythroughCamera() : CVtkCamera(false), m_vtkFlythroughCamera(0)
{
    CVtkFlythroughCamera::InitializeObjectDesc();
    setVtkObject(vtkFlythroughCamera::New());
}

CVtkFlythroughCamera::~CVtkFlythroughCamera()
{
    
}

void CVtkFlythroughCamera::setInitialPosition(Point3D point)
{
    m_vtkFlythroughCamera->SetInitialPosition(point.x, point.y, point.z);
}

Point3D CVtkFlythroughCamera::initialPosition() const
{
    return m_vtkFlythroughCamera->GetInitialPosition();
}

void CVtkFlythroughCamera::setInitialFocalPoint(Point3D point)
{
    m_vtkFlythroughCamera->SetInitialFocalPoint(point.x, point.y, point.z);
}

Point3D CVtkFlythroughCamera::initialFocalPoint() const
{
    return m_vtkFlythroughCamera->GetInitialFocalPoint();
}

void CVtkFlythroughCamera::setInitialViewUp(Vector3D point)
{
    m_vtkFlythroughCamera->SetInitialViewUp(point.x, point.y, point.z);
}

Vector3D CVtkFlythroughCamera::initialViewUp() const
{
    return m_vtkFlythroughCamera->GetInitialViewUp();
}

void CVtkFlythroughCamera::moveStart()
{
    m_vtkFlythroughCamera->MoveStart();
}

void CVtkFlythroughCamera::moveForward()
{
    m_vtkFlythroughCamera->MoveForward();
}

void CVtkFlythroughCamera::moveBackward()
{
    m_vtkFlythroughCamera->MoveBackward();
}

void CVtkFlythroughCamera::moveForwardSwapAround()
{
    m_vtkFlythroughCamera->MoveForwardSwapAround();
}

bool CVtkFlythroughCamera::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "FlythroughPath")
    {
        return m_vtkFlythroughCamera->GetFlythroughPath();
    }

    return CVtkCamera::hasInput(path);
}

bool CVtkFlythroughCamera::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "FlythroughPath")
    {
        IVtkPolyDataIOData* data = 0;
        bool success = inputData->queryInterface("IVtkPolyDataIOData", (void**)&data);
        if(success)
        {
            vtkPolyData* polyData = data->getVtkPolyData();
            m_vtkFlythroughCamera->SetFlythroughPath(polyData);
            return true;
        }
    }

    return CVtkCamera::setInput(path, inputData);
}

bool CVtkFlythroughCamera::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "FlythroughPath")
    {
        IVtkPolyDataIOData* data = 0;
        bool success = inputData->queryInterface("IVtkPolyDataIOData", (void**)&data);
        if(success && data->getVtkPolyData() == m_vtkFlythroughCamera->GetFlythroughPath())
        {
            m_vtkFlythroughCamera->SetFlythroughPath(0);
            return true;
        }
    }

    return CVtkCamera::removeInput(path, inputData);
}


