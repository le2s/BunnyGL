#include "Component1.h"

#include <QtDebug>
#include <QCalendarWidget>

GCF_DEFINE_COMPONENT(Component1)

struct Component1Data
{
    Component1Data() : calendar(0) { }
    QCalendarWidget* calendar;
};

Component1& Component1::instance()
{
    return *(GCF_CREATE_COMPONENT(Component1));
}

Component1::Component1()
{
    d = new Component1Data;
}

Component1::~Component1()
{
    delete d;
}

void Component1::initializeComponent()
{
    this->setComponentName("Component1");
    this->setGuiXmlFileName(":/GUIXML/Component1.xml");
}

void Component1::finalizeComponent()
{
}

QWidget* Component1::fetchWidget(const QString &completeName) const
{
    if(completeName == "Component1.Calendar")
    {
        if(!d->calendar)
            d->calendar = new QCalendarWidget;

        return d->calendar;
    }

    return 0;
}

/*
  Since this component will be loaded from a dynamic library, we will need
  to make use of the GCF_EXPORT_COMPONENT_PLUGIN macro to export the
  component class as a plugin. If this macro is not placed, then GCF
  applications will not be able to load this component.
  */
GCF_EXPORT_COMPONENT_PLUGIN(Component1)
