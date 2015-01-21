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

#ifndef SYSTEM_OBJECT_H
#define SYSTEM_OBJECT_H

#include <QObject>

class SystemObject : public QObject
{
    Q_OBJECT

public:
    static SystemObject& instance();
    ~SystemObject();

    Q_INVOKABLE bool Exec(const QString& progName);
    Q_INVOKABLE bool ExecWait(const QString& progName);
    Q_INVOKABLE bool DebugLog(const QString& logMsg);
    Q_INVOKABLE bool ErrorLog(const QString& logMsg);
    Q_INVOKABLE bool WarningLog(const QString& logMsg);
    Q_INVOKABLE bool CriticalLog(const QString& logMsg);
    Q_INVOKABLE bool MessageBox(const QString& msg);
    Q_INVOKABLE bool Progress(int pc, const QString& msg);
    Q_INVOKABLE bool RegisterComponent(const QString& compName, const QString& libName);
    Q_INVOKABLE bool UnregisterComponent(const QString& compName, const QString& libName);
    Q_INVOKABLE bool ShowLicensePlain(const QString& licenseFileName);
    Q_INVOKABLE bool ShowLicenseHtml(const QString& licenseFileName);

protected:
    bool showLicense(const QString& licenseFileName, bool html=true);
    SystemObject(QObject* parent=0);
};

#endif



