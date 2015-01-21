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
#include "CVtkDepthSortPolyData.h"
#include "IVtkProp3DIOData.h"
#include "IVtkCameraIOData.h"

DEFINE_VTK_OBJECT(CVtkDepthSortPolyData,CVtkPolyDataFilter,vtkDepthSortPolyData)
{
    pDesc->setNodeClassName("vtkDepthSortPolyData");
    pDesc->setNodeClassDescription("Sort poly data along camera view direction");

    // Prop3D Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Prop3D", IVisSystemNodeConnectionPath::InputPath, "vtkProp3D", 0, true)
        );

    // Camera Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Camera", IVisSystemNodeConnectionPath::InputPath, "vtkCamera", 0)
        );

    pDesc->setPropertyInfo("Direction", "SetDirection", "GetDirection", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("DepthSortMode", "SetDepthSortMode", "GetDepthSortMode", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Camera", "SetCamera", "GetCamera", QStringList() << "vtkCamera*", QStringList(), "void", "vtkCamera*");
    pDesc->setPropertyInfo("Prop3D", "SetProp3D", "GetProp3D", QStringList() << "vtkProp3D*", QStringList(), "void", "vtkProp3D*");
    pDesc->setPropertyInfo("Vector", "SetVector", "GetVector", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Origin", "SetOrigin", "GetOrigin", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("SortScalars", "SetSortScalars", "GetSortScalars", QStringList() << "int" , QStringList(), "void", "int");
}

CVtkDepthSortPolyData::CVtkDepthSortPolyData() : m_vtkDepthSortPolyData(0)
{
    CVtkDepthSortPolyData::InitializeObjectDesc();
    setVtkObject(vtkDepthSortPolyData::New());
}

CVtkDepthSortPolyData::~CVtkDepthSortPolyData()
{

}

CVtkDepthSortPolyData::Direction CVtkDepthSortPolyData::direction() const
{
    switch(m_vtkDepthSortPolyData->GetDirection())
    {
        case VTK_DIRECTION_FRONT_TO_BACK:return (FrontToBack);
        case VTK_DIRECTION_SPECIFIED_VECTOR:return (SpecifiedVector);
        default :return (FrontToBack);
    }
}

void CVtkDepthSortPolyData::setDirection(Direction val)
{
    switch(val)
    {
        case FrontToBack:
            m_vtkDepthSortPolyData->SetDirection(VTK_DIRECTION_FRONT_TO_BACK);
            break;
        case SpecifiedVector:
            m_vtkDepthSortPolyData->SetDirection(VTK_DIRECTION_SPECIFIED_VECTOR);
            break;
        default :
            m_vtkDepthSortPolyData->SetDirection(VTK_DIRECTION_FRONT_TO_BACK);
            break;
     }
}


CVtkDepthSortPolyData::DepthSortMode CVtkDepthSortPolyData::depthSortMode() const
{
    switch(m_vtkDepthSortPolyData->GetDepthSortMode())
    {
        case VTK_SORT_FIRST_POINT:return (FirstPoint);
        case VTK_SORT_BOUNDS_CENTER:return (BoundsCenter);
        case VTK_SORT_PARAMETRIC_CENTER:return (ParametricCenter);
        default :return (FirstPoint);
    }
}

void CVtkDepthSortPolyData::setDepthSortMode(DepthSortMode val)
{
    switch(val)
    {
        case FirstPoint:
            m_vtkDepthSortPolyData->SetDepthSortMode(VTK_SORT_FIRST_POINT);
            break;
        case BoundsCenter:
            m_vtkDepthSortPolyData->SetDepthSortMode(VTK_SORT_BOUNDS_CENTER);
            break;
        case ParametricCenter:
            m_vtkDepthSortPolyData->SetDepthSortMode(VTK_SORT_PARAMETRIC_CENTER);
            break;
        default :
            m_vtkDepthSortPolyData->SetDepthSortMode(VTK_SORT_FIRST_POINT);
            break;
     }
}


Point3D CVtkDepthSortPolyData::vector() const
{
    return Point3D(m_vtkDepthSortPolyData->GetVector());
}


void CVtkDepthSortPolyData::setVector(Point3D val)
{
    m_vtkDepthSortPolyData->SetVector(val.x, val.y, val.z);
}


Point3D CVtkDepthSortPolyData::origin() const
{
    return Point3D(m_vtkDepthSortPolyData->GetOrigin());
}


void CVtkDepthSortPolyData::setOrigin(Point3D val)
{
    m_vtkDepthSortPolyData->SetOrigin(val.x, val.y, val.z);
}


bool CVtkDepthSortPolyData::isSortScalars() const
{
    return m_vtkDepthSortPolyData->GetSortScalars();
}


void CVtkDepthSortPolyData::setSortScalars(bool val)
{
    m_vtkDepthSortPolyData->SetSortScalars(val);
}

bool CVtkDepthSortPolyData::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Prop3D")
    {
        return m_vtkDepthSortPolyData->GetProp3D() != 0;
    }

    if(path->pathName() == "Camera")
    {
        return m_vtkDepthSortPolyData->GetCamera() != 0;
    }

    return CVtkPolyDataFilter::hasInput(path);
}

bool CVtkDepthSortPolyData::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData || !m_vtkDepthSortPolyData)
        return false;

    if(path->pathName() == "Prop3D")
    {
        IVtkProp3DIOData* prop3DData = 0;
        bool success = inputData->queryInterface("IVtkProp3DIOData", (void**)&prop3DData);
        if(success && prop3DData)
        {
            m_vtkDepthSortPolyData->SetProp3D(prop3DData->getVtkProp3D());
            return true;
        }
        return false;
    }

    if(path->pathName() == "Camera")
    {
        IVtkCameraIOData* cameraData = 0;
        bool success = inputData->queryInterface("IVtkCameraIOData", (void**)&cameraData);
        if(success && cameraData)
        {
            m_vtkDepthSortPolyData->SetCamera(cameraData->getVtkCamera());
            return true;
        }
        return false;
    }

    return CVtkPolyDataFilter::setInput(path, inputData);
}

bool CVtkDepthSortPolyData::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Prop3D")
    {
        IVtkProp3DIOData* prop3DData = 0;
        bool success = inputData->queryInterface("IVtkProp3DIOData", (void**)&prop3DData);
        if(success && prop3DData && m_vtkDepthSortPolyData->GetProp3D() == prop3DData->getVtkProp3D())
        {
            m_vtkDepthSortPolyData->SetProp3D(0);
        }
        return false;
    }

    if(path->pathName() == "Camera")
    {
        IVtkCameraIOData* cameraData = 0;
        bool success = inputData->queryInterface("IVtkCameraIOData", (void**)&cameraData);
        if(success && cameraData && cameraData->getVtkCamera() == m_vtkDepthSortPolyData->GetCamera())
        {
            m_vtkDepthSortPolyData->SetCamera(0);
            return true;
        }
        return false;
    }

    return CVtkPolyDataFilter::removeInput(path, inputData);
}
