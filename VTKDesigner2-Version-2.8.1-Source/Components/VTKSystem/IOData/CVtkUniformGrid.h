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

#ifndef C_VTK_UNIFORM_GRID_H
#define C_VTK_UNIFORM_GRID_H

#include "IVtkDataObjectIOData.h"
#include "IVtkDataSetIOData.h"
#include "IVtkImageDataIOData.h"
#include "IVtkUniformGridIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkUniformGrid.h"

class CVtkUniformGrid : virtual public IVtkDataObjectIOData,
                        virtual public IVtkDataSetIOData,
                        virtual public IVtkImageDataIOData,
                        virtual public IVtkUniformGridIOData,
                        virtual public IVtkObjectIOData
{
public:
    CVtkUniformGrid(vtkUniformGrid* dataObject=0);
    ~CVtkUniformGrid();

    void setUniformGrid(vtkUniformGrid* dataObject);

    // IQueryable implementation
    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IVtkDataObjectIOData implementation
    vtkDataObject* getVtkDataObject();

    // IVtkDataSetIOData implementation
    vtkDataSet* getVtkDataSet();

    // IVtkImageDataIOData implementation
    vtkImageData* getVtkImageData();

    // IVtkUniformGridIOData implementation
    vtkUniformGrid* getVtkUniformGrid();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkUniformGrid* m_vtkUniformGrid;
};

#endif

