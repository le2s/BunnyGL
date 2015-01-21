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

#ifndef C_VTK_XYZ_MOL_READER_H
#define C_VTK_XYZ_MOL_READER_H

#include "vtkXYZMolReader.h"
#include "CVtkMoleculeReaderBase.h"

class CVtkXYZMolReader : public CVtkMoleculeReaderBase
{
    Q_OBJECT
    Q_PROPERTY(int TimeStep READ timeStep WRITE setTimeStep)
    DECLARE_VTK_OBJECT

public:

    CVtkXYZMolReader();
    ~CVtkXYZMolReader();

    int timeStep() const;
    void setTimeStep(int val);


private:
    vtkXYZMolReader *m_vtkXYZMolReader;
};
#endif//C_VTK_X_Y_Z_MOL_READER_H