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

#include "CVtkTimerObject.h"
#include "IVisSystemNodeConnectionPath.h"

DEFINE_VTK_OBJECT2(CVtkTimerObject, CVtkObject)
{
    pDesc->setNodeClassCategory("Control Objects");
    pDesc->setNodeClassName("Timer");
    pDesc->setNodeClassDescription("A simple timer object");
    pDesc->setPropertyInfo("Interval", "setInterval", "interval", QStringList() << "int", QStringList(), "void", "int");
}

CVtkTimerObject::CVtkTimerObject()
{
    CVtkTimerObject::InitializeObjectDesc();
    m_timerId = -1;
    m_interval = 100;

    static int count = 0;
    setNodeName(QString("timer%1").arg(count++));
}

CVtkTimerObject::~CVtkTimerObject()
{

}

void CVtkTimerObject::setActive(bool val)
{
    if(isActive() == val)
        return;

    if(val)
    {
        m_startTime = QTime::currentTime();
        m_lastTimeoutTime = m_startTime;
        m_timerId = startTimer(m_interval);
    }
    else
    {
        killTimer(m_timerId);
        m_timerId = -1;
    }
}

bool CVtkTimerObject::isActive() const
{
    return m_timerId >= 0;
}

void CVtkTimerObject::setInterval(int ival)
{
    m_interval = ival;
    if(isActive())
    {
        setActive(false);
        setActive(true);
    }
}

int CVtkTimerObject::interval() const
{
    return m_interval;
}

int CVtkTimerObject::msecsSinceLastTimeout() const
{
    return m_lastTimeoutTime.msecsTo( QTime::currentTime() );
}

int CVtkTimerObject::msecsSinceLastStart() const
{
    return m_startTime.msecsTo( QTime::currentTime() );
}

bool CVtkTimerObject::connectControl(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData* ctrlData)
{
    Q_UNUSED(path);
    Q_UNUSED(ctrlData);
    return true;
}

bool CVtkTimerObject::disconnectControl(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData* ctrlData)
{
    Q_UNUSED(path);
    Q_UNUSED(ctrlData);
    return true;
}

QRectF CVtkTimerObject::pathRect(IVisSystemNodeConnectionPath* path)
{
    if(path && path->pathType() == IVisSystemNodeConnectionPath::ControlPath && path->pathName() == "Timer")
    {
        // Show path rect to the right for this node.
        QRectF rect = CVtkObject::pathRect(path);
        QRectF nodeRect = CVtkObject::nodeRect();
        QPointF rectPoint = rect.center();

        rectPoint.setX(-rectPoint.x());
        rect = QRectF(rectPoint.x()-rect.width()/2.0f, rectPoint.y()-rect.height()/2.0f,
                      rect.width(), rect.height());
        return rect;
    }

    return CVtkObject::pathRect(path);
}

void CVtkTimerObject::timerEvent(QTimerEvent*)
{
    emit OnTimeout();
    m_lastTimeoutTime = QTime::currentTime();
}




