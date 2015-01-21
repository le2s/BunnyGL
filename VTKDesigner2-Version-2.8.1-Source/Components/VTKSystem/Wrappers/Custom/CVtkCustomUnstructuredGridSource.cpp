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

#include "CVtkCustomUnstructuredGridSource.h"
#include "CVtkVisSystemComponent.h"

#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"

#include "vtkPoints.h"
#include "vtkCellType.h"
#include "vtkCell.h"
#include "vtkUnstructuredGridAlgorithm.h"

DEFINE_VTK_OBJECT(CVtkCustomUnstructuredGridSource, CVtkUnstructuredGridAlgorithm, vtkCustomUnstructuredGridSource)
{
    pDesc->setNodeClassName("vtkCustomUnstructuredGridSource");
    pDesc->setNodeClassCategory("Scriptable Nodes");
    pDesc->setNodeClassDescription("Scriptable unstructured grid source");
	pDesc->setNodeIcon(qApp->windowIcon()); // vtkCustomFunction is not a standard VTK class
    pDesc->setPropertyInfo("StoreData", "setStoreData", "IsStoreData", QStringList() << "bool", QStringList(), "void", "bool");
}

CVtkCustomUnstructuredGridSource::CVtkCustomUnstructuredGridSource()
{
    CVtkCustomUnstructuredGridSource::InitializeObjectDesc();

    setVtkObject(vtkCustomUnstructuredGridSource::New());
    m_vtkCustomUnstructuredGridSource->SetCustomUnstructuredGridSource(this);
    m_vtkCustomUnstructuredGridSource->Update(); // ensure that the output grid is available, even if blank

    m_vtkUnstructuredGrid = m_vtkCustomUnstructuredGridSource->GetOutput();
}

CVtkCustomUnstructuredGridSource::~CVtkCustomUnstructuredGridSource()
{

}

void CVtkCustomUnstructuredGridSource::deleteAllPoints()
{
    vtkPoints* points = m_vtkUnstructuredGrid->GetPoints();
    points->Reset();
}

void CVtkCustomUnstructuredGridSource::setPoints(QList<double> pArray)
{
    vtkPoints* points = vtkPoints::New();
    points->SetNumberOfPoints(pArray.count()/3);
    for(int i=0; i<points->GetNumberOfPoints(); i++)
    {
        double x = pArray[i*3+0];
        double y = pArray[i*3+1];
        double z = pArray[i*3+2];
        points->SetPoint(i, x, y, z);
    }
    m_vtkUnstructuredGrid->SetPoints(points);
    points->Delete();
}

QList<double> CVtkCustomUnstructuredGridSource::points() const
{
    QList<double> pointsArray;
    vtkPoints* points = m_vtkUnstructuredGrid->GetPoints();
    if(points)
    {
        for(int i=0; i<points->GetNumberOfPoints(); i++)
        {
            double p[3];
            points->GetPoint(i, p);
            pointsArray.append(p[0]);
            pointsArray.append(p[1]);
            pointsArray.append(p[2]);
        }
    }
    return pointsArray;
}

void CVtkCustomUnstructuredGridSource::setPointCount(int count)
{
    vtkPoints* points = m_vtkUnstructuredGrid->GetPoints();
    points->SetNumberOfPoints((vtkIdType)count);
}

int CVtkCustomUnstructuredGridSource::pointCount() const
{
    return m_vtkUnstructuredGrid->GetNumberOfPoints();
}

void CVtkCustomUnstructuredGridSource::setPoint(int index, const Point3D& point)
{
    vtkPoints* points = m_vtkUnstructuredGrid->GetPoints();
    if(index < 0 || index >= points->GetNumberOfPoints())
        return;

    points->SetPoint(index, point.x, point.y, point.z);
}

Point3D CVtkCustomUnstructuredGridSource::point(int index) const
{
    Point3D retVal;

    vtkPoints* points = m_vtkUnstructuredGrid->GetPoints();
    if(index < 0 || index >= points->GetNumberOfPoints())
        return retVal;

    double* pt = points->GetPoint(index);
    retVal = Point3D(pt);
    return retVal;
}

void CVtkCustomUnstructuredGridSource::deleteAllCells()
{
    vtkCellArray* cellArray = m_vtkUnstructuredGrid->GetCells();
    cellArray = NULL;
}

void CVtkCustomUnstructuredGridSource::addPointCell(int pointIndex)
{
    vtkIdType nc[1];
    nc[0] = pointIndex;

    m_vtkUnstructuredGrid->InsertNextCell(VTK_VERTEX, 1, nc);
}

void CVtkCustomUnstructuredGridSource::addTriangleCell(int p1, int p2, int p3)
{
    vtkIdType nc[3];

    nc[0] = p1;
    nc[1] = p2;
    nc[2] = p3;

    m_vtkUnstructuredGrid->InsertNextCell(VTK_TRIANGLE, 3, nc);
}

void CVtkCustomUnstructuredGridSource::addQuadCell(int p1, int p2, int p3, int p4)
{
    vtkIdType nc[4];

    nc[0] = p1;
    nc[1] = p2;
    nc[2] = p3;
    nc[3] = p4;

    m_vtkUnstructuredGrid->InsertNextCell(VTK_QUAD, 4, nc);
}

void CVtkCustomUnstructuredGridSource::addPolyCell(const QList<int>& ptIndexArray)
{
    vtkIdType *nc = new vtkIdType [ptIndexArray.size()];

    for(int i=0; ptIndexArray.size();i++)
        nc[i] = ptIndexArray.at(i);

    m_vtkUnstructuredGrid->InsertNextCell(VTK_POLYGON, ptIndexArray.size(), nc);

    delete [] nc;
}

int CVtkCustomUnstructuredGridSource::cellCount() const
{
    return m_vtkUnstructuredGrid->GetNumberOfCells();
}

QList<int> CVtkCustomUnstructuredGridSource::cell(int index) const
{
    vtkCell* cell = m_vtkUnstructuredGrid->GetCell(index);
    vtkIdList* ptIds = cell->GetPointIds();

    QList<int> cellIds;
    cellIds.append(cell->GetCellType());
    for(int i=0; i<ptIds->GetNumberOfIds(); i++)
        cellIds.append(ptIds->GetId(i));

    return cellIds;
}

void CVtkCustomUnstructuredGridSource::setStoreData(bool val)
{
    m_storeData = val;
}

bool CVtkCustomUnstructuredGridSource::isStoreData() const
{
    return m_storeData;
}

QString CVtkCustomUnstructuredGridSource::projectPartID()
{
    return this->nodeName();
}

bool CVtkCustomUnstructuredGridSource::newProjectPart()
{
    //vtkUnstructuredGrid* uGrid = m_vtkCustomUnstructuredGridSource->GetOutput();
    //uGrid->ReleaseData();

    return true;
}

bool CVtkCustomUnstructuredGridSource::loadProjectPart(QDomDocument doc, QDomElement partE)
{
    return true;
}

bool CVtkCustomUnstructuredGridSource::initializeProjectPart()
{
    return true;
}

bool CVtkCustomUnstructuredGridSource::saveProjectPart(QDomDocument doc, QDomElement partParentE)
{
    return true;
}

bool CVtkCustomUnstructuredGridSource::closeProjectPart()
{
    return true;
}



////////////////////////////////////////////////////////////////////////////////////////////////
// vtkCustomUnstructuredGridSource implementation
////////////////////////////////////////////////////////////////////////////////////////////////

vtkCxxRevisionMacro(vtkCustomUnstructuredGridSource, "$Revision: 1.53 $");
vtkStandardNewMacro(vtkCustomUnstructuredGridSource);

void vtkCustomUnstructuredGridSource::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkUnstructuredGridAlgorithm::PrintSelf(os, indent);
}

void vtkCustomUnstructuredGridSource::SetCustomUnstructuredGridSource(CVtkCustomUnstructuredGridSource* source)
{
    m_CVtkCustomUnstructuredGridSource = source;
}

CVtkCustomUnstructuredGridSource* vtkCustomUnstructuredGridSource::GetCustomUnstructuredGridSource()
{
    return m_CVtkCustomUnstructuredGridSource;
}

vtkCustomUnstructuredGridSource::vtkCustomUnstructuredGridSource() : m_CVtkCustomUnstructuredGridSource(0)
{
    this->SetNumberOfInputPorts(0);
    this->RequestingData = 0;
}

vtkCustomUnstructuredGridSource::~vtkCustomUnstructuredGridSource()
{

}

int vtkCustomUnstructuredGridSource::RequestData(
            vtkInformation *vtkNotUsed(request),
            vtkInformationVector **vtkNotUsed(inputVector),
            vtkInformationVector *outputVector
        )
{
    if(this->RequestingData)
        return 1;

    this->RequestingData = 1;

    // get the info object
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    // get the ouptut
    vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    Q_UNUSED(output);

    // Have the wrapper emit the OnRequestData() signal
    if(m_CVtkCustomUnstructuredGridSource)
        m_CVtkCustomUnstructuredGridSource->OnRequestData();

    this->RequestingData = 0;

    return 1;
}


