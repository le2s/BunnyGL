#include "TicTacToeComponent.h"
#include "TicTacToeWidget.h"

#include <QMessageBox>
#include <QProcessEnvironment>

#include <GCF/Application>
#include <GCF/RemoteAppAccessPoint>
#include <GCF/RemoteObject>
#include <GCF/RemoteAppDiscovery>

#include <QtDebug>

GCF_DEFINE_COMPONENT(TicTacToeComponent)

TicTacToeComponent & TicTacToeComponent::instance()
{
    static TicTacToeComponent* ptr = GCF_CREATE_COMPONENT(TicTacToeComponent);
    return *ptr;
}

TicTacToeComponent::TicTacToeComponent()
{
    m_remoteTicTacToeWidget = 0;
}

TicTacToeComponent::~TicTacToeComponent()
{
}

// This function returns the name of this player, by looking up the
// value of the USERNAME environment variable
QString TicTacToeComponent::userName() const
{
    return QProcessEnvironment::systemEnvironment().value("USERNAME");
}

void TicTacToeComponent::initializeComponent()
{
    m_ticTacToeWidget = new TicTacToeWidget;
    gApp->setComponentHostWidget(m_ticTacToeWidget);

    m_ticTacToeWidget->resize(600, 600);
    m_ticTacToeWidget->show();
    QObject::connect(m_ticTacToeWidget, SIGNAL(closed()),
                     this, SLOT(slotIClosed()));

    this->setGuiXmlFileName(":/GUIXML/TicTacToeComponent.xml");
}

void TicTacToeComponent::finalizeComponent()
{
}

// This function is called after the creation of this component has been
// announced to all (if any at all) other components in the application.
// By re-implementing this function, we can be sure that the component
// has been initialized and its GUIXML has been processed fully.
void TicTacToeComponent::creationAnnounced()
{
    // Wait for another TicTacToe instance to join
    m_ticTacToeWidget->setWindowTitle("Waiting for another player to join...");
    m_ticTacToeWidget->setEnabled(false);

    // Enable remote app access
    gApp->setIPCName("TicTacToe");
    gApp->setIPCEnabled(true);

    // Look for another TicTacToe instance
    connect( &GCF::RemoteAppDiscovery::instance(),
             SIGNAL(discoveredRemoteApp(GCF::RemoteAppAccessPoint*)),
             this,
             SLOT(slotRemoteTicTacToeDiscovered(GCF::RemoteAppAccessPoint*)));
    GCF::RemoteAppDiscovery::instance().startDiscovery();
}

QObject* TicTacToeComponent::fetchObject(const QString &completeName) const
{
    QStringList comps = completeName.split(".");
    if(comps.last() == "core")
        return const_cast<TicTacToeComponent*>(this);

    return 0;
}

QWidget* TicTacToeComponent::fetchWidget(const QString &completeName) const
{
    QStringList comps = completeName.split(".");
    if(comps.last() == "gameBoard")
        return m_ticTacToeWidget;

    return 0;
}

void TicTacToeComponent::slotStartGame()
{
    m_ticTacToeWidget->setWindowTitle("Remote player joined. Start playing");
    m_ticTacToeWidget->setEnabled(true);

    GCF::RemoteAppAccessPoint* rap =
            m_remoteTicTacToeWidget->remoteAppAccessPoint();

    // We make use of GCF::RemoteObject::asyncCall static method to
    // asyncrhnously call the userName method on the TicTacToeComponent
    // instance of the remote application. When the response to that
    // call is received, we request GCF to call the slotOtherPlayerName()
    // slot in this class.
    GCF::RemoteObject::asyncCall(this, SLOT(slotOtherPlayerName(QVariant,bool)),
                   rap->peerAddress(), rap->peerPort(),
                    "TicTacToeComponent.core", "userName");
}

void TicTacToeComponent::slotRemoteTicTacToeDiscovered(GCF::RemoteAppAccessPoint *rap)
{
    if(m_remoteTicTacToeWidget)
        return;

    GCF::RemoteObject* remoteGameBoard =
            rap->getRemoteObject("TicTacToeComponent.gameBoard");
    remoteGameBoard->setParent(this);

    // We make use of GCF::connect to create a remote signal-slot
    // connection
    GCF::connect(remoteGameBoard, SIGNAL(marked(int,int)),
                 m_ticTacToeWidget, SLOT(mark(int,int)));
    GCF::connect(remoteGameBoard, SIGNAL(iWon()),
                 m_ticTacToeWidget, SLOT(iLost()));
    GCF::connect(remoteGameBoard, SIGNAL(cleared()),
                 m_ticTacToeWidget, SLOT(clear()));

    QObject::connect(remoteGameBoard, SIGNAL(activated()),
                     this, SLOT(slotStartGame()));
    QObject::connect(remoteGameBoard, SIGNAL(deactivated()),
                 this, SLOT(slotRemotePlayerQuit()));

    m_remoteTicTacToeWidget = remoteGameBoard;
}

void TicTacToeComponent::slotRemotePlayerQuit()
{
    QMessageBox::information(m_ticTacToeWidget, "Other player quit",
        "The other player has left the game. You can wait until "
        "another player joins, or quit!");

    m_remoteTicTacToeWidget->deleteLater();
    m_remoteTicTacToeWidget = 0;

    m_ticTacToeWidget->clear();
    m_ticTacToeWidget->setWindowTitle("Waiting for another player to join...");
    m_ticTacToeWidget->setEnabled(false);
}

void TicTacToeComponent::slotIClosed()
{
    if(m_remoteTicTacToeWidget)
        QObject::disconnect(m_remoteTicTacToeWidget, 0, this, 0);
}

void TicTacToeComponent::slotOtherPlayerName(const QVariant& name, bool success)
{
    if(success)
        m_ticTacToeWidget->setWindowTitle("Other player is : " + name.toString());
}


