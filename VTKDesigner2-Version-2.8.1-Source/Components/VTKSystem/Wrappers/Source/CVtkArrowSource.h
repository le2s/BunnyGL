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


#ifndef C_VTK_ARROW_SOURCE_H
#define C_VTK_ARROW_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkArrowSource.h"
#include "UserDefinedDataTypes.h"
#include "CVtkPolyData.h"

class CVtkArrowSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(double TipLength READ tipLength WRITE setTipLength)
    Q_PROPERTY(double TipRadius READ tipRadius WRITE setTipRadius)
    Q_PROPERTY(int TipResolution READ tipResolution WRITE setTipResolution)
    Q_PROPERTY(double ShaftRadius READ shaftRadius WRITE setShaftRadius)
    Q_PROPERTY(double ShaftResolution READ shaftResolution WRITE setShaftResolution)
    DECLARE_VTK_OBJECT

public:
    CVtkArrowSource();
    ~CVtkArrowSource();

    double tipLength() const;
    void setTipLength(double tl);

    double tipRadius() const;
    void setTipRadius(double tiprad);

    int tipResolution() const;
    void setTipResolution(int tipres);

    double shaftRadius() const;
    void setShaftRadius(double shrad);

    void setShaftResolution(int shres);
    int shaftResolution() const;

private:
    vtkArrowSource* m_vtkArrowSource;
};

#endif

