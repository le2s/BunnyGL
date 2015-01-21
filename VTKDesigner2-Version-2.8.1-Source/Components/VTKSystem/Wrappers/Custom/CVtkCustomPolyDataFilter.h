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

#ifndef C_VTK_CUSTOM_POLY_DATA_FILTER
#define C_VTK_CUSTOM_POLY_DATA_FILTER

#include "CVtkPolyDataFilter.h"
#include "vtkPolyDataAlgorithm.h"
#include "UserDefinedDataTypes.h"
#include <QList>

class vtkCustomPolyDataFilter;
class CVtkCustomPolyDataFilter : public CVtkPolyDataFilter
{
    Q_OBJECT
    DECLARE_VTK_OBJECT

public:
    CVtkCustomPolyDataFilter();
    ~CVtkCustomPolyDataFilter();

    Q_INVOKABLE uint pointCount() const;
    Q_INVOKABLE Point3D point(uint index) const;
    Q_INVOKABLE void setPoint(uint index, Point3D point);
    Q_INVOKABLE void setPoint(uint index, double x, double y, double z);

    Q_INVOKABLE Vector3D pointNormal(uint index) const;
    Q_INVOKABLE void setPointNormal(uint index, Vector3D vec);
    Q_INVOKABLE void setPointNormal(uint index, double x, double y, double z);

    Q_INVOKABLE double pointScalar(uint index) const;
    Q_INVOKABLE void setPointScalar(uint index, double val);

    Q_INVOKABLE QList<double> pointTCoord(uint index) const;
    Q_INVOKABLE void setPointTCoord(uint index, QList<double> tcoord);
    Q_INVOKABLE void setPointTCoord(uint index, double s, double t);

    Q_INVOKABLE uint triangleCount() const;
    Q_INVOKABLE void setTriangle(uint index, Triangle triangle);
    Q_INVOKABLE void setTriangle(uint index, uint a, uint b, uint c);

    Q_INVOKABLE uint lineCount() const;
    Q_INVOKABLE void setLine(uint index, LineSegment lineSeg);
    Q_INVOKABLE void setLine(uint index, uint a, uint b);

    Q_INVOKABLE uint vertCount() const;
    Q_INVOKABLE void setVert(uint index, uint a);

    Q_INVOKABLE void resetOutput();
    Q_INVOKABLE void insertPoint(Point3D point);
    Q_INVOKABLE void insertPoint(double x, double y, double z);
    Q_INVOKABLE void insertPointNormal(Vector3D normal);
    Q_INVOKABLE void insertPointNormal(double x, double y, double z);
    Q_INVOKABLE void insertPointScalar(double val);
    Q_INVOKABLE void insertPointTCoord(double s, double t);
    Q_INVOKABLE void insertLine(LineSegment lineSeg);
    Q_INVOKABLE void insertLine(uint a, uint b);
    Q_INVOKABLE void insertTriangle(Triangle tgl);
    Q_INVOKABLE void insertTriangle(uint a, uint b, uint c);
    Q_INVOKABLE void insertVert(uint v);
    Q_INVOKABLE void insertPolygon(QList<uint> poly);

protected:
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

signals:
    void OnRequestData();

private:
    friend class vtkCustomPolyDataFilter;
    vtkCustomPolyDataFilter* m_vtkCustomPolyDataFilter;
};

class vtkCustomPolyDataFilter : public vtkPolyDataAlgorithm
{
public:
    static vtkCustomPolyDataFilter *New();
    void SetCustomPolyDataFilter(CVtkCustomPolyDataFilter* source);
    CVtkCustomPolyDataFilter* GetCustomPolyDataFilter();

    bool IsRequestingData() { return RequestingData; }

protected:
    vtkCustomPolyDataFilter();
    ~vtkCustomPolyDataFilter();
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
    vtkCustomPolyDataFilter(const vtkCustomPolyDataFilter&);  // Not implemented.
    void operator=(const vtkCustomPolyDataFilter&);  // Not implemented.
    CVtkCustomPolyDataFilter* m_CVtkCustomPolyDataFilter;

private:
    CVtkCustomPolyDataFilter* CustomPolyDataFilterWrapper;
    int RequestingData;
};

#endif

