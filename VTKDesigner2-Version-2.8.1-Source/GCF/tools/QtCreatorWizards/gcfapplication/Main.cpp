#include <GCF/Application>
#include <GCF/MainWindow/MainWindowComponent>

int main(int argc, char** argv)
{
    GCF::Application a(argc, argv);
    a.setGuiXmlFileDirectory(a.applicationDirPath());

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
