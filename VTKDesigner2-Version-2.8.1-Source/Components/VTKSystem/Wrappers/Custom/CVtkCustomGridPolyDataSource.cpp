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

#include "CVtkCustomGridPolyDataSource.h"
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"

DEFINE_VTK_OBJECT(CVtkCustomGridPolyDataSource, CVtkPolyDataAlgorithm, vtkCustomGridPolyDataSource)
{
    pDesc->setNodeClassName("vtkCustomGridPolyDataSource");
    pDesc->setNodeClassCategory("Scriptable Nodes");
    pDesc->setNodeClassDescription("Scriptable polydata source");
	pDesc->setNodeIcon(qApp->windowIcon()); // vtkCustomFunction is not a standard VTK class
    pDesc->setPropertyInfo("GridCountX", "setGridCountX", "gridCountX", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("GridCountY", "setGridCountY", "gridCountY", QStringList() << "int", QStringList(), "void", "int");
}

CVtkCustomGridPolyDataSource::CVtkCustomGridPolyDataSource() : m_vtkCustomGridPolyDataSource(0)
{
    CVtkCustomGridPolyDataSource::InitializeObjectDesc();
    setVtkObject(vtkCustomGridPolyDataSource::New());
    m_vtkCustomGridPolyDataSource->SetCustomGridPolyDataSource(this);
    m_vtkCustomGridPolyDataSource->Update();
}

CVtkCustomGridPolyDataSource::~CVtkCustomGridPolyDataSource()
{

}

int CVtkCustomGridPolyDataSource::pointCount() const
{
    vtkPolyData* polyData = m_vtkCustomGridPolyDataSource->GetOutput();
    if(!polyData)
        return 0;
    return polyData->GetNumberOfPoints();
}

Point3D CVtkCustomGridPolyDataSource::point(int index) const
{
    Point3D ret;
    if(index < 0 || index >= pointCount())
        return ret;
    
    vtkPolyData* polyData = m_vtkCustomGridPolyDataSource->GetOutput();
    double* point = polyData->GetPoint(index);
    if(!point)
        return ret;

    return Point3D(point);
}

void CVtkCustomGridPolyDataSource::setPoint(int index, Point3D point)
{
    Point3D ret;
    if(index < 0 || index >= pointCount())
        return;
    
    vtkPolyData* polyData = m_vtkCustomGridPolyDataSource->GetOutput();
    vtkPoints* points = polyData->GetPoints();
    points->SetPoint(index, point.x, point.y, point.z);
}

void CVtkCustomGridPolyDataSource::setGridCountX(int count)
{
    if(count == gridCountX())
        return;

    int x, y;
    m_vtkCustomGridPolyDataSource->GetDimensions(x, y);
    x = count;
    m_vtkCustomGridPolyDataSource->SetDimensions(x, y);
}

int CVtkCustomGridPolyDataSource::gridCountX() const
{
    int x, y;
    m_vtkCustomGridPolyDataSource->GetDimensions(x, y);
    return x;
}

void CVtkCustomGridPolyDataSource::setGridCountY(int count)
{
    if(count == gridCountY())
        return;

    int x, y;
    m_vtkCustomGridPolyDataSource->GetDimensions(x, y);
    y = count;
    m_vtkCustomGridPolyDataSource->SetDimensions(x, y);
}

int CVtkCustomGridPolyDataSource::gridCountY() const
{
    int x, y;
    m_vtkCustomGridPolyDataSource->GetDimensions(x, y);
    return y;
}

void CVtkCustomGridPolyDataSource::setGridDistanceX(double d)
{
    if(d == gridDistanceX())
        return;

    double x, y;
    m_vtkCustomGridPolyDataSource->GetDistance(x, y);
    x = d;
    m_vtkCustomGridPolyDataSource->SetDistance(x, y);
}

double CVtkCustomGridPolyDataSource::gridDistanceX() const
{
    double x, y;
    m_vtkCustomGridPolyDataSource->GetDistance(x, y);
    return x;
}

void CVtkCustomGridPolyDataSource::setGridDistanceY(double d)
{
    if(d == gridDistanceY())
        return;

    double x, y;
    m_vtkCustomGridPolyDataSource->GetDistance(x, y);
    y = d;
    m_vtkCustomGridPolyDataSource->SetDistance(x, y);
}

double CVtkCustomGridPolyDataSource::gridDistanceY() const
{
    double x, y;
    m_vtkCustomGridPolyDataSource->GetDistance(x, y);
    return y;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// vtkCustomGridPolyDataSource implementation
////////////////////////////////////////////////////////////////////////////////////////////////

vtkCxxRevisionMacro(vtkCustomGridPolyDataSource, "$Revision: 1.53 $");
vtkStandardNewMacro(vtkCustomGridPolyDataSource);

void vtkCustomGridPolyDataSource::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkPolyDataAlgorithm::PrintSelf(os, indent);
}

void vtkCustomGridPolyDataSource::SetCustomGridPolyDataSource(CVtkCustomGridPolyDataSource* source)
{
    m_CVtkCustomGridPolyDataSource = source;
}

CVtkCustomGridPolyDataSource* vtkCustomGridPolyDataSource::GetCustomGridPolyDataSource()
{
    return m_CVtkCustomGridPolyDataSource;
}

vtkCustomGridPolyDataSource::vtkCustomGridPolyDataSource() : m_CVtkCustomGridPolyDataSource(0)
{
    this->SetNumberOfInputPorts(0);
    Dimensions[0] = Dimensions[1] = 10;
    Distance[0] = Distance[1] = 0.1;
    this->RequestingData = 0;
}

vtkCustomGridPolyDataSource::~vtkCustomGridPolyDataSource()
{

}

void vtkCustomGridPolyDataSource::SetDimensions(int x, int y)
{
    if(x >= 2)
        Dimensions[0] = x;
    else
        Dimensions[0] = 2;
    if(y >= 2)
        Dimensions[1] = y;
    else
        Dimensions[1] = 2;
    Modified();
}

void vtkCustomGridPolyDataSource::GetDimensions(int& x, int& y)
{
    x = Dimensions[0];
    y = Dimensions[1];
}

void vtkCustomGridPolyDataSource::SetDistance(double x, double y)
{
    Distance[0] = x;
    Distance[1] = y;
    Modified();
}

void vtkCustomGridPolyDataSource::GetDistance(double &x, double &y)
{
    x = Distance[0];
    y = Distance[1];
}

int vtkCustomGridPolyDataSource::RequestData(
            vtkInformation *vtkNotUsed(request),
            vtkInformationVector **vtkNotUsed(inputVector),
            vtkInformationVector *outputVector
        )
{
    if(this->RequestingData)
        return 1;

    // get the info object
    vtkInformation *outInfo = outputVector->GetInformationObject(0);
    
    // get the ouptut
    vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    
    // Construct the basic grid
    int grid_x = Dimensions[0];
    int grid_y = Dimensions[1];
    double dx = Distance[0];
    double dy = Distance[1];
    double xMin = -(dx * double(grid_x-1)) / 2.0;
    double yMin = -(dy * double(grid_y-1)) / 2.0;
    double zVal = 0.0;

    vtkPoints* points = vtkPoints::New();
    vtkDoubleArray* tcoords = vtkDoubleArray::New();
    points->SetNumberOfPoints(grid_x*grid_y);
    tcoords->SetNumberOfComponents(2);
    tcoords->SetNumberOfTuples(grid_x*grid_y);
    for(int i=0; i<grid_x; i++)
    {
        for(int j=0; j<grid_y; j++)
        {
            double x = xMin + double(i) * dx;
            double y = yMin + double(j) * dy;
            double z = zVal;
            points->SetPoint(j*grid_x+i, x, y, z);

            double s = double(i)/double(grid_x);
            double t = double(j)/double(grid_y);
            tcoords->SetTuple2(j*grid_x+i, s, t);
        }
    }
    output->SetPoints(points);
    points->Delete();
    output->GetPointData()->SetTCoords(tcoords);
    tcoords->Delete();

    vtkCellArray* cells = vtkCellArray::New();
    for(int i=0; i<grid_x-1; i++)
    {
        for(int j=0; j<grid_y-1; j++)
        {
            int base = j*grid_x + i; 
            int a = base;
            int b = base+1;
            int c = base+grid_x+1;
            int d = base+grid_x;

            vtkIdType tgl[3];
            tgl[2] = a; tgl[1] = b; tgl[0] = c;
            cells->InsertNextCell(3, tgl);
            tgl[2] = a; tgl[1] = c; tgl[0] = d;
            cells->InsertNextCell(3, tgl);
        }
    }
    output->SetStrips(cells);
    cells->Delete();

    if(m_CVtkCustomGridPolyDataSource)
    {
        this->RequestingData = 1;
        m_CVtkCustomGridPolyDataSource->OnRequestData();
        this->RequestingData = 0;
    }
    return 1;
}


