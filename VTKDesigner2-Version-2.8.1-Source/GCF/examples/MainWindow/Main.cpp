#include <GCF/Application>
#include <GCF/MainWindow/MainWindowComponent>

int main(int argc, char** argv)
{
    GCF::Application a(argc, argv);
    a.setApplicationName("StdMainWindowExample");

    // Set the GUIXML directory to wherever you are going to load
    // the ApplicationXML and component XML's from.
    a.setGuiXmlFileDirectory("../examples/MainWindow");

    // The following string variables should point to
    // appropriate image files either from the disk or from the
    // Qt resource file system.
    //
    // If the string variables are empty, GCF will choose
    // a default pixmap file for you.
    QString applicationLogoPixmapFile;
    QString applicationIconPixmapFile;
    QString splashScreenPixmapFile;

    GCF::UseStandardMainWindowContainer(applicationLogoPixmapFile,
                                        applicationIconPixmapFile,
                                        splashScreenPixmapFile);

    return a.exec();
}
