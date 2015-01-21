/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
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

#ifndef C_VTK_MOLECULE_READER_BASE_H
#define C_VTK_MOLECULE_READER_BASE_H

#include "vtkMoleculeReaderBase.h"
#include "CVtkPolyDataAlgorithm.h"

class CVtkMoleculeReaderBase : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(QString FileName READ fileName WRITE setFileName)
    Q_PROPERTY(double BScale READ bScale WRITE setBScale)
    Q_PROPERTY(double HBScale READ hBScale WRITE setHBScale)
    DECLARE_VTK_OBJECT

public:

    CVtkMoleculeReaderBase();
    ~CVtkMoleculeReaderBase();

    QString fileName() const;
    void setFileName(QString val);

    double bScale() const;
    void setBScale(double val);

    double hBScale() const;
    void setHBScale(double val);


private:
    vtkMoleculeReaderBase *m_vtkMoleculeReaderBase;
    QString m_fileName;
};
#endif//C_VTK_MOLECULE_READER_BASE_H