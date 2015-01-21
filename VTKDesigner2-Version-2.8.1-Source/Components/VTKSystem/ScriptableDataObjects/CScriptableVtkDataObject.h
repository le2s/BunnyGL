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

#ifndef C_SCRIPTABLE_VTK_DATA_OBJECT_H
#define C_SCRIPTABLE_VTK_DATA_OBJECT_H

#include <QObject>
#include "vtkDataObject.h"

class CVtkObject;
class CScriptableVtkDataObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ReleaseDataFlag READ isReleaseData WRITE setReleaseData)

public:
    CScriptableVtkDataObject(vtkDataObject* dataObject, CVtkObject* source);
    ~CScriptableVtkDataObject();

    Q_INVOKABLE QObject* source() const;
    vtkDataObject* dataObject() const;

    Q_INVOKABLE int dataObjectType() const;
    Q_INVOKABLE unsigned long updateTime() const;
    Q_INVOKABLE unsigned long actualMemorySize() const;

    void setReleaseData(bool val);
    bool isReleaseData() const;

private:
    vtkDataObject* m_vtkDataObject;
    CVtkObject* m_CVtkObject;
};

#endif
