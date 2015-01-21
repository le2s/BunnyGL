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

#ifndef C_VTK_DATASET_MAPPER_H
#define C_VTK_DATASET_MAPPER_H

#include "CVtkAlgorithm.h"
#include "vtkDataSetMapper.h"
#include "CVtkDataSetMapperData.h"

class CVtkDataSetMapper : public CVtkAlgorithm
{
    Q_OBJECT
    Q_PROPERTY(bool ShowScalars READ isShowScalars WRITE setShowScalars)
    DECLARE_VTK_OBJECT

public:
    CVtkDataSetMapper();
    ~CVtkDataSetMapper();

    void setShowScalars(bool val);
    bool isShowScalars() const;

protected slots:
    void command_ExportData();

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkDataSetMapper* m_vtkDataSetMapper;
    CVtkDataSetMapperData m_vtkDataSetMapperOutput;
    bool m_hasLookupTable;
};

#endif

