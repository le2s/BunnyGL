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

#include "CVtkDelaunay2D.h"
#include "IVtkPolyDataIOData.h"

DEFINE_VTK_OBJECT(CVtkDelaunay2D,CVtkPolyDataFilter,vtkDelaunay2D)
{
    pDesc->setNodeClassName("vtkDelaunay2d");
    pDesc->setNodeClassDescription("Create 2D Delaunay triangulation of input points");
//  PolyData input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("PolyData", IVisSystemNodeConnectionPath::InputPath, "vtkPolyData", 0)
        );

    pDesc->setPropertyInfo("Alpha","SetAlpha","GetAlpha",QStringList() << "double",QStringList(), "void", "double");
    pDesc->setPropertyInfo("Tolerance","SetTolerance","GetTolerance",QStringList() << "double",QStringList(), "void", "double");
    pDesc->setPropertyInfo("Offset","SetOffset","GetOffset",QStringList() << "double",QStringList(), "void", "double");
    pDesc->setPropertyInfo("BoundingTriangulation","SetBoundingTriangulation","GetBoundingTriangulation",QStringList() << "int",QStringList(), "void", "int"); 
    pDesc->setPropertyInfo("ProjectionPlaneMode","SetProjectionPlaneMode","GetProjectionPlaneMode",QStringList() << "double",QStringList(), "void", "double");
}

CVtkDelaunay2D::CVtkDelaunay2D() : m_vtkDelaunay2D(0)
{
    CVtkDelaunay2D::InitializeObjectDesc();
    setVtkObject(vtkDelaunay2D::New());
}

CVtkDelaunay2D::~CVtkDelaunay2D()
{

}

bool CVtkDelaunay2D::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "PolyData")
        return m_vtkDelaunay2D->GetSource() != 0;

    return CVtkPolyDataFilter::hasInput(path);
}

bool CVtkDelaunay2D::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "PolyData")
    {
        IVtkPolyDataIOData* polyData = 0;
        bool success = inputData->queryInterface("IVtkPolyDataIOData", (void**)&polyData);
        if(success && polyData)
        {
            m_vtkDelaunay2D->SetSource(polyData->getVtkPolyData());
            return true;
        }
    }

    return CVtkPolyDataFilter::setInput(path, inputData);
}

bool CVtkDelaunay2D::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "PolyData")
    {
        IVtkPolyDataIOData* polyData = 0;
        bool success = inputData->queryInterface("IVtkPolyDataIOData", (void**)&polyData);
        if(success && polyData && m_vtkDelaunay2D->GetSource() == polyData->getVtkPolyData())
        {
            m_vtkDelaunay2D->SetSource(0);
            return true;
        }
    }

    return CVtkPolyDataFilter::removeInput(path, inputData);
}


void CVtkDelaunay2D::setAlpha(double val)
{
    m_vtkDelaunay2D->SetAlpha(val);
}

double CVtkDelaunay2D::alpha() const
{
    return m_vtkDelaunay2D->GetAlpha();
}


void CVtkDelaunay2D::setTolerance(double val)
{
    m_vtkDelaunay2D->SetTolerance(val);
}

double CVtkDelaunay2D::tolerance() const
{
    return m_vtkDelaunay2D->GetTolerance();
}

void CVtkDelaunay2D::setOffset(double val)
{
    m_vtkDelaunay2D->SetOffset(val);
}

double CVtkDelaunay2D::offset() const
{
    return m_vtkDelaunay2D->GetOffset();
}

void CVtkDelaunay2D::setBoundingTriangulation(bool val)
{
    m_vtkDelaunay2D->SetBoundingTriangulation(val);
}

bool CVtkDelaunay2D::isBoundingTriangulation() const
{
    return m_vtkDelaunay2D->GetBoundingTriangulation();
}

void CVtkDelaunay2D::setProjectionPlaneMode(bool val)
{
    m_vtkDelaunay2D->SetProjectionPlaneMode(val);
}

bool CVtkDelaunay2D::isProjectionPlaneMode() const
{
    return m_vtkDelaunay2D->GetProjectionPlaneMode();
}

