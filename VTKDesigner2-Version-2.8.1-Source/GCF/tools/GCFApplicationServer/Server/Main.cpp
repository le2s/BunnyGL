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
#include "ApplicationServerComponent.h"

int main(int argc, char** argv)
{
    GCF::Application a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    a.setGuiXmlFileDirectory("GCFApplicationServer-GUIXML");

    ApplicationServerComponent::BrokerMode = (argc == 1);
    GCF_REGISTER_COMPONENT(ApplicationServerComponent);
    if(!ApplicationServerComponent::BrokerMode)
        ApplicationServerComponent::instance().setServerName( QString(argv[1]) );

    ApplicationServerComponent::instance().initialize();

    return a.exec();
}
