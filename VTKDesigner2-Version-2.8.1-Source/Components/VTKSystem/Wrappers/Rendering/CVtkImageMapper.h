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

#ifndef C_VTK_IMAGE_MAPPER_H
#define C_VTK_IMAGE_MAPPER_H

#include "CVtkMapper2D.h"
#include "vtkImageMapper.h"

class CVtkImageMapper : public CVtkMapper2D
{
    Q_OBJECT
    Q_PROPERTY(double ColorWindow READ colorWindow WRITE setColorWindow)
    Q_PROPERTY(double ColorLevel READ colorLevel WRITE setColorLevel)
    Q_PROPERTY(double ZSlice READ zSlice WRITE setZSlice)
    DECLARE_VTK_OBJECT

public:
    CVtkImageMapper();
    ~CVtkImageMapper();

    void setColorWindow(double val);
    double colorWindow() const;

    void setColorLevel(double val);
    double colorLevel() const;

    void setZSlice(double val);
    double zSlice() const;

    Q_INVOKABLE double getColorShift() const;
    Q_INVOKABLE double getColorScale() const;

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

protected:
    vtkImageMapper* m_vtkImageMapper;
};

#endif
