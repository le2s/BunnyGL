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

#ifndef C_VTK_IMAGE_DATA_GEOMETRY_FILTER_H
#define C_VTK_IMAGE_DATA_GEOMETRY_FILTER_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkImageDataGeometryFilter.h"

class CVtkImageDataGeometryFilter : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(QList<int> Extent READ extent WRITE setExtent)
    Q_PROPERTY(bool ThresholdCells READ thresholdCells WRITE setThresholdCells)
    Q_PROPERTY(double ThresholdValue READ thresholdValue WRITE setThresholdValue)
    Q_PROPERTY(bool OutputTriangles READ outputTriangles WRITE setOutputTriangles)
    DECLARE_VTK_OBJECT

public:
    CVtkImageDataGeometryFilter();
    ~CVtkImageDataGeometryFilter();

    void setExtent(const QList<int>& exts);
    QList<int> extent() const;

    void setThresholdCells(int val);
    bool thresholdCells() const;

    void setThresholdValue(double val);
    double thresholdValue() const;

    void setOutputTriangles(int val);
    bool outputTriangles() const;

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkImageDataGeometryFilter* m_vtkImageDataGeometryFilter;
};

#endif //CVtkImageDataGeometryFilter