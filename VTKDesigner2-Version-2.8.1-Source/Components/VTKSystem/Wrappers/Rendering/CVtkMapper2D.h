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

#ifndef C_VTK_MAPPER2D_H
#define C_VTK_MAPPER2D_H

#include "CVtkObject.h"
#include "vtkMapper2D.h"
#include "CVtkMapper2DData.h"

class CVtkMapper2D : public CVtkObject
{
    Q_OBJECT
    DECLARE_VTK_OBJECT

public:
    CVtkMapper2D();
    ~CVtkMapper2D();

protected:
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

protected:
    vtkMapper2D* m_vtkMapper2D;
    CVtkMapper2DData m_mapper2DData;
};

#endif
