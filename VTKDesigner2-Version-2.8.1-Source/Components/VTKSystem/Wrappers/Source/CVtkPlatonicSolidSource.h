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
#ifndef C_VTK_PLATONICSOLIDSOURCE_H
#define C_VTK_PLATONICSOLIDSOURCE_H

#include "vtkPlatonicSolidSource.h"
#include "CVtkPolyDataAlgorithm.h"

class CVtkPlatonicSolidSource : public CVtkPolyDataAlgorithm
{
    Q_OBJECT
    Q_ENUMS(SolidType)
    Q_PROPERTY(SolidType SolidType READ solidType WRITE setSolidType)
    DECLARE_VTK_OBJECT

public:
    enum SolidType
    {
        TetraHedron = VTK_SOLID_TETRAHEDRON,
        Cube =  VTK_SOLID_CUBE,
        Octahedron = VTK_SOLID_OCTAHEDRON,
        IcosaHedron = VTK_SOLID_ICOSAHEDRON,
        DoDecaHedron = VTK_SOLID_DODECAHEDRON
    };

    CVtkPlatonicSolidSource();
    ~CVtkPlatonicSolidSource();

    SolidType solidType() const;
    void setSolidType(SolidType val);


private:
    vtkPlatonicSolidSource *m_vtkPlatonicSolidSource;
};
#endif//C_VTK_PLATONICSOLIDSOURCE_H