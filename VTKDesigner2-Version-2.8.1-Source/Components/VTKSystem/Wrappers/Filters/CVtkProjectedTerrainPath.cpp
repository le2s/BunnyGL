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
#include "CVtkProjectedTerrainPath.h"
#include "IVtkImageDataIOData.h"

DEFINE_VTK_OBJECT(CVtkProjectedTerrainPath,CVtkPolyDataFilter,vtkProjectedTerrainPath)
{
    pDesc->setNodeClassName("vtkProjectedTerrainPath");
    pDesc->setNodeClassDescription("Project a polyline onto a terrain");

    // ImageData Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ImageData", IVisSystemNodeConnectionPath::InputPath, "vtkImageData", 0)
        );

    pDesc->setPropertyInfo("Source", "SetSource", "GetSource", QStringList() << "vtkImageData*", QStringList(), "void", "vtkImageData*");
    pDesc->setPropertyInfo("ProjectionMode", "SetProjectionMode", "GetProjectionMode", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("HeightOffset", "SetHeightOffset", "GetHeightOffset", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("HeightTolerance", "SetHeightTolerance", "GetHeightTolerance", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("MaximumNumberOfLines", "SetMaximumNumberOfLines", "GetMaximumNumberOfLines", QStringList() << "vtkIdType", QStringList(), "void", "vtkIdType");
}

CVtkProjectedTerrainPath::CVtkProjectedTerrainPath() : m_vtkProjectedTerrainPath(0)
{
    CVtkProjectedTerrainPath::InitializeObjectDesc();
    setVtkObject(vtkProjectedTerrainPath::New());
}

CVtkProjectedTerrainPath::~CVtkProjectedTerrainPath()
{

}

vtkImageData* CVtkProjectedTerrainPath::source() const
{
    return m_vtkProjectedTerrainPath->GetSource();
}


void CVtkProjectedTerrainPath::setSource(vtkImageData* val)
{
    m_vtkProjectedTerrainPath->SetSource(val);
}


CVtkProjectedTerrainPath::ProjectionMode CVtkProjectedTerrainPath::projectionMode() const
{
    switch(m_vtkProjectedTerrainPath->GetProjectionMode())
    {
        case SIMPLE_PROJECTION:return (Simple);
        case NONOCCLUDED_PROJECTION:return (NonOccluded);
        case HUG_PROJECTION:return (Hug);
        default :return (Simple);
    }
}

void CVtkProjectedTerrainPath::setProjectionMode(ProjectionMode val)
{
    switch(val)
    {
        case Simple:
            m_vtkProjectedTerrainPath->SetProjectionMode(SIMPLE_PROJECTION);
            break;
        case NonOccluded:
            m_vtkProjectedTerrainPath->SetProjectionMode(NONOCCLUDED_PROJECTION);
            break;
        case Hug:
            m_vtkProjectedTerrainPath->SetProjectionMode(HUG_PROJECTION);
            break;
        default :
            m_vtkProjectedTerrainPath->SetProjectionMode(SIMPLE_PROJECTION);
            break;
     }
}


double CVtkProjectedTerrainPath::heightOffset() const
{
    return m_vtkProjectedTerrainPath->GetHeightOffset();
}


void CVtkProjectedTerrainPath::setHeightOffset(double val)
{
    m_vtkProjectedTerrainPath->SetHeightOffset(val);
}


double CVtkProjectedTerrainPath::heightTolerance() const
{
    return m_vtkProjectedTerrainPath->GetHeightTolerance();
}


void CVtkProjectedTerrainPath::setHeightTolerance(double val)
{
    m_vtkProjectedTerrainPath->SetHeightTolerance(val);
}


int CVtkProjectedTerrainPath::maximumNumberOfLines() const
{
    return m_vtkProjectedTerrainPath->GetMaximumNumberOfLines();
}


void CVtkProjectedTerrainPath::setMaximumNumberOfLines(int val)
{
    m_vtkProjectedTerrainPath->SetMaximumNumberOfLines(vtkIdType(val));
}

bool CVtkProjectedTerrainPath::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path || !m_vtkProjectedTerrainPath)
        return false;

    if(path->pathName() == "ImageData")
    {
        return m_vtkProjectedTerrainPath->GetSource() != 0;
    }

    return CVtkPolyDataFilter::hasInput(path);
}

bool CVtkProjectedTerrainPath::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData || !m_vtkProjectedTerrainPath)
        return false;

    if(path->pathName() == "ImageData")
    {
        IVtkImageDataIOData* imageDataData = 0;
        bool success = inputData->queryInterface("IVtkImageDataIOData", (void**)&imageDataData);
        if(success && imageDataData)
        {
            m_vtkProjectedTerrainPath->SetSource(imageDataData->getVtkImageData());
            return true;
        }

        return false;
    }
    return CVtkPolyDataFilter::setInput(path, inputData);
}

bool CVtkProjectedTerrainPath::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData || !m_vtkProjectedTerrainPath)
        return false;

    if(path->pathName() == "ImageData")
    {
        IVtkImageDataIOData* imageDataData = 0;
        bool success = inputData->queryInterface("IVtkImageDataIOData", (void**)&imageDataData);
        if(success && imageDataData && imageDataData->getVtkImageData() == m_vtkProjectedTerrainPath->GetSource())
        {
            m_vtkProjectedTerrainPath->SetSource(0);
            return true;
        }

        return false;
    }
    return CVtkPolyDataFilter::removeInput(path, inputData);
}
