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
#ifndef C_VTK_D_I_C_O_M_IMAGE_READER_H
#define C_VTK_D_I_C_O_M_IMAGE_READER_H

#include "vtkDICOMImageReader.h"
#include "CVtkImageReader2.h"
#include <QVector>

class CVtkDICOMImageReader : public CVtkImageReader2
{
    Q_OBJECT
    Q_PROPERTY(QString DirectoryName READ directoryName WRITE setDirectoryName)
    DECLARE_VTK_OBJECT

public:

    CVtkDICOMImageReader();
    ~CVtkDICOMImageReader();

    QString directoryName() const;
    void setDirectoryName(QString val);

    Q_INVOKABLE QVector<double> pixelSpacing() const;
    Q_INVOKABLE int width() const;
    Q_INVOKABLE int height() const;
    Q_INVOKABLE QVector<double> imagePositionPatient() const;
    Q_INVOKABLE QVector<double> imageOrientationPatient() const;
    Q_INVOKABLE int bitsAllocated() const;
    Q_INVOKABLE int pixelRepresentation() const;
    Q_INVOKABLE int numberOfComponents() const;
    Q_INVOKABLE QString transferSyntaxUID() const;
    Q_INVOKABLE double rescaleSlope() const;
    Q_INVOKABLE double rescaleOffset() const;
    Q_INVOKABLE QString patientName() const;
    Q_INVOKABLE QString studyUID() const;
    Q_INVOKABLE QString studyID() const;
    Q_INVOKABLE double gantryAngle() const;

private:
    vtkDICOMImageReader *m_vtkDICOMImageReader;
};
#endif//C_VTK_D_I_C_O_M_IMAGE_READER_H