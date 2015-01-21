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

#ifndef C_VTK_TIMER_OBJECT_H
#define C_VTK_TIMER_OBJECT_H

#include "CVtkObject.h"
#include "CVtkObjectControlData.h"
#include <QTime>

class CVtkTimerObject : public CVtkObject
{
    Q_OBJECT
    Q_PROPERTY(bool Active READ isActive WRITE setActive)
    Q_PROPERTY(int Interval READ interval WRITE setInterval)
    Q_CLASSINFO("SectionName", "Timer");
    DECLARE_VTK_OBJECT

public:
    CVtkTimerObject();
    ~CVtkTimerObject();

    // Property methods.
    void setActive(bool val);
    bool isActive() const;

    void setInterval(int ival);
    int interval() const;

    Q_INVOKABLE int msecsSinceLastTimeout() const;
    Q_INVOKABLE int msecsSinceLastStart() const;

protected:
    bool connectControl(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData* ctrlData);
    bool disconnectControl(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData* ctrlData);
    QRectF pathRect(IVisSystemNodeConnectionPath* path);
    void timerEvent(QTimerEvent*);

signals:
    void OnTimeout();

private:
    int m_timerId;
    int m_interval;
    QTime m_startTime;
    QTime m_lastTimeoutTime;
};

#endif

