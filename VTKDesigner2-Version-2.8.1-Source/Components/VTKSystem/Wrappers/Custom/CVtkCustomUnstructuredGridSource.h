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

#ifndef C_VTK_CUSTOM_UNSTRUCTURED_GRID_SOURCE_H
#define C_VTK_CUSTOM_UNSTRUCTURED_GRID_SOURCE_H

#include "CVtkUnstructuredGridAlgorithm.h"
#include "vtkUnstructuredGridAlgorithm.h"
#include "IProjectPart.h"
#include <UserDefinedDataTypes>

class vtkCustomUnstructuredGridSource;

class CVtkCustomUnstructuredGridSource : public CVtkUnstructuredGridAlgorithm,
                                 virtual public IProjectPart
{
    Q_OBJECT
    Q_INTERFACES(IProjectPart)
    Q_PROPERTY(bool StoreData READ isStoreData WRITE setStoreData)
    DECLARE_VTK_OBJECT

public:
    CVtkCustomUnstructuredGridSource();
    ~CVtkCustomUnstructuredGridSource();

    // Method to clear point information
    Q_INVOKABLE void deleteAllPoints();

    // Set all the points at once
    Q_INVOKABLE void setPoints(QList<double> pArray);
    Q_INVOKABLE QList<double> points() const;

    // Configure the number of points. New points in the array will be set to 0,0,0
    Q_INVOKABLE void setPointCount(int count);
    Q_INVOKABLE int pointCount() const;

    // Set/Fetch a particular point based on point index
    Q_INVOKABLE void setPoint(int index, const Point3D& point);
    Q_INVOKABLE Point3D point(int index) const;

    // Method to clear cell information
    Q_INVOKABLE void deleteAllCells();

    // Methods to add/remove cells
    Q_INVOKABLE void addPointCell(int pointIndex);
    Q_INVOKABLE void addTriangleCell(int p1, int p2, int p3);
    Q_INVOKABLE void addQuadCell(int p1, int p2, int p3, int p4);
    Q_INVOKABLE void addPolyCell(const QList<int>& ptIndexArray);

    // Methods to fetch cells
    Q_INVOKABLE int cellCount() const;
    Q_INVOKABLE QList<int> cell(int index) const;

signals:
    void OnRequestData();

public:
    void setStoreData(bool val);
    bool isStoreData() const;

    // IContainer implementation, inherited from CVtkObject

    // IProjectPart implementation
    QString projectPartID();
    bool newProjectPart();
    bool loadProjectPart(QDomDocument doc, QDomElement partE);
    bool initializeProjectPart();
    bool saveProjectPart(QDomDocument doc, QDomElement partParentE);
    bool closeProjectPart();

signals:
    void projectPartModified();

private:
    friend class vtkCustomUnstructuredGridSource;
    vtkCustomUnstructuredGridSource* m_vtkCustomUnstructuredGridSource;
    vtkUnstructuredGrid* m_vtkUnstructuredGrid;
    bool m_storeData;
};

class vtkCustomUnstructuredGridSource : public vtkUnstructuredGridAlgorithm
{
public:
    static vtkCustomUnstructuredGridSource *New();
    vtkTypeRevisionMacro(vtkCustomUnstructuredGridSource, vtkUnstructuredGridAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    void SetCustomUnstructuredGridSource(CVtkCustomUnstructuredGridSource* source);
    CVtkCustomUnstructuredGridSource* GetCustomUnstructuredGridSource();

protected:
    vtkCustomUnstructuredGridSource();
    ~vtkCustomUnstructuredGridSource();
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
    vtkCustomUnstructuredGridSource(const vtkCustomUnstructuredGridSource&);  // Not implemented.
    void operator=(const vtkCustomUnstructuredGridSource&);  // Not implemented.

private:
    CVtkCustomUnstructuredGridSource* m_CVtkCustomUnstructuredGridSource;
    int RequestingData;
};

#endif

