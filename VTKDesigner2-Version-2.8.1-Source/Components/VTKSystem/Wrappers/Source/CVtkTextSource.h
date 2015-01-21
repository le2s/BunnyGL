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

#ifndef C_VTK_TEXT_SOURCE_H
#define C_VTK_TEXT_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkTextSource.h"

class CVtkTextSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(QString Text READ text WRITE setText)
    Q_PROPERTY(bool Backing READ isBacking WRITE setBacking)
    DECLARE_VTK_OBJECT

public:
    CVtkTextSource();
    ~CVtkTextSource();

    // Property methods
    void setText(QString str);
    QString text() const;
    
    void setBacking(bool res);
    bool isBacking() const;

private:
    vtkTextSource* m_vtkTextSource;
};

#endif

