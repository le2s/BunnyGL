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
#ifndef C_VTK_CUTTER_H
#define C_VTK_CUTTER_H

#include "vtkCutter.h"
#include "CVtkPolyDataFilter.h"

class CVtkCutter : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(SortBy)
    Q_PROPERTY(int NumberOfContours READ numberOfContours WRITE setNumberOfContours)
    Q_PROPERTY(bool GenerateCutScalars READ isGenerateCutScalars WRITE setGenerateCutScalars)
    Q_PROPERTY(SortBy SortBy READ sortBy WRITE setSortBy)
    DECLARE_VTK_OBJECT

public:
    enum SortBy
    {
        Value=VTK_SORT_BY_VALUE,
        Cell=VTK_SORT_BY_CELL,

    };
    CVtkCutter();
    ~CVtkCutter();

    int numberOfContours() const;
    void setNumberOfContours(int val);
    Q_INVOKABLE void setValue(int i, double val);

    bool isGenerateCutScalars() const;
    void setGenerateCutScalars(bool val);

    SortBy sortBy() const;
    void setSortBy(SortBy val);

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkCutter *m_vtkCutter;
};
#endif//C_VTK_CUTTER_H