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

#ifndef C_VTK_STRIPPER_H
#define C_VTK_STRIPPER_H

#include "vtkStripper.h"
#include "CVtkPolyDataFilter.h"

class CVtkStripper : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(int MaximumLength READ maximumLength WRITE setMaximumLength)
    Q_PROPERTY(bool PassCellDataAsFieldData READ isPassCellDataAsFieldData WRITE setPassCellDataAsFieldData)
    DECLARE_VTK_OBJECT

public:

    CVtkStripper();
    ~CVtkStripper();

    int maximumLength() const;
    void setMaximumLength(int val);

    bool isPassCellDataAsFieldData() const;
    void setPassCellDataAsFieldData(bool val);


private:
    vtkStripper *m_vtkStripper;
};
#endif//C_VTK_STRIPPER_H