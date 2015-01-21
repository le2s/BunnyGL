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

#ifndef C_VTK_IMPORTER_DATA_H
#define C_VTK_IMPORTER_DATA_H

#include "IVtkImporterIOData.h"
#include "IVtkObjectIOData.h"
#include "vtkImporter.h"

class CVtkImporterData : virtual public IVtkImporterIOData,
                         virtual public IVtkObjectIOData
{
public:
    CVtkImporterData(vtkImporter* widget=0);
    ~CVtkImporterData();

    void setImporter(vtkImporter* widget);

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IQueryable implementation.
    bool queryInterface(QString iface, void** ifacePtr);
    bool destroySelf();

    // IVtkImporterIOData implementation
    vtkImporter* getVtkImporter();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkImporter* m_vtkImporter;
};

#endif