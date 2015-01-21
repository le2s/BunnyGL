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
#include "CVtkVolumeReader.h"

DEFINE_VTK_OBJECT(CVtkVolumeReader,CVtkImageAlgorithm,vtkVolumeReader)
{
    pDesc->setNodeClassName("vtkVolumeReader");
    pDesc->setNodeClassDescription("read image files");

    pDesc->setPropertyInfo("FilePrefix", "SetFilePrefix", "GetFilePrefix", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("FilePattern", "SetFilePattern", "GetFilePattern", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("DataSpacing", "SetDataSpacing", "GetDataSpacing", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("DataOrigin", "SetDataOrigin", "GetDataOrigin", QStringList() << "double" << "double" << "double", QStringList(), "void", "double*");

    pDesc->setCreateFunction(0);
}

CVtkVolumeReader::CVtkVolumeReader() : m_vtkVolumeReader(0)
{
    CVtkVolumeReader::InitializeObjectDesc();
}

CVtkVolumeReader::~CVtkVolumeReader()
{

}

QString CVtkVolumeReader::filePrefix() const
{
    return m_vtkVolumeReader->GetFilePrefix();
}


void CVtkVolumeReader::setFilePrefix(QString val)
{
    m_vtkVolumeReader->SetFilePrefix(qPrintable(val));
}


QString CVtkVolumeReader::filePattern() const
{
    return m_vtkVolumeReader->GetFilePattern();
}


void CVtkVolumeReader::setFilePattern(QString val)
{
    m_vtkVolumeReader->SetFilePattern(qPrintable(val));
}


Point3D CVtkVolumeReader::dataSpacing() const
{
    return Point3D(m_vtkVolumeReader->GetDataSpacing());
}


void CVtkVolumeReader::setDataSpacing(Point3D val)
{
    m_vtkVolumeReader->SetDataSpacing(val.x, val.y, val.z);
}


Point3D CVtkVolumeReader::dataOrigin() const
{
    return Point3D(m_vtkVolumeReader->GetDataOrigin());
}


void CVtkVolumeReader::setDataOrigin(Point3D val)
{
    m_vtkVolumeReader->SetDataOrigin(val.x, val.y, val.z);
}


void CVtkVolumeReader::setImageRange(QList<QVariant> range)
{
    if(range.count() != 2)
        return;

    m_vtkVolumeReader->SetImageRange( range.first().toInt(), range.last().toInt() );
}


QList<QVariant> CVtkVolumeReader::imageRange() const
{
    QList<QVariant> ret = QList<QVariant>() << m_vtkVolumeReader->GetImageRange()[0]
                                  << m_vtkVolumeReader->GetImageRange()[1];
    return ret;
}




