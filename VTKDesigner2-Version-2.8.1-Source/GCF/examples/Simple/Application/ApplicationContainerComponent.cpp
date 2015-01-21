#include "ApplicationContainerComponent.h"
#include <GCF/Application>

#include <QWidget>
#include <QVBoxLayout>

struct ApplicationContainerComponentData
{
    QWidget* container;
};

// The GCF_DEFINE_COMPONENT macro must be used in the source file
// to generate code for the functions declared by the GCF_DECLARE_COMPONENT
// macro in the header file.
GCF_DEFINE_COMPONENT(ApplicationContainerComponent);

/*
  Instances of components must be created using the GCF_CREATE_COMPONENT
  macro. This is because, the macro internally makes use of GCF::ComponentFactory
  to create and register an instance of the component
  */
ApplicationContainerComponent& ApplicationContainerComponent::instance()
{
    return *(GCF_CREATE_COMPONENT(ApplicationContainerComponent));
}

ApplicationContainerComponent::ApplicationContainerComponent()
{
    d = new ApplicationContainerComponentData;

    // Create a container widget into which GUI resources offered
    // by components will be merged
    d->container = new QWidget;
    new QVBoxLayout(d->container);

    // Configure the widget as a container widget for the GCF application
    gApp->setComponentHostWidget(d->container);
}

ApplicationContainerComponent::~ApplicationContainerComponent()
{
    delete d;
}

/*
  This function must be implemented to set the name of the component and/or
  name of the component's GUIXML file.

  By default the name of the component is same as the name of the
  component class - in this case ApplicationContainerComponent. However, you
  can change the name to something shorter if you wish.

  By default the component looks for its GUIXML file within the GUIXML
  directory set using GCF::Application::setGuiXmlFileDirectory(). A file
  whose extension is xml and base name is same as the name of the component
  (context sensitive) is used as the GUIXML file of the component. You
  can change the name of the GUIXML file if you want - perhaps to a file
  name within the resource file system.

  Note: This function is called just after the component class is constructed,
  and just before the GUIXML of the component is processed.
  */
void ApplicationContainerComponent::initializeComponent()
{
    this->setComponentName("ApplicationContainer");
    this->setGuiXmlFileName(":/GUIXML/ApplicationContainer.xml");

    // Show the main container of the application
    d->container->resize(640, 480);
    d->container->show();
}

/*
  Reimplement this function to cleanup memory resources used by the component.

  Note: This function is called just before the UI/non-UI resources loaded through
  its GUIXML are unloaded (or unmerged).
  */
void ApplicationContainerComponent::finalizeComponent()
{
    // Do nothing
}

/*
  This function must be implemented to resolve complete names of widgets into
  actual widget pointers. A complete name is nothing but

  <component-name>.<widget-name>

  This function is called in response to the <widget> XML element in this
  component's GUIXML file.
  */
QWidget* ApplicationContainerComponent::fetchWidget(const QString &completeName) const
{
    if(completeName == "ApplicationContainer.Container")
        return d->container;

    return 0;
}

/*
  This function is called in response to the <object> XML element in the component's
  GUIXML file.
  */
QObject* ApplicationContainerComponent::fetchObject(const QString &completeName) const
{
    if(completeName == "ApplicationContainer.Core")
        return const_cast<ApplicationContainerComponent*>(this);

    return 0;
}

/*
  This function is called when a child widget belonging to this or any other component
  needs to be added to a parent widget that belongs to this component. The hint parameter
  can be used to customize the way the child widget is added to the parent.

  This function is called when GCF reads the <widget> XML element in a GUIXML file.
  */
void ApplicationContainerComponent::addChildWidget(QWidget *parent, QWidget *child, const QString &hint)
{
    Q_UNUSED(hint);

    if(parent == d->container)
    {
        if(child)
            d->container->layout()->addWidget(child);
    }
}

/*
  This function is called when a child widget belonging to this or any other component
  needs to be removed from a parent widget that belongs to this component. This function
  should ideally do the reverse of what addChildWidget() did for the same set of parameters.

  Note: This function should __not__ delete the child widget.
  */
void ApplicationContainerComponent::removeChildWidget(QWidget *parent, QWidget *child, const QString &hint)
{
    Q_UNUSED(hint);

    if(parent == d->container)
    {
        if(child && child->parentWidget() == parent)
            d->container->layout()->removeWidget(child);
    }
}

