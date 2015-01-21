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

#ifndef C_VTK_ABSTRACT_TRANSFORM_DATA_H
#define C_VTK_ABSTRACT_TRANSFORM_DATA_H

#include "IVtkAbstractTransformIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkAbstractTransform.h"

class CVtkAbstractTransformData : virtual public IVtkAbstractTransformIOData,
                                  virtual public IVtkObjectIOData
{
public:
    CVtkAbstractTransformData(vtkAbstractTransform* abstractTransform=0);
    ~CVtkAbstractTransformData();

    void setAbstractTransform(vtkAbstractTransform* abstractTransform);

    // IVisSystemNodeIOData implementation
        void* dataPointer();
        QString dataTypeName();

    // IQueryable implementation.
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IVtkAbstractTransform implementation
    vtkAbstractTransform* getVtkAbstractTransform();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkAbstractTransform* m_vtkAbstractTransform;
};

#endif