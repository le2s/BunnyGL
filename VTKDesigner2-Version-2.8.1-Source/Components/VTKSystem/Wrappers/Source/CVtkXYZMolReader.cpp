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

#include "CVtkXYZMolReader.h"

DEFINE_VTK_OBJECT(CVtkXYZMolReader,CVtkMoleculeReaderBase,vtkXYZMolReader)
{
    pDesc->setNodeClassName("vtkXYZMolReader");
    pDesc->setNodeClassDescription("Read Molecular Data files");

    pDesc->setPropertyInfo("TimeStep", "SetTimeStep", "GetTimeStep", QStringList() << "int", QStringList(), "void", "int");
}

CVtkXYZMolReader::CVtkXYZMolReader() : m_vtkXYZMolReader(0)
{
    CVtkXYZMolReader::InitializeObjectDesc();
    setVtkObject(vtkXYZMolReader::New());
}

CVtkXYZMolReader::~CVtkXYZMolReader()
{

}

int CVtkXYZMolReader::timeStep() const
{
    return m_vtkXYZMolReader->GetTimeStep();
}


void CVtkXYZMolReader::setTimeStep(int val)
{
    m_vtkXYZMolReader->SetTimeStep(val);
}

