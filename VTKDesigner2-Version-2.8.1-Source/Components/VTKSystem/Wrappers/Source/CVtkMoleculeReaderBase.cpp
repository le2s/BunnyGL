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

#include "CVtkMoleculeReaderBase.h"
#include "CVtkVisSystemComponent.h"

DEFINE_VTK_OBJECT(CVtkMoleculeReaderBase,CVtkPolyDataAlgorithm,vtkMoleculeReaderBase)
{
    pDesc->setNodeClassName("vtkMoleculeReaderBase");
    pDesc->setNodeClassDescription("Read Molecular Data files");
    pDesc->setNodeClassCategory("Readers");

    pDesc->setPropertyInfo("FileName", "SetFileName", "GetFileName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("BScale", "SetBScale", "GetBScale", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("HBScale", "SetHBScale", "GetHBScale", QStringList() << "double", QStringList(), "void", "double");
}

CVtkMoleculeReaderBase::CVtkMoleculeReaderBase() : m_vtkMoleculeReaderBase(0)
{
    CVtkMoleculeReaderBase::InitializeObjectDesc();
}

CVtkMoleculeReaderBase::~CVtkMoleculeReaderBase()
{

}

QString CVtkMoleculeReaderBase::fileName() const
{
    return m_fileName;
}


void CVtkMoleculeReaderBase::setFileName(QString val)
{
    m_fileName = val;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_fileName);
    m_vtkMoleculeReaderBase->SetFileName(qPrintable(fn));
}


double CVtkMoleculeReaderBase::bScale() const
{
    return m_vtkMoleculeReaderBase->GetBScale();
}


void CVtkMoleculeReaderBase::setBScale(double val)
{
    m_vtkMoleculeReaderBase->SetBScale(val);
}


double CVtkMoleculeReaderBase::hBScale() const
{
    return m_vtkMoleculeReaderBase->GetHBScale();
}


void CVtkMoleculeReaderBase::setHBScale(double val)
{
    m_vtkMoleculeReaderBase->SetHBScale(val);
}

