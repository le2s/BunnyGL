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

#ifndef C_VTK_MARCHING_SQUARES_H
#define C_VTK_MARCHING_SQUARES_H

#include "vtkMarchingSquares.h"
#include "CVtkPolyDataFilter.h"

class CVtkMarchingSquares : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(int NumberOfContours READ numberOfContours WRITE setNumberOfContours)
    DECLARE_VTK_OBJECT

public:
    CVtkMarchingSquares();
    ~CVtkMarchingSquares();

    void setNumberOfContours(int number);
    int numberOfContours ()const;

private:
    vtkMarchingSquares *m_vtkMarchingSquares;
};

#endif 
