#ifndef TICTACTOECOMPONENT_H
#define TICTACTOECOMPONENT_H

#include <GCF/AbstractComponent>

class TicTacToeWidget;
namespace GCF {
class RemoteAppAccessPoint;
class RemoteObject;
}

class TicTacToeComponent : public GCF::AbstractComponent
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(TicTacToeComponent)

    public:
        static TicTacToeComponent& instance();
    ~TicTacToeComponent();

    Q_INVOKABLE QString userName() const;

protected:
    TicTacToeComponent();
    void initializeComponent();
    void finalizeComponent();
    void creationAnnounced();
    QObject* fetchObject(const QString &completeName) const;
    QWidget* fetchWidget(const QString &completeName) const;

private slots:
    void slotStartGame();
    void slotRemoteTicTacToeDiscovered(GCF::RemoteAppAccessPoint* rap);
    void slotRemotePlayerQuit();
    void slotIClosed();
    void slotOtherPlayerName(const QVariant& name, bool success);

private:
    TicTacToeWidget* m_ticTacToeWidget;
    GCF::RemoteObject* m_remoteTicTacToeWidget;
};

#endif // TICTACTOECOMPONENT_H
