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
#ifndef C_VTK_PROJECTED_TERRAIN_PATH_H
#define C_VTK_PROJECTED_TERRAIN_PATH_H

#include "vtkProjectedTerrainPath.h"
#include "CVtkPolyDataFilter.h"

class CVtkProjectedTerrainPath : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(ProjectionMode)
    Q_PROPERTY(vtkImageData* Source READ source WRITE setSource)
    Q_PROPERTY(ProjectionMode ProjectionMode READ projectionMode WRITE setProjectionMode)
    Q_PROPERTY(double HeightOffset READ heightOffset WRITE setHeightOffset)
    Q_PROPERTY(double HeightTolerance READ heightTolerance WRITE setHeightTolerance)
    Q_PROPERTY(int MaximumNumberOfLines READ maximumNumberOfLines WRITE setMaximumNumberOfLines)
    DECLARE_VTK_OBJECT

public:
    enum      
    { SIMPLE_PROJECTION = 0, 
      NONOCCLUDED_PROJECTION, 
      HUG_PROJECTION  
    };

    enum ProjectionMode
    {
        Simple=SIMPLE_PROJECTION,
        NonOccluded=NONOCCLUDED_PROJECTION,
        Hug=HUG_PROJECTION
    };

    CVtkProjectedTerrainPath();
    ~CVtkProjectedTerrainPath();

    vtkImageData* source() const;
    void setSource(vtkImageData* val);

    ProjectionMode projectionMode() const;
    void setProjectionMode(ProjectionMode val);

    double heightOffset() const;
    void setHeightOffset(double val);

    double heightTolerance() const;
    void setHeightTolerance(double val);

    int maximumNumberOfLines() const;
    void setMaximumNumberOfLines(int val);

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);


private:
    vtkProjectedTerrainPath *m_vtkProjectedTerrainPath;
};
#endif//C_VTK_PROJECTED_TERRAIN_PATH_H