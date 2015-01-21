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

#include <GCF/AbstractComponent>
#include <GCF/ComponentGui>
#include <GCF/ComponentFactory>
#include <GCF/Menu>
#include <GCF/IComponentHostWindow>
#include <GCF/ComponentHostMainWindow>
#include <GCF/ComponentSettings>
#include <GCF/RemoteAppAccessPoint>

#include <QAction>
#include <QWidget>
#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QMap>
#include <QMainWindow>
#include <QPointer>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QDesktopServices>
#include <QDir>
#include <QProcessEnvironment>

#ifdef GCF_ACTIVEQT_AVAILABLE
#include <QAxWidget>
#endif

#ifdef GCF_KPARTS_AVAILABLE
#include "kparts/part.h"
#endif

// Change the version number is source.pro as well.
#ifndef GCF_MAJOR_VERSION
#define GCF_MAJOR_VERSION    2
#endif

#ifndef GCF_MINOR_VERSION
#define GCF_MINOR_VERSION    6
#endif

#ifndef GCF_BUILD_VERSION
#define GCF_BUILD_VERSION    1
#endif

static void GCFCleanupRoutine()
{
    GCF::finalize();
}

/**
\internal

This function initializes the Generic Component Framework layer. This function
must be called before calling any-other method in GCF. It is best to call
this function right after creating a QApplication instance.
*/
void GCF::initialize()
{
    static bool GCFInitialized = false;
    if( GCFInitialized )
        return;

    GCFInitialized = true;

    GCF::ComponentFactory::instance().initialize();
    qAddPostRoutine(GCFCleanupRoutine);
}

/**
\internal

This method should be called to cleanup the GCF layer. After the function returns
no component is active and the application is in the right state to shutdown.

The best place to call this function is within the closeEvent() handler of the
application's main window.

\note You must place a call to GCF::finalize(). Otherwise your application may never
shutdown properly.
*/
void GCF::finalize()
{
    static bool GCFFinalized = false;
    if( GCFFinalized )
        return;

    GCFFinalized = true;

    GCF::RemoteAppAccessPoint::closeAllRemoteAccessPoints();
    GCF::ComponentFactory::instance().finalize();
}

/**
\class GCF::AbstractComponent
\brief <code>#include <GCF/AbstractComponent></code><br> Base class for all components.

Components in a GCF application should be inherited from the GCF::AbstractComponent class.
The GCF::AbstractComponent class provides a basic set of methods that ensure that all
components "look" the same in your application. The GCF::AbstractComponent class has
the following functionality.

- It has to provide some basic meta data about the system like
    -# Component Name. (Preferably a name starting with a alphabet and containing no special
characters and spaces)
    -# Location of the component XML file. (This file is basically a description of how this component
wants to fit into an application.)
- Return (by creating if necessary) instances of actions, objects, widgets, menus and toolbars
referenced in the GUI XML file.
- Welcome and bid-goodbye to widgets and objects from other components.

If the component provides the above functionality, the "component framework" in return provides the
following services to the component
- It merges the GUI (actions, menus, toolbars, widgets and objects) of the component into the main
window of the application.
- It provides an opportunity to deal with how widgets from other components can be
placed/removed/hidden/shown on to widgets of this component.
- It informs the component when any other component gets created or destroyed in the system.
- It informs the component when any other component references a object within this component.
- Provides means by which the component can explore objects provided by other components in the
system.

\section gcfbrief1 GCF::AbstractComponent Subclassing Rules.

- Component classes must be singleton classes.
- The initializeComponent() and finalizeComponent() methods must be implemented
- Class declarations should contain a \c GCF_DECLARE_COMPONENT() line right
after \c Q_OBJECT. (Yes, please use \c Q_OBJECT for all components).
- Class implementations must contain a \c GCF_DEFINE_COMPONENT() line.
- Export the component class using the \c GCF_EXPORT_COMPONENT_PLUGIN() macro

Example
\code
// MyComponent.h
#include <GCF/AbstractComponent>

class MyComponent : public GCF::AbstractComponent
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(MyComponent)

public:
    static MyComponent & instance();
    ~MyComponent();

protected:
    MyComponent();
    void initializeComponent();
    void finalizeComponent();
};

// MyComponent.cpp
#include "MyComponent.h"

GCF_DEFINE_COMPONENT(MyComponent)

MyComponent & MyComponent::instance()
{
    static MyComponent* theInstance = CREATE_INSTANCE(MyComponent);
    return *theInstance;
}

MyComponent::MyComponent()
{
    // Constructor
}

MyComponent::~MyComponent()
{
    // Destructor
}

void MyComponent::initializeComponent()
{
    // Initialization code here
}

void MyComponent::finalizeComponent()
{
    // Finalization code here
}

GCF_EXPORT_COMPONENT_PLUGIN(MyComponent)
\endcode

\sa \ref gcfwithqtcreator

\section gcfbrief4 Component Initialization

Components are initialized when their initialize() method is called. When the initialize() method is called, the
following things happen

\li First the components initializeComponent() method is called. At this point, components can set
their component names and/or GUIXML file names and/or do any other initialization.
\li If setComponentName() was not used to set the name of the component in the initializeComponent(),
then the name of the component class is set as the component name.
\li If setGuiXmlFileName() was not used to set the name of the component's GUIXML file; then the
GUIXML file is assumed to be name of the component folled by the ".xml" extention. This GUIXML file
is then loaded and processed.
\li Component settings for this component is then loaded.
\li The componentCreated() method is called on each and every component that is already available in the
application. To this method pointer of this component is passed; that way the creation of this
component is made known to other component.
\li Finally the creationAnnounced() method on this component is called.

\section gcfbrief5 Component Finalization

A component is finalized by explicitly calling the finalize() method on it. If a component is not
explicitly finalized, then GCF::Application finalizes each component before quitting. Components have
to be finalized based on their dependencies. Dependent components have to be finalized before a component
is finalized.

When the finalize() method is called on a component, the following things happen

\li The componentDestroyed() method is called on each and every component in the system to inform
that this component is destroyed.
\li The destructionAnnounced() method is called on this component
\li Dependent components are finalized.
\li The finalizeComponent() method is called.
\li The component's GUI is un-initialized
\li Component settings are saved on to the disk
\li The component object is deleted

\section gcfbrief2 Static and Dynamic components

There are two types of components in GCF: Static and Dynamic. Static components, as the name suggests, are loaded
and initialized statically - typically by the \c main() function. Normally "Application Container" components are
statically loaded. See \ref gettingstarted_concepts for more information. Static components are registered using
the \c GCF_REGISTER_COMPONENT macro in the \c main() function. Take a look at the code sample below.

\includelineno Simple/Application/Main.cpp

Dynamic components are loaded at run time by the application by looking into either the \c ApplicationXML file
(\ref componentarch_applicationxml) or the \c ComponentLoaderComponent.xml file; which is explained next.

\section gcfbrief3 The GCF::ComponentLoaderComponent class

The GCF::ComponentLoaderComponent class loads components from shared object files (DLL/.so files). It has a function
called \c loadComponent2() that takes as parameter \c componentName and \c libName. The first parameter to this function
specifies the name of the component (which is the name of the component class or the name set via setComponentName()). The
second parameter specifies the library from which the component should be loaded.

\code
class GCF::ComponentLoaderComponent : public GCF::AbstractComponent
{
// ....
public slots:
    void loadComponent2(const QString& compName, const QString& libName);
// .....
};
\endcode

This function is usually never called by the developer, although there is no hindrance to calling it. Components are
loaded either by the ApplicationXML file (\ref componentarch_applicationxml) or by the GUIXML file of
GCF::ComponentLoaderComponent. By default GCF loads components from the ApplicationXML file first and then from
the \c ComponentLoaderComponent.xml file. Shown below is a sample ComponentLoaderComponent.xml file.

\code
<component title="Library Loader Component" name="ComponentLoaderComponent" >

    <object name="controller" />

  <method param1="MessageLogComponent"
          param2="MessageLog"
          methodName="ComponentLoaderComponent.controller::loadComponent2" />

  <method param1="UtilsComponent"
          param2="Utils"
          methodName="ComponentLoaderComponent.controller::loadComponent2" />

  <method param1="ProjectManagerComponent"
          param2="ProjectManager"
          methodName="ComponentLoaderComponent.controller::loadComponent2" />

  <method param1="PropertyEditorComponent"
          param2="PropertyEditor"
          methodName="ComponentLoaderComponent.controller::loadComponent2" />

</component>
\endcode
*/

struct GCF::AbstractComponentData
{
    AbstractComponentData() : initialized(false), initializing(false),
        active(false), inFinalize(false), inSetActive(false) { }

    QString componentName;
    QString guiXmlFile;
    bool initialized;
    bool initializing;
    GCF::ComponentGui gui;
    bool active;
    GCF::DataStore componentData;
    bool inFinalize;
    bool inSetActive;
    GCF::ComponentSettings* componentSettings;

    void saveComponentSettings();
    void loadComponentSettings();

    QMap<GCF::AbstractComponent*, int> dependentComps;
    QMap<GCF::AbstractComponent*, int> dependingComps;
};

static QString & guiXmlFileDir()
{
    static QString ret(".");
    return ret;
}

GCF::ComponentHostMainWindow & guiHostWindow(QMainWindow* mainWindow)
{
    static GCF::ComponentHostMainWindow theInstance(mainWindow);
    return theInstance;
}

static IComponentHostWindow* ComponentHostWindow = 0;
static bool MenuStripEnabled = false;

/**
\internal
*/
void GCF::AbstractComponent::setGuiXmlFileDirectory(const QString& absDirPath)
{
    guiXmlFileDir() = absDirPath;
}

/**
\internal
*/
QString GCF::AbstractComponent::guiXmlFileDirectory()
{
    return guiXmlFileDir();
}

/**
\internal
*/
void GCF::AbstractComponent::setMainWindow(QMainWindow* mainWindow)
{
    setComponentHostWindow( &(guiHostWindow(mainWindow)) );
}

/**
\internal
*/
QMainWindow* GCF::AbstractComponent::mainWindow()
{
    if( !componentHostWindow() )
        return 0;

    return qobject_cast<QMainWindow*>( componentHostWindow()->windowWidget() );
}

/**
\internal
*/
void GCF::AbstractComponent::setComponentHostWindow(IComponentHostWindow* hostWindow)
{
    ComponentHostWindow = hostWindow;
}

/**
\internal
*/
IComponentHostWindow* GCF::AbstractComponent::componentHostWindow()
{
    return ComponentHostWindow;
}

/**
\internal
*/
QWidget* GCF::AbstractComponent::componentHostWidget()
{
    if( ComponentHostWindow )
        return ComponentHostWindow->windowWidget();

    return 0;
}

/**
\internal
*/
void GCF::AbstractComponent::setMenuStripEnabled(bool)
{
    if(mainWindow())
        return;

    MenuStripEnabled = false;
    qDebug("Cannot enable MenuStrip from this verison of GCF");
}

/**
\internal
*/
bool GCF::AbstractComponent::isMenuStripEnabled()
{
    return MenuStripEnabled;
}

/**
Component constructor. The constructor is protected, because instances of this class can
only be created by subclassing.
*/
GCF::AbstractComponent::AbstractComponent():QObject(&GCF::ComponentFactory::instance())
{
    d = new GCF::AbstractComponentData;
    d->componentSettings = new GCF::ComponentSettings(this);
    d->gui.setComponent(this);

    GCF::ComponentFactory::instance().registerComponent(this);
}

/**
Destructor
*/
GCF::AbstractComponent::~AbstractComponent()
{
    delete d->componentSettings;
    delete d;
    GCF::ComponentFactory::instance().unregisterComponent(this);
}

/**
This method is used to determine the name of the component. While the
component name can be queried by any class in the system, the component name can
only be set by the component itself. Hence setComponentName() is declared as protected.
Subclasses of GCF::AbstractComponent must set their component name in their constructor
or within their implementation of initializeComponent(). If a component does not set
a component-name, by default the component name is assumed to be the name of the
component class itself.

\note Component names must be unique within the system. This rule has to be enforced
by developers themselves. GCF does not enforce or check for uniqueness of component names.
*/
QString GCF::AbstractComponent::componentName() const
{
    return d->componentName;
}

/**
\internal
*/
GCF::ComponentGui GCF::AbstractComponent::componentGui() const
{
    return d->gui;
}

/**
This method is used to figure out the name of the GUI XML file to read while loading the
GUI for this component. The file name returned by this method is assumed to be relative
to guiXmlFileDirectory().
*/
QString GCF::AbstractComponent::guiXmlFileName() const
{
    return d->guiXmlFile;
}

/**
This method returns true if the component is active. False otherwise.
*/
bool GCF::AbstractComponent::isActive() const
{
    return d->active;
}

/**
This method returns a list of pointers to components that depend on this component.
*/
QList<GCF::AbstractComponent*> GCF::AbstractComponent::dependentComponents() const
{
    return d->dependentComps.keys();
}

/**
This method returns a list of pointers to components the this component depends on.
*/
QList<GCF::AbstractComponent*> GCF::AbstractComponent::dependingComponents() const
{
    return d->dependingComps.keys();
}

/**
Generic data storage for the component. You can store any custom data about the component in this
data store.
*/
GCF::DataStore GCF::AbstractComponent::componentData() const
{
    return d->componentData;
}

/**
Initializes this component if it has not already been initialized. For static components,
this function should be explicitly called by developers. For dynamic components, this method
is automatically called by GCF upon loading the component.

\sa \ref gcfbrief2, \ref gcfbrief4
*/
void GCF::AbstractComponent::initialize()
{
    if(d->initialized || d->initializing)
        return;

    d->initializing = true;

    this->initializeComponent();

    if(d->componentName.isEmpty())
        setComponentName( QString("%1").arg(metaObject()->className()) );

    if(d->guiXmlFile.isEmpty())
        setGuiXmlFileName( QString("%1.xml").arg(d->componentName) );

    d->initialized = true;
    d->initializing = false;

    d->loadComponentSettings();

    GCF::ComponentFactory::instance().announceComponentCreation(this);
    this->creationAnnounced();
}

/**
Finalizes this component. By the time function returns, the component will be deleted
and the UI created by this component will be released.

\sa \ref gcfbrief5
*/
void GCF::AbstractComponent::finalize()
{
    if(d->inFinalize)
        return;

    d->inFinalize = true;

    GCF::ComponentFactory::instance().announceComponentDestruction(this);
    this->destructionAnnounced();

    if(d->dependentComps.count())
    {
        QList<GCF::AbstractComponent*> depComps = d->dependentComps.keys();
        for(int i=0; i<depComps.count(); i++)
        {
            GCF::AbstractComponent* comp = depComps[i];
            if(GCF_CHECK_COMPONENT_PTR(comp))
                comp->finalize();
        }
    }

    this->finalizeComponent();
    d->gui.clear();

    d->saveComponentSettings();

    ::delete this;
}


/**
This method can be used by subclasses to activate or deactivate the component. Active
components have their GUI as active. Inactive components have their GUI as inactive.
For example: menu and toolbar items associated with active components will appear
enabled for active components, disabled for inactive components.
*/
void GCF::AbstractComponent::setActive(bool val)
{
    if(d->inSetActive)
        return;

    if(d->active == val)
        return;

    d->inSetActive = true;

    if(val)
    {
        this->aboutToActivate();
        d->gui.showGui();
        d->active = val;
        this->justActivated();
    }
    else
    {
        this->aboutToDeactivate();
        d->gui.hideGui();
        d->active = val;
        this->justDeactivated();
    }

    d->inSetActive = false;
}


/**
This function can be used by subclasses to set the name of the component. If no
name is specified, then the component name is assumed to be the name of the component
class.

\note A component name, once set, cannot be changed ever!
\note Component names must not have space characters
\note Component names cannot have namespaces or '.'
*/
void GCF::AbstractComponent::setComponentName(const QString & name)
{
    if(!d->componentName.isEmpty())
        return;

    if(name.contains("::"))
    {
        QStringList comps = name.split("::");
        d->componentName = comps.last();
    }
    else
        d->componentName = name;
}

/**
The setGuiXmlFileName() method can be used by subclasses to specify the name of the GUIXML
file. Developers can specify the GUIXML file name from Qt Resource File System as well.
If no name is specified the GUIXML file name is assumed to be componentName() with a ".xml"
extention.

The GUIXML will be loaded and processed from the specified file before this function returns.
*/
void GCF::AbstractComponent::setGuiXmlFileName(const QString& name)
{
    if(d->initialized)
        qWarning("%s: Setting GUI XML filename after the component has been initialized\n",
                 d->componentName.toAscii().data());
    if(!d->guiXmlFile.isEmpty())
        return;

    d->guiXmlFile = name;

    if(d->componentName.isEmpty())
        setComponentName( QString("%1").arg(metaObject()->className()) );

    GCF::ComponentGuiXmlParser parser;
    this->aboutToActivate();
    if( name.startsWith(':') )
        parser.parseGui(d->gui, d->guiXmlFile);
    else
    {
        if(QFileInfo(name).exists())
            parser.parseGui(d->gui, d->guiXmlFile);
        else
            parser.parseGui(d->gui, QString("%1/%2").arg(guiXmlFileDir()).arg(d->guiXmlFile));
    }
    d->active = true;
    this->justActivated();
}

/**
This function establishes a dependencyu between two components. By calling this function
you are letting GCF know that \c comp2 depends on resources in \c comp1. This means that
\c comp2 should be finalized before \c comp1.
*/
void GCF::AbstractComponent::addDependency(GCF::AbstractComponent* comp1, GCF::AbstractComponent* comp2)
{
    if(!comp1 || !comp2)
        return;

    comp1->addDependentComponent(comp2);
    comp2->addDependingComponent(comp1);
}

/**
Removes dependency between components.
*/
void GCF::AbstractComponent::removeDependency(GCF::AbstractComponent* comp1, GCF::AbstractComponent* comp2)
{
    if(!comp1 || !comp2)
        return;

    comp1->removeDependentComponent(comp2);
    comp2->removeDependingComponent(comp1);
}

/**
\internal
*/
QStringList GCF::AbstractComponent::componentNames() const
{
    return GCF::ComponentFactory::instance().componentNames();
}

/**
\internal
*/
GCF::AbstractComponent* GCF::AbstractComponent::findComponent(const QString& componentName) const
{
    return GCF::ComponentFactory::instance().findComponent(componentName);
}

/**
\internal
*/
QAction* GCF::AbstractComponent::findAction(const QString& completeName, GCF::AbstractComponent** component) const
{
    if(component)
        *component = 0;

    GCF::ComponentGuiNode node = d->gui.findNode(completeName, true);
    if(node.isValid() && node.hasKey("QAction"))
    {
        if(component)
            *component = node.component();
        return node["QAction"].value<QAction*>();
    }
    return 0;
}


/**
\internal
*/
QObject* GCF::AbstractComponent::findObject(const QString& completeName, GCF::AbstractComponent** component) const
{
    if(component)
        *component = 0;

    GCF::ComponentGuiNode node = d->gui.findNode(completeName, true);
    QObject* ret = ObjectIterator::object(node);
    if(ret && component)
        *component = node.component();

    return ret;
}

/**
\internal
*/
QWidget* GCF::AbstractComponent::findWidget(const QString& completeName, GCF::AbstractComponent** component) const
{
    if(component)
        *component = 0;

    GCF::ComponentGuiNode node = d->gui.findNode(completeName, true);
    if(node.isValid() && node.hasKey("QWidget"))
    {
        if(component)
            *component = node.component();
        return node["QWidget"].value<QWidget*>();
    }
    return 0;
}


/**
\internal
*/
QMenu* GCF::AbstractComponent::findMenu(const QString& completeName, GCF::AbstractComponent** component) const
{
    if(component)
        *component = 0;

    GCF::ComponentGuiNode node = d->gui.findNode(completeName, true);
    if(node.isValid() && node.hasKey("QMenu"))
    {
        if(component)
            *component = node.component();
        return node["QMenu"].value<QMenu*>();
    }
    return 0;
}


/**
\internal
*/
QToolBar* GCF::AbstractComponent::findToolBar(const QString& completeName, GCF::AbstractComponent** component) const
{
    if(component)
        *component = 0;

    GCF::ComponentGuiNode node = d->gui.findNode(completeName, true);
    if(node.isValid() && node.hasKey("QToolBar"))
    {
        if(component)
            *component = node.component();
        return node["QToolBar"].value<QToolBar*>();
    }
    return 0;
}

/**
\internal
*/
QObject* GCF::AbstractComponent::findObject(const QString& ifaceName, QString& completeName, GCF::AbstractComponent** component) const
{
    if(component)
        *component = 0;
    completeName.clear();

    QStringList comps = ifaceName.split(".");
    QString compName = comps.count() == 2 ? comps.first() : "";
    QString iName = comps.last();

    ObjectIterator objIt(compName);
    objIt.setFirstComponent( const_cast<GCF::AbstractComponent*>(this) );

        QByteArray iNameBA = iName.toLatin1();

    objIt.firstObject();
    while(1)
    {
        QObject* object = objIt.currentObject();
        if(!object)
            break;

        if(object->inherits(iNameBA.data()))
        {
            completeName = objIt.currentObjectName();
            if(component)
                *component = objIt.currentObjectComponent();
            return object;
        }

        objIt.nextObject();
    }

    return 0;
}

/**
\internal
*/
QList<QObject*> GCF::AbstractComponent::findObjects(const QString& ifaceName, QStringList& completeNames, QList<GCF::AbstractComponent*>* components) const
{
    QList<QObject*> ret;

    if(components)
        (*components).clear();
    completeNames.clear();

    QStringList comps = ifaceName.split(".");
    QString compName = comps.count() == 2 ? comps.first() : "";
    QString iName = comps.last();

    ObjectIterator objIt(compName);
    objIt.setFirstComponent(const_cast<GCF::AbstractComponent*>(this));

    objIt.firstObject();
    while(1)
    {
        QObject* object = objIt.currentObject();
        if(!object)
            break;

        if(object->inherits(iName.toAscii().data()))
        {
            completeNames << objIt.currentObjectName();
            if(components)
                (*components) << objIt.currentObjectComponent();
            ret << object;
        }

        objIt.nextObject();
    }

    return ret;
}

/**
\fn QObject* GCF::findObject(const QString& completeName)

This function returns the QObject mapped for \c completeName.
*/

/**
\fn T GCF::findObject(QString* name=0, GCF::AbstractComponent** comp=0) const

This function can be used by to find an object in the application that implements
a specific interface or inherits from a specific subclass.

For example, lets say your component needs to make use of the property editor. So you might
want to look for the IPropertyEditor interface. But you dont know what component provides that
interface. You can simply use the following code snippet to look for the property editor

\code
IPropertyEditor* propertyEditor = findObject<IPropertyEditor*>();
if(propertyEditor)
{
    // Use the property editor here.
}
\endcode

Through the second parameter you can fetch the complete name of the object that implements
the requested interface. And through the third you can catch hold of the owner of the object.

\code
QString name;
GCF::AbstractComponent* comp = 0;
IPropertyEditor* propertyEditor = findObject<IPropertyEditor*>(&name, &comp);
if(propertyEditor)
{
    // Use the property editor here.
}
\endcode
*/

/**
\fn QList<T> GCF::findObjects(QStringList* names=0, QList<GCF::AbstractComponent*>* comps=0) const

Using this method you can fetch objects in the application that implement a specific interface or
inherits a specific class. Through the second and third parameters you can fetch an associative
list of object names and owning components.

Example
\code
QList<QTreeView*> treeViews = findObjects<QTreeView*>();
Q_FOREACH(QTreeView* tv, treeViews)
{
    // Use tv here
}
\endcode
*/

/**
\fn T GCF::findObject(GCF::AbstractComponent* component, QString* name=0) const

Using this method you can search for an object, within the component specified as fist paramter,
that implements an interface. Through the second parameter you can fetch the complete
name of the object.

Example
\code
GCF::AbstractComponent* comp = GCF::ComponentFactory::instance().findComponent("PropertyEditorComponent");
IPropertyEditor* propEditor = findObject<IPropertyEditor*>(comp);
\endcode
*/

/**
\fn QList<T> GCF::findObjects(GCF::AbstractComponent* component, QStringList* names=0) const

Using this method you can search for objects, within the component specified as fist paramter,
that implement an interface. Through the second and third parameters you can fetch an associative
list of object names and owning components.
*/

/**
\fn T GCF::findObject(const QString& componentName, QString* name=0) const

Using this method you can search for an object, within the component specified as fist paramter,
that implements an interface. Through the second parameter you can fetch the complete
name of the object.

Example
\code
IPropertyEditor* propEditor = findObject<IPropertyEditor*>("PropertyEditorComponent");
\endcode
*/

/**
\fn QList<T> GCF::findObjects(const QString& componentName, QStringList* names=0) const

Using this method you can search for objects, within the component specified as fist paramter,
that implement an interface. Through the second and third parameters you can fetch an associative
list of object names and owning components.
*/

/**
This function is called to resolve the \c completeName of a action into a QAction instance.
The function is called while parsing the GUIXML associated with this component and when
a \c <action> XML element is found in it.

The default implementation creates a \c QAction instance and returns it.
*/
QAction* GCF::AbstractComponent::fetchAction(const QString& completeName) const
{
    GCF::AbstractComponent* that = const_cast<GCF::AbstractComponent*>(this);
    QAction* ret = new QAction(that);
    ret->setObjectName(completeName);
    return ret;
}

/**
This function is called to resolve the \c completeName of a action into a QObject instance.
The function is called while parsing the GUIXML associated with this component and when
a \c <object> XML element is found in it.

The default implementation returns a NULL pointer.
*/
QObject* GCF::AbstractComponent::fetchObject(const QString& completeName) const
{
    Q_UNUSED(completeName);
    return 0;
}

/**
This function is called to resolve the \c completeName of a action into a QWidget instance.
The function is called while parsing the GUIXML associated with this component and when
a \c <widget> XML element is found in it.

The default implementation returns a NULL pointer.
*/
QWidget* GCF::AbstractComponent::fetchWidget(const QString& completeName) const
{
    Q_UNUSED(completeName);
    return 0;
}

/**
This function is called to resolve the \c completeName of a action into a QMenu instance.
The function is called while parsing the GUIXML associated with this component and when
a \c <menu> XML element is found in it.

The default implementation creates a \c QMenu instance and returns it.
*/
QMenu* GCF::AbstractComponent::fetchMenu(const QString& completeName) const
{
    QMenu* menu = new GCF::Menu;
    menu->setObjectName(completeName);
    return menu;
}

/**
This function is called to resolve the \c completeName of a action into a QAction instance.
The function is called while parsing the GUIXML associated with this component and when
a \c <toolbar> XML element is found in it.

The default implementation creates a \c QToolBar instance and returns it.
*/
QToolBar* GCF::AbstractComponent::fetchToolBar(const QString& completeName) const
{
    QToolBar* tb = new QToolBar;
    tb->setObjectName(completeName);
    return tb;
}

/**
This function is called when a \c child QObject created by this or any other component
needs to be made a child of \c parent QObject created and owned by this component. This
function is called while GCF processes the \c parentObject attribute in the \c <object> XML
element of a GUIXML file. The hint parameter contains value of \c parentObjectHint attribute
in the \c <object> XML element.

The default implementation simply reparents \c child to \c parent.
*/
void GCF::AbstractComponent::addChildObject(QObject* parent, QObject* child, const QString& hint)
{
    if(child && parent)
        child->setParent(parent);
    Q_UNUSED(hint);
}

/**
This function is called when a \c child QObject created by this or any other component
needs to be removed as a child of \c parent QObject created and owned by this component.

The default implementation simply reparents \c child to a NULL pointer.
*/
void GCF::AbstractComponent::removeChildObject(QObject* parent, QObject* child, const QString& hint)
{
    if(child && parent)
        child->setParent(0);
    Q_UNUSED(hint);
}


/**
This function is called when a \c child QWidget created by this or any other component
needs to be made a child of \c parent QWidget created and owned by this component. This
function is called while GCF processes the \c parentWidget attribute in the \c <widget> XML
element of a GUIXML file. The hint parameter contains value of \c parentWidgetHint attribute
in the \c <widget> XML element.

The default implementation simply reparents \c child to \c parent.
*/
void GCF::AbstractComponent::addChildWidget(QWidget* parent, QWidget* child, const QString& hint)
{
    if(child && parent)
        child->setParent(parent);
    Q_UNUSED(hint);
}

/**
This function is called when a \c child QWidget created by this or any other component
needs to be removed as a child of \c parent QWidget created and owned by this component.

The default implementation simply reparents \c child to a NULL pointer.
*/
void GCF::AbstractComponent::removeChildWidget(QWidget* parent, QWidget* child, const QString& hint)
{
    if(child && parent)
        child->setParent(0);
    Q_UNUSED(hint);
}

/**
This function is called when the \c child widget needs to be shown in \c parent, which is a widget owned
by this component. The default implementation calls \c show() on the \c child QWidget.
*/
void GCF::AbstractComponent::showChildWidget(QWidget* parent, QWidget* child, const QString& hint)
{
    Q_UNUSED(parent);
    if(child)
        child->show();
    Q_UNUSED(hint);
}

/**
This function is called when the \c child widget needs to be hidden in \c parent, which is a widget owned
by this component. The default implementation calls \c hide() on the \c child QWidget.
*/
void GCF::AbstractComponent::hideChildWidget(QWidget* parent, QWidget* child, const QString& hint)
{
    Q_UNUSED(parent);
    if(child)
        child->hide();
    Q_UNUSED(hint);
}

/**
This function is called when a new component was created in the application. The \c comp parameter
is never NULL, and is never pointing to this component itself. It will always be pointing to the
component that got created. The default implementation does nothing.

\sa \ref gcfbrief4
*/
void GCF::AbstractComponent::componentCreated(GCF::AbstractComponent* comp)
{
    Q_UNUSED(comp);
}

/**
This function is called when an existing component was destroyed in the application. The \c comp parameter
is never NULL, and is never pointing to this component itself. It will always be pointing to the
component that got destroyed. The default implementation does nothing.

\sa \ref gcfbrief5
*/
void GCF::AbstractComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    Q_UNUSED(comp);
}

/**
This function is called when the creation of this component has been announced to all the other
components in the application. The default implementaiton does nothing.

\sa \ref gcfbrief4
*/
void GCF::AbstractComponent::creationAnnounced()
{

}

/**
This function is called when the destruction of this component has been announced to all the other
components in the application. The default implementaiton does nothing.

\sa \ref gcfbrief5
*/
void GCF::AbstractComponent::destructionAnnounced()
{

}

/**
\internal
*/
void GCF::AbstractComponent::registerDependentComponent(GCF::AbstractComponent* comp)
{
    Q_UNUSED(comp);
}

/**
\internal
*/
void GCF::AbstractComponent::unregisterDependentComponent(GCF::AbstractComponent* comp)
{
    Q_UNUSED(comp);
}

/**
\internal
*/
void GCF::AbstractComponent::registerDependingComponent(GCF::AbstractComponent* comp)
{
    Q_UNUSED(comp);
}

/**
\internal
*/
void GCF::AbstractComponent::unregisterDependingComponent(GCF::AbstractComponent* comp)
{
    Q_UNUSED(comp);
}


/**
\fn void GCF::AbstractComponent::initializeComponent() = 0

These pure virtual functions are called when the component is about to be initialized or
about to be destroyed. The  methods are called by CF at appropriate times. Subclasses must
implement these functions to perform  initialization and finalization operations.
*/

/**
\fn void GCF::AbstractComponent::finalizeComponent() = 0

These pure virtual functions are called when the component is about to be initialized or
about to be destroyed. The  methods are called by CF at appropriate times. Subclasses must
implement these functions to perform  initialization and finalization operations.
*/

/**
This function is called when the component is just about to be activated. Within this function
the component is not yet active. The default implementation does nothing.
*/
void GCF::AbstractComponent::aboutToActivate()
{
    // TODO
}

/**
This function is called when the component has just been activated. Within this function the
component has become active. The default implementation does nothing.
*/
void GCF::AbstractComponent::justActivated()
{
    // TODO
}

/**
This function is called when the component is just about to be deactivated. Within this function
the component is still active. The default implementation does nothing.
*/
void GCF::AbstractComponent::aboutToDeactivate()
{
    // TODO
}

/**
This function is called when the component has just been deactivated. The default implementation
does nothing.
*/
void GCF::AbstractComponent::justDeactivated()
{
    // TODO
}

/**
Reimplement this function to release resources before the finalize() method is called. This
method is called in all components first. Only after this method has been called on all the
components; will the finalization of the components actually begin.

The default implementation does nothing.
*/
void GCF::AbstractComponent::aboutToFinalize()
{
    // TODO
}

/**
\internal
*/
void GCF::AbstractComponent::addDependentComponent(GCF::AbstractComponent* comp)
{
    if(!comp || comp == this)
        return;

    int depCount = 0;
    if(d->dependentComps.contains(comp))
        depCount = d->dependentComps[comp];
    ++depCount;
    d->dependentComps[comp] = depCount;
    if(depCount == 1)
        this->registerDependentComponent(comp);
}

/**
\internal
*/
void GCF::AbstractComponent::removeDependentComponent(GCF::AbstractComponent* comp)
{
    if(!comp || comp == this)
        return;

    if(!d->dependentComps.contains(comp))
        return;
    int depCount = d->dependentComps[comp];
    --depCount;
    if(!depCount)
    {
        d->dependentComps.remove(comp);
        this->unregisterDependentComponent(comp);
    }
}

/**
\internal
*/
void GCF::AbstractComponent::addDependingComponent(GCF::AbstractComponent* comp)
{
    if(!comp || comp == this)
        return;

    int depCount = 0;
    // if(d->dependentComps.contains(comp))
    if(d->dependingComps.contains(comp)) // Fix sent by Tony Rietwyk (tony.rietwyk@rightsoft.com.au)
        depCount = d->dependingComps[comp];
    ++depCount;
    d->dependingComps[comp] = depCount;
    if(depCount == 1)
        this->registerDependingComponent(comp);
}

/**
\internal
*/
void GCF::AbstractComponent::removeDependingComponent(GCF::AbstractComponent* comp)
{
    if(!comp || comp == this)
        return;

    if(!d->dependingComps.contains(comp))
        return;
    int depCount = d->dependingComps[comp];
    --depCount;
    if(!depCount)
    {
        d->dependingComps.remove(comp);
        this->unregisterDependingComponent(comp);
    }
}

/**
\internal
*/
void GCF::AbstractComponent::objectDestroyed(QObject* obj)
{
    if(!obj)
        return;

    GCF::ComponentGuiNode node = obj->property("_node_").value<GCF::ComponentGuiNode>();
    if(node.isValid())
    {
        bool removeNode = false;

        // check if any other key is a QObject, QWidget, QAction, QMenu or QToolBar
        QStringList keys = node.keys();
        for(int i=0; i<keys.count(); i++)
        {
            QString key = keys[i];
            QVariant & value = node[key];
            if(!qstrcmp(value.typeName(), "QObject*") && value.value<QObject*>()==obj)
            {
                value = qVariantFromValue<QObject*>(0);
                removeNode |= true;
            }
            else if(!qstrcmp(value.typeName(), "QWidget*") && value.value<QWidget*>()==obj)
            {
                value = qVariantFromValue<QWidget*>(0);
                removeNode |= true;
            }
            else if(!qstrcmp(value.typeName(), "QAction*") && value.value<QAction*>()==obj)
            {
                value = qVariantFromValue<QAction*>(0);
                removeNode |= true;
            }
            else if(!qstrcmp(value.typeName(), "QMenu*") && value.value<QMenu*>()==obj)
            {
                value = qVariantFromValue<QMenu*>(0);
                removeNode |= true;
            }
            else if(!qstrcmp(value.typeName(), "QToolBar*") && value.value<QToolBar*>()==obj)
            {
                value = qVariantFromValue<QToolBar*>(0);
                removeNode |= true;
            }
#ifdef GCF_ACTIVEQT_AVAILABLE
            else if(!qstrcmp(value.typeName(), "QAxWidget*") && value.value<QAxWidget*>()==obj)
            {
                value = qVariantFromValue<QAxWidget*>(0);
                removeNode |= true;
            }
#endif
#ifdef GCF_KPARTS_AVAILABLE
            else if(!qstrcmp(value.typeName(), "KParts::Part*") && value.value<KParts::Part*>()==obj)
            {
                value = qVariantFromValue<KParts::Part*>(0);
                removeNode |= true;
            }
#endif
        }

        if( removeNode && node.hasKey("autoDeleteNode") && node["autoDeleteNode"].toBool() )
        {
            if( node.parentNode().isValid() )
                node.parentNode().removeChild( node );
            else
                componentGui().removeNode(node);
        }
    }
}

/**
This function can be implemented in subclasses to create a new
instance of the object referenced by \c completeName. The caller
has to take the responsibility of actually deleting the object
returned by this function.

Implementations can return a null pointer to signify that a
duplicate copy of that object cannot be created.
*/
QObject* GCF::AbstractComponent::createObject(const QString& completeName) const
{
    Q_UNUSED(completeName);
    return 0;
}

/**
This function can be implemented in subclasses to create a new
instance of the object referenced by \c completeName. The caller
has to take the responsibility of actually deleting the object
returned by this function.

Implementations can return a null pointer to signify that a
duplicate copy of that object cannot be created.
*/
QWidget* GCF::AbstractComponent::createWidget(const QString& completeName) const
{
    Q_UNUSED(completeName);
    return 0;
}

/**
Delete operator overload. Just in case the developer forgets to call finalize(),
this overload ensures proper finalization of the component.
*/
void GCF::AbstractComponent::operator delete(void* mem)
{
    // Suggested by Till Adam of KDAB

    GCF::AbstractComponent* comp = static_cast<GCF::AbstractComponent*>(mem);
    // For some strange reason static_cast<> doesnt work :(. It always returns a null pointer
    if(comp)
        comp->finalize();
}

/**
\internal
*/
QList<QObject*> GCF::AbstractComponent::findObjects(const QString& ifaceName, QList< QPair<QString,GCF::AbstractComponent*> >* objectInfoList ) const
{
    // Suggested by Till Adam of KDAB

    QStringList compNames;
    QList<GCF::AbstractComponent*> comps;
    QList<QObject*> retList = findObjects(ifaceName, compNames, &comps);

    if( objectInfoList )
    {
        for(int i=0; i<comps.count(); i++)
            objectInfoList->append( QPair<QString,GCF::AbstractComponent*>(compNames[i], comps[i]) );
    }

    return retList;
}

/**
Returns a non-const reference to the component's settings object. This object is automatically loaded
and saved.

\note This method can only by used by subclasses of \ref AbstractComponent
*/
GCF::ComponentSettings& GCF::AbstractComponent::settings()
{
    return *(d->componentSettings);
}

/**
Returns a const reference to the component's settings object. This object is automatically loaded
and saved.
*/
const GCF::ComponentSettings& GCF::AbstractComponent::constSettings() const
{
    return *(d->componentSettings);
}

void GCF::AbstractComponentData::saveComponentSettings()
{
    if(qApp->organizationName().isEmpty())
        qApp->setOrganizationName("VCreateLogic");

#if 0
    QString settingsFile = QString("%1/%2Settings.xml").arg(guiXmlFileDir()).arg(componentName);
#else
#ifdef Q_OS_WIN
    QString dataFolder = QString("%1/%2/%3")
            .arg(QProcessEnvironment::systemEnvironment().value("APPDATA"))
            .arg(qApp->organizationName()).arg(qApp->applicationName());
#else
    QString dataFolder = QString("%1/.%2/%3")
            .arg(QDesktopServices::storageLocation(QDesktopServices::HomeLocation))
            .arg(qApp->organizationName()).arg(qApp->applicationName());
#endif
    QDir().mkpath(dataFolder);
    QString settingsFile = QString("%1/%2Settings.xml").arg(dataFolder).arg(componentName);
#endif

    if(this->componentSettings->isEmpty())
    {
        QFile::remove(settingsFile);
        return;
    }

    QFile file(settingsFile);
    if( !file.open(QFile::WriteOnly) )
    {
        qWarning("Cannot open %s for writing", qPrintable(settingsFile));
        return;
    }

    QDomDocument doc;
    QDomElement docE = doc.createElement("Settings");
    doc.appendChild(docE);

    this->componentSettings->writeSettings(doc, docE);

    QString docStr = doc.toString();
    file.write( docStr.toAscii() );
    file.close();
}

void GCF::AbstractComponentData::loadComponentSettings()
{
    if(qApp->organizationName().isEmpty())
        qApp->setOrganizationName("VCreateLogic");

#if 0
    QString settingsFile = QString("%1/%2Settings.xml").arg(guiXmlFileDir()).arg(componentName);
#else
#ifdef Q_OS_WIN
    QString dataFolder = QString("%1/%2/%3")
            .arg(QProcessEnvironment::systemEnvironment().value("APPDATA"))
            .arg(qApp->organizationName()).arg(qApp->applicationName());
#else
    QString dataFolder = QString("%1/.%2/%3")
            .arg(QDesktopServices::storageLocation(QDesktopServices::HomeLocation))
            .arg(qApp->organizationName()).arg(qApp->applicationName());
#endif
    QString settingsFile = QString("%1/%2Settings.xml").arg(dataFolder).arg(componentName);
#endif

    if(!QFile::exists(settingsFile))
        return;

    QFile file(settingsFile);
    if( !file.open(QFile::ReadOnly) )
    {
        qWarning("Cannot open %s for reading", qPrintable(settingsFile));
        return;
    }

    QDomDocument doc;
    doc.setContent(&file);

    QDomElement docE = doc.documentElement();
    this->componentSettings->readSettings(doc, docE);
}

#include <GCF/IComponentPlugin>

void IComponentPlugin::registerComponentInfo(const QString& className, CreateComponentFunction createFn, const QMetaObject* meta)
{
    GCF::ComponentFactory::instance().registerComponentInfo(className, createFn, meta);
}

QString IComponentPlugin::systemBuildKey()
{
    return GCF_BUILD_KEY;
}

//////////////////////////////////////////////////////////////////////////////////////////
// GCF::VersionInfo
//////////////////////////////////////////////////////////////////////////////////////////

/**
\class GCF::VersionInfo
\brief <code>#include <GCF/Common></code><br/> Provides version information of the GCF library
*/

/**
Returns the complete version information as a string
*/
QString GCF::VersionInfo::versionInfoString()
{
    return QString("Generic Component Framework from VCreate Logic - %1.%2.%3").
                arg( majorVersion() ).arg( minorVersion() ).arg( revisionNumber() );
}

/**
Returns the major version number
*/
int GCF::VersionInfo::majorVersion()
{
    return GCF_MAJOR_VERSION;
}

/**
Returns the minor version number
*/
int GCF::VersionInfo::minorVersion()
{
    return GCF_MINOR_VERSION;
}

/**
Returns a revision number
*/
int GCF::VersionInfo::revisionNumber()
{
    return GCF_BUILD_VERSION; // FIXME: Should pick up SVN revision number and show here.
}
