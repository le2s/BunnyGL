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
#ifndef C_VTK_VOLUME_READER_H
#define C_VTK_VOLUME_READER_H

#include "vtkVolumeReader.h"
#include "CVtkImageAlgorithm.h"
#include "UserDefinedDataTypes.h"

class CVtkVolumeReader : public CVtkImageAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(QString FilePrefix READ filePrefix WRITE setFilePrefix)
    Q_PROPERTY(QString FilePattern READ filePattern WRITE setFilePattern)
    Q_PROPERTY(Point3D DataSpacing READ dataSpacing WRITE setDataSpacing)
    Q_PROPERTY(Point3D DataOrigin READ dataOrigin WRITE setDataOrigin)
    Q_PROPERTY(QList<QVariant> ImageRange READ imageRange WRITE setImageRange)
    DECLARE_VTK_OBJECT

public:

    CVtkVolumeReader();
    ~CVtkVolumeReader();

    QString filePrefix() const;
    void setFilePrefix(QString val);

    QString filePattern() const;
    void setFilePattern(QString val);

    Point3D dataSpacing() const;
    void setDataSpacing(Point3D val);

    Point3D dataOrigin() const;
    void setDataOrigin(Point3D val);

    void setImageRange(QList<QVariant> range);
    QList<QVariant> imageRange() const;

private:
    vtkVolumeReader *m_vtkVolumeReader;
};
#endif//C_VTK_VOLUME_READER_H