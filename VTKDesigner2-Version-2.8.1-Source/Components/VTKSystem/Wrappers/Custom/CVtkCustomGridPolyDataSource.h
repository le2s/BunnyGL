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

#ifndef C_VTK_CUSTOM_GRID_POLY_DATA_SOURCE_H
#define C_VTK_CUSTOM_GRID_POLY_DATA_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkPolyDataSource.h"
#include "UserDefinedDataTypes.h"

class vtkCustomGridPolyDataSource;
class CVtkCustomGridPolyDataSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(int GridCountX READ gridCountX WRITE setGridCountX)
    Q_PROPERTY(int GridCountY READ gridCountY WRITE setGridCountY)
    Q_PROPERTY(double GridDistanceX READ gridDistanceX WRITE setGridDistanceX)
    Q_PROPERTY(double GridDistanceY READ gridDistanceY WRITE setGridDistanceY)
    DECLARE_VTK_OBJECT

public:
    CVtkCustomGridPolyDataSource();
    ~CVtkCustomGridPolyDataSource();

    Q_INVOKABLE int pointCount() const;
    Q_INVOKABLE Point3D point(int index) const;
    Q_INVOKABLE void setPoint(int index, Point3D point);

    void setGridCountX(int count);
    int gridCountX() const;

    void setGridCountY(int count);
    int gridCountY() const;

    void setGridDistanceX(double d);
    double gridDistanceX() const;

    void setGridDistanceY(double d);
    double gridDistanceY() const;

signals:
    void OnRequestData();

private:
    friend class vtkCustomGridPolyDataSource;
    vtkCustomGridPolyDataSource* m_vtkCustomGridPolyDataSource;
};

class vtkCustomGridPolyDataSource : public vtkPolyDataAlgorithm
{
public:
    static vtkCustomGridPolyDataSource *New();
    vtkTypeRevisionMacro(vtkCustomGridPolyDataSource, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    void SetCustomGridPolyDataSource(CVtkCustomGridPolyDataSource* source);
    CVtkCustomGridPolyDataSource* GetCustomGridPolyDataSource();

    void SetDimensions(int x, int y);
    void GetDimensions(int& x, int& y);

    void SetDistance(double x, double y);
    void GetDistance(double &x, double &y);

protected:
    vtkCustomGridPolyDataSource();
    ~vtkCustomGridPolyDataSource();
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
    vtkCustomGridPolyDataSource(const vtkCustomGridPolyDataSource&);  // Not implemented.
    void operator=(const vtkCustomGridPolyDataSource&);  // Not implemented.
    CVtkCustomGridPolyDataSource* m_CVtkCustomGridPolyDataSource;

private:
    int Dimensions[2];
    double Distance[2];
    bool RequestingData;
};

#endif

