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

#ifndef C_VTK_COLOR_TRANSFER_FUNCTION_H
#define C_VTK_COLOR_TRANSFER_FUNCTION_H

#include "CVtkScalarsToColors.h"
#include "vtkColorTransferFunction.h"

class CVtkColorTransferFunction : public CVtkScalarsToColors
{
    Q_OBJECT
    Q_PROPERTY(bool Clamping READ isClamping WRITE setClamping)
    DECLARE_VTK_OBJECT

public:
    CVtkColorTransferFunction();
    ~CVtkColorTransferFunction();

    Q_INVOKABLE int numberOfPoints();
    Q_INVOKABLE void addPoint(double value, double r, double g, double b);
    Q_INVOKABLE void addPoint(double value, QColor color);
    Q_INVOKABLE void removePoint(double value);
    Q_INVOKABLE void removeAllPoints();

    void setClamping(bool val);
    bool isClamping() const;

private:
    vtkColorTransferFunction* m_vtkColorTransferFunction;
};

#endif
