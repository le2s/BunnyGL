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

#include "CVtkImageToTerrain.h"
#include "vtkImageData.h"
#include "IVtkImageDataIOData.h"

DEFINE_VTK_OBJECT(CVtkImageToTerrain, CVtkPolyDataAlgorithm, vtkImageToTerrain)
{
    pDesc->setNodeClassName("vtkImageToTerrain");
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassDescription("Creates a terrain grid from an image");
	pDesc->setNodeIcon(qApp->windowIcon()); // vtkCustomFunction is not a standard VTK class
    pDesc->setPropertyInfo("GridCountX", "setGridCountX", "gridCountX", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("GridCountY", "setGridCountY", "gridCountY", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("HeightScaleFactor", "setHeightScaleFactor", "heightScaleFactor", QStringList() << "double", QStringList(), "void", "int");

    // Image input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("TerrainImage", IVisSystemNodeConnectionPath::InputPath, "vtkImageData", 0)
        );
}

CVtkImageToTerrain::CVtkImageToTerrain() : m_vtkImageToTerrain(0)
{
    CVtkImageToTerrain::InitializeObjectDesc();
    setVtkObject(vtkImageToTerrain::New());
}

CVtkImageToTerrain::~CVtkImageToTerrain()
{

}

void CVtkImageToTerrain::setGridCountX(int count)
{
    if(count == gridCountX())
        return;

    int x, y;
    m_vtkImageToTerrain->GetDimensions(x, y);
    x = count;
    m_vtkImageToTerrain->SetDimensions(x, y);
}

int CVtkImageToTerrain::gridCountX() const
{
    int x, y;
    m_vtkImageToTerrain->GetDimensions(x, y);
    return x;
}

void CVtkImageToTerrain::setGridCountY(int count)
{
    if(count == gridCountY())
        return;

    int x, y;
    m_vtkImageToTerrain->GetDimensions(x, y);
    y = count;
    m_vtkImageToTerrain->SetDimensions(x, y);
}

int CVtkImageToTerrain::gridCountY() const
{
    int x, y;
    m_vtkImageToTerrain->GetDimensions(x, y);
    return y;
}

void CVtkImageToTerrain::setGridDistanceX(double d)
{
    if(d == gridDistanceX())
        return;

    double x, y;
    m_vtkImageToTerrain->GetDistance(x, y);
    x = d;
    m_vtkImageToTerrain->SetDistance(x, y);
}

double CVtkImageToTerrain::gridDistanceX() const
{
    double x, y;
    m_vtkImageToTerrain->GetDistance(x, y);
    return x;
}

void CVtkImageToTerrain::setGridDistanceY(double d)
{
    if(d == gridDistanceY())
        return;

    double x, y;
    m_vtkImageToTerrain->GetDistance(x, y);
    y = d;
    m_vtkImageToTerrain->SetDistance(x, y);
}

double CVtkImageToTerrain::gridDistanceY() const
{
    double x, y;
    m_vtkImageToTerrain->GetDistance(x, y);
    return y;
}

void CVtkImageToTerrain::setHeightScaleFactor(double val)
{
    m_vtkImageToTerrain->SetHeightScaleFactor(val);
}

double CVtkImageToTerrain::heightScaleFactor()
{
    return m_vtkImageToTerrain->GetHeightScaleFactor();
}

double CVtkImageToTerrain::minHeight() const
{
    return m_vtkImageToTerrain->GetHeightRange()[0];
}

double CVtkImageToTerrain::maxHeight() const
{
    return m_vtkImageToTerrain->GetHeightRange()[1];
}

bool CVtkImageToTerrain::hasInput(IVisSystemNodeConnectionPath* path)
{
    if( !path )
        return false;

    if( path->pathName() == "TerrainImage" )
    {
        return m_vtkImageToTerrain->GetImage() != 0;
    }

    return CVtkPolyDataAlgorithm::hasInput(path);
}

bool CVtkImageToTerrain::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if( !path || !inputData )
        return false;

    if( path->pathName() == "TerrainImage" )
    {
        IVtkImageDataIOData* data = 0;
        bool success = inputData->queryInterface("IVtkImageDataIOData", (void**)&data);
        if( success && data )
        {
            vtkImageData* imageData = data->getVtkImageData();
            m_vtkImageToTerrain->SetImage(imageData);
            return true;
        }

        return false;
    }

    return CVtkPolyDataAlgorithm::setInput(path, inputData);
}

bool CVtkImageToTerrain::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if( !path || !inputData )
        return false;

    if( path->pathName() == "TerrainImage" )
    {
        IVtkImageDataIOData* data = 0;
        bool success = inputData->queryInterface("IVtkImageDataIOData", (void**)&data);
        if( success && data )
        {
            vtkImageData* imageData = data->getVtkImageData();
            if(m_vtkImageToTerrain->GetImage() == imageData)
            {
                m_vtkImageToTerrain->SetImage(0);
                return true;
            }
        }

        return false;
    }

    return CVtkPolyDataAlgorithm::removeInput(path, inputData);
}



