#include "BrowserWindowComponent.h"
#include "BrowserWindow.h"

#include <GCF/ComponentFactory>

struct BrowserWindowComponentData
{
    BrowserWindow* browserWindow;
};

GCF_DEFINE_COMPONENT(BrowserWindowComponent)

BrowserWindowComponent& BrowserWindowComponent::instance()
{
    return *GCF_CREATE_COMPONENT(BrowserWindowComponent);
}

BrowserWindowComponent::BrowserWindowComponent()
{
    d = new BrowserWindowComponentData;

    d->browserWindow = & BrowserWindow::instance();
    GCF::AbstractComponent::setMainWindow(d->browserWindow);
}

BrowserWindowComponent::~BrowserWindowComponent()
{
    delete d;
}

QObject* BrowserWindowComponent::fetchObject(const QString& completeName) const
{
    if(completeName == "BrowserWindowComponent.controller")
        return const_cast<BrowserWindowComponent*>(this);

    return 0;
}

QWidget* BrowserWindowComponent::fetchWidget(const QString& completeName) const
{
    if(completeName == "BrowserWindowComponent.browserWindow")
        return d->browserWindow;

    return 0;
}

void BrowserWindowComponent::initializeComponent() { }
void BrowserWindowComponent::finalizeComponent() { }


