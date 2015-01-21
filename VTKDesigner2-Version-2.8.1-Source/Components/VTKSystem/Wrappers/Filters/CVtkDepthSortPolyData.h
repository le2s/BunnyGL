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
#ifndef C_VTK_DEPTH_SORT_POLY_DATA_H
#define C_VTK_DEPTH_SORT_POLY_DATA_H

#include "CVtkObject.h"
#include "vtkDepthSortPolyData.h"
#include "CVtkPolyDataFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkDepthSortPolyData : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(Direction)
    Q_ENUMS(DepthSortMode)
    Q_PROPERTY(Direction Direction READ direction WRITE setDirection)
    Q_PROPERTY(DepthSortMode DepthSortMode READ depthSortMode WRITE setDepthSortMode)
    Q_PROPERTY(Point3D Vector READ vector WRITE setVector)
    Q_PROPERTY(Point3D Origin READ origin WRITE setOrigin)
    Q_PROPERTY(bool SortScalars READ isSortScalars WRITE setSortScalars)
    DECLARE_VTK_OBJECT

public:
    enum Direction
    {
        FrontToBack=VTK_DIRECTION_FRONT_TO_BACK,
        SpecifiedVector=VTK_DIRECTION_SPECIFIED_VECTOR
    };
    enum DepthSortMode
    {
        FirstPoint=VTK_SORT_FIRST_POINT,
        BoundsCenter=VTK_SORT_BOUNDS_CENTER,
        ParametricCenter=VTK_SORT_PARAMETRIC_CENTER
    };

    CVtkDepthSortPolyData();
    ~CVtkDepthSortPolyData();

    Direction direction() const;
    void setDirection(Direction val);

    DepthSortMode depthSortMode() const;
    void setDepthSortMode(DepthSortMode val);

    Point3D vector() const;
    void setVector(Point3D val);

    Point3D origin() const;
    void setOrigin(Point3D val);

    bool isSortScalars() const;
    void setSortScalars(bool val);

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkDepthSortPolyData *m_vtkDepthSortPolyData;
};
#endif//C_VTK_DEPTH_SORT_POLY_DATA_H