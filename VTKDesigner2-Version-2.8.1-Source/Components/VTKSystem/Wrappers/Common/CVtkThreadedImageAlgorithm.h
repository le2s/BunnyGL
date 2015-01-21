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

#ifndef C_VTK_THREADED_IMAGE_ALGORITHM_H
#define C_VTK_THREADED_IMAGE_ALGORITHM_H

#include "CVtkImageAlgorithm.h"
#include "vtkThreadedImageAlgorithm.h"
#include "CVtkImageData.h"

class CVtkThreadedImageAlgorithm : public CVtkImageAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(int NumberOfThreads READ numberOfThreads WRITE setNumberOfThreads)
    DECLARE_VTK_OBJECT

public:
    CVtkThreadedImageAlgorithm();
    ~CVtkThreadedImageAlgorithm();

    void setNumberOfThreads(int num);
    int numberOfThreads() const;

protected:
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkThreadedImageAlgorithm* m_vtkThreadedImageAlgorithm;
    CVtkImageData* m_imageData;
};

#endif

