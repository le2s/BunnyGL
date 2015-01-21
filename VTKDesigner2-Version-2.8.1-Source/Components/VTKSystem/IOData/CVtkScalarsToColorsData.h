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

#ifndef C_VTK_SCALARS_TO_COLORS_DATA_H
#define C_VTK_SCALARS_TO_COLORS_DATA_H

#include "IVtkScalarsToColorsIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkScalarsToColors.h"

class CVtkScalarsToColorsData : virtual public IVtkScalarsToColorsIOData,
                                virtual public IVtkObjectIOData
{
public:
    CVtkScalarsToColorsData(vtkScalarsToColors* scalarsTocolors=0);
    ~CVtkScalarsToColorsData();

    void setScalarsToColors(vtkScalarsToColors* scalarsTocolors);

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IQueryable implementation.
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IVtkScalarsToColors implementation
    vtkScalarsToColors* getVtkScalarsToColors();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkScalarsToColors* m_vtkScalarsToColors;
};

#endif


