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

#ifndef C_VTK_IMPLICIT_BOOLEAN_H
#define C_VTK_IMPLICIT_BOOLEAN_H

#include "CVtkImplicitFunction.h"
#include "vtkImplicitBoolean.h"

class CVtkImplicitBoolean : public CVtkImplicitFunction
{
    Q_OBJECT
    Q_ENUMS(Operation)
    Q_PROPERTY(Operation Operation READ operation WRITE setOperation)
    DECLARE_VTK_OBJECT

public:
    enum Operation
    {
        UnionOperation = VTK_UNION ,
        IntersectionOperation = VTK_INTERSECTION ,
        DifferenceOperation = VTK_DIFFERENCE ,
        UnionOfMagnitudesOperation = VTK_UNION_OF_MAGNITUDES
    };

    CVtkImplicitBoolean();
    ~CVtkImplicitBoolean();

    void setOperation(Operation oper);
    Operation operation() const;

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkImplicitBoolean* m_vtkImplicitBoolean;
};

#endif

