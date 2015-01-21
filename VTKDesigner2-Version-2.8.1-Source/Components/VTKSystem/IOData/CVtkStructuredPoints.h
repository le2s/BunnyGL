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

#ifndef C_VTK_STRUCTURED_POINTS_H
#define C_VTK_STRUCTURED_POINTS_H

#include "IVtkDataObjectIOData.h"
#include "IVtkDataSetIOData.h"
#include "IVtkImageDataIOData.h"
#include "IVtkStructuredPointsIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkStructuredPoints.h"

class CVtkStructuredPoints :  virtual public IVtkDataObjectIOData,
                              virtual public IVtkDataSetIOData,
                              virtual public IVtkImageDataIOData,
                              virtual public IVtkStructuredPointsIOData,
                              virtual public IVtkObjectIOData
{
public:
    CVtkStructuredPoints(vtkStructuredPoints* sPoints=0);
    ~CVtkStructuredPoints();

    void setStructuredPoints(vtkStructuredPoints* sPoints);

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

    // IVtkStructuredPointsIOData implementation
    vtkStructuredPoints* getVtkStructuredPoints();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkStructuredPoints* m_vtkStructuredPoints;
};

#endif

