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

#include "CScriptableVtkPointSet.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkDataArray.h"

CScriptableVtkPointSet::CScriptableVtkPointSet(vtkPointSet* pointSet, CVtkObject* source)
:CScriptableVtkDataSet(pointSet, source), m_vtkPointSet(pointSet)
{

}

CScriptableVtkPointSet::~CScriptableVtkPointSet()
{

}

QList<double> CScriptableVtkPointSet::points() const
{
    QList<double> pointsArray;
    vtkPoints* points = m_vtkPointSet->GetPoints();
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

void CScriptableVtkPointSet::setPoints(QList<double> pArray)
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
    m_vtkPointSet->SetPoints(points);
    points->Delete();
}

Point3D CScriptableVtkPointSet::point(int index) const
{
    if(index < 0 || index >= numberOfPoints())
        return Point3D();

    return m_vtkPointSet->GetPoints()->GetPoint(index);
}

void CScriptableVtkPointSet::setPoint(int index, Point3D point)
{
    if(index < 0 || index >= numberOfPoints())
        return;

    m_vtkPointSet->GetPoints()->SetPoint(index, point.x, point.y, point.z);
}

QList<double> CScriptableVtkPointSet::pointNormals() const
{
    QList<double> ret;
    vtkPointData* pointData = m_vtkPointSet->GetPointData();
    vtkDataArray* normals = pointData->GetNormals();
    if(normals)
    {
        for(int i=0; i<normals->GetNumberOfTuples(); i++)
        {
            double normal[4];
            normals->GetTuple(i, normal);
            ret.append(normal[0]);
            ret.append(normal[1]);
            ret.append(normal[2]);
        }
    }
    return ret;
}

void CScriptableVtkPointSet::setPointNormals(QList<double> normalsArray)
{
    vtkPointData* pointData = m_vtkPointSet->GetPointData();
    vtkDoubleArray* normals = vtkDoubleArray::New();
    int nrNormals = normalsArray.count()/3;

    normals->SetNumberOfComponents(3);
    normals->SetNumberOfTuples(nrNormals);

    for(int i=0; i<nrNormals; i++)
        normals->SetTuple3(i, normalsArray[i*3], normalsArray[i*3+1], normalsArray[i*3+2]);
    pointData->SetNormals(normals);
    normals->Delete();
}

Vector3D CScriptableVtkPointSet::pointNormal(int index) const
{
    if(index < 0 || index >= numberOfPoints())
        return Vector3D();

    vtkPointData* pointData = m_vtkPointSet->GetPointData();
    vtkDataArray* normals = pointData ? pointData->GetNormals() : 0;
    if(!normals)
        return Vector3D();

    return normals->GetTuple3(index);
}

void CScriptableVtkPointSet::setPointNormal(int index, Vector3D vec)
{
    if(index < 0 || index >= numberOfPoints())
        return;

    vtkPointData* pointData = m_vtkPointSet->GetPointData();
    vtkDataArray* normals = pointData ? pointData->GetNormals() : 0;
    if(!normals)
        return;

    return normals->SetTuple3(index, vec.x, vec.y, vec.z);
}

QList<double> CScriptableVtkPointSet::pointScalars() const
{
    QList<double> ret;
    vtkPointData* pointData = m_vtkPointSet->GetPointData();
    vtkDataArray* scalars = pointData->GetScalars();
    if(scalars)
    {
        for(int i=0; i<scalars->GetNumberOfTuples(); i++)
        {
            double scalar[4];
            scalars->GetTuple(i, scalar);
            ret.append(scalar[0]);
        }
    }
    return ret;
}

void CScriptableVtkPointSet::setPointScalars(QList<double> scalarsArray)
{
    vtkPointData* pointData = m_vtkPointSet->GetPointData();
    vtkDoubleArray* scalars = vtkDoubleArray::New();

    scalars->SetNumberOfComponents(1);
    scalars->SetNumberOfTuples(scalarsArray.count());
    for(int i=0; i<scalarsArray.count(); i++)
        scalars->SetTuple1(i, scalarsArray[i]);
    pointData->SetScalars(scalars);
    scalars->Delete();
}

double CScriptableVtkPointSet::pointScalar(int index) const
{
    if(index < 0 || index >= numberOfPoints())
        return 0;

    vtkPointData* pointData = m_vtkPointSet->GetPointData();
    vtkDataArray* scalars = pointData ? pointData->GetScalars() : 0;
    if(!scalars || scalars->GetNumberOfComponents() != 1)
        return 0;

    return scalars->GetTuple1(index);
}

void CScriptableVtkPointSet::setPointScalar(int index, double val)
{
    if(index < 0 || index >= numberOfPoints())
        return;

    vtkPointData* pointData = m_vtkPointSet->GetPointData();
    vtkDataArray* scalars = pointData ? pointData->GetScalars() : 0;
    if(!scalars || scalars->GetNumberOfComponents() != 1)
        return;

    return scalars->SetTuple1(index, val);
}

QList<double> CScriptableVtkPointSet::pointTCoords() const
{
    vtkPointData* pointData = m_vtkPointSet->GetPointData();
    vtkDataArray* tcoords = pointData->GetTCoords();
    QList<double> tcoordsArray;

    if(tcoords)
    {
        int tupleSize = tcoords->GetNumberOfComponents();

        for(int i=0; i<tcoords->GetNumberOfTuples(); i++)
        {
            double tcoord[4];
            tcoords->GetTuple(i, tcoord);
            if(tupleSize >= 1)
                tcoordsArray.append(tcoord[0]);
            else
                tcoordsArray.append(0);
            if(tupleSize >= 2)
                tcoordsArray.append(tcoord[0]);
            else
                tcoordsArray.append(0);
        }
    }

    return tcoordsArray;
}

void CScriptableVtkPointSet::setPointTCoords(QList<double> tcoordArray)
{
    vtkPointData* pointData = m_vtkPointSet->GetPointData();
    vtkDoubleArray* tcoords = vtkDoubleArray::New();
    int nrTcoords = tcoordArray.count() / 2;

    tcoords->SetNumberOfComponents(2);
    tcoords->SetNumberOfTuples(nrTcoords);
    for(int i=0; i<nrTcoords; i++)
    {
        tcoords->SetTuple2(i, tcoordArray[i*2+0], tcoordArray[i*2+1]);
    }
    pointData->SetTCoords(tcoords);
    tcoords->Delete();
}

QList<double> CScriptableVtkPointSet::pointTCoord(int index) const
{
    QList<double> ret;
    if(index < 0 || index >= numberOfPoints())
        return ret;

    vtkPointData* pointData = m_vtkPointSet->GetPointData();
    vtkDataArray* tcoords = pointData ? pointData->GetTCoords() : 0;
    if(!tcoords || tcoords->GetNumberOfComponents() != 2)
        return ret;

    double* retVec = tcoords->GetTuple2(index);
    ret << retVec[0] << retVec[1];
    return ret;
}

void CScriptableVtkPointSet::setPointTCoord(int index, QList<double> val)
{
    if(index < 0 || index >= numberOfPoints())
        return;

    vtkPointData* pointData = m_vtkPointSet->GetPointData();
    vtkDataArray* tcoords = pointData ? pointData->GetTCoords() : 0;
    if(!tcoords || tcoords->GetNumberOfComponents() != 2)
        return;

    tcoords->SetTuple2(index, val[0], val[1]);
}

int CScriptableVtkPointSet::numberOfPoints() const
{
    return m_vtkPointSet->GetNumberOfPoints();
}


