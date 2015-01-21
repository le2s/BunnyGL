#include <GCF/Application>
#include <GCF/MainWindow/MainWindowComponent>

int main(int argc, char** argv)
{
    GCF::Application a(argc, argv);
    a.setApplicationName("SimpleQMLApp");
    a.setGuiXmlFileDirectory("../examples/QML/SimpleQML");
    GCF::UseStandardMainWindowContainer();
    return a.exec();
}
