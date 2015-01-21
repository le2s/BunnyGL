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
#ifndef C_VTK_IMPLICIT_FUNCTION_H
#define C_VTK_IMPLICIT_FUNCTION_H

#include "vtkImplicitFunction.h"
#include "CVtkObject.h"
#include "CVtkImplicitFunctionData.h"

class CVtkImplicitFunction : public CVtkObject
{
    Q_OBJECT
    DECLARE_VTK_OBJECT

public:
    CVtkImplicitFunction();
    ~CVtkImplicitFunction();

    Q_INVOKABLE double evaluateFunction(double x, double y, double z);

protected:
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkImplicitFunction *m_vtkImplicitFunction;
    CVtkImplicitFunctionData m_ImplicitFunctionData;
};
#endif//C_VTK_IMPLICIT_FUNCTION_H