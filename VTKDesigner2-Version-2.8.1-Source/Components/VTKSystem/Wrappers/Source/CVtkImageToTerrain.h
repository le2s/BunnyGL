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

#ifndef C_VTK_IMAGE_TO_TERRAIN_H
#define C_VTK_IMAGE_TO_TERRAIN_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkImageToTerrain.h"
#include "CVtkPolyData.h"

class CVtkImageToTerrain : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(int GridCountX READ gridCountX WRITE setGridCountX)
    Q_PROPERTY(int GridCountY READ gridCountY WRITE setGridCountY)
    Q_PROPERTY(double GridDistanceX READ gridDistanceX WRITE setGridDistanceX)
    Q_PROPERTY(double GridDistanceY READ gridDistanceY WRITE setGridDistanceY)
    Q_PROPERTY(double HeightScaleFactor READ heightScaleFactor WRITE setHeightScaleFactor)
    DECLARE_VTK_OBJECT

public:
    CVtkImageToTerrain();
    ~CVtkImageToTerrain();

    void setGridCountX(int count);
    int gridCountX() const;

    void setGridCountY(int count);
    int gridCountY() const;

    void setGridDistanceX(double d);
    double gridDistanceX() const;

    void setGridDistanceY(double d);
    double gridDistanceY() const;

    void setHeightScaleFactor(double val);
    double heightScaleFactor();

    Q_INVOKABLE double minHeight() const;
    Q_INVOKABLE double maxHeight() const;

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkImageToTerrain* m_vtkImageToTerrain;
};

#endif

