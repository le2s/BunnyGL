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

#include "CVtkSTLReader.h"

DEFINE_VTK_OBJECT(CVtkSTLReader, CVtkPolyDataAlgorithm, vtkSTLReader)
{
    pDesc->setNodeClassCategory("PolyDataSource");
    pDesc->setNodeClassName("vtkSTLReader");
    pDesc->setNodeClassDescription("STL Reader");

    pDesc->setPropertyInfo("FileName", "SetFileName", "GetFileName", QStringList() << "QString", QStringList(), "void", "const char*");
}

CVtkSTLReader::CVtkSTLReader()
{
    CVtkSTLReader::InitializeObjectDesc();
    setVtkObject(vtkSTLReader::New());
}

CVtkSTLReader::~CVtkSTLReader()
{

}

void CVtkSTLReader::setFileName(const QString& fileName)
{
    QByteArray fileName2 = fileName.toLatin1();
    m_vtkSTLReader->SetFileName(fileName2);
}

QString CVtkSTLReader::fileName() const
{
    QByteArray fileName2 = QByteArray( m_vtkSTLReader->GetFileName() );
    return QString::fromLatin1(fileName2);
}
