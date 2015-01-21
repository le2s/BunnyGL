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
#include "CVtkPointLocator.h"
#include "vtkIdList.h"
#include "vtkPointSet.h"
#include "vtkPoints.h"

DEFINE_VTK_OBJECT(CVtkPointLocator, CVtkLocator, vtkPointLocator)
{
    pDesc->setNodeClassName("vtkPointLocator");
    pDesc->setNodeClassDescription("Quickly locate points in 3-space");
    pDesc->setNodeClassCategory("Locator");

    pDesc->setPropertyInfo("NumberOfPointsPerBucket", "SetNumberOfPointsPerBucket", "GetNumberOfPointsPerBucket", QStringList() << "int", QStringList(), "void", "int");
}

CVtkPointLocator::CVtkPointLocator() : m_vtkPointLocator(0)
{
    CVtkPointLocator::InitializeObjectDesc();
    setVtkObject(vtkPointLocator::New());
}

CVtkPointLocator::~CVtkPointLocator()
{

}

int CVtkPointLocator::numberOfPointsPerBucket() const
{
    return m_vtkPointLocator->GetNumberOfPointsPerBucket();
}


void CVtkPointLocator::setNumberOfPointsPerBucket(int val)
{
    m_vtkPointLocator->SetNumberOfPointsPerBucket(val);
}

void CVtkPointLocator::setDivisions(QList<QVariant> list)
{
    int listVec[3];
    listVec[0] = listVec[1] = listVec[2] = 0;
    if(list.count() >= 1)
        listVec[0] = list[0].toInt();
    if(list.count() >= 2)
        listVec[1] = list[1].toInt();
    if(list.count() >= 3)
        listVec[2] = list[2].toInt();
    m_vtkPointLocator->SetDivisions(listVec);
}

QList<QVariant> CVtkPointLocator::divisions() const
{
    QList<QVariant> ret;
    int* div = m_vtkPointLocator->GetDivisions();
    ret << div[0] << div[1] << div[2];
    return ret;
}

int CVtkPointLocator::findClosestPointIndex(Point3D point)
{
    if(m_vtkPointLocator->GetDataSet()->GetNumberOfPoints())
        return int(m_vtkPointLocator->FindClosestPoint((double*)&point));
    return -1;
}

int CVtkPointLocator::findClosestPointIndexWithinRadius(Point3D point, double radius)
{
    if(m_vtkPointLocator->GetDataSet()->GetNumberOfPoints() == 0)
        return -1;

    double dist = 0;
    double p[3];
    p[0] = point.x;
    p[1] = point.y;
    p[2] = point.z;
    return int(m_vtkPointLocator->FindClosestPointWithinRadius(radius, p, dist));
}

QList<int> CVtkPointLocator::findClosestNPointIndicies(Point3D point, int n)
{
    QList<int> ret;
    if(m_vtkPointLocator->GetDataSet()->GetNumberOfPoints() == 0)
        return ret;

    vtkIdList* result = vtkIdList::New();
    m_vtkPointLocator->FindClosestNPoints(n, (double*)&point, result);
    for(int i=0; i<result->GetNumberOfIds(); i++)
        ret << int(result->GetId(i));
    result->Delete();
    return ret;
}

QList<int> CVtkPointLocator::findPointIndiciesWithinRadius(Point3D point, double radius)
{
    QList<int> ret;
    if(m_vtkPointLocator->GetDataSet()->GetNumberOfPoints() == 0)
        return ret;

    vtkIdList* result = vtkIdList::New();
    m_vtkPointLocator->FindPointsWithinRadius(radius, (double*)&point, result);
    for(int i=0; i<result->GetNumberOfIds(); i++)
        ret << int(result->GetId(i));
    result->Delete();
    return ret;
}

Point3D CVtkPointLocator::findClosestPoint(Point3D point)
{
    if(m_vtkPointLocator->GetDataSet()->GetNumberOfPoints() == 0)
        return Point3D();

    vtkDataSet* dataSet = m_vtkPointLocator->GetDataSet();
    vtkPointSet* pointSet = dynamic_cast<vtkPointSet*>(dataSet);
    if(!pointSet)
        return Point3D();

    vtkIdType ptIndex = findClosestPointIndex(point);
    return pointSet->GetPoints()->GetPoint(ptIndex);
}

Point3D CVtkPointLocator::findClosestPointWithinRadius(Point3D point, double radius)
{
    if(m_vtkPointLocator->GetDataSet()->GetNumberOfPoints() == 0)
        return Point3D();

    vtkDataSet* dataSet = m_vtkPointLocator->GetDataSet();
    vtkPointSet* pointSet = dynamic_cast<vtkPointSet*>(dataSet);
    if(!pointSet)
        return Point3D();

    vtkIdType ptIndex = findClosestPointIndexWithinRadius(point, radius);
    return pointSet->GetPoints()->GetPoint(ptIndex);
}

QList<double> CVtkPointLocator::findClosestNPoints(Point3D point, int n)
{
    QList<double> ret;
    if(m_vtkPointLocator->GetDataSet()->GetNumberOfPoints() == 0)
        return ret;

    vtkDataSet* dataSet = m_vtkPointLocator->GetDataSet();
    vtkPointSet* pointSet = dynamic_cast<vtkPointSet*>(dataSet);
    if(!pointSet)
        return ret;

    vtkIdList* result = vtkIdList::New();
    m_vtkPointLocator->FindClosestNPoints(n, (double*)&point, result);
    for(int i=0; i<result->GetNumberOfIds(); i++)
    {
        double* pt = pointSet->GetPoints()->GetPoint(result->GetId(i));
        ret << pt[0] << pt[1] << pt[2];        
    }
    result->Delete();
    return ret;
}

QList<double> CVtkPointLocator::findPointsWithinRadius(Point3D point, double radius)
{
    QList<double> ret;
    if(m_vtkPointLocator->GetDataSet()->GetNumberOfPoints() == 0)
        return ret;

    vtkDataSet* dataSet = m_vtkPointLocator->GetDataSet();
    vtkPointSet* pointSet = dynamic_cast<vtkPointSet*>(dataSet);
    if(!pointSet)
        return ret;

    vtkIdList* result = vtkIdList::New();
    m_vtkPointLocator->FindPointsWithinRadius(radius, (double*)&point, result);
    for(int i=0; i<result->GetNumberOfIds(); i++)
    {
        double* pt = pointSet->GetPoints()->GetPoint(result->GetId(i));
        ret << pt[0] << pt[1] << pt[2];        
    }
    result->Delete();
    return ret;
}






