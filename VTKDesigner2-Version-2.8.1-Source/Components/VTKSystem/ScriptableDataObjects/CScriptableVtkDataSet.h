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

#ifndef C_VTK_SCRIPTABLE_VTK_DATA_SET_H
#define C_VTK_SCRIPTABLE_VTK_DATA_SET_H

#include "CScriptableVtkDataObject.h"
#include "vtkDataSet.h"
#include "UserDefinedDataTypes.h"

class CScriptableVtkDataSet : public CScriptableVtkDataObject
{
    Q_OBJECT

public:
    CScriptableVtkDataSet(vtkDataSet* dataSet, CVtkObject* source);
    ~CScriptableVtkDataSet();

    Q_INVOKABLE void computeBounds();
    
    Q_INVOKABLE BoundingBox3D boundingBox() const;
    Q_INVOKABLE Point3D center() const;
    Q_INVOKABLE double length() const;
    Q_INVOKABLE ValueRange scalarRange() const;

private:
    vtkDataSet* m_vtkDataSet;
};

#endif
