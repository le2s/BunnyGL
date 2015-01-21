#include "Component2.h"

#include <QtDebug>
#include <QDial>

GCF_DEFINE_COMPONENT(Component2)

struct Component2Data
{
    Component2Data() : dial(0) { }
    QDial* dial;
};

Component2& Component2::instance()
{
    return *(GCF_CREATE_COMPONENT(Component2));
}

Component2::Component2()
{
    d = new Component2Data;
}

Component2::~Component2()
{
    delete d;
}

void Component2::initializeComponent()
{
    this->setComponentName("Component2");
    this->setGuiXmlFileName(":/GUIXML/Component2.xml");
}

void Component2::finalizeComponent()
{
}

QWidget* Component2::fetchWidget(const QString &completeName) const
{
    if(completeName == "Component2.Dial")
    {
        if(!d->dial)
            d->dial = new QDial;

        return d->dial;
    }

    return 0;
}

/*
  Since this component will be loaded from a dynamic library, we will need
  to make use of the GCF_EXPORT_COMPONENT_PLUGIN macro to export the
  component class as a plugin. If this macro is not placed, then GCF
  applications will not be able to load this component.
  */
GCF_EXPORT_COMPONENT_PLUGIN(Component2)
