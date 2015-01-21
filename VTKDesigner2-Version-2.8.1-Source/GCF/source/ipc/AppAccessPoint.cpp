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

#include <GCF/AppAccessPoint>
#include <GCF/AbstractComponent>
#include <GCF/ComponentGui>
#include <GCF/ComponentFactory>
#include <GCF/Message>
#include <GCF/AppAccessPointComponent>
#include <GCF/IRemoteAccessControl>
#include <GCF/MethodInvocationHelper>
#include <GCF/DomAccess>

#include "AppAccessPoint_p.h"

#include <QCoreApplication>
#include <QTcpSocket>
#include <QMetaObject>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QGenericArgument>
#include <QTimer>
#include <QStack>
#include <QTextStream>
#include <QBuffer>

/**
\defgroup gcfipc Interprocess Communication (IPC) Module

For a quick description of the the IPC module in GCF, please read \ref ipcarch.
*/

/**
\class GCF::AppAccessPoint GCF/AppAccessPoint
\ingroup gcfipc
\brief Access point for other applications to access components/objects
in this application

This class can be configured to enable access to components and objects
of the host application to outside applications. This class provides the
core functionality to implement inter-component communication across
application boundaries.
*/

struct GCF::AppAccessPointData
{
    AppAccessPointData() : enableFlag(false), serverPort(0) {
        allowedApplications << qApp->applicationName();
        allowAllApplications = false;
    }

    bool enableFlag;
    quint16 serverPort;
    QList<GCF::RemoteApp*> remoteAppList;
    QStringList allowedApplications;
    bool allowAllApplications;

    QList<IMessageEncoderDecoder*> encoderDecoderList;
};

/**
Returns a reference to the only instance of AppAccessPoint object.
*/
GCF::AppAccessPoint & GCF::AppAccessPoint::instance()
{
    static AppAccessPoint theInstance(0);
    return theInstance;
}

/**
Constructor. This is never directly called by the user, it is called by
the \ref instance() method.
*/
GCF::AppAccessPoint::AppAccessPoint(QObject* parent)
    :QTcpServer(parent)
{
    d = new GCF::AppAccessPointData;

    //setAppAccessEnabled(true);
    connect(this, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    d->encoderDecoderList.append( new GCF::DefaultMessageEncoderDecoder(this) );
    d->encoderDecoderList.append( new GCF::HttpMessageEncoderDecoder(this) );
    d->encoderDecoderList.append( new GCF::JSONMessageEncoderDecoder(this) );
}

/**
Destructor
*/
GCF::AppAccessPoint::~AppAccessPoint()
{
    qDeleteAll( d->remoteAppList );
    delete d;
}

/**
Sets the application access point name. By default this is equal to the string
returned by \ref QApplication::applicationName(). GCF applications that have
the same access point name can communicate between each other.
*/
void GCF::AppAccessPoint::setAccessPointName(const QString& name)
{
    d->allowedApplications.first() = name;
}

/**
Returns the application access point name.
*/
QString GCF::AppAccessPoint::accessPointName() const
{
    return d->allowedApplications.first();
}

/**
Enables/disables the access point. If the access point is enabled other applications
can invoke methods on objects that have been exposed by different components.
*/
void GCF::AppAccessPoint::setAppAccessEnabled(bool val)
{
    if( d->enableFlag == val )
        return;

    if( d->enableFlag )
    {
        QList<GCF::RemoteApp*> apps = d->remoteAppList;
        d->remoteAppList.clear();
        qDeleteAll(apps);
    }

    d->enableFlag = val;
    if( d->enableFlag )
    {
        d->enableFlag &= listen(QHostAddress::Any, d->serverPort);
        if( !d->enableFlag )
            d->enableFlag &= listen();
    }
    else
        close();

    if( d->enableFlag )
    {
        // Print out a frendly message in the debug, so that I dont have to
        // go to the config dialog box to figure out the ip address and port
        qDebug("Application Access opened on %s:%d",
               qPrintable(GCF::AppAccessPoint::instance().serverAddress().toString()),
               GCF::AppAccessPoint::instance().serverPort());
    }
}

/**
Returns if the access point has been enabled or not.
*/
bool GCF::AppAccessPoint::isAppAccessEnabled() const
{
    return d->enableFlag;
}

/**
Sets the port on which this access point should listen for connections. The port
number is when this class calls listen() the next time.
*/
void GCF::AppAccessPoint::setPort(quint16 port)
{
    d->serverPort = port;
}

/**
Returns the port number this class will use the next time setAppAccessEnabled() is called.

By default the value returned by this function is 0, which means that the class
automatically picks up a port number.
*/
quint16 GCF::AppAccessPoint::port() const
{
    return d->serverPort;
}

/**
Use this function to allow any application to connect to this application.
By default this property is set to false, which means only the applications
whose name figure in the \ref allowedApplications() list are allowed to
connect to this application.
*/
void GCF::AppAccessPoint::setAllowAllApplications(bool val)
{
    d->allowAllApplications = val;
}

/**
Returns true if any application can connect to this application.
By default this property is set to false, which means only the applications
whose name figure in the \ref allowedApplications() list are allowed to
connect to this application.
*/
bool GCF::AppAccessPoint::isAllowAllApplications() const
{
    return d->allowAllApplications;
}

/**
Adds \c name to the list of applications to which this GCF application
will allow remote access.
*/
void GCF::AppAccessPoint::addAllowedApplication(const QString& name)
{
    if(d->allowedApplications.contains(name))
        return;

    d->allowedApplications.append(name);
}

/**
Removes \c name from the list of applications to which this GCF
application will allow remote access.
*/
void GCF::AppAccessPoint::removeAllowedApplication(const QString& name)
{
    if(d->allowedApplications.first() == name)
        return;

    d->allowedApplications.removeAll(name);
}

/**
Returns a list of allowed application names.
*/
QStringList GCF::AppAccessPoint::allowedApplications() const
{
    return d->allowedApplications;
}

/**
Returns true if \c name is a allowed application name, false otherwise.
*/
bool GCF::AppAccessPoint::isApplicationAllowed(const QString& name) const
{
    if(d->allowAllApplications)
        return true;

    return d->allowedApplications.contains(name);
}

/**
By default component objects exposed through the GUI XML file and marked with
remoteAccess flag are exposed for outside applications to access. Applications
can use this method to add more objects at runtime for remote access.

\note If the object is deleted, then it is automatically removed as an exposed
object. You need not call \ref removeExposedObject() before deleting the object.
*/
bool GCF::AppAccessPoint::addExposedObject(GCF::AbstractComponent* owner, QObject* object, const QString& objectName, GCF::AppAccessPoint::AllowedRemoteOperations allowedOps)
{
    if( !owner || !object )
        return false;

    if( !GCF::ComponentFactory::instance().components().contains(owner) )
        return false;

    object->setObjectName(objectName);

    // Fetch the component GUI of the owner..
    GCF::ComponentGui gui = owner->componentGui();

    // Now create a node of Other type and register it with the gui
    GCF::ComponentGuiNode guiNode;
    guiNode.setType(GCF::ComponentGuiNode::Other);
    guiNode.setName(objectName);
    guiNode["name"] = objectName;
    guiNode["QObject"] = qVariantFromValue<QObject*>(object);
    guiNode["autoDeleteNode"] = true;
    guiNode["exposedNode"] = true;

    // Hints for remote access validation
    guiNode["allowRemoteAccess"] = true;
    if(allowedOps&AllowRemoteMethodInvocation)
        guiNode["allowRemoteMethodInvocation"] = true;
    if(allowedOps&AllowRemoteGetProperty)
        guiNode["allowRemoteGetProperty"] = true;
    if(allowedOps&AllowRemoteSetProperty)
        guiNode["allowRemoteSetProperty"] = true;
    if(allowedOps&AllowRemoteSignalConnect)
        guiNode["allowRemoteSignalConnect"] = true;
    if(allowedOps&AllowRemoteSignalDelivery)
        guiNode["allowRemoteSignalDelivery"] = true;

    // Store the node as a property value in QObject
    object->setProperty("_node_", qVariantFromValue<GCF::ComponentGuiNode>(guiNode));
    connect(object, SIGNAL(destroyed(QObject*)), owner, SLOT(objectDestroyed(QObject*)));

    // Now add the node to the component gui
    gui.addNode(guiNode);

    return true;
}

/**
Removes an exposed object from the component. Note that the object is not deleted, however
it will not be remotely accessible anymore.
*/
bool GCF::AppAccessPoint::removeExposedObject(GCF::AbstractComponent* owner, QObject* object)
{
    if( !owner || !object )
        return false;

    if( !GCF::ComponentFactory::instance().components().contains(owner) )
        return false;

    // Fetch the component GUI of the owner..
    GCF::ComponentGui gui = owner->componentGui();

    // Find out the node which describes the object
    GCF::ComponentGuiNode guiNode;

    // Loop through GUI nodes of Other type and look for \c object.
    for(int i=0; i<gui.nodeCount(); i++)
    {
        GCF::ComponentGuiNode node = gui.node(i);
        if( node.type() != GCF::ComponentGuiNode::Other )
            continue;

        if( node.hasKey("exposedNode") && node["exposedNode"].toBool() )
        {
            QObject* nodeObjPtr = node.hasKey("QObject") ? node["QObject"].value<QObject*>() : 0;
            if( nodeObjPtr == object )
            {
                guiNode = node;
                break;
            }
        }
    }

    // Now unregister the node.
    gui.removeNode( guiNode );

    return true;
}

/**
Returns pointer to a QObject whose complete name is as specified in the parameter.

If the \c forRemoteAccess parameter is true, then only those objects that dont have
a disallowRemoteAccess="false" attribute associated with them in their component GUI XML
files will be searched. If unspecified, disallowRemoteAccess is assumed to be false.

The returned object's owner is returned via the \c owner parameter, if a non-null pointer
is passed.
*/
QObject* GCF::AppAccessPoint::findObject(const QString& completeName, GCF::AbstractComponent** owner, bool forRemoteAccess) const
{
    if(completeName.isEmpty())
        return 0;

    QString componentName = completeName.split('.').first();
    GCF::AbstractComponent* comp = GCF::ComponentFactory::instance().findComponent(componentName);
    if( !comp )
        return 0;

    if( forRemoteAccess )
    {
        GCF::ComponentGuiNode node = comp->componentGui().findNode(completeName);
        if( node.hasKey("disallowRemoteAccess") && node["disallowRemoteAccess"].toBool() )
            return 0;
    }

    QObject* retObj = comp->findObject(completeName);
    if( retObj && owner )
        *owner = comp;

    return retObj;
}

/**
Returns a list of sockets corresponding to remote applications
*/
QList<GCF::RemoteApp*> GCF::AppAccessPoint::connectedRemoteAppList() const
{
    return d->remoteAppList;
}

/**
This function registers a custom IMessageEncoderDecoder implementation.

Objects that implement the IMessageEncoderDecoder interface can provide support
for custom message formats (eg. HTTP, SOAP etc..)
*/
void GCF::AppAccessPoint::registerMessageEncoderDecoder(IMessageEncoderDecoder* encDec)
{
    GCF::DefaultMessageEncoderDecoder* defEnc =
            qobject_cast<GCF::DefaultMessageEncoderDecoder*>(encDec->containerObject());
    if(defEnc)
        return;

    d->encoderDecoderList.append(encDec);
}

/**
This function unregisters a IMessageEncoderDecoder implementation.

\note The default implementation can never be unregistered.
*/
void GCF::AppAccessPoint::unregisterMessageEncoderDecoder(IMessageEncoderDecoder* encDec)
{
    GCF::DefaultMessageEncoderDecoder* defEnc =
            qobject_cast<GCF::DefaultMessageEncoderDecoder*>(encDec->containerObject());
    if(defEnc)
        return;

    d->encoderDecoderList.removeAll(encDec);
}

/**
Returns the total number of IMessageEncoderDecoder implementaitons registered
with this class.
*/
int GCF::AppAccessPoint::messageEncoderDecoderCount() const
{
    return d->encoderDecoderList.count();
}

/**
Returns the IMessageEncoderDecoder
*/
IMessageEncoderDecoder* GCF::AppAccessPoint::messageEncoderDecoderAt(int index) const
{
    if(index < 0 || index >= d->encoderDecoderList.count())
        return 0;

    return d->encoderDecoderList.at(index);
}

/**
This function returns the index of the given \c encDec within the list of registered
\ref IMessageEncoderDecoder objects.
*/
int GCF::AppAccessPoint::indexOfMessageEncoderDecoder(IMessageEncoderDecoder* encDec) const
{
    return d->encoderDecoderList.indexOf(encDec);
}

/**
\internal
*/
GCF::Message* GCF::AppAccessPoint::handleMessage(GCF::Message* message) const
{
    if(message->messageType() == GCF::Message::AsyncInvokeMethod)
    {
        GCF::AsyncInvokeMethodMessage* msg = (GCF::AsyncInvokeMethodMessage*)message;
        GCF::AbstractComponent* owner = 0;
        QObject* reqObjectPtr = this->findObject(msg->objectName(), &owner, true);

        int methodIdx = GCF::findMethod(reqObjectPtr, msg->methodName(), msg->argList());

        QVariant result;
        bool success = false;

        if(reqObjectPtr)
        {
            reqObjectPtr->setProperty("CurrentClientGUID", msg->customData().value("ClientGUID").toString());
            reqObjectPtr->setProperty("ClientIP", msg->customData().value("ClientIP").toString());
            reqObjectPtr->setProperty("ClientPort", msg->customData().value("ClientPort").toString());
            reqObjectPtr->setProperty("ServerIP", msg->customData().value("ServerIP").toString());
            reqObjectPtr->setProperty("ServerPort", msg->customData().value("ServerPort").toString());

            if( this->isAllowObjectMethodInvoke(owner, reqObjectPtr, methodIdx, 0) )
            {
                if(methodIdx >= 0)
                {
                    QMetaMethod method = reqObjectPtr->metaObject()->method(methodIdx);
                    if( qstrcmp("GCF_NOT_WS", method.tag()) )
                        success = GCF::callMethod(reqObjectPtr, msg->methodName(), msg->argList(), result);
                    else
                        success = false;
                }
                else
                    success = false;
            }

            if(success)
            {
                // Save the method call counter in reqObjectPtr.
                GCF::DataStore callCounterStore;
                if( reqObjectPtr->dynamicPropertyNames().contains("_CallCounterStore_") )
                    callCounterStore = reqObjectPtr->property("_CallCounterStore_").value<GCF::DataStore>();

                int count = 0;
                if( callCounterStore.hasKey(msg->methodName()) )
                    count = callCounterStore.value(msg->methodName()).toInt();
                ++count;
                callCounterStore.addKey(msg->methodName(), count);
                if(count == 1)
                    reqObjectPtr->setProperty("_CallCounterStore_", qVariantFromValue<GCF::DataStore>(callCounterStore));
            }

            reqObjectPtr->setProperty("CurrentClientGUID", QVariant());
            reqObjectPtr->setProperty("ClientIP", QVariant());
            reqObjectPtr->setProperty("ClientPort", QVariant());
            reqObjectPtr->setProperty("ServerIP", QVariant());
            reqObjectPtr->setProperty("ServerPort", QVariant());
        }

        GCF::AsyncInvokeMethodResMessage* reply = new GCF::AsyncInvokeMethodResMessage(msg);
        reply->setObjectName(msg->objectName());
        reply->setMethodName(msg->methodName());
        reply->setSuccess(success);
        reply->setRetValue(result);
        if(!success)
        {
            if(!reqObjectPtr)
                reply->setErrorMessage("Cannot find object");
            else
                reply->setErrorMessage("Could not invoke method");
        }

        return reply;
    }

    if(message->messageType() == GCF::Message::AsyncInvokeMethods)
    {
        GCF::AsyncInvokeMethodsMessage* msg = (GCF::AsyncInvokeMethodsMessage*)message;
        QList<GCF::Method> methodList = msg->invokeMethodList();

        for(int i=0; i<methodList.count(); i++)
        {
            GCF::Method& method = methodList[i];

            GCF::AbstractComponent* owner = 0;
            QObject* reqObjectPtr = this->findObject(method.ObjectName, &owner, true);
            int methodIdx = GCF::findMethod(reqObjectPtr, method.MethodName, method.Args);

            if(reqObjectPtr)
            {
                reqObjectPtr->setProperty("CurrentClientGUID", msg->customData().value("ClientGUID").toString());
                reqObjectPtr->setProperty("ClientIP", msg->customData().value("ClientIP").toString());
                reqObjectPtr->setProperty("ClientPort", msg->customData().value("ClientPort").toString());
                reqObjectPtr->setProperty("ServerIP", msg->customData().value("ServerIP").toString());
                reqObjectPtr->setProperty("ServerPort", msg->customData().value("ServerPort").toString());

                if( this->isAllowObjectMethodInvoke(owner, reqObjectPtr, methodIdx, 0) )
                {
                    if(methodIdx >= 0)
                    {
                        QMetaMethod metaMethod = reqObjectPtr->metaObject()->method(methodIdx);
                        if( qstrcmp("GCF_NOT_WS", metaMethod.tag()) )
                            method.Success = GCF::callMethod(reqObjectPtr, method.MethodName, method.Args, method.Result);
                        else
                            method.Success = false;
                    }
                    else
                        method.Success = false;
                }

                if(method.Success)
                {
                    // Save the method call counter in reqObjectPtr.
                    GCF::DataStore callCounterStore;
                    if( reqObjectPtr->dynamicPropertyNames().contains("_CallCounterStore_") )
                        callCounterStore = reqObjectPtr->property("_CallCounterStore_").value<GCF::DataStore>();

                    int count = 0;
                    if( callCounterStore.hasKey(method.MethodName) )
                        count = callCounterStore.value(method.MethodName).toInt();
                    ++count;
                    callCounterStore.addKey(method.MethodName, count);

                    if(count == 1)
                        reqObjectPtr->setProperty("_CallCounterStore_", qVariantFromValue<GCF::DataStore>(callCounterStore));
                }

                reqObjectPtr->setProperty("CurrentClientGUID", QVariant());
                reqObjectPtr->setProperty("ClientIP", QVariant());
                reqObjectPtr->setProperty("ClientPort", QVariant());
                reqObjectPtr->setProperty("ServerIP", QVariant());
                reqObjectPtr->setProperty("ServerPort", QVariant());
            }

            if(!method.Success)
            {
                if(!reqObjectPtr)
                    method.ErrorMessage = "Cannot find object";
                else
                    method.ErrorMessage = "Could not invoke method";
            }
        }

        GCF::AsyncInvokeMethodsResMessage* reply = new GCF::AsyncInvokeMethodsResMessage(msg);
        reply->setInvokedMethodList(methodList);

        return reply;
    }

    return 0;
}

/**
\internal
*/
IMessageEncoderDecoder* GCF::AppAccessPoint::messageEncoderDecoderFor(const QByteArray& ba) const
{
    if(ba.isEmpty())
        return 0;

    for(int i=0; i<d->encoderDecoderList.count(); i++)
    {
        IMessageEncoderDecoder* encDec = d->encoderDecoderList.at(i);
        if(encDec->canDecodeMessage(ba))
            return encDec;
    }

    return 0;
}

void GCF::AppAccessPoint::slotNewConnection()
{
    while( this->hasPendingConnections() )
    {
        QTcpSocket* socket = this->nextPendingConnection();
        new GCF::RemoteApp(socket, this);
    }
}

void GCF::AppAccessPoint::addRemoteApp(GCF::RemoteApp* app)
{
    d->remoteAppList.append(app);
}

void GCF::AppAccessPoint::removeRemoteApp(GCF::RemoteApp* app)
{
    d->remoteAppList.removeAll(app);
}

/**
Returns true/false on whether the \c object belonging to \c comp component can be accessed remotely by
the remote application handler \c remoteApp.

The function makes use of IComponentRemoteAccessControl interface in component (if-found) and
IObjectRemoteAccessControl interface in object (if-found) for making the decision. The former interface's
decision is given priority over the latters.
*/
bool GCF::AppAccessPoint::isAllowObjectAccess(GCF::AbstractComponent* comp, QObject* object, GCF::RemoteApp* remoteApp) const
{
    if( !comp || !object /*|| !remoteApp*/ )
        return false;

    // Find out from code if remote access is allowed
    IComponentRemoteAccessControl* compRAC = qobject_cast<IComponentRemoteAccessControl*>(comp);
    if( compRAC )
    {
        if( !compRAC->isAllowComponentAccess(remoteApp) )
            return false;

        if( !compRAC->isAllowObjectAccess(object, remoteApp) )
            return false;
    }
    else
    {
        // Check what the GUIXML has to say.
        GCF::ComponentGui gui = comp->componentGui();
        bool allowComponentRemoteAccess = gui.hasKey("allowRemoteAccess") && gui["allowRemoteAccess"].toBool();

        if(!allowComponentRemoteAccess)
            return false;
    }

    if(object == comp)
        return true;

    IObjectRemoteAccessControl* objectRAC = qobject_cast<IObjectRemoteAccessControl*>(object);
    if( objectRAC )
    {
        if( !objectRAC->isAllowObjectAccess(remoteApp) )
            return false;
    }
    else
    {
        // Check what the GUIXML has to say.
        GCF::ComponentGuiNode node = object->property("_node_").value<GCF::ComponentGuiNode>();
        bool allowObjectRemoteAccess = node.isValid() && node.hasKey("allowRemoteAccess") && node["allowRemoteAccess"].toBool();

        if(!allowObjectRemoteAccess)
            return false;
    }

    return true;
}

/**
Returns true/false on whether the method in \c object belonging to \c comp component can be accessed
remotely by the remote application handler \c remoteApp.

The function makes use of IComponentRemoteAccessControl interface in component (if-found) and
IObjectRemoteAccessControl interface in object (if-found) for making the decision. The former interface's
decision is given priority over the latters.
*/
bool GCF::AppAccessPoint::isAllowObjectMethodInvoke(GCF::AbstractComponent* comp, QObject* object, int methodIdx, GCF::RemoteApp* remoteApp) const
{
    if( !object || /*!remoteApp ||*/ methodIdx < 0 || methodIdx >= object->metaObject()->methodCount())
        return false;

    if(!isAllowObjectAccess(comp, object, remoteApp))
        return false;

    IObjectRemoteAccessControl* objectRAC = qobject_cast<IObjectRemoteAccessControl*>(object);
    if( objectRAC )
    {
        if( !objectRAC->isAllowObjectMethodInvoke(methodIdx, remoteApp) )
            return false;
    }
    else
    {
        // Check what the GUIXML has to say.
        GCF::ComponentGuiNode node = object->property("_node_").value<GCF::ComponentGuiNode>();
        bool allowRMI = node.isValid() && node.hasKey("allowRemoteMethodInvocation") && node["allowRemoteMethodInvocation"].toBool();

        if(!allowRMI)
            return false;
    }

    return true;
}

/**
Returns true/false on whether the property in \c object belonging to \c comp component can be accessed
remotely by the remote application handler \c remoteApp.

The function makes use of IComponentRemoteAccessControl interface in component (if-found) and
IObjectRemoteAccessControl interface in object (if-found) for making the decision. The former interface's
decision is given priority over the latters.
*/
bool GCF::AppAccessPoint::isAllowObjectSetProperty(GCF::AbstractComponent* comp, QObject* object, int propIdx, GCF::RemoteApp* remoteApp) const
{
    if( !object || /*!remoteApp ||*/ propIdx < 0 || propIdx >= object->metaObject()->propertyCount())
        return false;

    if(!isAllowObjectAccess(comp, object, remoteApp))
        return false;

    IObjectRemoteAccessControl* objectRAC = qobject_cast<IObjectRemoteAccessControl*>(object);
    if( objectRAC )
    {
        if( !objectRAC->isAllowObjectSetProperty(propIdx, remoteApp) )
            return false;
    }
    else
    {
        // Check what the GUIXML has to say.
        GCF::ComponentGuiNode node = object->property("_node_").value<GCF::ComponentGuiNode>();
        bool allowRSP = node.isValid() && node.hasKey("allowRemoteSetProperty") && node["allowRemoteSetProperty"].toBool();

        if(!allowRSP)
            return false;
    }

    return true;
}

/**
Returns true/false on whether the property in \c object belonging to \c comp component can be accessed
remotely by the remote application handler \c remoteApp.

The function makes use of IComponentRemoteAccessControl interface in component (if-found) and
IObjectRemoteAccessControl interface in object (if-found) for making the decision. The former interface's
decision is given priority over the latters.
*/
bool GCF::AppAccessPoint::isAllowObjectGetProperty(GCF::AbstractComponent* comp, QObject* object, int propIdx, GCF::RemoteApp* remoteApp) const
{
    if( !object || /*!remoteApp ||*/ propIdx < 0 || propIdx >= object->metaObject()->propertyCount())
        return false;

    if(!isAllowObjectAccess(comp, object, remoteApp))
        return false;

    IObjectRemoteAccessControl* objectRAC = qobject_cast<IObjectRemoteAccessControl*>(object);
    if( objectRAC )
    {
        if( !objectRAC->isAllowObjectGetProperty(propIdx, remoteApp) )
            return false;
    }
    else
    {
        // Check what the GUIXML has to say.
        GCF::ComponentGuiNode node = object->property("_node_").value<GCF::ComponentGuiNode>();
        bool allowRGP = node.isValid() && node.hasKey("allowRemoteGetProperty") && node["allowRemoteGetProperty"].toBool();

        if(!allowRGP)
            return false;
    }

    return true;
}

/**
Returns true/false on whether the signal in \c object belonging to \c comp component can be accessed
remotely by the remote application handler \c remoteApp.

The function makes use of IComponentRemoteAccessControl interface in component (if-found) and
IObjectRemoteAccessControl interface in object (if-found) for making the decision. The former interface's
decision is given priority over the latters.
*/
bool GCF::AppAccessPoint::isAllowObjectSignalConnect(GCF::AbstractComponent* comp, QObject* object, int signalIdx, GCF::RemoteApp* remoteApp) const
{
    if( !object || /*!remoteApp ||*/ signalIdx < 0 || signalIdx >= object->metaObject()->methodCount())
        return false;

    if(!isAllowObjectAccess(comp, object, remoteApp))
        return false;

    IObjectRemoteAccessControl* objectRAC = qobject_cast<IObjectRemoteAccessControl*>(object);
    if( objectRAC )
    {
        if( !objectRAC->isAllowObjectSignalConnect(signalIdx, remoteApp) )
            return false;
    }
    else
    {
        // Check what the GUIXML has to say.
        GCF::ComponentGuiNode node = object->property("_node_").value<GCF::ComponentGuiNode>();
        bool allowRSC = node.isValid() && node.hasKey("allowRemoteSignalConnect") && node["allowRemoteSignalConnect"].toBool();

        if(!allowRSC)
            return false;
    }

    return true;
}

/**
Returns true/false on whether the signal in \c object belonging to \c comp component can be accessed
remotely by the remote application handler \c remoteApp.

The function makes use of IComponentRemoteAccessControl interface in component (if-found) and
IObjectRemoteAccessControl interface in object (if-found) for making the decision. The former interface's
decision is given priority over the latters.
*/
bool GCF::AppAccessPoint::isAllowObjectSignalDelivery(GCF::AbstractComponent* comp, QObject* object, int signalIdx, GCF::RemoteApp* remoteApp) const
{
    if( !object || /*!remoteApp ||*/ signalIdx < 0 || signalIdx >= object->metaObject()->methodCount())
        return false;

    if(!isAllowObjectAccess(comp, object, remoteApp))
        return false;

    IObjectRemoteAccessControl* objectRAC = qobject_cast<IObjectRemoteAccessControl*>(object);
    if( objectRAC )
    {
        if( !objectRAC->isAllowObjectSignalDelivery(signalIdx, remoteApp) )
            return false;
    }
    else
    {
        // Check what the GUIXML has to say.
        GCF::ComponentGuiNode node = object->property("_node_").value<GCF::ComponentGuiNode>();
        bool allowRSD = node.isValid() && node.hasKey("allowRemoteSignalDelivery") && node["allowRemoteSignalDelivery"].toBool();

        if(!allowRSD)
            return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////
// GCF::RemoteApp
////////////////////////////////////////////////////////////////////////////////////

#include <GCF/MethodInvocationHelper>

#include <QColor>
#include <QPixmap>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QByteArray>
#include <QBitArray>
#include <QImage>
#include <QBitmap>
#include <QPointF>
#include <QPoint>
#include <QLineF>
#include <QLine>
#include <QSizeF>
#include <QSize>
#include <QRectF>
#include <QRect>
#include <QMatrix>
#include <QTransform>
#include <QUrl>
#include <QSizePolicy>
#include <QBrush>

#include "MutexHelper.h"

/**
\internal
\class GCF::RemoteAppSession
*/
namespace GCF
{
class RemoteAppSession : public QObject
{
public:
    static QList<RemoteAppSession*> SessionList;
    static RemoteAppSession* FindSession(qint32 session);

    RemoteAppSession() : QObject(&GCF::AppAccessPoint::instance()),
        messageEncoderDecoder(0), m_refCount(1) {
        static qint32 SessionIdCounter = 0;
        this->m_sessionId = SessionIdCounter++;
        this->m_releaseFlag = false;

        SessionList.append(this);
    }

    ~RemoteAppSession() {
        SessionList.removeAll(this);
    }

    QMap<qint32, QObject*> objectIdMap;
    QMap<QObject*, GCF::AbstractComponent*> objectComponentMap;
    IMessageEncoderDecoder* messageEncoderDecoder;

    qint32 sessionId() const { return m_sessionId; }

    void setReleaseSessionFlag(bool val) {
        m_releaseFlag = val;
    }

    bool isReleaseSessionFlag() const {
        return m_releaseFlag;
    }

    void ref() {
        ++m_refCount;
        m_timeoutTimer.stop();
    }

    void deref() {
        --m_refCount;
        if(m_refCount > 0)
            return;

        if( qobject_cast<DefaultMessageEncoderDecoder*>(messageEncoderDecoder->containerObject()) || m_releaseFlag )
            deleteLater();
        else
        {
            static const int timeout = 120*60*1000;
            m_timeoutTimer.start(timeout, this);
        }
    }

    void timerEvent(QTimerEvent* /*te*/) {
        deleteLater();
    }

private:
    int m_refCount;
    QBasicTimer m_timeoutTimer;
    qint32 m_sessionId;
    bool m_releaseFlag;
};
}

QList<GCF::RemoteAppSession*> GCF::RemoteAppSession::SessionList;

GCF::RemoteAppSession* GCF::RemoteAppSession::FindSession(qint32 sessionId)
{
    Q_FOREACH(GCF::RemoteAppSession* session, SessionList)
        if(session->sessionId() == sessionId)
            return session;

    return 0;
}

/**
\class GCF::RemoteApp GCF/RemoteApp
\ingroup gcfipc
\brief Handles messages sent from a remote application

This class receives messages sent from a remote application. Its responsibility is
to interpret those messages and then respond to them. The class should only be
constructed by AppAccessPoint, and not by anyone else.
*/

static qint32 GenObjectID()
{
    static qint32 id = 0;
    return ++id;
}

namespace GCF
{
struct RemoteAppData
{
    RemoteAppData() : session(0), socket(0) { }

    GCF::RemoteAppSession* session;
    QAbstractSocket* socket;
    QByteArray unprocessedData;
    QMutex sendMessageLock;

    // Managing pending messages
    QMutex pendingMessageLock;
    QList<GCF::Message*> pendingMessages;
    void addPendingMessage(GCF::Message* msg) {
        pendingMessageLock.lock();
        if( !pendingMessages.contains(msg) )
            pendingMessages.append(msg);
        pendingMessageLock.unlock();
    }

    // List of signal spies maintained for signal delivery.
    QList<GCF::SignalSpy1*> signalSpyList;
    GCF::SignalSpy1* findSignalSpy(QObject* sender, const char* signal) {
        for(int i=0; i<signalSpyList.count(); i++) {
            GCF::SignalSpy1* spy = signalSpyList[i];
            if( spy->sender() == sender && !qstrcmp(spy->signal2(),signal) )
                return spy;
        }
        return 0;
    }
};
}

static QStack<GCF::RemoteApp*> & CurrentRemoteAppStack()
{
    static QStack<GCF::RemoteApp*> theStack;
    return theStack;
}

/**
When a method is called from within a RMI context, then this function
will return a pointer to the \ref GCF::RemoteApp from which the method
was called. At all other times, this function returns 0.

\note It is extremely important that you do not delete the object returned
by this function.
*/
GCF::RemoteApp* GCF::RemoteApp::currentRemoteApp()
{
    if( CurrentRemoteAppStack().count() )
        return CurrentRemoteAppStack().top();

    return 0;
}

/**
\internal
*/
GCF::RemoteApp::RemoteApp(QAbstractSocket* socket, QObject* parent)
    :QObject(parent)
{
    d = new GCF::RemoteAppData;

    d->session = 0;
    d->socket = 0;

    this->setSocket(socket);
    GCF::AppAccessPoint::instance().addRemoteApp(this);
}

/**
Sends a \ref GCF::Message::ReleaseAccess message to the peer socket and destroys self.
*/
GCF::RemoteApp::~RemoteApp()
{
    qDeleteAll(d->signalSpyList);
    d->signalSpyList.clear();

    // sendMessage( new CReleaseAccessMessage );
    if( d->socket )
        d->socket->close();

    if( d->session )
        d->session->deref();

    // Unlock mutexes to release deadlocks if any
    // d->sendMessageLock.unlock();
    // d->pendingMessageLock.unlock();
    qDeleteAll(d->pendingMessages);

    GCF::AppAccessPoint::instance().removeRemoteApp(this);

    delete d;
}

void GCF::RemoteApp::setSocket(QAbstractSocket* socket)
{
    if(d->socket)
        return;

    if(d->socket == socket || !socket)
        return;

    d->socket = socket;
    d->socket->setParent(this);

    // Make connections
    connect(d->socket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(d->socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError()));
    // connect(d->socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(slotStateChanged()));
    connect(d->socket, SIGNAL(aboutToClose()), this, SLOT(slotAboutToClose()));
    connect(d->socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

    // startTimer(2000);
}

/**
Returns the socket via which messages to the remote app are sent and
received.
*/
QAbstractSocket* GCF::RemoteApp::socket() const
{
    return d->socket;
}

/**
Returns the local socket information
*/
QString GCF::RemoteApp::localInfo() const
{
    return QString("%1:%2").arg(d->socket->localAddress().toString()).arg(d->socket->localPort());
}

/**
Returns the remote socket information
*/
QString GCF::RemoteApp::peerInfo() const
{
    return QString("%1:%2").arg(d->socket->peerAddress().toString()).arg(d->socket->peerPort());
}

/**
Sends a message to the peer. After sending, the message is deleted. Ensure that
the messages passed to this function are created on the heap.
*/
void GCF::RemoteApp::sendMessage(GCF::Message* message)
{
    if( !message )
        return;

    if( !d->socket || !d->socket->isValid() || !d->socket->isOpen() ||
            d->socket->state() != QAbstractSocket::ConnectedState ||
            !d->session )
    {
        delete message;
        return;
    }

    if( message->messageDirection() != GCF::Message::ServerToClient &&
            message->messageDirection() != GCF::Message::Both )
    {
        delete message;
        return;
    }

    // Try to acquire the send message lock. If thats not possible then
    // queue the message for sending at a later stage and quit.
    if( !d->sendMessageLock.tryLock() )
    {
        d->addPendingMessage(message);
        // I wouldnt use this because, queued connections are scheduled really quick.
        // I would much rather use a QTimer for this
        // QMetaObject::invokeMethod(this, "sendPendingMessages", Qt::QueuedConnection);
        QTimer::singleShot(500, this, SLOT("sendPendingMessages()"));
        return;
    }

    // Set the session id in the message.
    message->setSessionId(d->session->sessionId());

    // Prepare a MutexUnlocker to unlock when done
    MutexUnlocker unlocker(&(d->sendMessageLock));

    IMessageEncoderDecoder* encDec = d->session->messageEncoderDecoder;
    QByteArray ba = encDec->encodeMessage(message);
    if( d->socket->write(ba) < 0 )
    {
        qDebug("Error while sending message: %s", qPrintable(message->toString()));
        delete message;
        return;
    }
    if(message->messageType() == GCF::Message::SignalDelivery)
        d->socket->flush();

    // Log message if possible
    if( GCF::AppAccessPointComponent::instance().messageLog() )
    {
        QString log = QString("%1: Sent to %2 - ").arg(localInfo()).arg(peerInfo());
        log += message->toString();
        GCF::AppAccessPointComponent::instance().appendMessageLog( log );
    }

    delete message;

    if(encDec->isCloseSocketUponSendMessage())
    {
        d->socket->flush();
        d->socket->close();
    }
}

/**
\internal
*/
void GCF::RemoteApp::slotDisconnected()
{
    deleteLater();
}

/**
\internal
*/
void GCF::RemoteApp::slotError()
{
    qDebug("Socket Error: %s", qPrintable(d->socket->errorString()));
}

/**
\internal
*/
void GCF::RemoteApp::slotAboutToClose()
{
    // Do nothing
}

/**
\internal
*/
void GCF::RemoteApp::slotReadyRead()
{
    if( !d->socket->bytesAvailable() )
        return;

    // Lets first take unprocessed data from the last slotReadyRead()
    QByteArray ba;
    if( d->unprocessedData.size() )
    {
        ba = d->unprocessedData;
        d->unprocessedData.clear();
    }

    // Now add the new data to it.
    if( ba.size() )
        ba.append( d->socket->readAll() );
    else
        ba = d->socket->readAll();

    // Now unpack a message from the byte array
    while(ba.size())
    {
        // Find out the appropriate encoder/decoder
        IMessageEncoderDecoder* encDec = d->session ? d->session->messageEncoderDecoder : 0;
        if(!encDec)
            encDec = GCF::AppAccessPoint::instance().messageEncoderDecoderFor(ba);

        // Fetch the message from the packet..
        GCF::Message* msg = encDec ? encDec->decodeMessage(ba) : 0;
        if( !msg )
            break;

        // Ensure that we have the right session.
        if(!d->session)
        {
            if(msg->sessionId() < 0)
                d->session = new GCF::RemoteAppSession;
            else
            {
                d->session = GCF::RemoteAppSession::FindSession(msg->sessionId());
                d->session->ref();
            }
        }

        // Store the encoder-decoder for the session..
        if(d->session && !d->session->messageEncoderDecoder && encDec)
            d->session->messageEncoderDecoder = encDec;

        // Log message if possible
        if( GCF::AppAccessPointComponent::instance().messageLog() )
        {
            QString log = QString("%1: Received from %2 - ").arg(localInfo()).arg(peerInfo());
            log += msg->toString();
            GCF::AppAccessPointComponent::instance().appendMessageLog( log );
        }

        if( !d->session )
        {
            qDebug("Session(%d) requested by the message(%d) could not be found",
                   msg->sessionId(), msg->id());
        }
        else
        {
            // Store some basic information about the connection
            msg->customData().addKey("ClientIP", d->socket->peerAddress().toString());
            msg->customData().addKey("ClientPort", d->socket->peerPort());
            msg->customData().addKey("ServerIP", d->socket->localAddress().toString());
            msg->customData().addKey("ServerPort", d->socket->localPort());

            // Handle the incoming message
            bool success = handleMessage(msg);
            if( !success )
            {
                qDebug("Message ID %d of type %s was not handled", msg->id(),
                       qPrintable(GCF::Message::messageTypeString(msg->messageType())));
            }
        }

        // Delete the received message
        delete msg;

        if(!d->socket->isOpen())
            ba.clear();
    }

    // If there are any unprocessed bytes left, then lets keep it for the next batch
    if( ba.size() )
        d->unprocessedData = ba;

    // Lets just check the socket one more time if more bytes are available for us
    // to read. If yes, lets schedule a call to this function again
    if( d->socket->isOpen() && d->socket->bytesAvailable() )
        QMetaObject::invokeMethod(this, "slotReadyRead", Qt::QueuedConnection);
}

/**
\internal
*/
bool GCF::RemoteApp::handleMessage(GCF::Message* received)
{
    switch(received->messageType())
    {
    case GCF::Message::RequestAccess: {
        GCF::RequestAccessMessage* msg = (GCF::RequestAccessMessage*)received;
        GCF::RequestAccessResMessage* reply = new GCF::RequestAccessResMessage(msg);
        if( GCF::AppAccessPoint::instance().isApplicationAllowed( msg->applicationName() ) )
            reply->setAccessGranted( GCF::AppAccessPoint::instance().isAppAccessEnabled() );
        else
            reply->setAccessGranted( false );
        sendMessage( reply );

    } break;

    case GCF::Message::ReleaseAccess: {
        GCF::ReleaseAccessMessage* msg = (GCF::ReleaseAccessMessage*)received;
        GCF::ReleaseAccessResMessage* reply = new GCF::ReleaseAccessResMessage(msg);

        if( msg->sessionId() == d->session->sessionId() )
        {
            d->session->setReleaseSessionFlag(true);
            reply->setSuccess(true);
        }
        else
            reply->setSuccess(false);

        sendMessage(reply);
    } break;

    case GCF::Message::RequestObject: {

        GCF::RequestObjectMessage* msg = (GCF::RequestObjectMessage*)received;
        GCF::AbstractComponent* owner = 0;
        QObject* reqObjectPtr = GCF::AppAccessPoint::instance().findObject( msg->objectName(), &owner, true );
        qint32 id = -1;

        if( reqObjectPtr && isAllowObjectAccess(owner, reqObjectPtr, this) )
        {
            QList<QObject*> objects = d->session->objectIdMap.values();
            if( objects.contains(reqObjectPtr) )
                id = d->session->objectIdMap.key(reqObjectPtr);
            else
            {
                id = GenObjectID();
                d->session->objectIdMap[id] = reqObjectPtr;
                d->session->objectComponentMap[reqObjectPtr] = owner;

                // so we can send ReleaseObject signal when the object
                // gets destroyed
                connect(reqObjectPtr, SIGNAL(destroyed(QObject*)), this, SLOT(objectDestroyed(QObject*)));
            }
        }

        GCF::RequestObjectResMessage* reply = new GCF::RequestObjectResMessage(msg);
        reply->setObjectId(id);
        sendMessage( reply );

    } break;

    case GCF::Message::AsyncInvokeMethods:
    case GCF::Message::AsyncInvokeMethod: {
        d->session->setReleaseSessionFlag(true);

        GCF::Message* reply = GCF::AppAccessPoint::instance().handleMessage(received);
        sendMessage(reply);
    } break;

    case GCF::Message::InvokeMethod:
    case GCF::Message::SetProperty:
    case GCF::Message::GetProperty:
    case GCF::Message::GetPropertyNames:
    case GCF::Message::RequestConnection:
    case GCF::Message::ReleaseConnection:
    case GCF::Message::ReleaseObject: {

        CurrentRemoteAppStack().push(this);

        qint32 objectId = received->objectId();
        QObject* objectPtr = d->session->objectIdMap.contains(objectId) ? d->session->objectIdMap[objectId] : 0;
        GCF::AbstractComponent* objectOwner = d->session->objectComponentMap.contains(objectPtr) ? d->session->objectComponentMap[objectPtr] : 0;
        bool success = handleObjectMessage(objectId, objectOwner, objectPtr, received);

        CurrentRemoteAppStack().pop();

        return success;

    } break;

    default:
        return false;
    }

    return true;
}

bool GCF::RemoteApp::handleObjectMessage(qint32 objectId, GCF::AbstractComponent* objectOwner, QObject* objectPtr, GCF::Message* received)
{
    switch(received->messageType())
    {
    case GCF::Message::InvokeMethod: {

        GCF::InvokeMethodMessage* msg = (GCF::InvokeMethodMessage*)received;
        bool success = false;
        QVariant retVal;

        if( objectPtr )
        {
            //Todo
            /*int methodIdx = GCF::MethodInvocationHelper::findMethod(objectPtr, msg->methodName(), msg->argList());
            if( this->isAllowObjectMethodInvoke(objectOwner, objectPtr, methodIdx, this) )
                success = GCF::MethodInvocationHelper::callMethod(objectPtr, msg->methodName(), msg->argList(), retVal);*/
            int methodIdx = GCF::findMethod(objectPtr, msg->methodName(), msg->argList());
            if( this->isAllowObjectMethodInvoke(objectOwner, objectPtr, methodIdx, this) )
                success = GCF::callMethod(objectPtr, msg->methodName(), msg->argList(), retVal);
        }

        GCF::InvokeMethodResMessage* reply = new GCF::InvokeMethodResMessage(msg);
        reply->setObjectId(msg->objectId());
        reply->setMethodName(msg->methodName());
        reply->setRetValue(retVal);
        reply->setSuccess(success);
        sendMessage( reply );

    } break;

    case GCF::Message::SetProperty: {

        GCF::SetPropertyMessage* msg = (GCF::SetPropertyMessage*)received;
        bool success = false;
        QVariant retVal;

        if( objectPtr )
        {
            QByteArray propName = msg->propertyName().toAscii();
            int propIdx = objectPtr->metaObject()->indexOfProperty( propName.constData() );
            if( this->isAllowObjectSetProperty(objectOwner, objectPtr, propIdx, this) )
                success = objectPtr->setProperty( qPrintable(msg->propertyName()), msg->propertyValue() );

            if( success )
                retVal = msg->propertyValue();
            else
                retVal = objectPtr->property( qPrintable(msg->propertyName()) );
        }

        GCF::SetPropertyResMessage* reply = new GCF::SetPropertyResMessage(msg);
        reply->setObjectId(msg->objectId());
        reply->setPropertyName(msg->propertyName());
        reply->setPropertyValue(retVal);
        reply->setSuccess(success);
        sendMessage(reply);

    } break;

    case GCF::Message::GetProperty: {

        GCF::GetPropertyMessage* msg = (GCF::GetPropertyMessage*)received;
        bool success = false;
        QVariant retVal;

        if( objectPtr )
        {
            const QMetaObject* mo = objectPtr->metaObject();
            QByteArray propName = msg->propertyName().toAscii();
            int propIdx = mo->indexOfProperty( propName.constData() );
            if( this->isAllowObjectGetProperty(objectOwner, objectPtr, propIdx, this) )
            {
                retVal = objectPtr->property( qPrintable(msg->propertyName()) );
                success = true;
            }
        }

        GCF::GetPropertyResMessage* reply = new GCF::GetPropertyResMessage(msg);
        reply->setObjectId(msg->objectId());
        reply->setPropertyName(msg->propertyName());
        reply->setPropertyValue(retVal);
        reply->setSuccess(success);
        sendMessage(reply);

    } break;

    case GCF::Message::GetPropertyNames: {

        GCF::GetPropertyMessage* msg = (GCF::GetPropertyMessage*)received;
        QStringList propNames;

        if( objectPtr && this->isAllowObjectAccess(objectOwner, objectPtr, this) )
        {
            const QMetaObject* mo = objectPtr->metaObject();
            for(int i=0; i<mo->propertyCount(); i++)
                propNames << QString( mo->property(i).name() );
        }

        GCF::GetPropertyNamesResMessage* reply = new GCF::GetPropertyNamesResMessage(msg);
        reply->setObjectId(msg->objectId());
        reply->setPropertyNames(propNames);
        sendMessage(reply);

    } break;

    case GCF::Message::RequestConnection:{

        GCF::RequestConnectionMessage* msg = (GCF::RequestConnectionMessage*)received;

        QByteArray signal = msg->signal().toAscii();
        QByteArray signalSignature = QMetaObject::normalizedSignature( signal.constData()+1 );
        int signalIdx = objectPtr->metaObject()->indexOfSignal( signalSignature.constData() );
        GCF::SignalSpy1* spy = 0;

        if( this->isAllowObjectSignalConnect(objectOwner, objectPtr, signalIdx, this) )
        {
            spy = d->findSignalSpy(objectPtr, signal.constData());
            if( !spy )
            {
                spy = new GCF::SignalSpy1(this, objectOwner, objectId, objectPtr, signal.constData());
                if( !spy->isValid() )
                {
                    delete spy;
                    spy = 0;
                }
                else
                    d->signalSpyList.append(spy);
            }
        }

        GCF::RequestConnectionResMessage* reply = new GCF::RequestConnectionResMessage(msg);
        reply->setObjectId(objectId);
        if( spy )
            reply->setSignal(spy->signal());
        else
            reply->setSignal(msg->signal());
        reply->setSuccess(spy != 0);
        sendMessage(reply);

    } break;

    case GCF::Message::ReleaseConnection: {

        GCF::ReleaseConnectionMessage* msg = (GCF::ReleaseConnectionMessage*)received;
        QByteArray signal = msg->signal().toAscii();
        GCF::SignalSpy1* spy = d->findSignalSpy(objectPtr, signal.constData());
        if(spy)
        {
            d->signalSpyList.removeAll(spy);
            delete spy;
        }

        GCF::ReleaseConnectionResMessage* reply = new GCF::ReleaseConnectionResMessage(msg);
        reply->setObjectId(objectId);
        reply->setSignal(msg->signal());
        reply->setSuccess(spy != 0);
        sendMessage(reply);

    } break;

    case GCF::Message::ReleaseObject: {

        GCF::ReleaseObjectMessage* msg = (GCF::ReleaseObjectMessage*)received;

        // Remove from the object ID map
        d->session->objectIdMap.remove(objectId);
        d->session->objectComponentMap.remove(objectPtr);

        // Remove all signal spies, spying on this object
        for(int i=0; i<d->signalSpyList.count(); i++)
        {
            GCF::SignalSpy1* spy = d->signalSpyList[i];
            if( spy->sender() == objectPtr )
            {
                d->signalSpyList.removeAt(i);
                --i;
                delete spy;
            }
        }

        // Send a confirmation
        GCF::ReleaseObjectResMessage* reply = new GCF::ReleaseObjectResMessage(msg);
        reply->setObjectId(objectId);
        reply->setSuccess(true);

    } break;

    default:
        return false;
    }

    return true;
}

void GCF::RemoteApp::objectDestroyed(QObject* obj)
{
    qint32 id = -1;

    // Find out if this slot was called by one of our objects
    QList<QObject*> objects = d->session->objectIdMap.values();
    if( objects.contains(obj) )
        id = d->session->objectIdMap.key(obj);

    // Discard the call if the object is not ours
    if(id < 0)
        return;

    // Unregister the object
    d->session->objectIdMap.remove(id);

    // Send a ReleaseObject message to the other end.
    GCF::ReleaseObjectMessage* msg = new GCF::ReleaseObjectMessage;
    msg->setObjectId(id);
    sendMessage(msg);
}

void GCF::RemoteApp::sendPendingMessages()
{
    if( !d->pendingMessageLock.tryLock() )
    {
        // schedule self again
        // I wouldnt use this because, queued connections are scheduled really quick.
        // I would much rather use a QTimer for this
        // QMetaObject::invokeMethod(this, "sendPendingMessages", Qt::QueuedConnection);
        QTimer::singleShot(500, this, SLOT("sendPendingMessages()"));
        return;
    }

    QList<GCF::Message*> msgList = d->pendingMessages;
    d->pendingMessages.clear();
    d->pendingMessageLock.unlock();

    for(int i=0; i<msgList.count(); i++)
    {
        GCF::Message* msg = msgList[i];
        sendMessage(msg);
    }
}

bool GCF::RemoteApp::isAllowObjectAccess(AbstractComponent* comp, QObject* object, RemoteApp* remoteApp) const
{
    return GCF::AppAccessPoint::instance().isAllowObjectAccess(comp, object, remoteApp);
}

bool GCF::RemoteApp::isAllowObjectMethodInvoke(AbstractComponent* comp, QObject* object, int methodIdx, RemoteApp* remoteApp) const
{
    return GCF::AppAccessPoint::instance().isAllowObjectMethodInvoke(comp, object, methodIdx, remoteApp);
}

bool GCF::RemoteApp::isAllowObjectSetProperty(AbstractComponent* comp, QObject* object, int propIdx, RemoteApp* remoteApp) const
{
    return GCF::AppAccessPoint::instance().isAllowObjectSetProperty(comp, object, propIdx, remoteApp);
}

bool GCF::RemoteApp::isAllowObjectGetProperty(AbstractComponent* comp, QObject* object, int propIdx, RemoteApp* remoteApp) const
{
    return GCF::AppAccessPoint::instance().isAllowObjectGetProperty(comp, object, propIdx, remoteApp);
}

bool GCF::RemoteApp::isAllowObjectSignalConnect(AbstractComponent* comp, QObject* object, int signalIdx, RemoteApp* remoteApp) const
{
    return GCF::AppAccessPoint::instance().isAllowObjectSignalConnect(comp, object, signalIdx, remoteApp);
}

bool GCF::RemoteApp::isAllowObjectSignalDelivery(AbstractComponent* comp, QObject* object, int signalIdx, RemoteApp* remoteApp) const
{
    return GCF::AppAccessPoint::instance().isAllowObjectSignalDelivery(comp, object, signalIdx, remoteApp);
}

////////////////////////////////////////////////////////////////////////////////////
// GCF::SignalSpy1
////////////////////////////////////////////////////////////////////////////////////

struct GCF::SignalSpy1Data
{
    SignalSpy1Data() : remoteApp(0), objectOwner(0), objectId(-1) { }

    GCF::RemoteApp* remoteApp;
    GCF::AbstractComponent* objectOwner;
    qint32 objectId;
    QByteArray signal;
};

GCF::SignalSpy1::SignalSpy1(GCF::RemoteApp* parent, GCF::AbstractComponent* objectOwner, qint32 objectId, QObject* sender, const char* signal)
    :GCF::GenericSignalSpy(parent, sender, signal)
{
    d = new GCF::SignalSpy1Data;
    d->remoteApp = parent;
    d->objectId = objectId;
    d->objectOwner = objectOwner;
    d->signal = QMetaObject::normalizedSignature(signal);
}

GCF::SignalSpy1::~SignalSpy1()
{
    // do nothing
}

QByteArray GCF::SignalSpy1::signal2() const
{
    return d->signal;
}

void GCF::SignalSpy1::handleSignalEmission(const QVariantList& args)
{
    if( !d->remoteApp->isAllowObjectSignalDelivery(d->objectOwner, sender(), signalMethodIndex(), d->remoteApp) )
        return;

    // Create a signal delivery message
    GCF::SignalDeliveryMessage* msg = new GCF::SignalDeliveryMessage;
    msg->setObjectId(d->objectId);
    msg->setSignal( QString(this->signal()) );
    msg->setArgList( args );

    // Send the signal delivery message via my remote application
    d->remoteApp->sendMessage(msg);
}

////////////////////////////////////////////////////////////////////////////////////
// GCF::DefaultMessageEncoderDecoder
////////////////////////////////////////////////////////////////////////////////////

GCF::DefaultMessageEncoderDecoder::DefaultMessageEncoderDecoder(QObject* parent)
    : QObject(parent)
{

}

GCF::DefaultMessageEncoderDecoder::~DefaultMessageEncoderDecoder()
{

}

QObject* GCF::DefaultMessageEncoderDecoder::containerObject()
{
    return this;
}

bool GCF::DefaultMessageEncoderDecoder::canDecodeMessage(const QByteArray& ba) const
{
    qint32 msgPacketSize = 0;
    QByteArray byteArray;

    QDataStream ds(ba);
    ds >> msgPacketSize;
    ds >> byteArray;
    if( byteArray.size() != msgPacketSize )
        return false;

    return true;
}

GCF::Message* GCF::DefaultMessageEncoderDecoder::decodeMessage(QByteArray& ba) const
{
    GCF::Message* msg = GCF::Message::fromByteArray(ba);
    return msg;
}

QByteArray GCF::DefaultMessageEncoderDecoder::encodeMessage(const GCF::Message* msg)
{
    QByteArray ba = msg->toByteArray();
    return ba;
}

bool GCF::DefaultMessageEncoderDecoder::isCloseSocketUponSendMessage() const
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
// GCF::HttpMessageEncoderDecoder
////////////////////////////////////////////////////////////////////////////////////

namespace GCF
{
struct HttpPacket
{
    HttpPacket() : Size(0) { }

    QMap<QByteArray,QByteArray> Header;
    QByteArray Data;
    quint32 Size;

    bool isValid() const {
        if( !Header.count() )
            return false;

        if( Header["RequestedUrl"].size() > 1 )
            return true;

        if( Header.contains("Content-Type") )
        {
            static const QByteArray supportedContentType("GCFMessage");
            return Header["Content-Type"] == supportedContentType;
        }

        return false;
    }
};

GCF::HttpPacket ExtractPacket(const QByteArray& ba);
QByteArray NextLine(const QByteArray& rawData, int& offset);
}

GCF::HttpPacket GCF::ExtractPacket(const QByteArray& rawBytes)
{
    GCF::HttpPacket ret;

    int offset = 0;
    while(1)
    {
        QByteArray line = GCF::NextLine(rawBytes, offset);
        if(line.isEmpty())
            break;

        if(!ret.Header.count())
        {
            if( !(line.startsWith("POST") || line.startsWith("GET") || line.startsWith("PUT")) )
                return ret;

            QList<QByteArray> comps = line.split(' ');

            ret.Header["RequestType"] = comps.first();
            ret.Header["HttpVersion"] = comps.last();
            ret.Header["RequestedUrl"] = comps.count() == 3 ? comps.at(1) : QByteArray();
        }
        else
        {
            int colonIndex = line.indexOf(':');
            QByteArray key = line.left(colonIndex);
            QByteArray value = line.mid(colonIndex+2);
            ret.Header[key] = value;
        }
    }

    // Extract data part
    int contentLength = ret.Header.contains("Content-Length") ?
                ret.Header["Content-Length"].toInt() : -1;
    ret.Data = rawBytes.mid(offset, contentLength);

    // Store size of HTTP packet in rawbytes..
    ret.Size = offset + ret.Data.size();

    return ret;
}

QByteArray GCF::NextLine(const QByteArray& rawData, int& offset)
{
    static const QByteArray delim("\r\n");
    int index = rawData.indexOf(delim, offset);
    if(index < 0)
        return QByteArray();

    QByteArray ret;
    if(index > offset)
        ret = rawData.mid(offset, index-offset);
    offset = index + delim.size();

    return ret;
}

GCF::HttpMessageEncoderDecoder::HttpMessageEncoderDecoder(QObject* parent)
    : QObject(parent)
{

}

GCF::HttpMessageEncoderDecoder::~HttpMessageEncoderDecoder()
{

}

QObject* GCF::HttpMessageEncoderDecoder::containerObject()
{
    return this;
}

bool GCF::HttpMessageEncoderDecoder::canDecodeMessage(const QByteArray& ba) const
{
    GCF::HttpPacket packet = GCF::ExtractPacket(ba);
    return packet.isValid();
}

GCF::Message* GCF::HttpMessageEncoderDecoder::decodeMessage(QByteArray& ba) const
{
    GCF::HttpPacket packet = GCF::ExtractPacket(ba);
    if(packet.isValid())
    {
        QByteArray requestedUrl = packet.Header["RequestedUrl"];
        return this->decodeMessageFromUrl(requestedUrl);

        GCF::Message* msg = GCF::Message::fromByteArray(packet.Data);
        ba.remove(0, packet.Size);
        return msg;
    }

    return 0;
}

QByteArray GCF::HttpMessageEncoderDecoder::encodeMessage(const GCF::Message* msg)
{
    QByteArray ret;

    if(msg->inceptionMessage() && msg->inceptionMessage()->customData().hasKey("RESTCall"))
    {
        QString responseType = msg->inceptionMessage()->customData().value("ResponseType").toString();

        if(responseType != "RESULT" || responseType.isEmpty())
        {
            static IMessageEncoderDecoder* xmlEncDec = 0;
            for(int i=0; i<GCF::AppAccessPoint::instance().messageEncoderDecoderCount(); i++)
            {
                xmlEncDec = GCF::AppAccessPoint::instance().messageEncoderDecoderAt(i);
                if( !qstrcmp(xmlEncDec->containerObject()->metaObject()->className(), "XMLGCFMessageHandler") )
                    break;
                xmlEncDec = 0;
            }

            if(xmlEncDec)
            {
                ret = xmlEncDec->encodeMessage(msg);
                return ret;
            }
        }
        else
        {
            GCF::AsyncInvokeMethodResMessage* responseMsg = (GCF::AsyncInvokeMethodResMessage*)msg;
            QVariant result = responseMsg->retValue();
            if(!responseMsg->isSuccess())
                return ret;

            switch(result.type())
            {
            case QVariant::Image: {
                QImage image = result.value<QImage>();
                QBuffer buffer(&ret);
                buffer.open(QIODevice::WriteOnly);
                image.save(&buffer, "PNG");
                buffer.close();
                return ret;
                } break;
            case QVariant::Pixmap: {
                QPixmap image = result.value<QPixmap>();
                QBuffer buffer(&ret);
                buffer.open(QIODevice::WriteOnly);
                image.save(&buffer, "PNG");
                buffer.close();
                return ret;
                } break;
            case QVariant::Bitmap: {
                QBitmap image = result.value<QBitmap>();
                QBuffer buffer(&ret);
                buffer.open(QIODevice::WriteOnly);
                image.save(&buffer, "PNG");
                buffer.close();
                return ret;
                } break;
            default:
                break;
            }

            ret = result.toString().toLatin1();
            return ret;
        }
    }

    QByteArray msgBa = msg->toByteArray();

    QTextStream ts(&ret, QIODevice::WriteOnly);
    ts << "HTTP/1.1 200 Ok\r\n";
    ts << "Content-Type: GCFMessage\r\n";
    ts << "Content-Length: " << msgBa.size() << "\r\n";
    ts << "\r\n";
    ts.flush();

    ret += msgBa;

    return ret;
}

bool GCF::HttpMessageEncoderDecoder::isCloseSocketUponSendMessage() const
{
    return true;
}

static QString urlDecode(const QString& input)
{
    QString result = input;
    result = result.replace("+", " ");

    int index = 0;
    while(1)
    {
        index = result.indexOf("%", index);
        if(index < 0)
            break;

        QString asciiCodeStr = result.mid(index+1,2).toLatin1();
        uchar asciiCode = asciiCodeStr.at(0).digitValue()*16 + asciiCodeStr.at(1).digitValue();
        QChar ch( asciiCode );

        result.replace(index, 3, ch);
    }

    return result;
}

GCF::Message* GCF::HttpMessageEncoderDecoder::decodeMessageFromUrl(const QByteArray& reqUrl) const
{
    QByteArray requestedUrl = reqUrl;
    if(requestedUrl.startsWith("/?") || requestedUrl.startsWith("/xml?") || requestedUrl.startsWith("/result?"))
    {
        QString responseType = "XML";
        if(requestedUrl.startsWith("/result?"))
            responseType = "RESULT";

        requestedUrl.prepend(QByteArray("http://localhost"));
        if( requestedUrl.size() )
        {
            QUrl url( requestedUrl );
            if( !url.hasQueryItem("objectName") || !url.hasQueryItem("methodName") )
                return 0;

            GCF::AsyncInvokeMethodMessage* msg = new GCF::AsyncInvokeMethodMessage;
            msg->customData()["RESTCall"] = true;
            msg->customData()["ResponseType"] = responseType;

            msg->setObjectName( url.queryItemValue("objectName") );
            msg->setMethodName( url.queryItemValue("methodName") );

            if(url.hasQueryItem("guid"))
                msg->customData()["ClientGUID"] = url.queryItemValue("guid");
            else if(url.hasQueryItem("GUID"))
                msg->customData()["ClientGUID"] = url.queryItemValue("GUID");

            QVariantList args;
            int paramIndex = 1;
            while(1)
            {
                QString key = QString("p%1").arg(paramIndex++);
                if(!url.hasQueryItem(key))
                    break;

                args << url.queryItemValue(key);
            }

            msg->setArgList(args);
            return msg;
        }
    }

    QString url(reqUrl);
    QStringList urlComps = url.split("/", QString::SkipEmptyParts);
    if(urlComps.count() < 1)
        return 0;

    QString responseType = urlComps.at(0).toUpper();
    if(responseType.indexOf(".") >= 0)
    {
        responseType = "XML";
        urlComps.prepend(responseType);
    }

    QString objectName = urlComps.at(1).section(".", 0, 1);
    QString methodName = urlComps.at(1).section(".", 2, 2);
    if(objectName.isEmpty() || methodName.isEmpty())
        return 0;

    GCF::AsyncInvokeMethodMessage* msg = new GCF::AsyncInvokeMethodMessage;
    msg->customData()["RESTCall"] = true;
    msg->customData()["ResponseType"] = responseType;
    msg->setObjectName(objectName);
    msg->setMethodName(methodName);

    QVariantList args;
    for(int i=2; i<urlComps.count(); i++)
        args << urlDecode(urlComps.at(i));

    msg->setArgList(args);

    return msg;
}

///////////////////////////////////////////////////////////////////////////////

#ifndef GCF_NO_JSON
#include <QString>
#include <QVariant>
#include <QtScript>
#include <QVariantMap>
#include <QVariantList>

namespace GCF
{
class JSON
{
public:
    static JSON& instance();
    ~JSON() { }

    QVariantMap parse(const QString& string, bool* ok=0);
    QString serialize(const QVariant& value);

protected:
    JSON();

private:
    QScriptEngine m_engine;
    QScriptValue m_parseFn;
    QScriptValue m_streamFn;

    QScriptValue createValue(const QVariant& value);
};
}

GCF::JSON& GCF::JSON::instance()
{
    static GCF::JSON theInstance;
    return theInstance;
}


GCF::JSON::JSON()
{
    const QString script = "function parse_json(string) { return JSON.parse(string); }\n"
                           "function stream_json(object) { return JSON.stringify(object); }";
    m_engine.evaluate(script);

    m_parseFn = m_engine.globalObject().property("parse_json");
    m_streamFn = m_engine.globalObject().property("stream_json");
}

QVariantMap GCF::JSON::parse(const QString& string, bool *ok)
{
    QScriptValue result = m_parseFn.call(QScriptValue(), QScriptValueList() << QScriptValue(string));
    if(result.isError())
    {
        if(ok)
            *ok = false;
        return QVariantMap();
    }

    QVariantMap resultMap = result.toVariant().toMap();
    if(ok)
        *ok = true;
    return resultMap;
}

QString GCF::JSON::serialize(const QVariant& value)
{
    QScriptValue arg = this->createValue(value);
    QScriptValue result = m_streamFn.call(QScriptValue(), QScriptValueList() << arg);
    QString resultString = result.toString();
    return resultString;
}

QScriptValue GCF::JSON::createValue(const QVariant& value)
{
    if(value.type() == QVariant::Map)
    {
        QScriptValue obj = m_engine.newObject();

        QVariantMap map = value.toMap();
        QVariantMap::const_iterator it = map.begin();
        QVariantMap::const_iterator end = map.end();
        while(it != end)
        {
            obj.setProperty( it.key(), this->createValue(it.value()) );
            ++it;
        }

        return obj;
    }

    if(value.type() == QVariant::List)
    {
        QVariantList list = value.toList();
        QScriptValue array = m_engine.newArray(list.length());
        for(int i=0; i<list.count(); i++)
            array.setProperty(i, this->createValue(list.at(i)));

        return array;
    }

    switch(value.type())
    {
    case QVariant::String:
        return QScriptValue(value.toString());
    case QVariant::Int:
        return QScriptValue(value.toInt());
    case QVariant::UInt:
        return QScriptValue(value.toUInt());
    case QVariant::Bool:
        return QScriptValue(value.toBool());
    case QVariant::ByteArray:
        return QScriptValue(QLatin1String(value.toByteArray()));
    case QVariant::Double:
        return QScriptValue((qsreal)value.toDouble());
    default:
        break;
    }

    if(value.isNull())
        return QScriptValue(QScriptValue::NullValue);

    return m_engine.newVariant(value);
}
#endif

GCF::JSONMessageEncoderDecoder::JSONMessageEncoderDecoder(QObject* parent)
    : QObject(parent)
{
}

GCF::JSONMessageEncoderDecoder::~JSONMessageEncoderDecoder()
{
}

QObject* GCF::JSONMessageEncoderDecoder::containerObject()
{
    return this;
}

bool GCF::JSONMessageEncoderDecoder::canDecodeMessage(const QByteArray& ba) const
{
#ifdef GCF_NO_JSON
    Q_UNUSED(ba)
    return false;
#else
    QByteArray messageBytes;
    int index = 0;
    int bracketCount = 0;
    while(index < ba.length())
    {
        char ch = ba.at(index++);
        messageBytes.append(ch);

        if(ch == '{')
            ++bracketCount;
        else if(ch == '}')
            --bracketCount;

        if(bracketCount <= 0)
            break;
    }

    bool success = false;
    GCF::JSON::instance().parse(messageBytes, &success);
    return success;
#endif
}

GCF::Message* GCF::JSONMessageEncoderDecoder::decodeMessage(QByteArray& ba) const
{
#ifdef GCF_NO_JSON
    Q_UNUSED(ba)
    return 0;
#else
    QByteArray messageBytes;
    int bracketCount = 0;
    while(ba.length())
    {
        char ch = ba.at(0);
        ba = ba.remove(0, 1);
        messageBytes.append(ch);

        if(ch == '{')
            ++bracketCount;
        else if(ch == '}')
            --bracketCount;

        if(bracketCount <= 0)
            break;
    }

    bool success = false;
    QVariantMap map = GCF::JSON::instance().parse(messageBytes, &success);
    if(!success)
        return 0;

    if(!map.contains("ObjectName") || !map.contains("MethodName"))
        return new GCF::Message; // invalid message

    qint32 msgId = map.value("ID", -1).toInt();
    QString objectName = map["ObjectName"].toString();
    QString methodName = map["MethodName"].toString();
    QVariantList arguments = map["Arguments"].toList();

    GCF::AsyncInvokeMethodMessage* msg = new GCF::AsyncInvokeMethodMessage(msgId);
    msg->setObjectName(objectName);
    msg->setMethodName(methodName);
    msg->setArgList(arguments);
    return msg;
#endif
}

QByteArray GCF::JSONMessageEncoderDecoder::encodeMessage(const GCF::Message* msg)
{
#ifdef GCF_NO_JSON
    Q_UNUSED(msg)
    return QByteArray();
#else
    if(msg->messageType() != GCF::Message::AsyncInvokeMethodRes)
        return QByteArray();

    GCF::AsyncInvokeMethodResMessage* msg2 = (GCF::AsyncInvokeMethodResMessage*)msg;

    QVariantMap map;
    map["ID"] = msg2->id();
    map["ObjectName"] = msg2->objectName();
    map["MethodName"] = msg2->methodName();
    map["Success"] = msg2->isSuccess();
    if(msg2->isSuccess())
        map["Result"] = msg2->retValue();
    else
        map["Error"] = msg2->errorMessage();

    QByteArray ba = GCF::JSON::instance().serialize(map).toLatin1();
    return ba;
#endif
}

bool GCF::JSONMessageEncoderDecoder::isCloseSocketUponSendMessage() const
{
    return true;
}


