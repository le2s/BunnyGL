#include "SimpleIRC.h"
#include <GCF/Application>
#include <GCF/RemoteAppAccessPoint>
#include <GCF/RemoteObject>
#include <GCF/RemoteAppDiscovery>

#include "IRCWindow.h"
#include "ContactList.h"

GCF_DEFINE_COMPONENT(SimpleIRC)

SimpleIRC & SimpleIRC::instance()
{
    static SimpleIRC* ptr = GCF_CREATE_COMPONENT(SimpleIRC);
    return *ptr;
}

SimpleIRC::SimpleIRC() : m_ircWindow(0)
{
    m_ircWindow = new IRCWindow;
    gApp->setComponentHostWidget(m_ircWindow);
}

SimpleIRC::~SimpleIRC()
{
}

void SimpleIRC::initializeComponent()
{
    this->setGuiXmlFileName(":/SimpleIRC/GUIXML");
}

void SimpleIRC::finalizeComponent()
{
    GCF::RemoteAppDiscovery::instance().stopDiscovery();
}

void SimpleIRC::creationAnnounced()
{
    m_ircWindow->setWindowTitle("SimpleIRC - Based on GCF from VCreate Logic");
    m_ircWindow->resize(640, 480);
    m_ircWindow->show();

    // Enable IPC
    gApp->setIPCName("SimpleIRC");
    gApp->setIPCEnabled(true);

    // We begin looking for instances of SimpleIRC launched on the LAN
    connect(&GCF::RemoteAppDiscovery::instance(),
            SIGNAL(discoveredRemoteApp(GCF::RemoteAppAccessPoint*)),
            this,
            SLOT(slotFoundFriend(GCF::RemoteAppAccessPoint*)));
    GCF::RemoteAppDiscovery::instance().startDiscovery();
}

QObject * SimpleIRC::fetchObject(const QString &completeName) const
{
    QStringList comps = completeName.split(".");
    if(comps.last() == "Core")
        return const_cast<SimpleIRC*>(this);

    return 0;
}

QWidget * SimpleIRC::fetchWidget(const QString &completeName) const
{
    QStringList comps = completeName.split(".");
    if(comps.last() == "IRCWindow")
        return m_ircWindow;

    return 0;
}

void SimpleIRC::slotFoundFriend(GCF::RemoteAppAccessPoint *rap)
{
    // Here we try to figure out the name of the user on the other side
    if(!rap)
        return;

    GCF::RemoteObject* remoteIRCWindow =
            rap->getRemoteObject("SimpleIRC.IRCWindow");
    connect(remoteIRCWindow, SIGNAL(methodInvocationResult(qint32,bool,QVariant,QString)),
            this, SLOT(slotFriendNameFound(qint32,bool,QVariant,QString)));

    remoteIRCWindow->call("userName");
    connect(rap, SIGNAL(disconnectedFromApp()), rap, SLOT(deleteLater()));
}

void SimpleIRC::slotFriendNameFound(qint32 callId, bool success,
                   const QVariant& result, const QString& method)
{
    Q_UNUSED(callId);

    if(method == "userName" && success)
    {
        // Here we register the contact on the other side and let him/her
        // on the chat.
        QString userName = result.toString();
        GCF::RemoteObject* remoteIRCWindow = qobject_cast<GCF::RemoteObject*>(sender());

        m_ircWindow->addContact(userName, remoteIRCWindow);
        connect(remoteIRCWindow, SIGNAL(deactivated()), remoteIRCWindow, SLOT(deleteLater()));
    }
}
