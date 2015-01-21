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

#include <GCF/Application>
#include <GCF/MainWindow/MainWindowComponent>
#include <GCF/MainWindow/MainWindow>

#ifdef Q_WS_WIN
#include <QtDotNetStyle>
#endif

int main(int argc, char** argv)
{
#ifdef Q_OS_MAC
    QApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
#endif

    GCF::Application a(argc, argv);
    a.setApplicationName("VTKDesigner2");
    a.setApplicationVersion("2.8.1");
#ifdef Q_WS_WIN
    a.setStyle(new QtDotNetStyle(QtDotNetStyle::Standard));
#endif

    // Q_INIT_RESOURCE(StdMainWindow);
    Q_INIT_RESOURCE(Main);

    GCF::UseStandardMainWindowContainer(":/VTKDesigner2/vtkdesigner.png",
                                        QString(),
                                        ":/VTKDesigner2/splash.png");

    // Configure the main-window
    GCF::Components::MainWindow::instance().setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    return a.exec();
}
