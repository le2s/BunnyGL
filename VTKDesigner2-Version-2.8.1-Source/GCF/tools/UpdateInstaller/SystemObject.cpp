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

#include "SystemObject.h"
#include "UpdateInstaller.h"

#include <QCoreApplication>
#include <QProcess>
#include <QDomDocument>
#include <QDomElement>
#include <QMessageBox>
#include <QPointer>
#include <QDialog>
#include <QPushButton>
#include <QFile>

#include "ui_LicenseDialog.h"

SystemObject& SystemObject::instance()
{
    static QPointer<SystemObject> ptr = new SystemObject(qApp);
    return *(ptr.data());
}

SystemObject::SystemObject(QObject* parent) : QObject(parent)
{

}

SystemObject::~SystemObject()
{

}

bool SystemObject::Exec(const QString& progName)
{
    QString progName2 = UpdateInstaller::instance().resolveFileName(progName);
    QProcess* proc = new QProcess(this);
    proc->start(progName2);
    bool success = proc->waitForStarted();
    return success;
}

bool SystemObject::ExecWait(const QString& progName)
{
    QString progName2 = UpdateInstaller::instance().resolveFileName(progName);
    return QProcess::execute(progName2);
}

bool SystemObject::DebugLog(const QString& logMsg)
{
    QString logMsg2 = UpdateInstaller::instance().resolveFileName(logMsg);
    GCF_UINST_DEBUG( logMsg2 );
    return true;
}

bool SystemObject::ErrorLog(const QString& logMsg)
{
    QString logMsg2 = "  " + UpdateInstaller::instance().resolveFileName(logMsg);
    GCF_UINST_ERROR( logMsg2 );
    return true;
}

bool SystemObject::WarningLog(const QString& logMsg)
{
    QString logMsg2 = "  " + UpdateInstaller::instance().resolveFileName(logMsg);
    GCF_UINST_WARNING( logMsg2 );
    return true;
}

bool SystemObject::CriticalLog(const QString& logMsg)
{
    QString logMsg2 = "  " + UpdateInstaller::instance().resolveFileName(logMsg);
    GCF_UINST_FATAL( logMsg2 );
    QMessageBox::information( UpdateInstaller::instance().installerWindow(),
                              "CRITICAL Message",
                               logMsg2);
    return true;
}

bool SystemObject::MessageBox(const QString& msg)
{
    QString msg2 = UpdateInstaller::instance().resolveFileName(msg);
    GCF_UINST_LOG( msg2 );
    QMessageBox::information( UpdateInstaller::instance().installerWindow(),
                              "Message",
                               msg2);
    return true;
}

bool SystemObject::Progress(int pc, const QString& msg)
{
    QString msg2 = "  " + UpdateInstaller::instance().resolveFileName(msg);
    UpdateInstaller::instance().progress(pc, msg2);
    return true;
}

bool SystemObject::RegisterComponent(const QString& compName, const QString& libName)
{
    QDomDocument doc = UpdateInstaller::instance().clcGuiXml();
    if(doc.isNull())
        return false;

    QDomElement compE = doc.documentElement();
    if(compE.isNull())
        return false;

    // Check if the component is already registered
    QDomElement methodE = compE.firstChildElement("method");
    while(!methodE.isNull())
    {
        QString p1 = methodE.attribute("param1");
        QString p2 = methodE.attribute("param2");
        if( p1 == compName )
        {
            if( p2 != libName )
                methodE.setAttribute("param2", libName);

            return true;
        }

        methodE = methodE.nextSiblingElement("method");
    }

    // component not already registered. So lets register now.
    methodE = doc.createElement("method");
    methodE.setAttribute("param1", compName);
    methodE.setAttribute("param2", libName);
    methodE.setAttribute("methodName", "ComponentLoaderComponent.controller::loadComponent2");
    compE.appendChild(methodE);

    return true;
}

bool SystemObject::UnregisterComponent(const QString& compName, const QString& /*libName*/)
{
    QDomDocument doc = UpdateInstaller::instance().clcGuiXml();
    if(doc.isNull())
        return false;

    QDomElement compE = doc.documentElement();
    if(compE.isNull())
        return false;

    // Check if the component is already registered
    QDomElement methodE = compE.firstChildElement("method");
    while(!methodE.isNull())
    {
        QString p1 = methodE.attribute("param1");
        if( p1 == compName )
        {
            compE.removeChild(methodE);
            return true;
        }

        methodE = methodE.nextSiblingElement("method");
    }

    return false;
}

bool SystemObject::ShowLicensePlain(const QString& licenseFileName)
{
    return showLicense(licenseFileName, false);
}

bool SystemObject::ShowLicenseHtml(const QString& licenseFileName)
{
    return showLicense(licenseFileName, true);
}

bool SystemObject::showLicense(const QString& licenseFileName, bool html)
{
    // Create the license dialog
    QDialog dialog( &UpdateInstaller::instance() );
    Ui::LicenseDialog ui;
    ui.setupUi(&dialog);

    // Bring the window title to context
    QString windowTitle = QString("%1 - License Confirmation")
        .arg(UpdateInstaller::instance().currentUpdate());
    dialog.setWindowTitle( windowTitle );
    
    // Show "Accept"/"Reject" instead of "Ok"/"Cancel"
    QPushButton* okButton = ui.buttonBox->button(QDialogButtonBox::Ok);
    okButton->setText("&Accept");
    QPushButton* cancelButton = ui.buttonBox->button(QDialogButtonBox::Cancel);
    cancelButton->setText("&Reject");

    // Show the license text
    QFile file(licenseFileName);
    file.open(QFile::ReadOnly);
    if(html)
        ui.textEdit->setHtml( file.readAll() );
    else
        ui.textEdit->setPlainText( file.readAll() );
    file.close();

    // Ask the user to accept the license.
    return dialog.exec() == QDialog::Accepted;
}
