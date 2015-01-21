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

#ifndef C_VTK_POLYDATA_STREAMER_H
#define C_VTK_POLYDATA_STREAMER_H

#include "CVtkPolyDataFilter.h"
#include "vtkPolyDataStreamer.h"

class CVtkPolyDataStreamer : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(int NumberOfStreamDivisions READ numberOfStreamDivisions WRITE setNumberOfStreamDivisions)
    Q_PROPERTY(bool ColorByPiece READ isColorByPiece WRITE setColorByPiece)
    DECLARE_VTK_OBJECT

public:
    CVtkPolyDataStreamer();
    ~CVtkPolyDataStreamer();

    void setNumberOfStreamDivisions(int num);
    int    numberOfStreamDivisions() const;

    void     setColorByPiece(bool val);
    bool     isColorByPiece() const;

private:
    vtkPolyDataStreamer *m_vtkPolyDataStreamer;
};

#endif