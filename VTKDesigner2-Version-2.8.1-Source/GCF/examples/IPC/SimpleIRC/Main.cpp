#include <GCF/Application>
#include "SimpleIRC.h"

int main(int argc, char** argv)
{
    GCF::Application a(argc, argv);

    GCF_REGISTER_COMPONENT(SimpleIRC);
    SimpleIRC::instance().initialize();

    return a.exec();
}
