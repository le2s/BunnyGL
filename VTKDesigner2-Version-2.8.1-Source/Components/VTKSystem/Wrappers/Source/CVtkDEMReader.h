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

#ifndef C_VTK_DEM_READER_H
#define C_VTK_DEM_READER_H

#include "CVtkImageAlgorithm.h"
#include "vtkDEMReader.h"

class CVtkDEMReader : public CVtkImageAlgorithm
{
    Q_OBJECT
    Q_ENUMS(ElevReference)
    Q_PROPERTY(QString FileName READ fileName WRITE setFileName)
    Q_PROPERTY(ElevReference ElevationReference READ elevReference WRITE setElevReference)
    DECLARE_VTK_OBJECT

public:
    enum ElevReference
    {
        Sealevel = vtkDEMReader::REFERENCE_SEA_LEVEL,
        ElevationBounds = vtkDEMReader::REFERENCE_ELEVATION_BOUNDS
    };

    CVtkDEMReader();
    ~CVtkDEMReader();

    void setFileName(QString fileName);
    QString fileName() const;

    void setElevReference(ElevReference ref);
    ElevReference elevReference() const;

    Q_INVOKABLE QString mapLabel() const;
    Q_INVOKABLE int demLevel() const;
    Q_INVOKABLE int elevationPattern() const;
    Q_INVOKABLE int groundSystem() const;
    Q_INVOKABLE int groundZone() const;
    Q_INVOKABLE QList<double> projectParameters() const;
    Q_INVOKABLE int planeUnitOfMeasure() const;
    Q_INVOKABLE int elevationUnitOfMeasure() const;
    Q_INVOKABLE int polygonSize() const;
    Q_INVOKABLE QList<double> elevationBounds() const;
    Q_INVOKABLE double localRotation() const;
    Q_INVOKABLE int accuracyCode() const;
    Q_INVOKABLE QList<double> spatialResolution() const;
    Q_INVOKABLE QList<int> profileDimension() const;

private:
    vtkDEMReader* m_vtkDEMReader;
    QString m_fileName;
};

#endif

