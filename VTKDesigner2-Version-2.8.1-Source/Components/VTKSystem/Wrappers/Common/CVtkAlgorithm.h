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

#ifndef C_VTK_ALGORITHM_H
#define C_VTK_ALGORITHM_H

#include "CVtkObject.h"
#include "vtkAlgorithm.h"
#include "CVtkAlgorithmData.h"

class CVtkAlgorithm : public CVtkObject
{
    Q_OBJECT
    Q_PROPERTY(int HasExecutive READ getHasExecutive)
    Q_PROPERTY(bool AbortExecution WRITE setAbortExecution READ getAbortExecution)
    Q_PROPERTY(bool ReleaseDataFlag WRITE setReleaseDataFlag READ getReleaseDataFlag)
    DECLARE_VTK_OBJECT

public:
    CVtkAlgorithm();
    ~CVtkAlgorithm();

    // Property Methods for CVtkAlgorithm
    bool getHasExecutive() const;
    void setAbortExecution(bool val);
    bool getAbortExecution() const;
    void setReleaseDataFlag(bool val);
    bool getReleaseDataFlag() const;

    Q_INVOKABLE void markModified();
    Q_INVOKABLE bool isModified() const;
    Q_INVOKABLE void update();
    Q_INVOKABLE void forceUpdate();

    Q_INVOKABLE int getNumberOfOutputPorts() const;
    Q_INVOKABLE QString getOutputDataObjectType(int index) const;
    Q_INVOKABLE QObject* getOutputDataObject(int index) const;

protected slots:
    void command_Update();
    void command_ForceUpdate();

protected:
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkAlgorithm* m_vtkAlgorithm;
    CVtkAlgorithmData m_algorithmData;
    mutable QMap<int, QObject*> m_outputDataObjectMap;
};

#endif

