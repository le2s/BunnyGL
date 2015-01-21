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
#ifndef C_VTK_WEIGHTED_TRANSFORM_FILTER_H
#define C_VTK_WEIGHTED_TRANSFORM_FILTER_H

#include "vtkWeightedTransformFilter.h"
#include "CVtkPointSetFilter.h"

class CVtkWeightedTransformFilter : public CVtkPointSetFilter
{
    Q_OBJECT
    Q_PROPERTY(QString WeightArray READ weightArray WRITE setWeightArray)
    Q_PROPERTY(QString TransformIndexArray READ transformIndexArray WRITE setTransformIndexArray)
    Q_PROPERTY(QString CellDataWeightArray READ cellDataWeightArray WRITE setCellDataWeightArray)
    Q_PROPERTY(QString CellDataTransformIndexArray READ cellDataTransformIndexArray WRITE setCellDataTransformIndexArray)
    Q_PROPERTY(int NumberOfTransforms READ numberOfTransforms WRITE setNumberOfTransforms)
    Q_PROPERTY(bool AddInputValues READ isAddInputValues WRITE setAddInputValues)
    DECLARE_VTK_OBJECT

public:
    CVtkWeightedTransformFilter();
    ~CVtkWeightedTransformFilter();

    QString weightArray() const;
    void setWeightArray(QString val);

    QString transformIndexArray() const;
    void setTransformIndexArray(QString val);

    QString cellDataWeightArray() const;
    void setCellDataWeightArray(QString val);

    QString cellDataTransformIndexArray() const;
    void setCellDataTransformIndexArray(QString val);

    int numberOfTransforms() const;
    void setNumberOfTransforms(int val);

    bool isAddInputValues() const;
    void setAddInputValues(bool val);


private:
    vtkWeightedTransformFilter *m_vtkWeightedTransformFilter;
};
#endif//C_VTK_WEIGHTED_TRANSFORM_FILTER_H