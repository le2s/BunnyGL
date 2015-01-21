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

#ifndef C_VTK_INTERACTOR_OBSERVER_H
#define C_VTK_INTERACTOR_OBSERVER_H

#include "CVtkObject.h"
#include "vtkInteractorObserver.h"
#include "CVtkInteractorObserverData.h"

class CVtkInteractorObserver : public CVtkObject
{
    Q_OBJECT
    Q_PROPERTY(bool Enabled READ isEnabled WRITE setEnabled)
    Q_PROPERTY(double Priority READ priority WRITE setPriority)
    Q_PROPERTY(bool KeyPressActivation READ isKeyPressActivation WRITE setKeyPressActivation)
    Q_PROPERTY(QChar KeyPressActivationValue READ keyPressActivationValue WRITE setKeyPressActivationValue)
    DECLARE_VTK_OBJECT

public:
    CVtkInteractorObserver();
    ~CVtkInteractorObserver();

    void setEnabled(bool val);
    bool isEnabled() const;

    void setPriority(double val);
    double priority() const;

    void setKeyPressActivation(bool val);
    bool isKeyPressActivation() const;

    void setKeyPressActivationValue(QChar ch);
    QChar keyPressActivationValue() const;

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkInteractorObserver* m_vtkInteractorObserver;
    CVtkInteractorObserverData m_outputData;
    bool m_enabled;
};

#endif
