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

#ifndef C_VTK_PIECEWISE_FUNCTION_H
#define C_VTK_PIECEWISE_FUNCTION_H

#include "CVtkObject.h"
#include "vtkPiecewiseFunction.h"
#include "CVtkPiecewiseFunctionData.h"

class CVtkPiecewiseFunction : public CVtkObject
{
    Q_OBJECT
    Q_PROPERTY(bool Clamping READ isClamping WRITE setClamping)
    DECLARE_VTK_OBJECT

public:
    CVtkPiecewiseFunction();
    ~CVtkPiecewiseFunction();

    Q_INVOKABLE int size() const;
    Q_INVOKABLE void addPoint(double x, double val);
    Q_INVOKABLE void removePoint(double x);
    Q_INVOKABLE double value(double x) const;
    Q_INVOKABLE void adjustRange(double min, double max);
    Q_INVOKABLE void removeAllPoints();

    void setClamping(bool val);
    bool isClamping() const;

protected:
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkPiecewiseFunction* m_vtkPiecewiseFunction;
    CVtkPiecewiseFunctionData m_piecewiseFunctionData;
};

#endif
