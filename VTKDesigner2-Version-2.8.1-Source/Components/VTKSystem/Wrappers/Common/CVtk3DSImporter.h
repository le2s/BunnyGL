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

#ifndef C_VTK_3DSIMPORTER_H
#define C_VTK_3DSIMPORTER_H

#include "CVtkImporter.h"
#include "vtk3DSImporter.h"

class CVtk3DSImporter : public CVtkImporter
{
    Q_OBJECT
    Q_PROPERTY(QString FileName READ fileName WRITE setFileName)
    Q_PROPERTY(bool ComputeNormals READ setComputeNormals READ isComputeNormals)
    DECLARE_VTK_OBJECT

public:
    CVtk3DSImporter();
    ~CVtk3DSImporter();

    void setFileName(QString fileName);
    QString fileName() const;

    void setComputeNormals(bool val);
    bool isComputeNormals() const;

private:
    vtk3DSImporter* m_vtk3DSImporter;
    QString m_fileName;
};

#endif

