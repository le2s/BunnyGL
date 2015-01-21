#include <GCF/Application>
#include "MediaPlayerComponent.h"

int main(int argc, char** argv)
{
    GCF::Application a(argc, argv);
    a.setApplicationName("Media Player Desktop - GCF Example Application");
    a.setGuiXmlFileDirectory(":/");

    GCF_REGISTER_COMPONENT(MediaPlayerComponent);
    MediaPlayerComponent::instance().initialize();

    return a.exec();
}
