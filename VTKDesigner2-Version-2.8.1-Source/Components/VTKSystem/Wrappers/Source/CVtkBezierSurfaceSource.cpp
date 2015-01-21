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

#include "CVtkBezierSurfaceSource.h"

DEFINE_VTK_OBJECT(CVtkBezierSurfaceSource, CVtkPolyDataAlgorithm, vtkBezierSurfaceSource)
{
    pDesc->setNodeClassName("vtkBezierSurfaceSource");
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassDescription("Generates a bezier surface based based on an array of m,n control points");
    pDesc->setNodeIcon(qApp->windowIcon()); // vtkCustomFunction is not a standard VTK class

    pDesc->setPropertyInfo("Visible", "SetVisibility", "GetVisibility", QStringList() << "bool", QStringList(), "void", "bool");
    
    // Control Points vtkPolyData output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ControlPointsGrid", IVisSystemNodeConnectionPath::OutputPath, "vtkPolyData", 0)
        );
}

CVtkBezierSurfaceSource::CVtkBezierSurfaceSource()
{
    CVtkBezierSurfaceSource::InitializeObjectDesc();
    setVtkObject(vtkBezierSurfaceSource::New());
}

CVtkBezierSurfaceSource::~CVtkBezierSurfaceSource()
{

}

void CVtkBezierSurfaceSource::setControlPointCountX(int m)
{
    int* v = m_vtkBezierSurfaceSource->GetNumberOfControlPoints();
    m_vtkBezierSurfaceSource->SetNumberOfControlPoints(m, v[1]);
}

int CVtkBezierSurfaceSource::controlPointCountX() const
{
    int* v = m_vtkBezierSurfaceSource->GetNumberOfControlPoints();
    return v[0];
}

void CVtkBezierSurfaceSource::setControlPointCountY(int n)
{
    int* v = m_vtkBezierSurfaceSource->GetNumberOfControlPoints();
    m_vtkBezierSurfaceSource->SetNumberOfControlPoints(v[0], n);
}

int CVtkBezierSurfaceSource::controlPointCountY() const
{
    int* v = m_vtkBezierSurfaceSource->GetNumberOfControlPoints();
    return v[1];
}

void CVtkBezierSurfaceSource::setGridCountX(int count)
{
    int* d = m_vtkBezierSurfaceSource->GetDimensions();
    m_vtkBezierSurfaceSource->SetDimensions(count, d[1]);
}

int CVtkBezierSurfaceSource::gridCountX() const
{
    int* d = m_vtkBezierSurfaceSource->GetDimensions();
    return d[0];
}

void CVtkBezierSurfaceSource::setGridCountY(int count)
{
    int* d = m_vtkBezierSurfaceSource->GetDimensions();
    m_vtkBezierSurfaceSource->SetDimensions(d[0], count);
}

int CVtkBezierSurfaceSource::gridCountY() const
{
    int* d = m_vtkBezierSurfaceSource->GetDimensions();
    return d[1];
}

void CVtkBezierSurfaceSource::setControlPoint(int m, int n, const Point3D& pt)
{
    double arr[3];
    arr[0] = pt.x;
    arr[1] = pt.y;
    arr[2] = pt.z;
    m_vtkBezierSurfaceSource->SetControlPoint(m, n, arr);
}

Point3D CVtkBezierSurfaceSource::controlPoint(int m, int n) const
{
    double* ret = m_vtkBezierSurfaceSource->GetControlPoint(m, n);
    return Point3D(ret);
}

void CVtkBezierSurfaceSource::resetControlPoints()
{
    m_vtkBezierSurfaceSource->ResetControlPoints();
}

bool CVtkBezierSurfaceSource::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ControlPointsGrid")
    {
        vtkPolyData* pd = m_vtkBezierSurfaceSource->GetControlPointsOutput();
        m_controlPointOutput.setPolyData( pd );
        *outputData = &m_controlPointOutput;
        return true;
    }

    return CVtkPolyDataAlgorithm::fetchOutput(path, outputData);
}

bool CVtkBezierSurfaceSource::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ControlPointsGrid")
        return true;

    return CVtkPolyDataAlgorithm::outputDerefed(path, outputData);
}

