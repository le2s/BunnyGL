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
#include "CVtkDICOMImageReader.h"

DEFINE_VTK_OBJECT(CVtkDICOMImageReader,CVtkImageReader2,vtkDICOMImageReader)
{
    pDesc->setNodeClassName("vtkDICOMImageReader");
    pDesc->setNodeClassDescription("Reads some DICOM images");
    pDesc->setNodeClassCategory("Image Readers");

    pDesc->setPropertyInfo("DirectoryName", "SetDirectoryName", "GetDirectoryName", QStringList() << "QString", QStringList(), "void", "QString");
}

CVtkDICOMImageReader::CVtkDICOMImageReader() : m_vtkDICOMImageReader(0)
{
    CVtkDICOMImageReader::InitializeObjectDesc();
    setVtkObject(vtkDICOMImageReader::New());
}

CVtkDICOMImageReader::~CVtkDICOMImageReader()
{

}

QString CVtkDICOMImageReader::directoryName() const
{
    return m_vtkDICOMImageReader->GetDirectoryName();
}


void CVtkDICOMImageReader::setDirectoryName(QString val)
{
    m_vtkDICOMImageReader->SetDirectoryName(qPrintable(val));
}


/**
 \todo 
*/ 
QVector<double> CVtkDICOMImageReader::pixelSpacing() const
{
	QVector<double> ret(3);
    memcpy(ret.data(), m_vtkDICOMImageReader->GetPixelSpacing(), sizeof(double)*3);
    return ret;
}

/**
 \todo 
*/ 
int CVtkDICOMImageReader::width() const
{
	return m_vtkDICOMImageReader->GetWidth();
}

/**
 \todo 
*/ 
int CVtkDICOMImageReader::height() const
{
	return m_vtkDICOMImageReader->GetHeight();
}

/**
 \todo 
*/ 
QVector<double> CVtkDICOMImageReader::imagePositionPatient() const
{
	QVector<double> ret(3);
    ret[0] = (double)m_vtkDICOMImageReader->GetImagePositionPatient()[0];
    ret[1] = (double)m_vtkDICOMImageReader->GetImagePositionPatient()[1];
    ret[2] = (double)m_vtkDICOMImageReader->GetImagePositionPatient()[2];
    return ret;
}

/**
 \todo 
*/ 
QVector<double> CVtkDICOMImageReader::imageOrientationPatient() const
{
	QVector<double> ret(3);
    ret[0] = (double)m_vtkDICOMImageReader->GetImageOrientationPatient()[0];
    ret[1] = (double)m_vtkDICOMImageReader->GetImageOrientationPatient()[1];
    ret[2] = (double)m_vtkDICOMImageReader->GetImageOrientationPatient()[2];
    return ret;
}

/**
 \todo 
*/ 
int CVtkDICOMImageReader::bitsAllocated() const
{
	return m_vtkDICOMImageReader->GetBitsAllocated();
}

/**
 \todo 
*/ 
int CVtkDICOMImageReader::pixelRepresentation() const
{
	return m_vtkDICOMImageReader->GetPixelRepresentation();
}

/**
 \todo 
*/ 
int CVtkDICOMImageReader::numberOfComponents() const
{
	return m_vtkDICOMImageReader->GetNumberOfComponents();
}

/**
 \todo 
*/ 
QString CVtkDICOMImageReader::transferSyntaxUID() const
{
	return QString(m_vtkDICOMImageReader->GetTransferSyntaxUID());
}

/**
 \todo 
*/ 
double CVtkDICOMImageReader::rescaleSlope() const
{
	return m_vtkDICOMImageReader->GetRescaleSlope();
}

/**
 \todo 
*/ 
double CVtkDICOMImageReader::rescaleOffset() const
{
	return m_vtkDICOMImageReader->GetRescaleOffset();
}

/**
 \todo 
*/ 
QString CVtkDICOMImageReader::patientName() const
{
	return QString(m_vtkDICOMImageReader->GetPatientName());
}

/**
 \todo 
*/ 
QString CVtkDICOMImageReader::studyUID() const
{
	return QString(m_vtkDICOMImageReader->GetStudyUID());
}

/**
 \todo 
*/ 
QString CVtkDICOMImageReader::studyID() const
{
	return QString(m_vtkDICOMImageReader->GetStudyID());
}

/**
 \todo 
*/ 
double CVtkDICOMImageReader::gantryAngle() const
{
	return m_vtkDICOMImageReader->GetGantryAngle();
}

