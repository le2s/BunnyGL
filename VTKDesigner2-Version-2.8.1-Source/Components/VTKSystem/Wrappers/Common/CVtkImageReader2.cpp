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

#include "CVtkImageReader2.h"
#include "CScriptableVtkImageData.h"
#include "CVtkVisSystemComponent.h"

DEFINE_VTK_OBJECT(CVtkImageReader2, CVtkImageAlgorithm, vtkImageReader2)
{
    pDesc->setNodeClassName("vtkImageReader2");
    pDesc->setNodeClassCategory("Image Readers");
    pDesc->setNodeClassDescription("Generic Image Reader");
    pDesc->setCreateFunction(0);

    pDesc->setPropertyInfo("FileName", "SetFileName", "GetFileName", QStringList() << "const char*",
                            QStringList(), "void", "const char*");
    pDesc->setPropertyInfo("DataScalarType", "SetDataScalarType", "GetDataScalarType",
                            QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("FileDimensionality", "SetFileDimensionality", "GetFileDimensionality",
                            QStringList() << "int", QStringList(), "void", "int");
}

CVtkImageReader2::CVtkImageReader2() : m_vtkImageReader2(0), m_imageDataObject(0)
{
    CVtkImageReader2::InitializeObjectDesc();
}

CVtkImageReader2::~CVtkImageReader2()
{
    if(m_imageDataObject)
        delete m_imageDataObject;
}

void CVtkImageReader2::setFileName(QString fileName)
{
    if(m_vtkImageReader2)
    {
        m_fileName = fileName;
        QString fn = CVtkVisSystemComponent::instance().resolvePath(m_fileName);
        m_vtkImageReader2->SetFileName(qPrintable(fn));
    }
}

QString CVtkImageReader2::fileName() const
{
    return m_fileName;
}

void CVtkImageReader2::setDataScalarType(CVtkImageReader2::DataScalarType val)
{
    if(!m_vtkImageReader2)
        return;

    switch(val)
    {
    case Float: m_vtkImageReader2->SetDataScalarType(VTK_FLOAT); break;
    case Double: m_vtkImageReader2->SetDataScalarType(VTK_DOUBLE); break;
    case Int: m_vtkImageReader2->SetDataScalarType(VTK_INT); break;
    case Short: m_vtkImageReader2->SetDataScalarType(VTK_SHORT); break;
    case UnsignedShort: m_vtkImageReader2->SetDataScalarType(VTK_UNSIGNED_SHORT); break;
    case UnsignedChar: m_vtkImageReader2->SetDataScalarType(VTK_UNSIGNED_CHAR); break;
    default: break;
    }
}

CVtkImageReader2::DataScalarType CVtkImageReader2::dataScalarType() const
{
    int type = m_vtkImageReader2 ? m_vtkImageReader2->GetDataScalarType() : -1;

    switch(type)
    {
    case VTK_FLOAT: return Float;
    case VTK_DOUBLE: return Double;
    case VTK_INT: return Int;
    case VTK_SHORT: return Short;
    case VTK_UNSIGNED_SHORT: return UnsignedShort;
    case VTK_UNSIGNED_CHAR: return UnsignedChar;
    default: break;
    }
    return Float;
}


void CVtkImageReader2::setFileDimensionality(int dim)
{
    if(m_vtkImageReader2)
        m_vtkImageReader2->SetFileDimensionality(dim);
}

int CVtkImageReader2::fileDimensionality() const
{
    if(m_vtkImageReader2)
        return m_vtkImageReader2->GetFileDimensionality();

    return 2;
}

QObject* CVtkImageReader2::imageDataObject()
{
    if(!m_imageDataObject)
        m_imageDataObject = new CScriptableVtkImageData(m_vtkImageReader2->GetOutput(), this);
    return m_imageDataObject;
}
