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

#include "CVtkCustomPolyDataFilter.h"
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPolyData.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkCellArray.h"
#include "vtkDataArray.h"
#include "vtkDoubleArray.h"
#include "IVtkPolyDataIOData.h"

DEFINE_VTK_OBJECT(CVtkCustomPolyDataFilter, CVtkPolyDataFilter, vtkCustomPolyDataFilter)
{
    pDesc->setNodeClassName("vtkCustomPolyDataFilter");
    pDesc->setNodeClassCategory("Scriptable Nodes");
    pDesc->setNodeClassDescription("Scriptable polydata filter");
	pDesc->setNodeIcon(qApp->windowIcon()); // vtkCustomFunction is not a standard VTK class
}

CVtkCustomPolyDataFilter::CVtkCustomPolyDataFilter() : m_vtkCustomPolyDataFilter(0)
{
    setVtkObject(vtkCustomPolyDataFilter::New());
    m_vtkCustomPolyDataFilter->SetCustomPolyDataFilter(this);
    m_vtkCustomPolyDataFilter->Update();
}

CVtkCustomPolyDataFilter::~CVtkCustomPolyDataFilter()
{

}

uint CVtkCustomPolyDataFilter::pointCount() const
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return 0;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPoints* points = output->GetPoints();
    if(!points)
        return 0;

    return points->GetNumberOfPoints();
}

Point3D CVtkCustomPolyDataFilter::point(uint index) const
{
    if(index >= this->pointCount())
        return Point3D();

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPoints* points = output->GetPoints();
    return points->GetPoint( vtkIdType(index) );
}

void CVtkCustomPolyDataFilter::setPoint(uint index, Point3D point)
{
    if(index >= this->pointCount())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPoints* points = output->GetPoints();
    points->SetPoint( vtkIdType(index), point.x, point.y, point.z );
}

void CVtkCustomPolyDataFilter::setPoint(uint index, double x, double y, double z)
{
    if(index >= this->pointCount())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPoints* points = output->GetPoints();
    points->SetPoint( vtkIdType(index), x, y, z );  
}


Vector3D CVtkCustomPolyDataFilter::pointNormal(uint index) const
{
    if(index >= this->pointCount())
        return Vector3D();

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPointData* pointData = output->GetPointData();
    if(!pointData)
        return Vector3D();

    vtkDataArray* normals = pointData->GetNormals();
    if(!normals)
        return Vector3D();

    return Vector3D(normals->GetTuple( vtkIdType(index) ));
}

void CVtkCustomPolyDataFilter::setPointNormal(uint index, Vector3D vec)
{
    if(index >= this->pointCount())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPointData* pointData = output->GetPointData();
    if(!pointData)
        return;
    vtkDataArray* normals = pointData->GetNormals();
    if(!normals)
        return;

    normals->SetTuple3( vtkIdType(index), vec.x, vec.y, vec.z );
}

void CVtkCustomPolyDataFilter::setPointNormal(uint index, double x, double y, double z)
{
    if(index >= this->pointCount())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPointData* pointData = output->GetPointData();
    if(!pointData)
        return;
    vtkDataArray* normals = pointData->GetNormals();
    if(!normals)
        return;

    normals->SetTuple3( vtkIdType(index), x, y, z );
}


double CVtkCustomPolyDataFilter::pointScalar(uint index) const
{
    if(index >= this->pointCount())
        return 0;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPointData* pointData = output->GetPointData();
    if(!pointData)
        return 0;

    vtkDataArray* scalars = pointData->GetScalars();
    if(scalars && scalars->GetNumberOfComponents() == 1)
        return scalars->GetTuple1( vtkIdType(index) );

    return 0;
}

void CVtkCustomPolyDataFilter::setPointScalar(uint index, double val)
{
    if(index >= this->pointCount())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPointData* pointData = output->GetPointData();
    if(!pointData)
        return;

    vtkDataArray* scalars = pointData->GetScalars();
    if(scalars && scalars->GetNumberOfComponents() == 1)
        scalars->SetTuple1( vtkIdType(index), val );
}


QList<double> CVtkCustomPolyDataFilter::pointTCoord(uint index) const
{
    QList<double> ret;

    if(index >= this->pointCount())
        return ret;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPointData* pointData = output->GetPointData();
    if(!pointData)
        return ret;

    vtkDataArray* tcoords = pointData->GetTCoords();
    if(!tcoords)
        return ret;

    if(tcoords->GetNumberOfComponents() >= 2)
    {
        double* tuple = tcoords->GetTuple( vtkIdType(index) );
        ret << tuple[0] << tuple[1];
    }

    return ret;
}

void CVtkCustomPolyDataFilter::setPointTCoord(uint index, QList<double> tcoord)
{
    if(index >= this->pointCount())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPointData* pointData = output->GetPointData();
    if(!pointData)
        return;

    vtkDataArray* tcoords = pointData->GetTCoords();
    if(!tcoords)
        return;

    if(tcoords->GetNumberOfComponents() >= 2)
    {
        double tcoordVec[4];
        tcoordVec[0] = tcoord.count() >= 1 ? tcoord[0] : 0;
        tcoordVec[1] = tcoord.count() >= 2 ? tcoord[1] : 0;

        tcoords->SetTuple( vtkIdType(index), tcoordVec );
    }
}

void CVtkCustomPolyDataFilter::setPointTCoord(uint index, double s, double t)
{
    if(index >= this->pointCount())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPointData* pointData = output->GetPointData();
    if(!pointData)
        return;

    vtkDataArray* tcoords = pointData->GetTCoords();
    if(!tcoords)
        return;

    if(tcoords->GetNumberOfComponents() >= 2)
    {
        double tcoordVec[4];
        tcoordVec[0] = s;
        tcoordVec[1] = t;

        tcoords->SetTuple( vtkIdType(index), tcoordVec );
    }
}

uint CVtkCustomPolyDataFilter::triangleCount() const
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return 0;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkCellArray* strips = output->GetStrips();
    if(!strips)
        return 0;

    return strips->GetNumberOfCells();
}

void CVtkCustomPolyDataFilter::setTriangle(uint index, Triangle triangle)
{
    if(index >= this->triangleCount())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkCellArray* strips = output->GetStrips();
    
    vtkIdType* tgl = strips->GetPointer() + (index*4);
    tgl[1] = triangle.a;
    tgl[2] = triangle.b;
    tgl[3] = triangle.c;
}

void CVtkCustomPolyDataFilter::setTriangle(uint index, uint a, uint b, uint c)
{
    if(index >= this->triangleCount())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkCellArray* strips = output->GetStrips();
    
    vtkIdType* tgl = strips->GetPointer() + (index*4);
    tgl[1] = a;
    tgl[2] = b;
    tgl[3] = c;
}

uint CVtkCustomPolyDataFilter::lineCount() const
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return 0;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkCellArray* lines = output->GetLines();
    if(!lines)
        return 0;

    return lines->GetNumberOfCells();
}

void CVtkCustomPolyDataFilter::setLine(uint index, LineSegment lineSeg)
{
    if(index >= this->lineCount())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkCellArray* lines = output->GetLines();

    vtkIdType* line = lines->GetPointer() + (index*3);
    line[1] = lineSeg.a;
    line[2] = lineSeg.b;
}

void CVtkCustomPolyDataFilter::setLine(uint index, uint a, uint b)
{
    if(index >= this->lineCount())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkCellArray* lines = output->GetLines();

    vtkIdType* line = lines->GetPointer() + (index*3);
    line[1] = a;
    line[2] = b;
}

uint CVtkCustomPolyDataFilter::vertCount() const
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return 0;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkCellArray* verts = output->GetLines();
    if(!verts)
        return 0;

    return verts->GetNumberOfCells();
}

void CVtkCustomPolyDataFilter::setVert(uint index, uint a)
{
    if(index >= this->vertCount())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkCellArray* verts = output->GetLines();

    vtkIdType* vert = verts->GetPointer() + (index*2);
    vert[1] = a;
}

void CVtkCustomPolyDataFilter::resetOutput()
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    output->Reset();
    output->Squeeze();
}

void CVtkCustomPolyDataFilter::insertPoint(Point3D point)
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPoints* points = output->GetPoints();
    if(!points)
    {
        points = vtkPoints::New();
        output->SetPoints(points);
        points->Delete();
    }

    points->InsertNextPoint(point.x, point.y, point.z);
}

void CVtkCustomPolyDataFilter::insertPoint(double x, double y, double z)
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPoints* points = output->GetPoints();
    if(!points)
    {
        points = vtkPoints::New();
        output->SetPoints(points);
        points->Delete();
    }

    points->InsertNextPoint(x, y, z);
}

void CVtkCustomPolyDataFilter::insertPointNormal(Vector3D normal)
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPointData* pointData = output->GetPointData();
    vtkDataArray* normals = pointData->GetNormals();
    if(!normals)
    {
        normals = vtkDoubleArray::New();
        normals->SetNumberOfComponents(3);
        pointData->SetNormals(normals);
        normals->Delete();
    }

    normals->InsertNextTuple3(normal.x, normal.y, normal.z);
}

void CVtkCustomPolyDataFilter::insertPointNormal(double x, double y, double z)
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPointData* pointData = output->GetPointData();
    vtkDataArray* normals = pointData->GetNormals();
    if(!normals)
    {
        normals = vtkDoubleArray::New();
        normals->SetNumberOfComponents(3);
        pointData->SetNormals(normals);
        normals->Delete();
    }

    normals->InsertNextTuple3(x, y, z);
}

void CVtkCustomPolyDataFilter::insertPointScalar(double val)
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPointData* pointData = output->GetPointData();
    vtkDataArray* scalars = pointData->GetScalars();
    if(!scalars)
    {
        scalars = vtkDoubleArray::New();
        scalars->SetNumberOfComponents(1);
        pointData->SetScalars(scalars);
        scalars->Delete();
    }

    scalars->InsertNextTuple1(val);
}

void CVtkCustomPolyDataFilter::insertPointTCoord(double s, double t)
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkPointData* pointData = output->GetPointData();
    vtkDataArray* tcoords = pointData->GetTCoords();
    if(!tcoords)
    {
        tcoords = vtkDoubleArray::New();
        tcoords->SetNumberOfComponents(2);
        pointData->SetTCoords(tcoords);
        tcoords->Delete();
    }

    tcoords->InsertNextTuple2(s, t);
}

void CVtkCustomPolyDataFilter::insertLine(LineSegment lineSeg)
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkCellArray* lines = output->GetLines();
    if(!lines)
    {
        lines = vtkCellArray::New();
        output->SetLines(lines);
        lines->Delete();
    }

    vtkIdType pts[2];
    pts[0] = lineSeg.a;
    pts[1] = lineSeg.b;
    lines->InsertNextCell(2, pts);
}

void CVtkCustomPolyDataFilter::insertLine(uint a, uint b)
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkCellArray* lines = output->GetLines();
    if(!lines)
    {
        lines = vtkCellArray::New();
        output->SetLines(lines);
        lines->Delete();
    }

    vtkIdType pts[2];
    pts[0] = a;
    pts[1] = b;
    lines->InsertNextCell(2, pts);
}

void CVtkCustomPolyDataFilter::insertTriangle(Triangle tgl)
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkCellArray* strips = output->GetStrips();
    if(!strips)
    {
        strips = vtkCellArray::New();
        output->SetStrips(strips);
        strips->Delete();
    }

    vtkIdType pts[3];
    pts[0] = tgl.a;
    pts[1] = tgl.b;
    pts[2] = tgl.c;
    strips->InsertNextCell(3, pts);
}

void CVtkCustomPolyDataFilter::insertTriangle(uint a, uint b, uint c)
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkCellArray* strips = output->GetStrips();
    if(!strips)
    {
        strips = vtkCellArray::New();
        output->SetStrips(strips);
        strips->Delete();
    }

    vtkIdType pts[3];
    pts[0] = a;
    pts[1] = b;
    pts[2] = c;
    strips->InsertNextCell(3, pts);
}

void CVtkCustomPolyDataFilter::insertVert(uint v)
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkCellArray* verts = output->GetVerts();
    if(!verts)
    {
        verts = vtkCellArray::New();
        output->SetVerts(verts);
        verts->Delete();
    }

    vtkIdType pts[1];
    pts[0] = v;
    verts->InsertNextCell(1, pts);
}

void CVtkCustomPolyDataFilter::insertPolygon(QList<uint> poly)
{
    if(!m_vtkCustomPolyDataFilter->IsRequestingData())
        return;

    vtkPolyData* output = m_vtkCustomPolyDataFilter->GetOutput();
    vtkCellArray* polys = output->GetPolys();
    if(!polys)
    {
        polys = vtkCellArray::New();
        output->SetPolys(polys);
        polys->Delete();
    }

    vtkIdType npts = poly.count();
    vtkIdType *pts = new vtkIdType[npts];
    for(int i=0; i<poly.count(); i++)
        pts[i] = poly[i];
    polys->InsertNextCell(npts, pts);
    delete [] pts;
}

bool CVtkCustomPolyDataFilter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!m_vtkCustomPolyDataFilter || !path || !inputData)
        return false;

    if(path->pathName() == "DataObject")
    {
        IVtkPolyDataIOData* data = 0;
        bool success = inputData->queryInterface("IVtkPolyDataIOData", (void**)&data);
        if(success && data)
        {
            vtkPolyData* polyData = data->getVtkPolyData();
            if(polyData)
            {
                m_vtkCustomPolyDataFilter->SetInput(polyData);
                return true;
            }
        }
        return false;
    }

    return CVtkPolyDataFilter::setInput(path, inputData);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// CVtkCustomPolyDataFilter
////////////////////////////////////////////////////////////////////////////////////////////////////

vtkCustomPolyDataFilter *vtkCustomPolyDataFilter::New()
{
    return new vtkCustomPolyDataFilter();
}

void vtkCustomPolyDataFilter::SetCustomPolyDataFilter(CVtkCustomPolyDataFilter* source)
{
    this->CustomPolyDataFilterWrapper = source;
}

CVtkCustomPolyDataFilter* vtkCustomPolyDataFilter::GetCustomPolyDataFilter()
{
    return this->CustomPolyDataFilterWrapper;
    this->RequestingData = 0;
}

vtkCustomPolyDataFilter::vtkCustomPolyDataFilter() : CustomPolyDataFilterWrapper(0), RequestingData(false)
{

}

vtkCustomPolyDataFilter::~vtkCustomPolyDataFilter()
{

}

int vtkCustomPolyDataFilter::RequestData(vtkInformation *vtkNotUsed(request),
            vtkInformationVector **inputVector,
            vtkInformationVector *outputVector)
{
    if(this->RequestingData)
        return 1;

    vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
    vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));

    vtkInformation *outInfo = outputVector->GetInformationObject(0);
    vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    // Make output as a duplicate copy of input
    output->DeepCopy(input);

    if(CustomPolyDataFilterWrapper)
    {
        this->RequestingData = 1;
        CustomPolyDataFilterWrapper->OnRequestData();
        this->RequestingData = 0;
    }

    return 1;
}


