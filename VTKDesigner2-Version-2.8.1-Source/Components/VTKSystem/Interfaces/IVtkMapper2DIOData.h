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

#ifndef I_VTK_MAPPER_2D_IO_DATA_H
#define I_VTK_MAPPER_2D_IO_DATA_H

#include "IVisSystemNodeIOData.h"

class vtkMapper2D;
class IVtkMapper2DIOData : virtual public IVisSystemNodeIOData
{
public:
    virtual vtkMapper2D* getVtkMapper2D() = 0;
};

#endif

