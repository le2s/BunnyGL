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

#ifndef C_VTK_APPEND_POLY_DATA_H
#define C_VTK_APPEND_POLY_DATA_H

#include "vtkAppendPolyData.h"
#include "CVtkPolyDataFilter.h"

class CVtkAppendPolyData : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(bool UserManagedInputs READ isUserManagedInputs WRITE setUserManagedInputs)
    Q_PROPERTY(bool ParallelStreaming READ isParallelStreaming WRITE setParallelStreaming)
    DECLARE_VTK_OBJECT

public:

    CVtkAppendPolyData();
    ~CVtkAppendPolyData();

    bool isUserManagedInputs() const;
    void setUserManagedInputs(bool val);

    bool isParallelStreaming() const;
    void setParallelStreaming(bool val);

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkAppendPolyData *m_vtkAppendPolyData;
};
#endif//C_VTK_APPEND_POLY_DATA_H