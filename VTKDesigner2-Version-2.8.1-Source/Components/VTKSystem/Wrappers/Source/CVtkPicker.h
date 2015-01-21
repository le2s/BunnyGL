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
#ifndef C_VTK_PICKER_H
#define C_VTK_PICKER_H

#include "vtkPicker.h"
#include "CVtkAbstractPropPicker.h"
#include "UserDefinedDataTypes.h"

class CVtkPicker : public CVtkAbstractPropPicker
{
    Q_OBJECT
    Q_PROPERTY(double Tolerance READ tolerance WRITE setTolerance)
    DECLARE_VTK_OBJECT

public:
    CVtkPicker();
    ~CVtkPicker();

    double tolerance() const;
    void setTolerance(double val);

    Q_INVOKABLE Point3D mapperPosition() const;
    Q_INVOKABLE int pickedPositionCount() const;
    Q_INVOKABLE Point3D pickedPosition(int index) const;

private:
    vtkPicker *m_vtkPicker;
};
#endif//C_VTK_PICKER_H