#include <GCF/Application>
#include "ApplicationContainerComponent.h"

int main(int argc, char** argv)
{
    GCF::Application a(argc, argv);
    a.setApplicationName("ExtendingGUIXML");
    a.setGuiXmlFileDirectory("../examples/ExtendingGUIXML/Application");

    GCF_REGISTER_COMPONENT(ApplicationContainerComponent);
    ApplicationContainerComponent::instance().initialize();

    return a.exec();
}

