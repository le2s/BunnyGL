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

#ifndef C_VTK_STRUCTURED_GRID_GEOMETRY_FILTER_H
#define C_VTK_STRUCTURED_GRID_GEOMETRY_FILTER_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkStructuredGridGeometryFilter.h"

class CVtkStructuredGridGeometryFilter : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(QList<int> Extent READ extent WRITE setExtent)
    DECLARE_VTK_OBJECT

public:
    CVtkStructuredGridGeometryFilter();
    ~CVtkStructuredGridGeometryFilter();

    void setExtent(const QList<int>& exts);
    QList<int> extent() const;

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkStructuredGridGeometryFilter* m_vtkStructuredGridGeometryFilter;
};

#endif
