#include "SimpleWebService.h"

GCF_DEFINE_COMPONENT(SimpleWebService)

SimpleWebService& SimpleWebService::instance()
{
    return *GCF_CREATE_COMPONENT(SimpleWebService);
}

SimpleWebService::SimpleWebService() { }
SimpleWebService::~SimpleWebService() { }
void SimpleWebService::finalizeComponent() { }

QString SimpleWebService::toUpper(const QString &string) const
{
    return string.toUpper();
}

void SimpleWebService::initializeComponent()
{
    this->setGuiXmlFileName(":/SimpleWebService/SimpleWebService.xml");
}

QObject* SimpleWebService::fetchObject(const QString &completeName) const
{
    if(completeName == "SimpleWebService.Core")
        return const_cast<SimpleWebService*>(this);

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(SimpleWebService)
