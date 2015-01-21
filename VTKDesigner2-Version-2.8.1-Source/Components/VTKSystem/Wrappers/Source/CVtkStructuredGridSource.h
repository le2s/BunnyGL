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

#ifndef C_VTK_STRUCTURED_GRID_SOURCE_H
#define C_VTK_STRUCTURED_GRID_SOURCE_H

#include "vtkStructuredGridSource.h"
#include "CVtkAlgorithm.h"
#include "CVtkDataObject.h"
#include "CVtkStructuredGrid.h"

class CVtkStructuredGridSource : public CVtkAlgorithm
{
    Q_OBJECT
    DECLARE_VTK_OBJECT

public:
    CVtkStructuredGridSource();
    ~CVtkStructuredGridSource();

protected:
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkStructuredGridSource *m_vtkStructuredGridSource;
    CVtkStructuredGrid m_CVtkStructuredGrid;
};


#endif //C_VTK_UNSTRUCTURED_GRID_AGLORITHM_H
