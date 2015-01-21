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

#include "CVtkDEMReader.h"
#include "CVtkVisSystemComponent.h"

DEFINE_VTK_OBJECT(CVtkDEMReader, CVtkImageAlgorithm, vtkDEMReader)
{
    pDesc->setNodeClassName("vtkDEMReader");
    pDesc->setNodeClassCategory("Readers");
    pDesc->setNodeClassDescription("read a digital elevation model (DEM) file");
}

CVtkDEMReader::CVtkDEMReader() : m_vtkDEMReader(0)
{
    CVtkDEMReader::InitializeObjectDesc();
    setVtkObject(vtkDEMReader::New());
}

CVtkDEMReader::~CVtkDEMReader()
{

}

void CVtkDEMReader::setFileName(QString fileName)
{
    m_fileName = fileName;
    QString fn = CVtkVisSystemComponent::instance().resolvePath(m_fileName);
    m_vtkDEMReader->SetFileName( qPrintable(fn) );
}

QString CVtkDEMReader::fileName() const
{
    return m_fileName;
}

void CVtkDEMReader::setElevReference(CVtkDEMReader::ElevReference ref)
{
    m_vtkDEMReader->SetElevationReference ( (int)ref );
}

CVtkDEMReader::ElevReference CVtkDEMReader::elevReference() const
{
    return CVtkDEMReader::ElevReference( m_vtkDEMReader->GetElevationReference() );
}

QString CVtkDEMReader::mapLabel() const
{
    return QString( m_vtkDEMReader->GetMapLabel() );
}

int CVtkDEMReader::demLevel() const
{
    return m_vtkDEMReader->GetDEMLevel();
}

int CVtkDEMReader::elevationPattern() const
{
    return m_vtkDEMReader->GetElevationPattern();
}

int CVtkDEMReader::groundSystem() const
{
    return m_vtkDEMReader->GetGroundSystem();
}

int CVtkDEMReader::groundZone() const
{
    return m_vtkDEMReader->GetGroundZone();
}

QList<double> CVtkDEMReader::projectParameters() const
{
    QList<double> ret;
    float retArray[15];
    m_vtkDEMReader->GetProjectionParameters(retArray);
    for(int i=0; i<15; i++)
        ret << (double)retArray[i];
    return ret;
}

int CVtkDEMReader::planeUnitOfMeasure() const
{
    return m_vtkDEMReader->GetPlaneUnitOfMeasure();
}

int CVtkDEMReader::elevationUnitOfMeasure() const
{
    return m_vtkDEMReader->GetElevationUnitOfMeasure();
}

int CVtkDEMReader::polygonSize() const
{
    return m_vtkDEMReader->GetPolygonSize();
}

QList<double> CVtkDEMReader::elevationBounds() const
{
    QList<double> ret;
    float retArray[2];
    m_vtkDEMReader->GetElevationBounds(retArray);
    ret << (double)retArray[0] << (double)retArray[1];
    return ret;
}

double CVtkDEMReader::localRotation() const
{
    return (double)(m_vtkDEMReader->GetLocalRotation());
}

int CVtkDEMReader::accuracyCode() const
{
    return m_vtkDEMReader->GetAccuracyCode();
}

QList<double> CVtkDEMReader::spatialResolution() const
{
    QList<double> ret;
    float retArray[3];
    m_vtkDEMReader->GetSpatialResolution(retArray);
    ret << (double)retArray[0] << (double)retArray[1] << (double)retArray[2];
    return ret;
}

QList<int> CVtkDEMReader::profileDimension() const
{
    QList<int> ret;
    int retArray[2];
    m_vtkDEMReader->GetProfileDimension(retArray);
    ret << retArray[0] << retArray[1];
    return ret;
}


