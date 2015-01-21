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

#include "CVtkBYUReader.h"
#include "CVtkVisSystemComponent.h"

DEFINE_VTK_OBJECT(CVtkBYUReader,CVtkPolyDataAlgorithm,vtkBYUReader)
{
    pDesc->setNodeClassName("vtkBYUReader");
    pDesc->setNodeClassDescription("Read MOVIE.BYU polygon files");
    pDesc->setNodeClassCategory("PolyDataSource");

    pDesc->setPropertyInfo("GeometryFileName", "SetGeometryFileName", "GetGeometryFileName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("FileName", "SetFileName", "GetFileName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("DisplacementFileName", "SetDisplacementFileName", "GetDisplacementFileName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("ScalarFileName", "SetScalarFileName", "GetScalarFileName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("TextureFileName", "SetTextureFileName", "GetTextureFileName", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("ReadDisplacement", "SetReadDisplacement", "GetReadDisplacement", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ReadScalar", "SetReadScalar", "GetReadScalar", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ReadTexture", "SetReadTexture", "GetReadTexture", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("PartNumber", "SetPartNumber", "GetPartNumber", QStringList() << "int", QStringList(), "void", "int");
}

CVtkBYUReader::CVtkBYUReader() : m_vtkBYUReader(0)
{
    CVtkBYUReader::InitializeObjectDesc();
    setVtkObject(vtkBYUReader::New());
}

CVtkBYUReader::~CVtkBYUReader()
{

}

QString CVtkBYUReader::geometryFileName() const
{
    return m_geometryFileName;
}


void CVtkBYUReader::setGeometryFileName(QString val)
{
    m_geometryFileName = val;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_geometryFileName);
    m_vtkBYUReader->SetGeometryFileName(qPrintable(fn));
}


QString CVtkBYUReader::fileName() const
{
    return m_fileName;
}


void CVtkBYUReader::setFileName(QString val)
{
    m_fileName = val;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_fileName);
    m_vtkBYUReader->SetFileName(qPrintable(fn));
}


QString CVtkBYUReader::displacementFileName() const
{
    return m_displacementFileName;
}


void CVtkBYUReader::setDisplacementFileName(QString val)
{
    m_displacementFileName = val;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_displacementFileName);
    m_vtkBYUReader->SetDisplacementFileName(qPrintable(fn));
}


QString CVtkBYUReader::scalarFileName() const
{
    return m_scalarFileName;
}


void CVtkBYUReader::setScalarFileName(QString val)
{
    m_scalarFileName = val;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_scalarFileName);
    m_vtkBYUReader->SetScalarFileName(qPrintable(fn));
}


QString CVtkBYUReader::textureFileName() const
{
    return m_textureFileName;
}


void CVtkBYUReader::setTextureFileName(QString val)
{
    m_textureFileName = val;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_textureFileName);
    m_vtkBYUReader->SetTextureFileName(qPrintable(fn));
}


bool CVtkBYUReader::isReadDisplacement() const
{
    return m_vtkBYUReader->GetReadDisplacement();
}


void CVtkBYUReader::setReadDisplacement(bool val)
{
    m_vtkBYUReader->SetReadDisplacement(val);
}


bool CVtkBYUReader::isReadScalar() const
{
    return m_vtkBYUReader->GetReadScalar();
}


void CVtkBYUReader::setReadScalar(bool val)
{
    m_vtkBYUReader->SetReadScalar(val);
}


bool CVtkBYUReader::isReadTexture() const
{
    return m_vtkBYUReader->GetReadTexture();
}


void CVtkBYUReader::setReadTexture(bool val)
{
    m_vtkBYUReader->SetReadTexture(val);
}


int CVtkBYUReader::partNumber() const
{
    return m_vtkBYUReader->GetPartNumber();
}


void CVtkBYUReader::setPartNumber(int val)
{
    m_vtkBYUReader->SetPartNumber(val);
}

