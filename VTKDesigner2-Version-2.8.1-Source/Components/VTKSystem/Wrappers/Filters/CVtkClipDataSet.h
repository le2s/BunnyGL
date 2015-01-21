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
#ifndef C_VTK_CLIP_DATA_SET_H
#define C_VTK_CLIP_DATA_SET_H

#include "vtkClipDataSet.h"
#include "CVtkDataSetFilter.h"
#include "CVtkUnstructuredGrid.h"

class CVtkClipDataSet : public CVtkDataSetFilter
{
    Q_OBJECT
    Q_PROPERTY(double Value READ value WRITE setValue)
    Q_PROPERTY(bool InsideOut READ isInsideOut WRITE setInsideOut)
    Q_PROPERTY(bool GenerateClipScalars READ isGenerateClipScalars WRITE setGenerateClipScalars)
    Q_PROPERTY(bool GenerateClippedOutput READ generateClippedOutput WRITE setGenerateClippedOutput)
    Q_PROPERTY(double MergeTolerance READ mergeTolerance WRITE setMergeTolerance)
    DECLARE_VTK_OBJECT

public:

    CVtkClipDataSet();
    ~CVtkClipDataSet();

    double value() const;
    void setValue(double val);

    bool isInsideOut() const;
    void setInsideOut(bool val);

    bool isGenerateClipScalars() const;
    void setGenerateClipScalars(bool val);

    bool generateClippedOutput() const;
    void setGenerateClippedOutput(bool val);

    double mergeTolerance() const;
    void setMergeTolerance(double val);

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkClipDataSet *m_vtkClipDataSet;
    CVtkUnstructuredGrid m_clippedOutput;
};
#endif//C_VTK_CLIP_DATA_SET_H