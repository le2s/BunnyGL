/****************************************************************************
**
** Copyright (C) VCreate Logic Pvt Ltd
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Pvt Ltd.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef Q_IMAGE_TO_VTK_IMAGE_DATA_H
#define Q_IMAGE_TO_VTK_IMAGE_DATA_H

#include "vtkImageAlgorithm.h"
#include <QImage>

class vtkLookupTable;

class QImageToVtkImageData : public vtkImageAlgorithm
{
public:
    static QImageToVtkImageData* New();
    vtkTypeRevisionMacro(QImageToVtkImageData, vtkImageAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    void SetImage(const QImage& image) {
        m_image = image;
        this->Modified();
    }
    QImage GetImage() const {
        return m_image;
    }

    vtkLookupTable* GetLookupTable() {
        return m_lookupTable;
    }

protected:
    QImageToVtkImageData();
    ~QImageToVtkImageData();
    int RequestInformation(vtkInformation*, vtkInformationVector**, vtkInformationVector*);
    void ExecuteData(vtkDataObject *out);

private:
    QImageToVtkImageData(const QImageToVtkImageData&);  // Not implemented.
    void operator=(const QImageToVtkImageData&);  // Not implemented.

private:
    QImage m_image;
    vtkLookupTable* m_lookupTable;
};

#endif
