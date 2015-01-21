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

#ifndef C_VTK_LOCATOR_H
#define C_VTK_LOCATOR_H

#include "CVtkObject.h"
#include "vtkLocator.h"
#include "CVtkLocatorData.h"

class CVtkLocator : public CVtkObject
{
    Q_OBJECT
    Q_PROPERTY(int MaxLevel READ maxLevel WRITE setMaxLevel)
    Q_PROPERTY(bool Automatic READ isAutomatic WRITE setAutomatic)
    Q_PROPERTY(double Tolerance READ tolerance WRITE setTolerance)
    DECLARE_VTK_OBJECT

public:
    CVtkLocator();
    ~CVtkLocator();

    void setMaxLevel(int level);
    int maxLevel() const;

    Q_INVOKABLE int level() const;

    void setAutomatic(bool val);
    bool isAutomatic() const;

    void setTolerance(double val);
    double tolerance() const;

    Q_INVOKABLE void buildLocator();

protected slots:
    void command_BuildLocator();

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkLocator* m_vtkLocator;
    CVtkLocatorData m_locatorData;
};

#endif

