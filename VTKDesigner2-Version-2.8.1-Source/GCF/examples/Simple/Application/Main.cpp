#include <GCF/Application>
#include "ApplicationContainerComponent.h"

int main(int argc, char** argv)
{
    // Create an instance of GCF::Application. This is a must for GCF
    // applications.
    GCF::Application a(argc, argv);
    a.setApplicationName("SimpleApplication");

    // Specify the directory where the application's XML file can be
    // found. In this case, path to SimpleApplication.xml
    a.setGuiXmlFileDirectory("../examples/Simple/Application");

    // Register the main container component of the GCF application
    GCF_REGISTER_COMPONENT(ApplicationContainerComponent);

    // and initialize it
    ApplicationContainerComponent::instance().initialize();

    // Now load start the GCF application's event loop
    return a.exec();
}

