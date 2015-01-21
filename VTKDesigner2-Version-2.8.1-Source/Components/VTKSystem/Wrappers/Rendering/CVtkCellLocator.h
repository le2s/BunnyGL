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
#ifndef C_VTK_CELL_LOCATOR_H
#define C_VTK_CELL_LOCATOR_H

#include "vtkCellLocator.h"
#include "CVtkLocator.h"

class CVtkCellLocator : public CVtkLocator
{
    Q_OBJECT
    Q_PROPERTY(int NumberOfCellsPerBucket READ numberOfCellsPerBucket WRITE setNumberOfCellsPerBucket)
    Q_PROPERTY(bool CacheCellBounds READ isCacheCellBounds WRITE setCacheCellBounds)
    DECLARE_VTK_OBJECT

public:
    CVtkCellLocator();
    ~CVtkCellLocator();

    int numberOfCellsPerBucket() const;
    void setNumberOfCellsPerBucket(int val);

    bool isCacheCellBounds() const;
    void setCacheCellBounds(bool val);

private:
    vtkCellLocator *m_vtkCellLocator;
};
#endif//C_VTK_CELL_LOCATOR_H