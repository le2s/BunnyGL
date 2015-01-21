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

#ifndef C_VTK_IMPLICIT_FUNCTION_DATA_H
#define C_VTK_IMPLICIT_FUNCTION_DATA_H

#include "IVtkImplicitFunctionIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkImplicitFunction.h"

class CVtkImplicitFunctionData : virtual public IVtkImplicitFunctionIOData,
                                 virtual public IVtkObjectIOData
{
public:
    CVtkImplicitFunctionData(vtkImplicitFunction* function=0);
    ~CVtkImplicitFunctionData();

    void setImplicitFunction(vtkImplicitFunction* function);

    // IQueryable implementation
    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IVtkImplicitFunctionIOData implementation
    vtkImplicitFunction* getVtkImplicitFunction();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkImplicitFunction* m_vtkImplicitFunction;
};

#endif

