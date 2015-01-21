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

#include "CVtkGaussianCubeReader.h"
#include "CVtkVisSystemComponent.h"

DEFINE_VTK_OBJECT(CVtkGaussianCubeReader,CVtkMoleculeReaderBase,vtkGaussianCubeReader)
{
    pDesc->setNodeClassName("vtkGaussianCubeReader");
    pDesc->setNodeClassDescription("Read ASCII Gaussian Cube Data files");

    pDesc->setPropertyInfo("FileName", "SetFileName", "GetFileName", QStringList() << "QString", QStringList(), "void", "QString");
}

CVtkGaussianCubeReader::CVtkGaussianCubeReader() : m_vtkGaussianCubeReader(0)
{
    CVtkGaussianCubeReader::InitializeObjectDesc();
    setVtkObject(vtkGaussianCubeReader::New());
}

CVtkGaussianCubeReader::~CVtkGaussianCubeReader()
{

}

QString CVtkGaussianCubeReader::fileName() const
{
    return m_fileName;
}


void CVtkGaussianCubeReader::setFileName(QString val)
{
    m_fileName = val;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_fileName);
    m_vtkGaussianCubeReader->SetFileName(qPrintable(fn));
}

