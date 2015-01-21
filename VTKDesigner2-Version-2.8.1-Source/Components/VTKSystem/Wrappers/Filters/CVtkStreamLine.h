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

#ifndef C_VTK_STREAM_LINE_H
#define C_VTK_STREAM_LINE_H

#include "vtkStreamLine.h"
#include "CVtkStreamer.h"

class CVtkStreamLine : public CVtkStreamer
{
    Q_OBJECT
    Q_PROPERTY(double StepLength READ stepLength WRITE setStepLength)
    DECLARE_VTK_OBJECT

public:

    CVtkStreamLine();
    ~CVtkStreamLine();

    double stepLength() const;
    void setStepLength(double val);


private:
    vtkStreamLine *m_vtkStreamLine;
};
#endif//C_VTK_STREAM_LINE_H