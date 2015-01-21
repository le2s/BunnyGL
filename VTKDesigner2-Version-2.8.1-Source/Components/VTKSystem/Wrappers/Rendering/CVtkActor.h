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

#ifndef C_VTK_ACTOR_H
#define C_VTK_ACTOR_H

#include "CVtkProp3D.h"
#include "vtkActor.h"
#include "CVtkActorData.h"

class CVtkActor : public CVtkProp3D
{
    Q_OBJECT
    Q_PROPERTY(QColor Color WRITE setColor READ color)
    Q_PROPERTY(double Opacity WRITE setOpacity READ opacity)
    DECLARE_VTK_OBJECT

public:
    CVtkActor();
    ~CVtkActor();

    QColor color() const;
    void setColor(QColor c);

    void setOpacity(double val);
    double opacity() const;

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkActor* m_vtkActor;
    bool m_hasProperty;
    CVtkActorData m_actorData;
};


#endif

