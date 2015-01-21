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

#ifndef C_VTK_OPEN_HAPTICS_Effect_IO_DATA_H
#define C_VTK_OPEN_HAPTICS_Effect_IO_DATA_H

#include "IVtkOpenHapticsEffectIOData.h"
#include "IVtkObjectIOData.h"

class CVtkOpenHapticsEffectIOData : virtual public IVtkOpenHapticsEffectIOData,
                                     virtual public IVtkObjectIOData
{
public:
    CVtkOpenHapticsEffectIOData(vtkOpenHapticsEffect* object=0);
    ~CVtkOpenHapticsEffectIOData();

    void setObject(vtkOpenHapticsEffect* object);

    // IQueryable implementation
    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IvtkOpenHapticsEffectIOData implementation
    vtkOpenHapticsEffect* getVtkOpenHapticsEffect();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkOpenHapticsEffect* m_vtkOpenHapticsEffect;
};

#endif

