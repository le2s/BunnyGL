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
#ifndef C_VTK_CLIP_POLY_DATA_H
#define C_VTK_CLIP_POLY_DATA_H

#include "vtkClipPolyData.h"
#include "CVtkPolyDataFilter.h"
#include "CVtkPolyData.h"

class CVtkClipPolyData : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(double Value READ value WRITE setValue)
    Q_PROPERTY(bool InsideOut READ isInsideOut WRITE setInsideOut)
    Q_PROPERTY(bool GenerateClipScalars READ isGenerateClipScalars WRITE setGenerateClipScalars)
    Q_PROPERTY(bool GenerateClippedOutput READ generateClippedOutput WRITE setGenerateClippedOutput)
    DECLARE_VTK_OBJECT

public:

    CVtkClipPolyData();
    ~CVtkClipPolyData();

    double value() const;
    void setValue(double val);

    bool isInsideOut() const;
    void setInsideOut(bool val);

    bool isGenerateClipScalars() const;
    void setGenerateClipScalars(bool val);

    bool generateClippedOutput() const;
    void setGenerateClippedOutput(bool val);

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkClipPolyData *m_vtkClipPolyData;
    CVtkPolyData m_clippedOutput;
};
#endif//C_VTK_CLIP_POLY_DATA_H