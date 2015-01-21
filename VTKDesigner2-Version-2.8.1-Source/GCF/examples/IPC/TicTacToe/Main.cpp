#include <GCF/Application>
#include "TicTacToeComponent.h"

int main(int argc, char** argv)
{
    GCF::Application a(argc, argv);

    GCF_REGISTER_COMPONENT(TicTacToeComponent);
    TicTacToeComponent::instance().initialize();

    return a.exec();
}
