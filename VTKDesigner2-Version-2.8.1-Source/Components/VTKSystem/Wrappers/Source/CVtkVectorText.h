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

#ifndef C_VTK_VECTOR_TEXT_H
#define C_VTK_VECTOR_TEXT_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkVectorText.h"

class CVtkVectorText : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(QString Text READ text WRITE setText)
    DECLARE_VTK_OBJECT

public:
    CVtkVectorText();
    ~CVtkVectorText();

    void setText(QString str);
    QString text() const;
    
private:
    vtkVectorText* m_vtkVectorText;
};
#endif

