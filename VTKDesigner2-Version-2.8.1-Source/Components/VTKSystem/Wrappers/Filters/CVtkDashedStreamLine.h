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

#ifndef C_VTK_DASHED_STREAM_LINE_H
#define C_VTK_DASHED_STREAM_LINE_H

#include "vtkDashedStreamLine.h"
#include "CVtkStreamLine.h"

class CVtkDashedStreamLine : public CVtkStreamLine
{
    Q_OBJECT
    Q_PROPERTY(double DashFactor READ dashFactor WRITE setDashFactor)
    DECLARE_VTK_OBJECT

public:

    CVtkDashedStreamLine();
    ~CVtkDashedStreamLine();

    double dashFactor() const;
    void setDashFactor(double val);


private:
    vtkDashedStreamLine *m_vtkDashedStreamLine;
};
#endif//C_VTK_DASHED_STREAM_LINE_H