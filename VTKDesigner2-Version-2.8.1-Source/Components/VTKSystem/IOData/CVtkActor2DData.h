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

#ifndef C_VTK_ACTOR2D_DATA_H
#define C_VTK_ACTOR2D_DATA_H

#include "IVtkActor2DIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkActor2D.h"

class CVtkActor2DData : virtual public IVtkActor2DIOData,
                        virtual public IVtkObjectIOData
{
public:
    CVtkActor2DData(vtkActor2D* actor=0);
    ~CVtkActor2DData();

    void setActor2D(vtkActor2D* actor);

    // IQueryable implementation
    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IVtkActor2DIOData implementation
    vtkActor2D* getVtkActor2D();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkActor2D* m_vtkActor2D;
};

#endif

