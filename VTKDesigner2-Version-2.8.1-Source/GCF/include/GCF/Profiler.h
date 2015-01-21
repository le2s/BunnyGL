/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
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

#ifndef PROFILER_H
#define PROFILER_H

#include <QTime>

namespace GCF
{
class FunctionProfiler
{
public:
    FunctionProfiler(const QString& func) {
        m_function = QMetaObject::normalizedSignature( qPrintable(func) );
        m_function = m_function.remove("__thiscall");
        m_function = m_function.remove("__stdcall");
        m_function = m_function.remove("__cdecl");

        m_startTime = QTime::currentTime();
    }

    ~FunctionProfiler() {
        qint32 msecs = m_startTime.msecsTo( QTime::currentTime() );
        qDebug("PROFILE: (%5d) %s", msecs, qPrintable(m_function));
    }

private:
    QString m_function;
    QTime m_startTime;
};
}

#define GCF_PROFILE_THIS_FUNCTION GCF::FunctionProfiler functionProfiler(Q_FUNC_INFO);

#endif

