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

#include "CScriptableVtkPolyData.h"
#include "vtkCellArray.h"
#include "vtkCell.h"
#include "vtkCellData.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"

#include <QVector>

CScriptableVtkPolyData::CScriptableVtkPolyData(vtkPolyData* polyData, CVtkObject* source)
:CScriptableVtkPointSet(polyData, source), m_vtkPolyData(polyData)
{

}

CScriptableVtkPolyData::~CScriptableVtkPolyData()
{

}

void CScriptableVtkPolyData::setVerts(QList<int> v)
{
    vtkCellArray* cells = vtkCellArray::New();
    for(int i=0; i<v.count(); i++)
    {
        vtkIdType vIndex = v[i];
        cells->InsertNextCell(1, &vIndex);
    }
    m_vtkPolyData->SetVerts(cells);
    cells->Delete();
}

QList<int> CScriptableVtkPolyData::verts() const
{
    vtkCellArray* cells = m_vtkPolyData->GetVerts();
    QList<int> vertArray;
    for(int i=0; i<cells->GetNumberOfCells(); i++)
    {
        vtkIdType nPts;
        vtkIdType* pts;
        cells->GetNextCell(nPts, pts);
        vertArray.append(*pts);
    }
    return vertArray;
}

int CScriptableVtkPolyData::vert(int index) const
{
    if(index < 0 || index >= this->numberOfVerts())
        return 0;

    vtkCellArray* cells = m_vtkPolyData->GetVerts();
    vtkIdType nPts;
    vtkIdType* pts;
    cells->GetCell((vtkIdType)index, nPts, pts);
    return *pts;
}

void CScriptableVtkPolyData::setVert(int index, int vert) const
{
    if(index < 0 || index >= this->numberOfVerts())
        return;

    vtkCellArray* cells = m_vtkPolyData->GetVerts();
    vtkIdType vertIdx = vtkIdType(vert);
    cells->ReplaceCell(1, 1, &vertIdx);
}

void CScriptableVtkPolyData::setLines(QList<int> lines)
{
    vtkCellArray* cells = vtkCellArray::New();
    int nrLines = lines.count()/2;
    for(int i=0; i<nrLines; i++)
    {
        vtkIdType vIndex[2];
        vIndex[0] = lines[i*2+0];
        vIndex[1] = lines[i*2+1];
        cells->InsertNextCell(2, vIndex);
    }
    m_vtkPolyData->SetLines(cells);
    cells->Delete();
}

QList<int> CScriptableVtkPolyData::lines() const
{
    vtkCellArray* cells = m_vtkPolyData->GetLines();
    QList<int> lineArray;
    for(int i=0; i<cells->GetNumberOfCells(); i++)
    {
        vtkIdType nPts;
        vtkIdType *pts;
        cells->GetNextCell(nPts, pts);
        lineArray.append(pts[0]);
        lineArray.append(pts[1]);
    }
    return lineArray;
}

LineSegment CScriptableVtkPolyData::line(int index) const
{
    LineSegment lineSegment;
    if(index < 0 || index >= this->numberOfLines())
        return lineSegment;

    vtkCellArray* cells = m_vtkPolyData->GetLines();
    vtkIdType nPts;
    vtkIdType *pts;
    cells->GetCell(index, nPts, pts);
    lineSegment.a = pts[0];
    lineSegment.b = pts[1];
    
    return lineSegment;
}

void CScriptableVtkPolyData::setLine(int index, LineSegment lineSeg) const
{
    if(index < 0 || index >= this->numberOfLines())
        return;

    vtkCellArray* cells = m_vtkPolyData->GetLines();
    vtkIdType vIndex[2];
    vIndex[0] = lineSeg.a;
    vIndex[1] = lineSeg.b;
    cells->ReplaceCell(index, 2, vIndex);
}

void CScriptableVtkPolyData::setStrips(QList<int> strips)
{
    vtkCellArray* cells = vtkCellArray::New();
    int nrLines = strips.count()/3;
    for(int i=0; i<nrLines; i++)
    {
        vtkIdType vIndex[3];
        vIndex[0] = strips[i*3+0];
        vIndex[1] = strips[i*3+1];
        vIndex[2] = strips[i*3+2];
        cells->InsertNextCell(3, vIndex);
    }
    m_vtkPolyData->SetStrips(cells);
    cells->Delete();
}

QList<int> CScriptableVtkPolyData::strips() const
{
    vtkCellArray* cells = m_vtkPolyData->GetStrips();
    QList<int> stripsArray;
    for(int i=0; i<cells->GetNumberOfCells(); i++)
    {
        vtkIdType nPts;
        vtkIdType *pts;
        cells->GetNextCell(nPts, pts);
        stripsArray.append(pts[0]);
        stripsArray.append(pts[1]);
        stripsArray.append(pts[2]);
    }
    return stripsArray;
}

Triangle CScriptableVtkPolyData::strip(int index) const
{
    Triangle tgl;
    if(index < 0 || index >= this->numberOfStrips())
        return tgl;

    vtkCellArray* cells = m_vtkPolyData->GetStrips();
    vtkIdType nPts;
    vtkIdType *pts;
    cells->GetCell(index, nPts, pts);
    tgl.a = pts[0];
    tgl.b = pts[1];
    tgl.c = pts[2];
    return tgl;
}

void CScriptableVtkPolyData::setStrip(int index, Triangle tgl) const
{
    if(index < 0 || index >= this->numberOfStrips())
        return;

    vtkCellArray* cells = m_vtkPolyData->GetStrips();
    vtkIdType vIndex[3];
    vIndex[0] = tgl.a;
    vIndex[1] = tgl.b;
    vIndex[2] = tgl.c;
    cells->ReplaceCell(index, 3, vIndex);
}

void CScriptableVtkPolyData::setPolys(QList<int> polys)
{
    vtkCellArray* cells = vtkCellArray::New();
    int index = 0;
    while(1)
    {
        QVector<vtkIdType> ptsVec;
        int nrPts = polys[index++];
        for(int i=0; i<nrPts; i++)
            ptsVec << polys[index++];
        cells->InsertNextCell(nrPts, ptsVec.constData());
    }
    m_vtkPolyData->SetPolys(cells);
    cells->Delete();
}

QList<int> CScriptableVtkPolyData::polys() const
{
    vtkCellArray* cells = m_vtkPolyData->GetPolys();
    QList<int> polysArray;
    for(int i=0; i<cells->GetNumberOfCells(); i++)
    {
        vtkIdType nPts;
        vtkIdType *pts;
        cells->GetNextCell(nPts, pts);
        polysArray.append(nPts);
        for(int i=0; i<nPts; i++)
            polysArray.append(pts[i]);
    }
    return polysArray;
}

QList<int> CScriptableVtkPolyData::poly(int index) const
{
    QList<int> polyList;
    if(index < 0 || index >= numberOfPolys())
        return polyList;

    vtkCellArray* cells = m_vtkPolyData->GetPolys();
    vtkIdType nPts;
    vtkIdType *pts;
    cells->GetCell(index, nPts, pts);
    for(int i=0; i<nPts; i++)
        polyList.append(pts[i]);
    return polyList;
}


QList<double> CScriptableVtkPolyData::cellNormals() const
{
    QList<double> ret;
    vtkCellData* cellData = m_vtkPolyData->GetCellData();
    vtkDataArray* normals = cellData->GetNormals();
    for(int i=0; i<normals->GetNumberOfTuples(); i++)
    {
        double normal[4];
        normals->GetTuple(i, normal);
        ret.append(normal[0]);
        ret.append(normal[1]);
        ret.append(normal[2]);
    }
    return ret;
}

void CScriptableVtkPolyData::setCellNormals(QList<double> normalsArray)
{
    vtkCellData* cellData = m_vtkPolyData->GetCellData();
    vtkDoubleArray* normals = vtkDoubleArray::New();
    int nrNormals = normalsArray.count()/3;

    normals->SetNumberOfComponents(3);
    normals->SetNumberOfTuples(nrNormals);
    for(int i=0; i<nrNormals; i++)
        normals->SetTuple3(i, normalsArray[i*3], normalsArray[i*3+1], normalsArray[i*3+2]);
    cellData->SetNormals(normals);
    normals->Delete();
}

Vector3D CScriptableVtkPolyData::cellNormal(int index) const
{
    if(index < 0 || index >= this->numberOfCells())
        return Vector3D();

    vtkCellData* cellData = m_vtkPolyData->GetCellData();
    vtkDataArray* normals = cellData ? cellData->GetNormals() : 0;

    if(!normals)
        return Vector3D();

    return normals->GetTuple3(index);
}

void CScriptableVtkPolyData::setCellNormal(int index, Vector3D normalVec) const
{
    if(index < 0 || index >= this->numberOfCells())
        return;

    vtkCellData* cellData = m_vtkPolyData->GetCellData();
    vtkDataArray* normals = cellData ? cellData->GetNormals() : 0;    
    if(!normals)
        return;

    normals->SetTuple3(index, normalVec.x, normalVec.y, normalVec.z);
}

QList<double> CScriptableVtkPolyData::cellScalars() const
{
    QList<double> ret;
    vtkCellData* cellData = m_vtkPolyData->GetCellData();
    vtkDataArray* scalars = cellData->GetScalars();
    for(int i=0; i<scalars->GetNumberOfTuples(); i++)
    {
        double scalar[4];
        scalars->GetTuple(i, scalar);
        ret.append(scalar[0]);
    }
    return ret;
}

void CScriptableVtkPolyData::setCellScalars(QList<double> scalarsArray)
{
    vtkCellData* cellData = m_vtkPolyData->GetCellData();
    vtkDoubleArray* scalars = vtkDoubleArray::New();

    scalars->SetNumberOfComponents(1);
    scalars->SetNumberOfTuples(scalarsArray.count());
    for(int i=0; i<scalarsArray.count(); i++)
        scalars->SetTuple1(i, scalarsArray[i]);
    cellData->SetScalars(scalars);
    scalars->Delete();
}

double CScriptableVtkPolyData::cellScalar(int index) const
{
    if(index < 0 || index >= this->numberOfCells())
        return 0;

    vtkCellData* cellData = m_vtkPolyData->GetCellData();
    vtkDataArray* scalars = cellData ? cellData->GetScalars() : 0;

    if(!scalars || scalars->GetNumberOfComponents() != 1)
        return 0;

    return scalars->GetTuple1(index);
}

void CScriptableVtkPolyData::setCellScalar(int index, double value) const
{
    if(index < 0 || index >= this->numberOfCells())
        return;

    vtkCellData* cellData = m_vtkPolyData->GetCellData();
    vtkDataArray* scalars = cellData ? cellData->GetScalars() : 0;

    if(!scalars || scalars->GetNumberOfComponents() != 1)
        return;

    scalars->SetTuple1(index, value);
}

int CScriptableVtkPolyData::numberOfVerts() const
{
    return m_vtkPolyData->GetNumberOfVerts();
}

int CScriptableVtkPolyData::numberOfLines() const
{
    return m_vtkPolyData->GetNumberOfLines();
}

int CScriptableVtkPolyData::numberOfStrips() const
{
    return m_vtkPolyData->GetNumberOfStrips();
}

int CScriptableVtkPolyData::numberOfPolys() const
{
    return m_vtkPolyData->GetNumberOfPolys();
}

int CScriptableVtkPolyData::numberOfCells() const
{
    return m_vtkPolyData->GetNumberOfCells();
}



