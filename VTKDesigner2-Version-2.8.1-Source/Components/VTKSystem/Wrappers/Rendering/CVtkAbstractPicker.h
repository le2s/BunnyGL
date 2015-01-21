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

#ifndef C_VTK_ABSTRACT_PICKER_H
#define C_VTK_ABSTRACT_PICKER_H

#include "CVtkObject.h"
#include "vtkAbstractPicker.h"
#include "CVtkAbstractPickerData.h"
#include "UserDefinedDataTypes.h"

class vtkRenderer;
class CVtkAbstractPicker : public CVtkObject
{
    Q_OBJECT
    DECLARE_VTK_OBJECT

public:
    CVtkAbstractPicker();
    ~CVtkAbstractPicker();

    Q_INVOKABLE Point3D getSelectionPoint();
    Q_INVOKABLE Point3D getPickPosition();
    Q_INVOKABLE int pick(Point3D point);
    Q_INVOKABLE int pick(Point3D point, QObject* rendererWrapper);

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkAbstractPicker* m_vtkAbstractPicker;
    CVtkAbstractPickerData m_abstractPickerData;
    vtkRenderer* m_vtkRenderer;
};

#endif
