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
#include "CVtkPlatonicSolidSource.h"

DEFINE_VTK_OBJECT(CVtkPlatonicSolidSource,CVtkPolyDataAlgorithm,vtkPlatonicSolidSource)
{
    pDesc->setNodeClassName("vtkPlatonicSolidSource");
    pDesc->setNodeClassDescription("Produce polygonal Platonic solids");
    pDesc->setNodeClassCategory("PolyDataSource");

    pDesc->setPropertyInfo("SolidType", "SetSolidType", "GetSolidType", QStringList() << "int", QStringList(), "void", "int");
}

CVtkPlatonicSolidSource::CVtkPlatonicSolidSource() : m_vtkPlatonicSolidSource(0)
{
    CVtkPlatonicSolidSource::InitializeObjectDesc();
    setVtkObject(vtkPlatonicSolidSource::New());
}

CVtkPlatonicSolidSource::~CVtkPlatonicSolidSource()
{

}

CVtkPlatonicSolidSource::SolidType CVtkPlatonicSolidSource::solidType() const
{
    switch(m_vtkPlatonicSolidSource->GetSolidType())
    {
        case VTK_SOLID_TETRAHEDRON:return (TetraHedron);
        case VTK_SOLID_CUBE:return (Cube);
        case VTK_SOLID_OCTAHEDRON:return (Octahedron);
        case VTK_SOLID_ICOSAHEDRON:return (IcosaHedron);
        case VTK_SOLID_DODECAHEDRON:return (DoDecaHedron);
        default: return (TetraHedron);
    }
}

void CVtkPlatonicSolidSource::setSolidType(SolidType val)
{
    switch(val)
    {
        case TetraHedron:
            m_vtkPlatonicSolidSource->SetSolidType(VTK_SOLID_TETRAHEDRON);
            break;
        case Cube:
            m_vtkPlatonicSolidSource->SetSolidType(VTK_SOLID_CUBE);
            break;
        case Octahedron:
            m_vtkPlatonicSolidSource->SetSolidType(VTK_SOLID_OCTAHEDRON);
            break;
        case IcosaHedron:
            m_vtkPlatonicSolidSource->SetSolidType(VTK_SOLID_ICOSAHEDRON);
            break;
        case DoDecaHedron:
            m_vtkPlatonicSolidSource->SetSolidType(VTK_SOLID_DODECAHEDRON);
            break;
        default:  m_vtkPlatonicSolidSource->SetSolidType(VTK_SOLID_TETRAHEDRON);
            break;
 
     }
}

