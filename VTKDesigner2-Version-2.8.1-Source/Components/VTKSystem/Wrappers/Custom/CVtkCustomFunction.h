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

#ifndef C_VTK_CUSTOM_FUNCTION_H
#define C_VTK_CUSTOM_FUNCTION_H

#include "CVtkImplicitFunction.h"
#include "UserDefinedDataTypes.h"

class vtkCustomFunction;
class CVtkCustomFunction : public CVtkImplicitFunction
{
    Q_OBJECT
    DECLARE_VTK_OBJECT

public:
    CVtkCustomFunction();
    ~CVtkCustomFunction();

    Q_INVOKABLE Point3D functionPoint() const;
    Q_INVOKABLE void setFunctionValue(double val);
    Q_INVOKABLE void setFunctionGradient(Vector3D val);

    double functionValue() const;
    Vector3D functionGradient() const;
    void setFunctionPoint(Point3D val);

    Q_INVOKABLE void markModified();

protected slots:
    void command_Modify();

signals:
    void OnFunctionValueRequest(Point3D val);
    void OnFunctionGradientRequest(Point3D val);

private:
    friend class vtkCustomFunction;
    vtkCustomFunction* m_vtkCustomFunction;
    Point3D m_functionPoint;
    double m_functionValue;
    Vector3D m_functionGradient;
};

class vtkCustomFunction : public vtkImplicitFunction
{
public:
    static vtkCustomFunction *New();

    vtkSetMacro(CustomFunctionWrapper, CVtkCustomFunction*);
    vtkGetMacro(CustomFunctionWrapper, CVtkCustomFunction*);

    double EvaluateFunction(double val[3]);
    void EvaluateGradient(double val[3], double grad[3]);

protected:
    vtkCustomFunction();
    ~vtkCustomFunction();

private:
    vtkCustomFunction(const vtkCustomFunction&);  // Not implemented.
    void operator=(const vtkCustomFunction&);  // Not implemented.

    CVtkCustomFunction* CustomFunctionWrapper;
    bool InEvaluateFunction;
    bool InEvaluateGradient;
};

#endif

