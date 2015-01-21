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

#ifndef C_SCRIPTABLE_VTK_IMAGE_DATA_H
#define C_SCRIPTABLE_VTK_IMAGE_DATA_H

#include "CScriptableVtkDataSet.h"
#include "vtkImageData.h"
#include <QList>
      
class CScriptableVtkImageData : public CScriptableVtkDataSet
{
    Q_OBJECT

public:
    CScriptableVtkImageData(vtkImageData* imageData, CVtkObject* source);
    ~CScriptableVtkImageData();

    Q_INVOKABLE QList<int> dimensions() const;
    Q_INVOKABLE void setDimensions(QList<int> dim);

    Q_INVOKABLE QList<double> spacing() const;
    Q_INVOKABLE void setSpacing(QList<double> dim);

    Q_INVOKABLE Point3D origin() const;
    Q_INVOKABLE void setOrigin(Point3D point); 

    Q_INVOKABLE QString scalarType() const;
    Q_INVOKABLE void setScalarTypeToDouble();
    Q_INVOKABLE void setScalarTypeToChar();
    Q_INVOKABLE void setScalarTypeToInt();
    Q_INVOKABLE void setScalarTypeToUnsignedChar();
    Q_INVOKABLE void setScalarTypeToUnsignedInt();

    Q_INVOKABLE int numberOfScalarComponents() const;
    Q_INVOKABLE void setNumberOfScalarComponents(int val);

    Q_INVOKABLE void reset();
    Q_INVOKABLE void allocateScalars();

    Q_INVOKABLE int numberOfPoints() const;
    Q_INVOKABLE Point3D point(int index) const;
    Q_INVOKABLE Point3D point(int x, int y, int z) const;

    Q_INVOKABLE QList<double> scalarComponent(int x, int y, int z) const;
    Q_INVOKABLE void setScalarComponent(int x, int y, int z, QList<double> scalar);

private:
    vtkImageData* m_vtkImageData;
};

#endif

