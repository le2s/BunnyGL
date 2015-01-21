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

#ifndef CVtkPiecewiseFunctionDataData_H
#define CVtkPiecewiseFunctionDataData_H

#include "IVtkDataObjectIOData.h"
#include "IVtkDataSetIOData.h"
#include "IVtkPiecewiseFunctionIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkPiecewiseFunction.h"

class CVtkPiecewiseFunctionData : virtual public IVtkDataObjectIOData,
                              virtual public IVtkPiecewiseFunctionIOData,
                              virtual public IVtkObjectIOData
{
public:
    CVtkPiecewiseFunctionData(vtkPiecewiseFunction* pwFn=0);
    ~CVtkPiecewiseFunctionData();

    void setPiecewiseFunction(vtkPiecewiseFunction* pwFn);

    // IQueryable implementation
    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IVtkDataObjectIOData implementation
    vtkDataObject* getVtkDataObject();

    // IVtkPiecewiseFunctionIOData implementation
    vtkPiecewiseFunction* getVtkPiecewiseFunction();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkPiecewiseFunction* m_vtkPiecewiseFunction;
};

#endif

