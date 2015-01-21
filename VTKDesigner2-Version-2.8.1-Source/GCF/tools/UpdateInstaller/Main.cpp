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

#include <QApplication>
#include <QMessageBox>
#include <QProcess>

#include "UpdateInstaller.h"

int main(int argc, char** argv)
{
    /*
    UINST program is called by GCF::UpdaterComponent. It is usually never invoked
    by the user via command prompt. If the user does so we should issue a warning.

    Arguments to UINST program are as follows
        - Name of the application for which updates are being installed
        - Complete path of where the application program can be found
        - GUIXML directory path
        - Updates directory path

    Example
        UINST --appName <application-name> --appPath < > --guixmlPath < > --updatesPath < >
    */

    QApplication a(argc, argv);

    if( argc == 1 )
    {
        QMessageBox::information(0, "Error launching Update Installer",
            "Update installer is automatically invoked by your GCF application.\n"
            "It should not be invoked by the user.");
        return 1;
    }

    for(int i=1; i<argc; i++)
    {
        if( i+1 >= argc )
            break;

        QString key( argv[i] );
        QString value( argv[i+1] );

        if( key == "--appName" )
            UpdateInstaller::ApplicationName = value;
        else if( key == "--appPath" )
            UpdateInstaller::ApplicationPath = value;
        else if( key == "--guixmlPath" )
            UpdateInstaller::GuiXmlDirPath = value;
        else if( key == "--updatesPath" )
            UpdateInstaller::UpdatesDirPath = value;
    }

    if( UpdateInstaller::ApplicationName.isNull() )
    {
        QMessageBox::information(0, "Error launching Update Installer",
            "Application name not provided");

        return 1;
    }

    if( UpdateInstaller::ApplicationPath.isNull() )
    {
        QMessageBox::information(0, "Error launching Update Installer",
            "Application path not provided");

        return 1;
    }

    if( UpdateInstaller::GuiXmlDirPath.isNull() )
    {
        QMessageBox::information(0, "Error launching Update Installer",
            "GUIXML path not provided");

        return 1;
    }

    if( UpdateInstaller::UpdatesDirPath.isNull() )
    {
        QMessageBox::information(0, "Error launching Update Installer",
            "Updates name not provided");

        return 1;
    }

    UpdateInstaller::instance().show();

    int retCode = a.exec();

    GCF_UINST_LOG("Launching the application now");
    QString prog = QString("%1/%2").arg(UpdateInstaller::ApplicationPath).arg(UpdateInstaller::ApplicationName);
    bool success = QProcess::startDetached(prog);
    if(!success)
        GCF_UINST_ERROR( QString("Error launching %1 after installing updates").arg(prog) );
    else
        GCF_UINST_LOG("All done!!!");

    return retCode;
}
