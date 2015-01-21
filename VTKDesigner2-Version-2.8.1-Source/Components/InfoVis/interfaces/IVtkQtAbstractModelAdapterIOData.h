/****************************************************************************
**
** Copyright (C) UNO and VCreate Logic
**
** Use of this file is limited according to the terms specified by
** UNO and VCreate Logic.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef I_VTK_QT_ABSTRACT_MODEL_ADAPTER_IO_DATA_H
#define I_VTK_QT_ABSTRACT_MODEL_ADAPTER_IO_DATA_H

#include "IVisSystemNodeIOData.h"

class vtkQtAbstractModelAdapter;
class IVtkQtAbstractModelAdapterIOData : virtual public IVisSystemNodeIOData
{
public:
    virtual vtkQtAbstractModelAdapter* getVtkQtAbstractModelAdapter() = 0;
};

#endif

