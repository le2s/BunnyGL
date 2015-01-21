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

#ifndef C_VTK_PNG_READER_H
#define C_VTK_PNG_READER_H

#include "CVtkImageReader2.h"
#include "vtkPNGReader.h"

class CVtkPNGReader : public CVtkImageReader2
{
    Q_OBJECT
    DECLARE_VTK_OBJECT

public:
    CVtkPNGReader();
    ~CVtkPNGReader();

private:
    vtkPNGReader* m_vtkPNGReader;
};

#endif
