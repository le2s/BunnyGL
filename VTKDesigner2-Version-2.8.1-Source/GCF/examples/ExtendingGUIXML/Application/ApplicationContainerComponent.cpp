#include "ApplicationContainerComponent.h"
#include "GUIExtenders.h"

#include <GCF/Application>
#include <QMainWindow>
#include <QTabWidget>
#include <QDockWidget>

struct ApplicationContainerComponentData
{
    QMainWindow* mainWindow;
    QTabWidget* workspace;
    QTabWidget* menuWidget;
};

GCF_DEFINE_COMPONENT(ApplicationContainerComponent)

ApplicationContainerComponent& ApplicationContainerComponent::instance()
{
    return *(GCF_CREATE_COMPONENT(ApplicationContainerComponent));
}

ApplicationContainerComponent::ApplicationContainerComponent()
{
    d = new ApplicationContainerComponentData;

    d->mainWindow = new QMainWindow;
    d->mainWindow->setMinimumSize(640, 480);
    gApp->setComponentHostWidget(d->mainWindow);

    d->workspace = new QTabWidget;
    d->mainWindow->setCentralWidget(d->workspace);

    d->menuWidget = new QTabWidget;
    d->menuWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    d->menuWidget->setFixedHeight(80);
    d->mainWindow->setMenuWidget(d->menuWidget);

    GCF::registerGuiXmlExtender(new MenuWidgetTabExtender);
    GCF::registerGuiXmlExtender(new MenuWidgetTabItemExtender);
}

ApplicationContainerComponent::~ApplicationContainerComponent()
{
    delete d;
}

void ApplicationContainerComponent::initializeComponent()
{
    this->setComponentName("ApplicationContainer");
    this->setGuiXmlFileName(":/GUIXML/ApplicationContainer.xml");
}

void ApplicationContainerComponent::finalizeComponent()
{
    // Do nothing
}

QTabWidget* ApplicationContainerComponent::menuWidget() const
{
    return d->menuWidget;
}

QTabWidget* ApplicationContainerComponent::workspace() const
{
    return d->workspace;
}

QWidget* ApplicationContainerComponent::fetchWidget(const QString &completeName) const
{
    if(completeName == "ApplicationContainer.MainWindow")
        return d->mainWindow;

    if(completeName == "ApplicationContainer.Workspace")
        return d->workspace;

    return 0;
}

QObject* ApplicationContainerComponent::fetchObject(const QString &completeName) const
{
    if(completeName == "ApplicationContainer.Core")
        return const_cast<ApplicationContainerComponent*>(this);

    return 0;
}

void ApplicationContainerComponent::addChildWidget(QWidget *parent, QWidget *child, const QString &hint)
{
    if(parent == d->workspace)
        d->workspace->addTab(child, child->windowTitle());
    else if(parent == d->mainWindow)
    {
        QDockWidget* dockWidget = new QDockWidget;
        dockWidget->setWidget(child);
        dockWidget->setWindowTitle(child->windowTitle());

        if(hint.toLower() == "leftdockarea")
            d->mainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
        else if(hint.toLower() == "rightdockarea")
            d->mainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget);
        else if(hint.toLower() == "topdockarea")
            d->mainWindow->addDockWidget(Qt::TopDockWidgetArea, dockWidget);
        else if(hint.toLower() == "bottomdockarea")
            d->mainWindow->addDockWidget(Qt::BottomDockWidgetArea, dockWidget);
        else
            dockWidget->setFloating(true);
    }
}

void ApplicationContainerComponent::removeChildWidget(QWidget *parent, QWidget *child, const QString &hint)
{
    if(parent == d->workspace)
        d->workspace->removeTab( d->workspace->indexOf(child) );
    else if(parent == d->mainWindow)
    {
        QDockWidget* dockWidget = 0;
        QWidget* parentWidget = child->parentWidget();
        while(parentWidget)
        {
            dockWidget = qobject_cast<QDockWidget*>(parentWidget);
            if(dockWidget)
                break;
            parentWidget = parentWidget->parentWidget();
        }

        if(!dockWidget)
            return;

        d->mainWindow->removeDockWidget(dockWidget);
        child->setParent(0);
        delete dockWidget;
    }

    Q_UNUSED(hint)
}

